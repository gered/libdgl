# libDGL

libDGL is a "retro coding" C library aimed at MS-DOS [VGA Mode 13h](https://en.wikipedia.org/wiki/Mode_13h) game development. It is developed with DJGPP with 486 DX2 systems as a baseline target for performance.

### Why?

Because it's fun.

If you were looking for something for "retro" MS-DOS game development that is fully featured, really performant and bug-tested that's ready right now, have a look at Allegro 3.x or possibly 4.x depending on what hardware you're targetting.

### Status

This library is currently under early development and is lacking many things and probably chock-full of bugs.

**Done / Mostly Done**

* Primitive drawing support
* Text rendering
* Blitting
* Graphics clipping
* Keyboard input
* Mouse input
* PCX loading/saving
* Math library (general helpers, 2D vector, 3x3 matrix)
* Basic error handling / results

**TODO**

* Scaled/rotated blitting support
* Blending
* "Mode 7" like support
* Custom font loading (BIOS-like format?)
* Joystick / Gravis GamePad support
* PC speaker sounds
* Sound Blaster compatible sound/music
* Gravis Ultrasound compatible sound/music
* Fixed point math
* Sine/cosine lookup table optimizations
* BMP, LBM, GIF image loading (and saving?)

### DJGPP Toolchain

Along the same theme as this library itself, I am using an older version of DJGPP from the late 90's. I have not tested with any other versions of the DJGPP toolchain other than:

```
15 Jan 1998   bnu281b.zip
24 Oct 1997   csdpmi4b.zip
31 Oct 1996   djdev201.zip
18 Jan 1997   faq210b.zip
 6 Jun 1998   gcc281b.zip
18 Oct 1996   gdb416b.zip
 6 Jun 1998   gpp281b.zip
 1 Mar 1998   mak3761b.zip
30 Sep 1997   rhide14b.zip
```

This is a pretty arbitrary selection that was based on a cut-off of about 1998 (which was when I first came across DJGPP when I was younger).

