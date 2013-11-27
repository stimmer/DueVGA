// Arduino Due VGA Library by stimmer
// v0.404 (4/4/2013)
//
//
// Added NTSC and PAL colour composite modes
// Fixed some timing problems
//
// Known bugs:
// Sync timing code is all wrong
// Drawing functions could be faster
// Doesn't work well with Native USB
// White line / missing first pixel in mono modes
//
// TODO:
// Bitmap / pixmap graphics
// Larger fonts
// printf

/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef _VGA_H
#define _VGA_H 

#include "Arduino.h"
#include "Print.h"

extern unsigned char _vga_font8x8 [];

const int _v_vsync=42;
const int _v_hsync=43;


inline void _v_digitalWriteDirect(int pin, boolean val){
  if(val) g_APinDescription[pin].pPort -> PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort -> PIO_CODR = g_APinDescription[pin].ulPin;
}

#define VGA_MONO 1
#define VGA_COLOUR 2
#define VGA_COLOR VGA_COLOUR 
#define VGA_NTSC 18
#define VGA_PAL 34

class Vga : public Print {
    
public:
  int  begin(int x, int y, int m=VGA_MONO);
  int  beginPAL();
  int  beginNTSC();  
  void end();
  
  void clear(int c=0);
  void drawPixel(int x, int y, int col);
  void drawLine(int x0, int y0, int x1, int y1, int col);
  void drawLinex(int x0, int y0, int x1, int y1, int col);
  void drawHLine(int y,int x0, int x1, int col);
  void drawTri(int x0,int y0,int x1,int y1,int x2,int y2,int col);
  void fillTri(int x0,int y0,int x1,int y1,int x2,int y2,int col);
  void drawRect(int x0, int y0, int x1, int y1, int col);
  void fillRect(int x0, int y0, int x1, int y1, int col);
  void drawCircle(int x, int y, int r, int col);
  void fillCircle(int x, int y, int r, int col);
  void drawEllipse(int x0, int y0, int x1, int y1, int col);
  void fillEllipse(int x0, int y0, int x1, int y1, int col);
  void drawText(char *text, int x, int y, int fgcol, int bgcol= -256, int dir=0);  
  void scroll(int x, int y, int w, int h, int dx, int dy,int col=0);
  
  void moveCursor(int column, int line);
  void setPrintWindow(int left, int top, int width, int height);
  void unsetPrintWindow(){tww=tw;twh=th;twx=twy=0;}
  void clearPrintWindow();
  void scrollPrintWindow();
  void setInk(int i){ink=i;}
  void setPaper(int p){paper=p;}
  virtual size_t write(const uint8_t *buffer, size_t size);
  virtual size_t write(uint8_t c);
  
  void waitBeam(){while((*(volatile int *)&line) < ysize);}
  void waitSync(){while((*(volatile int *)&line) >= ysize);
		  while((*(volatile int *)&line) < ysize);}
  
  int up; // whether we are running or not
  
  // modeline 
  int pclock; // must divide 84000000
  int xsize,xsyncstart,xsyncend,xtotal;
  int ysize,ysyncstart,ysyncend,ytotal;
  bool vsyncpol, hsyncpol;
  
  //PAL
  const uint16_t *cbt[2];
  const uint16_t *crt[2];
  uint16_t *dmabuf;
  int phase,poff;
  
  // various display parameters
  int mode,line,linedouble,synced,framecount; 
  int xclocks,xstart,xsyncwidth,xscale,yscale,lfreq,ffreq,ltot,debug;
  
  int lfreqmin,lfreqmax,ffreqmin,ffreqmax;
  void setMonitorFreqRange(int hmin, int hmax, int vmin, int vmax){
    lfreqmin=hmin;lfreqmax=hmax;ffreqmin=vmin;ffreqmax=vmax;}
  void setSyncPolarity(bool h, bool v){hsyncpol=h;vsyncpol=v;}
  
  uint16_t *pb; // Pixel buffer memory address
  int pw;	// Count of words from one line to the next (aka stride or pitch)
  int pbsize;   // Total size of pixel buffer (note these sizes are 16-bit words)

  uint32_t *pbb; // Pixel buffer bit-banding alias address (read the datasheet p75)
  int pbw;	 // Pixel buffer bit-banding stride (in 32-bit words)
  // To help understand usage of these, look at the following functions:
  void putPPixelFast(int x, int y, int c){pbb[y*pbw+(x^15)]=c;}
  int getPPixelFast(int x, int y){return pbb[y*pbw+(x^15)];}
  
  uint8_t *cb;   // Colour buffer memory address
  int cw;	 // Colour buffer stride, in bytes
  int cbsize;	 // Size of colour buffer in bytes
  void putCPixelFast(int x, int y, int c){cb[y*cw+x]=c;}
  int getCPixelFast(int x, int y){return cb[y*cw+x];}
  
  int tx,ty;	// Text cursor
  int tw,th;	// Text width / height
  int twx,twy,tww,twh; // Text window
  int ink,paper;// Text colours
  
private:
  int calcmodeline();
  int allocvideomem();
  void freevideomem();
  void startinterrupts();
  void stopinterrupts();
  void starttimers();
  void stoptimers();
  void startmono();
  void stopmono();
  void startcolour();
  void stopcolour();
  void dmapri();
  
};

extern Vga VGA;


#endif
