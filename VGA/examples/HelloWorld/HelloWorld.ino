// Hello World using VGA library by stimmer

// include VGA library
#include <VGA.h>

void setup() {
// start 640x480 mode
  VGA.begin(640,480);
}

int i=0;
void loop() {
  i=1-i;

// change text colour
  VGA.setInk(i);
  
// print message
  VGA.print(" Hello world! ");
  
// this line reduces flicker
  VGA.waitSync(); 
}
