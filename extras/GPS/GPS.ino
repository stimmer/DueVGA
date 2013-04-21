/* Due VGA library - GPS example by stimmer
   Decodes GPGGA and GPGSV messages and displays map and satellite positions  
   Adjust GPSPORT and GPSBAUD defines to match your GPS
  
   If you do not have a GPS, change GPSPORT to Serial and use the serial monitor to enter messages
   Try copy/paste these: (from http://en.wikipedia.org/wiki/NMEA_0183) (use CR/LF)
   
$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76
$GPGSV,3,1,11,10,63,137,17,07,61,098,15,05,59,290,20,08,54,157,30*70
$GPGSV,3,2,11,02,39,223,19,13,28,070,17,26,23,252,,04,14,186,14*79
$GPGSV,3,3,11,29,09,301,24,16,09,020,,36,,,*76

(You need to enter all 3 GPGSV lines before the satellites show. You may need to enter the 
lines a few times before it works. A real GPS reports several times per second so this is
not a problem)

*/

#define GPSPORT Serial1
#define GPSBAUD 115200

#include <string.h>
#include <stdio.h>
#include <VGA.h>
#include <math.h>

void setup() {

  GPSPORT.begin(GPSBAUD);
  VGA.begin(800,600);
  drawEarth(0,0,0,0);
  
  VGA.setPrintWindow(73,0,27,75);
}

char buf[200];
int p;  
  
void loop() {
  while(GPSPORT.available()){
    int r=GPSPORT.read();
    if ((r==10) || (r==13)){
      buf[p]=0;      
      
      if(p>0) processLine();
      
      p=0;
    }
    else if(r>32)buf[p++]=r;
  } 
}

int hh,mm,ss,mss;
double lati,longi,clt,clg;

// print 2 digit number with leading zero if needed
void printz(int i){
  if(i<10)VGA.print(0);
  VGA.print(i);
}

int sat[32][4],nsat[32][4];
int sats,satns,satnr,sati,satch;
void processLine(){
   if(strncmp(buf,"$GPGGA",6)==0){
  //Serial.println(buf);
     char *p=strtok(buf+7,",");
     sscanf(p,"%02d%02d%02d.%03d",&hh,&mm,&ss,&mss);
     VGA.moveCursor(11,5);
     VGA.println("Time UTC");
     VGA.moveCursor(11,6);
     printz(hh);VGA.print(":");
     printz(mm);VGA.print(":");
     printz(ss);VGA.print("   ");

     p=strtok(0,",");
     lati=atof(p)/100.0;
     p=strtok(0,",");
     if(p[0]=='S')lati= -lati;

     p=strtok(0,",");
     longi=atof(p)/100.0;
     p=strtok(0,",");
     if(p[0]=='W')longi= -longi;
     
     VGA.moveCursor(8,9);
     VGA.println("GPS position");
     VGA.moveCursor(8,10);
     VGA.print(abs(lati),2);VGA.print(lati>=0 ? "N ":"S ");
     VGA.print(abs(longi),2);VGA.print(longi>=0 ? "E  ":"W  ");
     VGA.drawRect(644,36,763,91,1);

     if(abs(clt-lati)>0.1 || abs(clg-longi)>0.1){
       clt=lati;clg=longi;
       drawEarth(clt,clg,0,0);
     }

   } 
   if(strncmp(buf,"$GPGSV",6)==0){
      //Serial.println(buf);
      char *p=buf+7;
      int nr=atoi(p);
      p=strchr(p,',')+1;
      int rn=atoi(p);
      p=strchr(p,',')+1;
      int ns=atoi(p);
      //Serial.println(sats);Serial.println(rn);
      
      if((sats==-1) || (sats==0)){
        if(rn==1){
          sats=1;satnr=nr;satns=ns;sati=0;
        }
        else {sats=0;return;}
      }
      else if(sats>0){
        if(rn==sats+1)sats++;
        else {sats=0;return;}
      }
      
      int t;
      for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
          p=strchr(p,',')+1;        
          nsat[sati][j]=(*p!=',')?atoi(p):-1000;
        }
        
        sati++;
        if(sati == satns){
          for(int j=sati;j<32;j++)nsat[j][0]=-999;
          int sc=0;for(int j=0;j<satns;j++)if(nsat[j][3]>0)sc++;
          if(memcmp(sat,nsat,32*4*4)){
            drawSatellites();
            memcpy(sat,nsat,32*4*4);
            VGA.moveCursor(5,50);
            VGA.print(sc);VGA.print("/");
            VGA.print(satns);VGA.print(" satellites  ");
            VGA.drawRect(612,396,763,412,1);
            VGA.moveCursor(0,52);
            VGA.print("num\tele\tazi\tdB");
            for(int j=0;j<20;j++){
              VGA.moveCursor(0,54+j);
              if(j<satns){
                for(int k=0;k<4;k++){
                  if(sat[j][k]!= -1000)VGA.print(sat[j][k]);if(k<3)VGA.print("\t");}
              }
              else VGA.print("\t\t\t  ");
            }
            VGA.drawRect(580,412,796,596,1);
          }
          sats = -1;
          break;
        }
      }
   }
}


void drawSatellites(){
  for(int i=0;i<=31;i++)if(memcmp(sat[i],nsat[i],12) || ((sat[i][3]>0) != (nsat[i][3]>0)))
      {
        clearSatellite(sat[i][0],sat[i][1],sat[i][2],sat[i][3]); 
        drawSatellite(nsat[i][0],nsat[i][1],nsat[i][2],nsat[i][3]);
      }
  VGA.drawCircle(300,300,297,1);
  for(int i=0;i<=31;i++)if(memcmp(sat[i],nsat[i],12) || ((sat[i][3]>0) != (nsat[i][3]>0)))
      drawSatellite(nsat[i][0],nsat[i][1],nsat[i][2],nsat[i][3]);
 
}

void drawSatellite(int prn, int ele, int azi, int db){
    if((prn == -999) || (ele == -1000) || (azi == -1000))return;
    double r=296.5*cos(ele/180.0*M_PI);
    int x=300.0+r*sin(azi/180.0*M_PI),y=300.0-r*cos(azi/180.0*M_PI);
    if(db>0){VGA.fillCircle(x,y,9,1);VGA.drawCircle(x,y,10,0);}
    else {VGA.fillCircle(x,y,9,0);VGA.drawCircle(x,y,9,1);}
    char s[4];
    sprintf(s,"%d",prn);
    VGA.drawText(s,x-3.5*strlen(s),y-4,db<=0);    
}
  
void clearSatellite(int prn, int ele, int azi, int db){
    if((prn == -999) || (ele == -1000) || (azi == -1000))return;
    double r=296.5*cos(ele/180.0*M_PI);
    int x=300.0+r*sin(azi/180.0*M_PI),y=300.0-r*cos(azi/180.0*M_PI);
    VGA.fillCircle(x,y,9,0);
    if((x>220) && (x<380) && (y>220) && (y<380))drawEarth(clt,clg,x,y);
}

