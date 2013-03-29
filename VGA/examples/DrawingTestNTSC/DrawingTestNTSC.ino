#include <VGA.h>

// DrawingTestNTSC
// VGA.beginNTSC starts a 320x200 NTSC colour mode
// Output via an R-2R ladder on pins 34-41 (34=LSB 41=MSB)

void setup() {
  VGA.beginNTSC();
  Serial.begin(9600);
}

double a=0;

void loop() {
  Serial.print(VGA.synced);
  int x0=random(320),y0=random(200),x1=random(320),y1=random(200),x2=random(320),y2=random(200);
  VGA.fillTri(x0,y0,x1,y1,y2,y2,random(512)-256);
  VGA.drawTri(x0,y0,x1,y1,y2,y2,random(512)-256);

  x0=random(320);y0=random(200);x1=random(320);y1=random(200);
  VGA.fillRect(x0,y0,x1,y1,random(512)-256);
  VGA.drawRect(x0,y0,x1,y1,random(512)-256);
  
  x0=random(320);y0=random(200);x1=random(320);y1=random(200);
  VGA.fillEllipse(x0,y0,x1,y1,random(512)-256);
  VGA.drawEllipse(x0,y0,x1,y1,random(512)-256);
    
  x0=random(320);y0=random(200);int r=random(70);
  VGA.fillCircle(x0,y0,r,random(512)-256);
  VGA.drawCircle(x0,y0,r,random(512)-256);
  
  x0=random(320);y0=random(200);r=random(5)-1;
  VGA.drawText("Hello Arduino!",x0,y0,random(256),random(256),r);
  
  x0=random(320);y0=random(200);r=random(5)-1;
  VGA.drawText("Due VGA Library NTSC",x0,y0,random(256),random(256),r);
  
  VGA.scroll(0,0,320,200,8*sin(a),8*cos(a));
  a+=0.05;
}
