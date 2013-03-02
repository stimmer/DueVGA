#include <VGA.h>

void setup() {
  VGA.begin(800,600);
}

void loop() {
  for(int t=1;;t++){
    int j=ctz(t)-113;
    for(int i=j;i<800-j;i++)VGA.drawLine(i,j-100,799-i,699-j,-1);
    for(int i=799-j;i>=j;i--)VGA.drawLine(j,i-100,799-j,699-i,-1);
  }
}
