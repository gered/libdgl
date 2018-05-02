# libDGL

libDGL is a "retro coding" C library aimed at MS-DOS 
[VGA Mode 13h](https://en.wikipedia.org/wiki/Mode_13h) game development. It targets 486 DX2 systems as a baseline for performance.

The project is developed with Watcom C 10.0b and Borland Turbo Assembler 4.0.

### Why?

Because it's fun.

If you were looking for something for "retro" MS-DOS game development that is fully featured, 
really performant and bug-tested that's ready right now, have a look at Allegro 3.x or possibly 
4.x depending on what hardware you're targetting.

### Status

This library is currently under early development and is lacking many things and probably 
chock-full of bugs.

**Done / Mostly Done**

* Primitive drawing support
* Text rendering
* Blitting
* Graphics clipping
* Keyboard input
* Mouse input
* PCX loading/saving
* Math library (general helpers, 2D vector, 3x3 matrix)
* Fixed point math
* Basic error handling / results

**TODO**

* Scaled/rotated blitting support
* Blending
* "Mode 7" like support
* Custom font loading (BIOS-like format?)
* Joystick / Gravis GamePad support
* Input device (keyboard/mouse/joystick) events
* PC speaker sounds
* Sound Blaster compatible sound/music
* Gravis Ultrasound compatible sound/music
* Sine/cosine lookup table optimizations
* BMP, LBM, GIF image loading (and saving?)
* Simple immediate mode GUI

### Building

You can use `WMAKE` to build using the included `MAKEFILE`.

To build the library:

```
> wmake target_config=debug build           ; debug build

> wmake target_config=release build         ; release build
```

Will output `DGL.LIB` in the project directory.

To build and run the (incomplete) test suite, look under the "TEST" directory. There you will
find a separate `MAKEFILE` which assumes that you are running `wmake` from that directory and
that `DGL.LIB` has already been built.

```
> cd test
test> wmake run                                 ; run test suite
test> wmake debug                               ; run through WD debugger
```

