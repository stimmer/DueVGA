// Hello World Colour using VGA library by stimmer

// include VGA library
#include <VGA.h>

void setup() {
  // start 320x240 mode
  VGA.begin(320,240,VGA_COLOUR);
}

void loop() {  
  // text colour
  VGA.setInk(random(256));
 
  // text background colour
  VGA.setPaper(random(256)); 
  
  // print message
  VGA.print(" Hello Arduino ");
  
  VGA.waitSync();
}
