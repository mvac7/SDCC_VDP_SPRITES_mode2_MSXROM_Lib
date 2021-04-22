# VDP SPRITES mode 2 MSX ROM SDCC Library (fR3eL Project)

```
Author: mvac7 [mvac7303b@gmail.com]
Architecture: MSX
Format: C Object (SDCC .rel)
Programming language: C and Z80 assembler
```



## Description

Open source library with functions to display mode 2 sprites in V9938 graphics mode 3.

The graphic mode 3 (screen 4) is similar to the tile-based graphic mode 2 of the TMS9918A, 
except that you can take advantage of the V9938 sprites: display up to 8 sprites per line and draw a different color on each line.

Provides a set of functions to display Sprites, both complete (PUTSPRITE) and specific (positioning, pattern, visibility and EarlyClock).

This library writes over the sprite attribute table (OAM) of the VRAM and modifies the EC bit in the color table. To work, it requires that the patterns 
and colors of the planes of the Sprites be written in the VRAM beforehand. 
Remember that you can have up to 64 16x16 sprite patterns and that you can display up to 32 on the screen. The colors have to be assigned to each of the 
planes. If you modify the pattern that is shown on a plane, you must write the data of the colors (if they are different).

The V9938 mode 2 sprites allow assigning the EC (Early Clock) offset in each line of the sprite, but the functions of this library do not take advantage of 
this feature and affect all of them.

In the absence of a specific library for the V9938, you can use the [VDP TMS9918A MSX ROM Library](https://github.com/mvac7/SDCC_VDP_TMS9918A_MSXROM_Lib) 
to initialize the screen (screen mode and sprites).

It uses the functions from the MSX BIOS, so it is designed to create applications in ROM format.
  
Use them for developing MSX applications using [Small Device C Compiler (SDCC)](http://sdcc.sourceforge.net/) cross compiler.

In the source code (\examples), you can find applications for testing and learning purposes.

This library is part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

Enjoy it!



### About the Sprite libraries

I'm developing several libraries for the management of MSX Sprites, both for the creation of ROMs (using the BIOS), and MSX-DOS executables.
 
They are designed for the development of applications for the first generation of MSX but some of the variations are directed in how certain functions behave when using the G3 mode of the video processor V9938 and thus be able to take advantage of it, when it is executed in a MSX2 or higher.

The projected libraries are the following:
- [SPRITES Small](https://github.com/mvac7/SDCC_VDP_SPRITES_S_MSXROM_Lib) - Basic functions for managing Sprites. It is more compact so it takes up less space in our application.
- [SPRITES](https://github.com/mvac7/SDCC_VDP_SPRITES_MSXROM_Lib) - It includes the same functions as the Small version and adds to access specific parameters (positioning, color, pattern, visibility and EarlyClock).
- [SPRITES mode 1/2](https://github.com/mvac7/SDCC_VDP_SPRITES_12_MSXROM_Lib) - Same as SPRITES but in the G3 screen mode (V9938), it treats the color parameters in a simplified way. Assign the color parameters and EarlyClock to all the lines of the Sprites.
- SPRITES DUMP (In development) - Uses a buffer in RAM that has to be dumped in each interruption. Includes a Sprite Flicker, to allow viewing up to 8 sprites per line.

To optimize our development, I recommend building your library with only those functions you need.



## History of versions

* 1.0 (28/3/2021) first version



## Requirements

* Small Device C Compiler (SDCC) v3.9 http://sdcc.sourceforge.net/
* Hex2bin v2.5 http://hex2bin.sourceforge.net/
* [VDP TMS9918A MSX ROM Library](https://github.com/mvac7/SDCC_VDP_TMS9918A_MSXROM_Lib).



## Acknowledgments
  
I want to give a special thanks to all those who freely share their knowledge with the MSX developer community.

* Avelino Herrera > [WEB](http://msx.atlantes.org/index_es.html)
* Nerlaska > [Blog](http://albertodehoyonebot.blogspot.com.es)
* Marq/Lieves!Tuore > [Marq](http://www.kameli.net/marq/) [Lieves!Tuore](http://www.kameli.net/lt/)
* Fubukimaru [gitHub](https://github.com/Fubukimaru) > [Blog](http://www.gamerachan.org/fubu/)
* Andrear > [Blog](http://andrear.altervista.org/home/msxsoftware.php)
* Ramones > [MSXblog](https://www.msxblog.es/tutoriales-de-programacion-en-ensamblador-ramones/) - [MSXbanzai](http://msxbanzai.tni.nl/dev/faq.html)
* Sapphire/Z80ST > [WEB](http://z80st.auic.es/)
* Fernando García > [youTube](https://www.youtube.com/user/bitvision)
* Eric Boez > [gitHub](https://github.com/ericb59)
* MSX Assembly Page > [WEB](http://map.grauw.nl/resources/msxbios.php)
* Portar MSX Tech Doc > [WEB](http://nocash.emubase.de/portar.htm)
* MSX Resource Center > [WEB](http://www.msx.org/)
* Karoshi MSX Community (RIP 2007-2020)
* BlueMSX emulator >> [WEB](http://www.bluemsx.com/)
* OpenMSX emulator >> [WEB](http://openmsx.sourceforge.net/)
* Meisei emulator >> ?



## Functions

* void **ClearSprites**() - Initialises all sprite data.
* void **PUTSPRITE**(char plane, char x, char y, char pattern) - Displays the sprite pattern.
* void **SetSpritePattern**(char plane, char pattern) - Assign a pattern to a sprite plane.
* void **SetSpritePosition**(char plane, char x, char y) - Assigns the position coordinates of a sprite plane.
* void **SetSpriteVisible**(char plane, boolean state) - Hides or shows a sprite plane.
* void **SetEarlyClock**(char plane) - Apply the Early Clock of a sprite plane. Move 32 points to the left the X position of the sprite.
* void **UnsetEarlyClock**(char plane) - Disables the Early Clock. Restore the position of a sprite plane.


## Documentation

* V9938 MSX-VIDEO [Technical Data Book](http://map.grauw.nl/resources/video/v9938/v9938.xhtml)
* MSX2-Technical-Handbook · CHAPTER 4 - VDP AND DISPLAY SCREEN (Sections 1 to 5) [3.5 GRAPHIC 2, GRAPHIC 3 modes](https://konamiman.github.io/MSX2-Technical-Handbook/md/Chapter4a.html#35-graphic-2-graphic-3-modes)
* MSX2-Technical-Handbook · CHAPTER 4 - VDP AND DISPLAY SCREEN (Sections 1 to 5) [5.3 Sprite Mode 2](https://konamiman.github.io/MSX2-Technical-Handbook/md/Chapter4a.html#53-sprite-mode-2)
