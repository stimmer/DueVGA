#include <VGA.h>
// This is the ASCIITable example by Nicholas Zambetti and Tom Igoe, modified for Due VGA library.

void setup() {
  VGA.begin(800,600);
  VGA.println("ASCII Table ~ Character Map");  
}
  
byte thisByte = 33; 

void loop() {  
  VGA.write(thisByte);    
  VGA.print(", dec: "); 
  VGA.print(thisByte);      
  VGA.print(", hex: "); 
  VGA.print(thisByte, HEX);     
  VGA.print(", oct: "); 
  VGA.print(thisByte, OCT);     
  VGA.print(", bin: "); 
  VGA.println(thisByte, BIN);   
  thisByte++;  
  
  delay(300);
}
