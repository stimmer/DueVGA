#include <VGA.h>

void setup() {
  VGA.begin(320,240,VGA_COLOUR);
  
  pinMode(13,OUTPUT);
}

void loop() {
  for(int t=1;;t++){
    int j=ctz(t)-10;
    for(int i=j;i<320-j;i++)VGA.drawLine(i,j-40,320-i,280-j,-(t*19));
    for(int i=320-j;i>=j;i--)VGA.drawLine(j,i-40,320-j,280-i,-(t*29));
    
    digitalWrite(13,t&1);
  }
}
