// Newton fractal

#include <VGA.h>
#include <complex>
using namespace std;

const byte cmap[]={0b00000000,0b11100000,0b11100100,0b11101000,0b11101100,0b11110000,0b11110100,0b11111000,0b11111100,
                   0b11011100,0b10111100,0b10011100,0b01111100,0b01011100,0b00111100,0b00011100,0b00011101,0b00011110,
                   0b00011111,0b00011011,0b00010111,0b00010011,0b00001111,0b00001011,0b00000111,0b00000011,0b00100011,
                   0b01000011,0b01100011,0b10000011,0b10100011,0b11000011,0b11100011,0b11100010,0b11100001,0b11100000,0b00000000};

void setup() {
  VGA.beginPAL();
}

void loop(){
  for(int i=0;i<320;i++){
   VGA.setPaper(0b1010011);
   VGA.moveCursor(0,0);
   VGA.println("DueVGA Library");
   VGA.println("Newton Fractal");
   VGA.println("x^3 - 1 = 0");
    for(int j=0;j<240;j++){     
      complex<float> c(0,0),z((i-160.0)/20.0,(j-120.0)/15.0),zz,zzz;
      complex<float> one(1.0,0.0),two(2.0,0.0),three(3.0,0.0);
      int n;
      for(n=1;n<sizeof(cmap);n++){        
        zz=z*z; zzz=zz*z;
        c=(zzz-one)/(zz*three);
        z-=c;        
        if(norm(c)<0.000001)break;
      }
      VGA.drawPixel(i,j,cmap[sizeof(cmap)-n]);
    }
  }

  for(;;);
}

