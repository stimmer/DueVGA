#include <VGA.h>

// DrawingTest
// test of all drawing functions

void setup() {
  VGA.beginPAL();
}

double a=0;

void loop() {
  int x0=random(320),y0=random(240),x1=random(320),y1=random(240),x2=random(320),y2=random(240);
  VGA.fillTri(x0,y0,x1,y1,y2,y2,random(512)-256);
  VGA.drawTri(x0,y0,x1,y1,y2,y2,random(512)-256);

  x0=random(320);y0=random(240);x1=random(320);y1=random(240);
  VGA.fillRect(x0,y0,x1,y1,random(512)-256);
  VGA.drawRect(x0,y0,x1,y1,random(512)-256);
  
  x0=random(320);y0=random(240);x1=random(320);y1=random(240);
  VGA.fillEllipse(x0,y0,x1,y1,random(512)-256);
  VGA.drawEllipse(x0,y0,x1,y1,random(512)-256);
    
  x0=random(320);y0=random(240);int r=random(70);
  VGA.fillCircle(x0,y0,r,random(512)-256);
  VGA.drawCircle(x0,y0,r,random(512)-256);
  
  x0=random(320);y0=random(240);r=random(5)-1;
  VGA.drawText("Hello Arduino!",x0,y0,random(256),random(256),r);
  
  x0=random(320);y0=random(240);r=random(5)-1;
  VGA.drawText("Due VGA Library PAL",x0,y0,random(256),random(256),r);
  
  VGA.scroll(0,0,320,240,8*sin(a),8*cos(a));
  a+=0.05;
}
