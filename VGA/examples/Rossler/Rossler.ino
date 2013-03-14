// Rössler chaotic attractor
// by JLS1

#undef printf
#include <VGA.h>

  float a = 0.2;
  float b = 0.2; 
  float c = 8;

  float x,y,z;

  float xnn = 0;
  float ynn = 0;
  float znn = 0;

  float dt = 0.02;
  
  int cnt = 0;
  
  boolean vis;

void setup() {
  
  VGA.begin(800,600);
  VGA.clear();
  VGA.drawText(" R\x94ssler chaotic attractor ",25,25,0,1);
  VGA.drawText("dx/dt = -y - z",25,50,1);
  VGA.drawText("dy/dt = x + ax",25,60,1);
  VGA.drawText("dz/dt = b + z(x-c)",25,70,1);
  VGA.drawText("a = 0.2",25,90,1);
  VGA.drawText("b = 0.2",25,100,1);
  VGA.drawText("c = 8",25,110,1);
}

void loop() {
  
       xnn = -(y+z);
       ynn = x+a*y;
       znn = b+x*z-c*z;

       x = x+xnn*dt;
       y = y+ynn*dt;
       z = z+znn*dt;
  
       VGA.drawPixel(350+(16*((z/2)-x)), 200+(12*((z/2)-y)), vis);
       
       if (cnt == 1500) { 
         vis = !vis;
         cnt =0; 
       }
       
       cnt ++;
}
