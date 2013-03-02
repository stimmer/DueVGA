// Ripple
// 3d graph of sinc function
// demonstrates low-level access to colour buffer

#include <VGA.h>

void setup() {
  VGA.begin(320,240,VGA_COLOUR);
}

void loop(){
  
  for(int x=0;x<320;x++){
    int my=240;
    for(float Z=-15.02;Z<15.0;Z+=0.01){
      float X=(x-159.98)/16.0;
      float R=sqrt(X*X+Z*Z);
      float Y=150.0*(1.0+sin(R)/R); // sinc(x) == sin(x)/x
      int y=320.0-(Z*4.0)-Y;
      if(y<my && y>=0){
        uint16_t c=Y;
        while(c>255)c-=255;
        VGA.putCPixelFast(x,y,c); // built in low-level function
        my=y;
      }
    }
  }
  for(;;)for(int y=0;y<240;y++)for(int x=0;x<320;x++){
    if(VGA.cb[y*VGA.cw+x]){ // direct low-level access
      ++VGA.cb[y*VGA.cw+x]; // for fast colour cycling
      if(!VGA.cb[y*VGA.cw+x])++VGA.cb[y*VGA.cw+x];
    } 
  }
}
