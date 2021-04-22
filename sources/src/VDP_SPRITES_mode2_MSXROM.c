/* ============================================================================= 
  VDP SPRITE mode2 MSX ROM SDCC Library (fR3eL Project)
  Version: 1.0 (28/3/2021)
  Author: mvac7 [mvac7303b@gmail.com]
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C and Z80 assembler
  project: https://github.com/mvac7/SDCC_VDP_SPRITES_mode2_MSXROM_Lib

  Description:
    Opensource library with functions for use Sprites from V9938 using
    BIOS functions.
       
    It's complemented with the SDCC TMS9918A MSX ROM Library.
       https://github.com/mvac7/SDCC_VDP_TMS9918A_MSXROM_Lib
    
    For SDCC 3.9 or higher. 
    
  History of versions:
    v0.9 (28/3/2021)                   
============================================================================= */ 

#include "../include/VDP_SPRITES_mode2.h"

#include "../include/msxSystemVars.h"
#include "../include/msxBIOS.h"


#define YHIDDEN  217


char SPRITE_YBUFF[32];




/* =============================================================================
 ClearSprites
 Description: 
             Initialises all sprite data The sprite pattern is cleared to
             null, the sprite number to the sprite plane number, the
             sprite colour to the foregtound colour. The vertical location
             of the sprite is set to 209 (mode 0 to 3) or 217 (mode 4 to 8).
 Input:       -
 Output:      -
============================================================================= */
void ClearSprites() __naked
{
__asm
  push IX
  call CLRSPR
  pop  IX
  ret
__endasm;
}    



/* =============================================================================
 PUTSPRITE
 Description: Displays the sprite pattern.
              In G4 screen mode (V9938), fills the 16 rows of color values.
 Input:       [char] sprite plane (0-31) 
              [char] x 
              [char] y
              [char] pattern
 Output:      -
============================================================================= */
void PUTSPRITE(char plane, char x, char y, char pattern) __naked
{
plane;x;y;pattern;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   A,4(IX) ;num sprite plane
  call CALATR ;get the VRAM address of the sprite attribute table
  
  ld   B,A
  ld   A,6(IX) ;y
  call WRITEY
  
  inc  HL  
  ld   A,5(IX) ;x
  call WRTVRM
  
  inc  HL  
  ld   E,7(IX)
  call SPRPATNOV  ;pattern
    
  pop  IX
  ret

__endasm;
}



/* =============================================================================
 SetSpritePattern
 Description: Assign a pattern to a sprite plane.
 Input:       [char] sprite plane (0-31) 
              [char] pattern
 Output:      -
============================================================================= */
void SetSpritePattern(char plane, char pattern) __naked
{
plane;pattern; 
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(ix) ;num sprite plane  
  call CALATR  ;get the VRAM address of the sprite attribute table
  inc  HL
  inc  HL
  
  ld   E,5(ix) ;number of pattern to assign  
  call SPRPATNOV
  
  pop  IX
  ret
  


; set sprite pattern number
; Multiply * 4 when its a 16x16 sprite.
; E - Sprite Number  
SPRPATNOV:  ;sprite pattern no calc vram address
  call GSPSIZ          ;0x008A get sprite size in bytes 
  cp   #32             ;16x16
  jr   NZ,WRTPAT

  ;si es 16x16
  SLA  E
  SLA  E ;multiplica x4  

WRTPAT:
  LD   A,E
  jp   WRTVRM
  
__endasm;
}



/* =============================================================================
 SetSpriteColor
 Description: Assign a color to a sprite plane.
              In G4 screen mode (V9938), fills the 16 rows of color values.
 Input:       [char] sprite plane (0-31) 
              [char] color (0-15)
 Output:      -
============================================================================= */
/*void SetSpriteColor(char plane, char color) __naked
{
plane;color;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   A,4(IX) ;num sprite plane
;  ld   E,5(IX) ;color

;  ld   A,D
;  push DE  
  call getVRAMcolorspr
;  pop  DE
  
  ld   A,5(IX) ;color
  ld   BC,#16
  call FILVRM
  
  pop  IX
  ret

__endasm;
}*/



/* =============================================================================
 SetSpritePosition
 Description: Assigns the position coordinates of a sprite plane.
 Input:       [char] sprite plane (0-31) 
              [char] x 
              [char] y
 Output:      -
============================================================================= */
void SetSpritePosition(char plane, char x, char y) __naked
{
plane;x;y;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(ix) ;num sprite
  call CALATR  ;get in HL the VRAM address of the sprite attribute table
  
  ld   B,A
  ld   A,6(ix)   ;y
  call WRITEY
  
  inc  HL
  ld   A,5(ix)   ;x
  call WRTVRM
  
  pop  IX
  ret
  
WRITEY:
  ld   IY,#_SPRITE_YBUFF
  ld   D,#0
  ld   E,B
  ADD  IY,DE  
  ld   (IY),A        ; save Y position
  jp   WRTVRM  
   
__endasm;
}



/* =============================================================================
 SetSpriteVisible
 Description: Hides or shows a sprite plane.
 Input:       [char] sprite plane (0-31) 
              [boolean] visible state
 Output:      -
============================================================================= */
void SetSpriteVisible(char plane, boolean state) __naked
{
plane;state;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(ix) ;num sprite
  
  ld   IY,#_SPRITE_YBUFF
  ld   D,#0
  ld   E,A
  ADD  IY,DE
  
  call CALATR   ;get in HL the VRAM address of the sprite attribute table

  ld   A,5(ix) ;state
  or   A ;0 = off
  jr   Z,SPRITEOFF
  
;sprite ON
  ld   A,(IY)  ;get Y value
  
  call WRTVRM
    
  pop  IX
  ret


;sprite OFF
SPRITEOFF:
  call RDVRM
  cp   #YHIDDEN
  jr   Z,ENDOFF ;if not visible then Dont overwrite. 

  ld   A,#YHIDDEN
  call WRTVRM
  
ENDOFF:
  pop  IX
  ret
__endasm;
}



/* =============================================================================
 SetEarlyClock
 Description: Apply the Early Clock of a sprite. Move 32 points to the left           
              the X position of the sprite.
 Input:       [char] sprite plane (0-31) 
 Output:      -
============================================================================= */
void SetEarlyClock(char plane) __naked
{
plane;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX)  
  call getVRAMcolorspr
  
  ld   B,#16
nextSetEC:
  push HL
  call NRDVRM   ;VPEEK V9938
  OR   #128
  pop  HL
  call WRTVRM   ;VPOKE V9938
  inc  HL  
  djnz nextSetEC

  pop  IX
  ret
  
__endasm;
}



/* =============================================================================
 UnsetEarlyClock
 Description: Disables the Early Clock. Restore the position of a sprite.
 Input:       [char] sprite plane (0-31)
 Output:      -
============================================================================= */
void UnsetEarlyClock(char plane) __naked
{
plane;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   A,4(IX)
  call getVRAMcolorspr
  
  ld   B,#16
nextUnsetEC:
  call NRDVRM   ;VPEEK V9938
  and  #127
  call WRTVRM   ;VPOKE V9938
  inc  HL 
  djnz nextUnsetEC
  
  pop  IX
  ret




;ASM FUNCTIONS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

; >A - sprite plane
; <HL - VRAM addr
getVRAMcolorspr:
  ld   HL,#0x1C00    ;vram address color table base (screen 4)
  ld   D,#0          
  ld   E,A
  SLA  E             ;multiplico A por 16 para identificar la posicion en la VRAM
  RL   D             ;con cuatro rotaciones
  SLA  E
  RL   D
  SLA  E
  RL   D
  SLA  E
  RL   D
  add  HL,DE  
   
  ret
  
__endasm;
}