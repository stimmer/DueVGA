// Mandelbrot fractal
// the very first demo written for Due VGA out.

#include <VGA.h>
#include <complex>
using namespace std;

const byte cmap[]={0b00000000,0b11100000,0b11100100,0b11101000,0b11101100,0b11110000,0b11110100,0b11111000,0b11111100,
                   0b11011100,0b10111100,0b10011100,0b01111100,0b01011100,0b00111100,0b00011100,0b00011101,0b00011110,
                   0b00011111,0b00011011,0b00010111,0b00010011,0b00001111,0b00001011,0b00000111,0b00000011,0b00100011,
                   0b01000011,0b01100011,0b10000011,0b10100011,0b11000011,0b11100011,0b11100010,0b11100001,0b11100000,0b00000000};

void setup() {
  VGA.begin(320,240,VGA_COLOUR);
}

void loop(){
  for(int i=0;i<320;i++){
    for(int j=0;j<240;j++){     
      complex<float> z(0,0),c((i+180.0)/1280.0,(j+640.0)/1280.0);
      int n;
      for(n=1;n<sizeof(cmap);n++){
        z=z*z+c;
        if(norm(z)>4.0)break;
      }
      VGA.drawPixel(i,j,cmap[sizeof(cmap)-n]);
    }
  }
  for(;;);
}

