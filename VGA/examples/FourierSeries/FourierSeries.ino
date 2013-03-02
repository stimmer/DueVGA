#undef printf
#include <VGA.h>

void setup() {
  VGA.begin(800,600);
}

void scale(){
  char buf[20];
  VGA.clear();
  VGA.drawText("Fourier series",260,25,0,1); 
  VGA.drawLine(100,300,799,300,1);
  VGA.drawText("Time",400,340,1);
  for(int n=0;n<=100;n++){
    int l=4;
    if((n%5)==0)l+=4;
    if((n%10)==0){
      l+=4;
      sprintf(buf,"%d",n/10);
      VGA.drawText(buf,96+n*7,315,1);
    }      
    VGA.drawLine(100+n*7,300,100+n*7,300+l,1);
  }
  
  VGA.drawLine(100,0,100,599,1);
  VGA.drawText("Amplitude",45,330,1,0,1);
    for(int n=0;n<=40;n++){
    int l=4;
    if((n%5)==0){
      l+=4;
      if((n%10)==0) l+=4;
      sprintf(buf,"%0.1f",(-n+20.0)/10.0);
      VGA.drawText(buf,60,n*15-4,1);
    }      
    VGA.drawLine(100,n*15,100-l,n*15,1);
  }
}

void loop() {
  scale();  
  VGA.drawText("Sawtooth",300,40,0,1); 
  for(int h=1;h<19;h++){
    double ox=0,oy=0,x,y;
    for(int xi=100;xi<800;xi++){
      x=(xi-100.0)/70.0 - (h-1)/3.0;
      y=0;
      for(int i=1;i<=h;i++)y+=sin(x*i)/i;
      VGA.drawLine(xi-1,300-oy*150,xi,300-y*150,1);
      ox=x;oy=y;
    }
  }  
  delay(3000);
  
  scale(); 
  VGA.drawText("Square",300,40,0,1); 
  for(int h=1;h<38;h+=2){
    double ox=0,oy=0,x,y;
    for(int xi=100;xi<800;xi++){
      x=(xi-100.0)/70.0 - (h-1)/6.0;
      y=0;
      for(int i=1;i<=h;i+=2)y+=sin(x*i)/i;
      VGA.drawLine(xi-1,300-oy*150,xi,300-y*150,1);
      ox=x;oy=y;
    }
  }  
  delay(3000);
  
  scale();  
  VGA.drawText("Triangle",300,40,0,1);   
  for(int h=1;h<38;h+=2){    
    double ox=0,oy=0,x,y;
    for(int xi=100;xi<800;xi++){
      double m=1;
      x=(xi-100.0)/70.0 - (h-1)/6.0;
      y=0;
      for(int i=1;i<=h;i+=2){y+=m*sin(x*i)/(i*i);m*=-1;}
      VGA.drawLine(xi-1,300-oy*150,xi,300-y*150,1);
      ox=x;oy=y;
    }
  }  
  delay(3000);
}
