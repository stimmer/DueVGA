// DueVGA library - Anim demo by stimmer
// Requires the SdFat library SdFatBeta20130207
// NOTE you must alter SdFat to not use DMA 
//   In file Sd2Card.cpp on line 85:
//   Change "#define USE_SAM3X_DMAC 1" to "#define USE_SAM3X_DMAC 0"
// Copy the file anim.dat to your SD card first!

#include <VGA.h>
#include <SdFat.h>

const int chipSelect = SS; // you may need to change this to the pin you connected SS to

SdFat sd;
SdFile file;

void setup() {
  VGA.begin(320,240,VGA_COLOUR);
  sd.begin(chipSelect, SPI_FULL_SPEED);
  file.open("anim.dat", O_READ);
}

void loop() {
  file.rewind();
  while(file.available()){
    file.read(VGA.cb,320*240);
  }
}
