/* =============================================================================
  test VDP SPRITE mode2 MSXROM Library
  Version: 1.0 (22/04/2021) 
  Author: mvac7
  Architecture: MSX
  Format: ROM
  Programming language: C
    
History of versions: 
 - v1.0 (22/04/2021) First version
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxBIOS.h"
#include "../include/msxSystemVars.h"

#include "../include/VDP_TMS9918A_MSXROM.h"
#include "../include/VDP_SPRITES_mode2.h"




#define  HALT __asm halt __endasm   //wait for the next interrupt




//  definition of functions  ---------------------------------------------------
void WAIT(uint cicles);
char INKEY();

void CLS();

void VPRINT(byte column, byte line, char* text);  //print in screen 1 or 2
void VPOKEARRAY(uint vaddr, char* text);

void LOCATE(char x, char y);
void PRINT(char* text);

char PEEK(uint address);
uint PEEKW(uint address);

void setFont();

void setSpritesPatterns();
void showSprites();

void testSPRITES();
void testSpritePosition();
//void testSpriteColor();
void testSpritePattern();
void testSpriteVisible();




// constants  ------------------------------------------------------------------

const char text01[] = "Test SDCC SPRITES mode2 Lib"; 
const char text02[] = "          v1.0 (22/04/2021)";


const char sprcol[8]={12,2,3,7,6,8,9,14};


// Project: New Project
// SpriteSet Pattern Data - Size:16x16
// Sprite range: 0 to 7
// Size= 256
const char SPRITE_DATA[]={
0x0F,0x1F,0x3D,0x3F,0x7B,0x7C,0xBF,0xBF,0x9F,0xEF,0x6F,0x1F,0x0F,0x05,0x05,0x1D,
0xC0,0xE0,0x60,0xC0,0x40,0x80,0x80,0x84,0x86,0xDE,0xC0,0xC0,0x80,0x00,0x00,0xC0,
0x03,0x07,0x4F,0xCF,0x4D,0x4F,0x2C,0x1E,0x3F,0x3F,0x3F,0x3F,0x1F,0x02,0x02,0x06,
0xC0,0xE2,0xE6,0xE2,0xA2,0xF4,0x38,0x7C,0xFE,0xFE,0xFE,0xFC,0xF8,0x40,0x40,0x60,
0x05,0x07,0x1A,0x2E,0x6F,0x5B,0x5C,0x5B,0x5F,0x58,0x6F,0x2D,0x16,0x02,0x02,0x06,
0x40,0xC0,0xB8,0xD4,0xD4,0x6A,0xEA,0x6A,0xEA,0x6A,0xD4,0xD4,0xF8,0x40,0x40,0x60,
0x05,0x07,0x1F,0x3F,0x7F,0x7F,0x7F,0x7D,0x7F,0x7F,0x78,0x3F,0x1F,0x04,0x04,0x0C,
0x40,0xC0,0xF0,0xF8,0xFC,0xFC,0xFC,0x7C,0xFC,0xFC,0x3C,0xF8,0xF0,0x40,0x40,0x60,
0x07,0x1F,0x3F,0x3F,0x7F,0x71,0x7F,0x7F,0x7F,0x7E,0x3E,0x3F,0x1F,0x07,0x02,0x06,
0xE0,0xF8,0xFC,0xFC,0xFE,0x8E,0xFE,0xFE,0xFE,0x7E,0x7C,0xFC,0xF8,0xE0,0x40,0x60,
0x00,0x07,0x1F,0x3F,0x7F,0x71,0xFF,0xFF,0x7B,0x7C,0x3F,0x1F,0x07,0x02,0x02,0x06,
0x00,0xE0,0xF8,0xFC,0xFE,0x8E,0xFF,0xFF,0xDE,0x3E,0xFC,0xF8,0xE0,0x40,0x40,0x60,
0x05,0x1E,0x3F,0x6F,0x57,0x6E,0x3F,0x00,0x07,0x05,0x07,0x04,0x06,0x03,0x02,0x06,
0x60,0xF8,0xF4,0xEA,0x76,0xBE,0x7C,0x00,0xE0,0x60,0xE0,0x20,0x60,0xC0,0x40,0x60,
0x05,0x1E,0x3F,0x6F,0x57,0x6E,0x3F,0x07,0x07,0x05,0x07,0x04,0x06,0x03,0x02,0x06,
0x60,0xF8,0xF4,0xEA,0x76,0xBE,0x7C,0xE0,0xE0,0x60,0xE0,0x20,0x60,0xC0,0x40,0x60};

// Project: New Project
// SpriteSet Color Data
// Sprite range: 0 to 7
// Size= 128
const char SPRITE_DATA_COLOR[]={
0x0C,0x0C,0x0C,0x0C,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x02,0x0C,0x0F,0x0F,0x07,
0x0C,0x02,0x02,0x02,0x0C,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x0C,0x0E,0x0E,0x05,
0x0A,0x0A,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0E,0x0E,0x05,
0x0C,0x0C,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x05,0x0F,0x07,
0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x07,0x07,
0x0F,0x0A,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0A,0x0E,0x05,0x05,
0x08,0x09,0x09,0x09,0x09,0x09,0x08,0x0F,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0E,0x0E,
0x0A,0x0E,0x0A,0x0E,0x0A,0x0E,0x0A,0x05,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0E,0x0E};



//coordinate tables with a circular route 
const char posX[]={
0xAA,0xAA,0xAA,0xAA,0xAA,0xA9,0xA9,0xA9,0xA9,0xA8,0xA8,0xA7,0xA7,0xA6,0xA6,0xA5,
0xA4,0xA4,0xA3,0xA2,0xA1,0xA0,0x9F,0x9E,0x9D,0x9C,0x9B,0x9A,0x99,0x98,0x96,0x95,
0x94,0x93,0x91,0x90,0x8E,0x8D,0x8B,0x8A,0x88,0x87,0x85,0x84,0x82,0x80,0x7F,0x7D,
0x7B,0x7A,0x78,0x76,0x74,0x73,0x71,0x6F,0x6D,0x6B,0x6A,0x68,0x66,0x64,0x62,0x60,
0x5F,0x5D,0x5B,0x59,0x57,0x55,0x53,0x52,0x50,0x4E,0x4C,0x4A,0x49,0x47,0x45,0x43,
0x42,0x40,0x3E,0x3D,0x3B,0x3A,0x38,0x36,0x35,0x33,0x32,0x30,0x2F,0x2E,0x2C,0x2B,
0x29,0x28,0x27,0x26,0x25,0x23,0x22,0x21,0x20,0x1F,0x1E,0x1D,0x1C,0x1C,0x1B,0x1A,
0x19,0x19,0x18,0x18,0x17,0x16,0x16,0x16,0x15,0x15,0x15,0x14,0x14,0x14,0x14,0x14,
0x14,0x14,0x14,0x14,0x14,0x15,0x15,0x15,0x16,0x16,0x16,0x17,0x18,0x18,0x19,0x19,
0x1A,0x1B,0x1C,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x25,0x26,0x27,0x28,0x29,
0x2B,0x2C,0x2E,0x2F,0x30,0x32,0x33,0x35,0x36,0x38,0x39,0x3B,0x3D,0x3E,0x40,0x42,
0x43,0x45,0x47,0x49,0x4A,0x4C,0x4E,0x50,0x52,0x53,0x55,0x57,0x59,0x5B,0x5D,0x5F,
0x60,0x62,0x64,0x66,0x68,0x6A,0x6B,0x6D,0x6F,0x71,0x73,0x74,0x76,0x78,0x7A,0x7B,
0x7D,0x7F,0x80,0x82,0x84,0x85,0x87,0x88,0x8A,0x8B,0x8D,0x8E,0x90,0x91,0x93,0x94,
0x95,0x96,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,0xA0,0xA1,0xA2,0xA3,0xA4,0xA4,
0xA5,0xA6,0xA6,0xA7,0xA7,0xA8,0xA8,0xA9,0xA9,0xA9,0xA9,0xAA,0xAA,0xAA,0xAA,0xAA};

  
const char posY[]={
0x5F,0x61,0x63,0x65,0x66,0x68,0x6A,0x6C,0x6E,0x6F,0x71,0x73,0x75,0x77,0x78,0x7A,
0x7C,0x7E,0x7F,0x81,0x82,0x84,0x86,0x87,0x89,0x8A,0x8C,0x8D,0x8F,0x90,0x92,0x93,
0x94,0x95,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9E,0x9F,0x9F,0xA0,0xA1,0xA2,0xA3,0xA4,
0xA4,0xA5,0xA6,0xA6,0xA7,0xA7,0xA8,0xA8,0xA9,0xA9,0xA9,0xA9,0xAA,0xAA,0xAA,0xAA,
0xAA,0xAA,0xAA,0xAA,0xAA,0xA9,0xA9,0xA9,0xA8,0xA8,0xA8,0xA7,0xA7,0xA6,0xA5,0xA5,
0xA4,0xA3,0xA3,0xA2,0xA1,0xA0,0x9F,0x9E,0x9D,0x9C,0x9B,0x9A,0x99,0x97,0x96,0x95,
0x94,0x92,0x91,0x8F,0x8E,0x8D,0x8B,0x8A,0x88,0x86,0x85,0x83,0x82,0x80,0x7E,0x7D,
0x7B,0x79,0x77,0x76,0x74,0x72,0x70,0x6F,0x6D,0x6B,0x69,0x67,0x65,0x64,0x62,0x60,
0x5E,0x5C,0x5A,0x59,0x57,0x55,0x53,0x51,0x4F,0x4E,0x4C,0x4A,0x48,0x47,0x45,0x43,
0x41,0x40,0x3E,0x3C,0x3B,0x39,0x38,0x36,0x34,0x33,0x31,0x30,0x2F,0x2D,0x2C,0x2A,
0x29,0x28,0x27,0x25,0x24,0x23,0x22,0x21,0x20,0x1F,0x1E,0x1D,0x1C,0x1B,0x1B,0x1A,
0x19,0x19,0x18,0x17,0x17,0x16,0x16,0x16,0x15,0x15,0x15,0x14,0x14,0x14,0x14,0x14,
0x14,0x14,0x14,0x14,0x15,0x15,0x15,0x15,0x16,0x16,0x17,0x17,0x18,0x18,0x19,0x1A,
0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x1F,0x20,0x22,0x23,0x24,0x25,0x26,0x27,0x29,0x2A,
0x2B,0x2C,0x2E,0x2F,0x31,0x32,0x34,0x35,0x37,0x38,0x3A,0x3C,0x3D,0x3F,0x40,0x42,
0x44,0x46,0x47,0x49,0x4B,0x4D,0x4F,0x50,0x52,0x54,0x56,0x58,0x59,0x5B,0x5D,0x5F};



char _LineLength;  //sprites per line. TMS9918=4; V9938=8



// Functions -------------------------------------------------------------------


//
void main(void)
{
  SCREEN(1);
  COLOR(15,4,5);
  
__asm
  ld   HL,#0xF3B0 ;LINLEN system variable
  ld   A,#32
  ld   (HL),A     ;WIDTH(32)  
__endasm;
  
  LOCATE(2,10);
  PRINT(text01);
  LOCATE(2,11);
  PRINT(text02);
  

  
//---------------------------------------------------------------------only MSX2
  if (PEEK(MSXVER)>0)
  {
    INKEY();
    
    SCREEN(4);  //G4 V9938 mode
    CLS();
    setFont();  
    
    VPRINT(0,0,"screen 4 (G3)");
    
    _LineLength=8; 
    testSPRITES();  
  }else{
    LOCATE(0,0);
    PRINT("Only for MSX2 or higher.");
  
    INKEY();
  
  }  
//------------------------------------------------------------------------------  
  

  
//END --------------------------------------------------------------------------  
  
  //COLOR(15,4,4);
  //SCREEN(1);
  
  //VPRINT(0,0,"End of the test...");
  //WAIT(30*10);
  
     

//EXIT MSXDOS
/*  screen(0);
    
__asm
 	ld b,4(ix)
	ld c,#0x62
	call 5 
__endasm;*/
//end EXIT

}




/* =============================================================================
One character input (waiting)
============================================================================= */
char INKEY() __naked
{
__asm
  push IX
  ld   IX,#0
  add  IX,SP  

  call CHGET

  ld   L,A
  pop  IX
  ret
__endasm;
}



// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second. 
void WAIT(uint cicles)
{
  uint i;
  for(i=0;i<cicles;i++) HALT;
}



void CLS()
{
  FillVRAM (BASE10, 0x300, 32);
}



//print in screen 1 or 2
void VPRINT(byte column, byte line, char* text)
{
  uint vaddr = BASE10 + (line*32)+column; // calcula la posicion en la VRAM
  VPOKEARRAY(vaddr, text);
}



void VPOKEARRAY(uint vaddr, char* text)
{
  while(*(text)) VPOKE(vaddr++,*(text++));
}



/* =============================================================================
   Set a position the cursor in to the specified location
   Posiciona el cursor hasta la ubicacion especificada
   x(byte) - column (0 to 31 or 39)
   y(byte) - line   (0 to 23)
============================================================================= */
void LOCATE(char x, char y) __naked
{
x;y;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX) ;x
  inc  A       ;incrementa las posiciones para que se situen correctamente en la pantalla
  ld   H,A
  ld   A,5(IX) ;y
  inc  A
  ld   L,A   
  call POSIT
  
  pop  IX
  ret
__endasm;

}



/* =============================================================================
   Print a text in screen
============================================================================= */
void PRINT(char* text) __naked
{ 
text;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   L,4(ix)
  ld   H,5(ix)
  
nextCHAR:  
  ld   A,(HL)
  or   A
  jr   Z,ENDnext   
  call CHPUT //Displays one character (BIOS)
  inc  HL
  jr   nextCHAR
  
ENDnext:  
  pop  IX
  ret
__endasm; 
}



char PEEK(uint address) __naked
{
  address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   L,4(IX)
  ld   H,5(IX)
  
  ld   L,(HL)
  
  pop  IX
  ret  
__endasm;
}



uint PEEKW(uint address) __naked
{
  address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   L,4(ix)
  ld   H,5(ix)
  ld   E,(HL)
  inc  HL
  ld   D,(HL)
  ex   DE,HL  
  
  pop  IX
  ret  
__endasm;
}




void setFont()
{
  uint ROMfont = PEEKW(CGTABL);

  CopyToVRAM(ROMfont,BASE12,0x800);        //MSX font pattern
  CopyToVRAM(ROMfont,BASE12+0x800,0x800);  //MSX font pattern
  CopyToVRAM(ROMfont,BASE12+0x1000,0x800); //MSX font pattern
  FillVRAM(BASE11,0x1800,0xF4);            //colors

  return;
}




// TEST SPRITES  ###############################################################
void testSPRITES()
{
  char posY=2;
  char key;
  
  setSpritesPatterns();
   
  // test sprites 16x16
  //setupSprites(1,false);
//  VPRINT(0,posY++, "SetSpritesSize(1) SPRITES 16x16");
  SetSpritesSize(1);
//  showSprites();
//  WAIT(50);
 
  // test sprites 16x16 + zoom  
//  VPRINT(0,posY++, "SetSpritesZoom(true) SPRITES x2");
//  SetSpritesZoom(true);
//  WAIT(50);
  

  VPRINT(0,posY++, "PUTSPRITE(plane,x,y,nPattern)");
  showSprites();                                                                           
 
  WAIT(100);

//test clear sprites data
  VPRINT(0,posY++, "ClearSprites()");
  WAIT(50);
  ClearSprites();
  WAIT(100);
  
  setSpritesPatterns();
  showSprites();
  WAIT(50); 
  
  VPRINT(0,posY++, "SetSpriteVisible(plane,state)");
  testSpriteVisible();
  
  VPRINT(0,posY++, "SetSpritePattern(plane,nPattern)");
  VPRINT(0,posY++, "(Keeps the sprite plane colors)"); 
  testSpritePattern();
  
  //VPRINT(0,posY++, "SetSpriteColor(plane,color)"); 
  //testSpriteColor();
  
  VPRINT(0,posY++, "SetSpritePosition(plane,x,y)");
  testSpritePosition();
  
  WAIT(50);
  
  VPRINT(0,posY++, "SetEarlyClock(plane)");
  SetEarlyClock(7);
  WAIT(50);
  
  VPRINT(0,posY++, "UnsetEarlyClock(plane)");
  WAIT(20);
  UnsetEarlyClock(7);
  WAIT(50);
  
  posY+=3;
  VPRINT(0,posY, "Press any key.");
  LOCATE(12,posY);
  key = INKEY();
  
  return;
}




// Copy sprites data from memory to VRAM
void setSpritesPatterns()
{
  CopyToVRAM((uint) SPRITE_DATA,BASE14,32*8);
  
  CopyToVRAM((uint) SPRITE_DATA_COLOR,0x1C00,32*8);
}



// TEST PUTSPRITE  #############################################################
void showSprites()
{
  char X=0,Y=3;
  char i=0;
  
  for(i=0;i<8;i++)
  {
    PUTSPRITE(i, X*32, Y*32, i);
    X++;
    if(X==_LineLength)
    {
      X=0;
      Y++;
    }
  }

  return;
}



// TEST SETSPRITEVISIBLE  ######################################################
void testSpriteVisible()
{
  byte i,o;

  SetSpriteVisible(0,false);
  
  for(i=0;i<8;i++)
  {
    for(o=0;o<8;o++)
    {
      if (o==i) SetSpriteVisible(o,true);
      else SetSpriteVisible(o,false);  
    }
    WAIT(25);  
  }  
  
  return;
}





// TEST SETSPRITEPATTERN  ######################################################
void testSpritePattern()
{
  byte i;

  
  for(i=0;i<8;i++)
  {
    SetSpritePattern(7, i);
    WAIT(25);  
  }
  
  SetSpritePattern(7, 7);  
  
  return;
}



// TEST SETSPRITECOLOR  ########################################################
/*void testSpriteColor()
{
  byte i;

  
  for(i=0;i<16;i++)
  {
    SetSpriteColor(7, i);
    WAIT(25);  
  }  
  
  return;
}*/



// TEST SETSPRITEPOSITION  #####################################################
void testSpritePosition()
{
  uint i=0;
  uint posT = 0;

  //SetSpriteColor(7, 8);  

  for(i=0;i<630;i++)
  {
    HALT;
    SetSpritePosition(7, posX[posT], posY[posT]-32);
    posT++;
    if(posT>255) posT=0;  
  }
  
  return;
}








