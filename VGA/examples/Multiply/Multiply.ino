#include <VGA.h>

const int xres=400,yres=300;

void setup() {
  VGA.begin(xres,yres);
}

void loop() {
  for(int m=1;;m+=2)
  for(int b=1;b<17;b++){
    int t;
    for(int y=0;y<yres;y++){
      t=m*m*y;
      for(int x=0;x<xres;x++){
        int p=((t*x)&(1<<b)) ? 1:0;
        VGA.putPPixelFast(x,y,p);
      }
    }
    delay(500);    
    VGA.waitSync();
  } 
}
