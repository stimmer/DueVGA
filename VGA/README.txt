Due VGA Library
by stimmer

V0.404 - 4/4/2013

1) Quick start
==============

Place the VGA folder in your libraries directory
Start or restart the Arduino IDE


For VGA mono:
-------------
Upload File->Examples->VGA->HelloWorld

Connect pin 42 to VGA VSync (pin 14 on VGA D-connector)
Connect pin 43 to VGA HSync (pin 13 on VGA D-connector)
Connect GND to VGA GND (pins 5,6,7,8,10 on VGA D-connector)
Connect SPI MOSI pin (bottom middle pin on 6-pin SPI connector) through
a 100 ohm resistor to VGA Red, Green and Blue pins (pins 1,2 and 3 on D-connector)
Turn on monitor :-)

If the monitor has trouble, try some small resistors between 42 and VSync, and 43 
and HSync (10 to 100 ohm)


For VGA colour:
---------------
Upload File->Examples->VGA->HelloWorldColour
Connect Due to monitor as follows:

Due pin 34 -> 820R resistor -> VGA pin 3 (blue)
Due pin 35 -> 390R resistor -> VGA pin 3 (blue)

Due pin 36 -> 2k2 resistor -> VGA pin 2 (green)
Due pin 37 -> 1k resistor -> VGA pin 2 (green)
Due pin 38 -> 470R resistor -> VGA pin 2 (green)

Due pin 39 -> 2k2 resistor -> VGA pin 1 (red)
Due pin 40 -> 1k resistor -> VGA pin 1 (red)
Due pin 41 -> 470R resistor -> VGA pin 1(red)

Due pin 42 -> VGA pin 14 (VSync)
Due pin 43 -> VGA pin 13 (HSync)

Due pin GND -> VGA pins 5,6,7,8,10


For NTSC/PAL:
-------------
Upload File->Examples->VGA->DrawingTestNTSC (or DrawingTestPAL)
Connect Due to TV as follows:

Due pin 36 -> 3k3  resistor -> Video In
Due pin 37 -> 1k6  resistor -> Video In
Due pin 38 -> 820R resistor -> Video In
Due pin 39 -> 390R resistor -> Video In
Due pin 40 -> 200R resistor -> Video In
Due pin 41 -> 100R resistor -> Video In

Due pin GND -> Video GND

On some TVs this will not work. You may need a 100uF capacitor between
the resistors and Video In. 

2) Basic usage
==============

The library is very simple to use - see the examples
#include <VGA.h> at the top of your sketch,

VGA.begin(640,480); in setup for mono or
VGA.begin(320,240,VGA_COLOUR); for colour. 

Then call various drawing functions, eg:
drawPixel(x, y, col); sets a single pixel to colour col
drawLine(x0, y0, x1, y1, col); draws a line from (x0,y0) to (x1,y1)
fillCircle(x, y, r, col); circle, center (x,y) radius r

For text, use VGA.print(...) and VGA.println(...) just like Serial.print(...) and
Serial.println(...) 

3) Colours
==========

In mono mode, 1 is white, 0 is black, and -1 inverts the pixel.

In colour mode (and PAL and NTSC modes), colours are interpreted in 
binary as RRRGGGBB. So for example:

0b11111111 = 255 = white    0b11110000 = 240 = orange
0b11100000 = 224 = red      0b10010010 = 146 = grey
0b00011100 = 28 = green	    0b10000011 = 227 = purple
0b00000011 = 3 = blue       0b00000000 = 0 = black

In the draw* and fill* functions, negative colour values XOR with the current colour 
of the pixel, ie newcol=oldcol^(-col)

4) API
======

int  VGA.begin(int x, int y, int m=VGA_MONO);
  
  Initializes the display, resolution (x,y). m is VGA_MONO or VGA_COLOUR
  Returns: 0 on success, negative on failure
  Not all resolutions work on all monitors. Highest recommended resolutions are 
  800x600 in mono and 320x240 in colour. 
    

int  VGA.beginNTSC();

  Initializes the display in NTSC mode, 320x200 pixels.
  Returns: 0 on success, negative on failure

  
int  VGA.beginPAL();

  Initializes the display in PAL mode, 320x240 pixels.
  Returns: 0 on success, negative on failure

  
void VGA.end();
  
  Stops the display, and frees memory.

  
void VGA.clear();
  
  Clears the screen to black.


void VGA.drawPixel(int x, int y, int col);
  
  Draws pixel at (x,y) in colour col

  
void VGA.drawLine(int x0, int y0, int x1, int y1, int col);
  
  Draws line from (x0,y0) to (x1,y1) in colour col


void VGA.drawTri(int x0,int y0,int x1,int y1,int x2,int y2,int col);
void VGA.fillTri(int x0,int y0,int x1,int y1,int x2,int y2,int col);

  Draws or fills triangle (x0,y0),(x1,y1),(x2,y2) in colour col
  

void VGA.drawRect(int x0, int y0, int x1, int y1, int col);
void VGA.fillRect(int x0, int y0, int x1, int y1, int col);

  Draws or fills rectangle with corners (x0,y0),(x1,y1) in colour col
  

void VGA.drawCircle(int x, int y, int r, int col);
void VGA.fillCircle(int x, int y, int r, int col);

  Draws or fills circle center (x,y) radius r in colour col
  

void VGA.drawEllipse(int x0, int y0, int x1, int y1, int col);
void VGA.fillEllipse(int x0, int y0, int x1, int y1, int col);

  Draws or fills ellipse bounded by rectangle (x0,y0),(x1,y1) in colour col


void VGA.drawText(char *text, int x, int y, int fgcol, int bgcol= -256, int dir=0);

  Draws text at any pixel position. 
  (x,y) is the top-left corner of the text before rotation.
  fgcol is the colour of the text. bgcol is the colour of the text background
  or -256 for a transparent background.
  dir is the direction of the text. 0 is normal, 1 is up, 2 is upside-down and 3
  is down. -1 draws the text going downwards but with the individual characters 
  oriented normally.

void VGA.scroll(int x, int y, int w, int h, int dx, int dy,int col=0);

  Scrolls an area of the screen, top left corner (x,y), width w, height h
  by (dx,dy) pixels. If dx>0 scrolling is right, dx<0 is left. dy>0 is down,
  dy<0 is up.
  
  
void VGA.moveCursor(int column, int line);

  Moves the print position to (column, line)  
  
  
void VGA.setPrintWindow(int left, int top, int width, int height);

  Restricts the printing window to an area of width x height characters, at a 
  position left characters in and top characters down.

  
void VGA.unsetPrintWindow(){tww=tw;twh=th;twx=twy=0;}

  Restores the print window to be the whole screen.
  
  
void VGA.clearPrintWindow();

  Clears the print window to the current text paper colour.
  
  
void VGA.scrollPrintWindow();

  Scrolls the print window up one line and moves the print position to the bottom.
  
  
void VGA.setInk(int i){ink=i;}

  Sets the text colour to i.
  
  
void VGA.setPaper(int p){paper=p;}

  Sets the text background colour to p
  
  
virtual size_t VGA.write(const uint8_t *buffer, size_t size);
virtual size_t VGA.write(uint8_t c);

  These functions are similar to Serial.write, except output gets printed to
  the screen. These functions enable print and println to work correctly.
  

void VGA.waitBeam()
void VGA.waitSync()

  These functions wait for the beam position to be off-screen. waitBeam will return 
  immediately if the beam is already off-screen, waitSync will always wait for the 
  next frame. These can be used to reduce flicker.


void VGA.setMonitorFreqRange(int hmin, int hmax, int vmin, int vmax)
  
  This function can be tried if your monitor has trouble syncing to some modes.
  All frequencies are in Hz, eg setMonitorFreqRange(27000, 82000, 58, 65);
  Call this before VGA.begin(...)
  

void VGA.setSyncPolarity(bool h, bool v)
  
  Swaps the polarity of the sync signals. Most monitors don't need this.
  Call this before VGA.begin(...)

  
void VGA.putPPixelFast(int x, int y, int c)
int VGA.getPPixelFast(int x, int y)
void VGA.putCPixelFast(int x, int y, int c)
int VGA.getCPixelFast(int x, int y)
  
  Fast inline pixel put/get routines. The P functions are for mono modes and the
  C functions are for colour modes.
  
5) Low Level
============

In mono modes, the screen is arranged as an array of 16-bit words. Each line has enough words for the pixels (eg 40 words in 640x480 mode) plus 2 spare words which are used for blanking and must always be 0.

  uint16_t *VGA.pb; // Pixel buffer memory address
  int VGA.pw;	// Count of words from one line to the next (aka stride or pitch)
  int VGA.pbsize; // Total size of pixel buffer (note these sizes are 16-bit words)
  
The SAM3X also supports a bit-banding alias region in memory, where each bit is mapped to a 32-bit address.

  uint32_t *VGA.pbb; // Pixel buffer bit-banding alias address (read datasheet p75)
  int VGA.pbw;	 // Pixel buffer bit-banding stride (in 32-bit words)
  
To help understand usage of these, look at the following functions:
  void putPPixelFast(int x, int y, int c){pbb[y*VGA.pbw+(x^15)]=c;}
  int getPPixelFast(int x, int y){return pbb[y*VGA.pbw+(x^15)];}
  
In colour modes there are no spare words needed so the colour buffer is just an
array of uint8_t[width*height].

  uint8_t *VGA.cb;   // Colour buffer memory address
  int VGA.cw;	 // Colour buffer stride, in bytes
  int VGA.cbsize;	 // Size of colour buffer in bytes

6) How it works
===============

Don't ask ;-)

Basically it uses nested interrupts. Each line the PWM hardware generates the 
horizontal sync signal then interrupts the processor. Unfortunately it interrupts
at a very slightly different time each line, which would cause a shimmering
effect. So the PWM interrupt puts the processor to sleep, waiting to be interrupted 
again by a timer interrupt at higher priority. Since the processor wasn't doing
anything, this interrupt is precisely synchronized to the timer and the display
is rock solid :-)

The mono modes use DMA and SPI hardware (42MHz for 800x600!) and are therefore
quite fast. The SPI DMA uses another interrupt to signal the end of a line. 
Unfortunately at the end of transmission the SPI output goes HIGH, which would
cause a white line. That's why there are spare blanking words at the end of the
line - when the interrupt fires the DMA is out of data but the blanking words are 
still being output, so the interrupt routine quickly sets the pin back to PIO mode, 
putting it low, and avoiding the white line.

The colour mode uses DMA to the SMC - static memory controller - to output 8 bits
of data at a time to pins 34-41. No end-of-line interrupt is needed here because
the output goes high impedance when data is not being written. The pixel width
is controlled by changing the memory timing registers in the SMC.

The NTSC and PAL modes use a pair of DMA buffers. One is outputting the signal for
the line whilst the interrupt routine is filling the other buffer for the next line.
The colour burst and chroma phase tables have been precalculated (which is why these
modes use so much flash).

Have fun!
