#include "VGA.h"

Vga VGA;

__attribute__ ((section(".text"))) 
const uint16_t colourfunc[3072] __attribute__((aligned(256))) = {0x4770};

void __attribute__((aligned(256))) TC0_Handler()
{
  static int disp=0;
    long dummy=REG_TC0_SR0; 

    int c=REG_PWM_CCNT2;
    if(!VGA.synced)
      if((c<=VGA.xstart+1)&&(c>=VGA.xstart)){
	REG_PWM_CPRDUPD2=VGA.xclocks;
	VGA.synced=1;
      }
                   
    if(disp==VGA_MONO){
      REG_DMAC_CTRLA4=0x12030000 + (VGA.pw>>1);  
      REG_DMAC_CHER=1<<4;
      asm volatile(".rept 14\n\t nop\n\t .endr\n\t");
      REG_PIOA_PDR=1<<26; 
    }    
    
    if(disp==VGA_COLOUR){
        asm volatile (
	"movw r1,#0x1238 \n\t"
        "movt r1,#0x400e \n\t"
        "mov r3,%[bytes] \n\t"

	"orr %[fp],#1 \n\t"
	"push {lr} \n\t"
	"blx %[fp] \n\t"
        "mov r0,#0 \n\t"
        "str r0,[r1] \n\t"
	"mov %[bytes],r3 \n\t"
	"pop {lr} \n\t"
	:[bytes]"+r"(VGA.lptr)	
        :[fp]"r"(colourfunc)
        :"r0","r1","r2","r3"
        );  
      
    }
    
    if(VGA.line==VGA.ysyncstart) _v_digitalWriteDirect(_v_vsync, VGA.vsyncpol); 
    if(VGA.line==VGA.ysyncend) _v_digitalWriteDirect(_v_vsync,!VGA.vsyncpol); 
    VGA.linedouble++; 
    if(VGA.linedouble == VGA.yscale){VGA.linedouble=0;VGA.line++;}
    if(VGA.line == VGA.ysize)disp=0;
    if(VGA.line == VGA.ytotal){
      if(VGA.mode == VGA_MONO)REG_DMAC_SADDR4=(uint32_t)VGA.pb;
      else VGA.lptr=VGA.cb;
      VGA.line=0;disp=VGA.mode;VGA.framecount++;
    }
}

void PWM_Handler()
{
    long t=(REG_PWM_ISR1);
    if(VGA.linedouble){
      if(VGA.mode == VGA_MONO)REG_DMAC_SADDR4-=(VGA.pw<<1);
      else if(VGA.mode == VGA_COLOUR)VGA.lptr-=VGA.cw;
    }
    VGA.debug=REG_TC0_CV0;
    asm volatile("wfe \n\t");
}

void __attribute__((aligned(64))) DMAC_Handler()
{
    REG_PIOA_PER  = 1<<26;     
    uint32_t dummy=REG_DMAC_EBCISR;
}

__attribute__ ((section(".ramfunc"))) 
uint32_t _v_efc_perform_fcr(Efc *p_efc, uint32_t ul_fcr) {
    volatile uint32_t ul_status;
    p_efc->EEFC_FCR = ul_fcr;
    do {
        ul_status = p_efc->EEFC_FSR;
    } while ((ul_status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);
    return (ul_status & (EEFC_FSR_FLOCKE | EEFC_FSR_FCMDE));
}

int Vga::calcmodeline()
{
    //try to find a suitable modeline
  for (xscale = 16; xscale > 1; xscale--)
   for(int yti=0;yti<50;yti++){
    pclock = 84000000 / xscale;
    xtotal = (xsize * 5 / 4);
    if(xtotal & 1) xtotal -= 1;
    xsyncstart=((10*xsize+2*xtotal)/12);
    xsyncend=((5*xsize+7*xtotal)/12);
    ytotal = ((ysize*25)/24)+yti;
    ysyncstart=((10*ysize+2*ytotal)/12)+1;
    ysyncend=((8*ysize+4*ytotal)/12)+1;
    if(ysyncstart <= ysize)ysyncstart=ysize+1;
    if(ysyncend <= ysyncstart)ysyncend=ysyncstart+1;
        
    lfreq = pclock / xtotal;
    for(yscale=1;yscale<=8;yscale++){
        ffreq = lfreq / (ytotal * yscale);
        ltot = ysize * yscale;
	if ((lfreq > lfreqmin) && (lfreq < lfreqmax) && 
	    (ffreq > ffreqmin) && (ffreq < ffreqmax))
	      goto foundmode;
    }
  }
  foundmode:;
  if(xscale==1)return -1;
  if(mode == VGA_COLOUR && xscale < 6)return -1;
    
  // calculate timings from modeline data
  xclocks=(xtotal*xscale) &~ 1;
  xstart=(xtotal - xsyncend)*xscale - 80;
  if(xstart < 130)xstart = 130;
  xsyncwidth = (xsyncend - xsyncstart)*xscale;
  
  return 0;
}

int Vga::allocvideomem()
{
    if(mode==VGA_MONO){
    pw=((xsize+31)/32)*2+2;
    pbsize=pw*ysize;
    pb=(uint16_t *)calloc(pbsize,2);
    if(pb==0)return -2;
    
    pbb=(uint32_t *)((int(pb-0x20000000)*32)+0x22000000);
    pbw=pw*16;
  }
  if(mode==VGA_COLOUR){
    cw=xsize;
    cbsize=cw*ysize;
    cb=(uint8_t *)calloc(cbsize,1);
    if(cb==0)return -2;
  }
  return 0;
}

void Vga::freevideomem()
{
  if(pb){free(pb);pb=0;}
  if(cb){free(cb);cb=0;}  
}

void Vga::startinterrupts()
{
  for(int i=0;i<45;i++)    NVIC_SetPriority(IRQn_Type (i),3);  
  NVIC_SetPriority(PWM_IRQn,3);
  NVIC_SetPriority(TC0_IRQn,1); 
  NVIC_SetPriority(UOTGHS_IRQn,2); 
  
  if(mode==VGA_MONO) NVIC_EnableIRQ(DMAC_IRQn);
  NVIC_EnableIRQ(TC0_IRQn); 
  NVIC_EnableIRQ(PWM_IRQn); 
}

void Vga::stopinterrupts()
{
  NVIC_DisableIRQ(PWM_IRQn);
  NVIC_DisableIRQ(TC0_IRQn);
  NVIC_DisableIRQ(DMAC_IRQn); 
}

void Vga::starttimers()
{
  REG_PIOA_PDR  =1<<20;
  REG_PIOA_ABSR|=1<<20;
  REG_PMC_PCER1= 1<<4;
  REG_PWM_WPCR= 0x50574dfc;
  REG_PWM_CLK= 0x00010001;
  REG_PWM_DIS= 1<<2;
  REG_PWM_CMR2=hsyncpol ? 0x0 : 0x200;
  REG_PWM_CPRD2=xclocks+1;
  REG_PWM_CDTY2=xclocks-xsyncwidth;
  REG_PWM_SCM=0;
  REG_PWM_IER1=1<<2;
  REG_PWM_ENA= 1<<2;  

  REG_PMC_PCER0= 1<<27;
  REG_TC0_WPMR=0x54494D00;
  REG_TC0_CMR0=0b00000000000010011100010000000000;
  REG_TC0_RC0=xclocks/2; 
  REG_TC0_RA0=0;  
  REG_TC0_CCR0=0b101;    
  REG_TC0_IER0=0b00010000; 
  REG_TC0_IDR0=0b11101111; 
}

void Vga::stoptimers()
{
    REG_TC0_CCR0=0b10;    
    REG_TC0_IDR0=0b00010000; 
    REG_PMC_PCDR0= 1<<27;

    REG_PWM_DIS= 1<<2;
    REG_PWM_IDR1=1<<2;
}

void Vga::startmono(){
    
    for(int i=34;i<=41;i++)pinMode(i,INPUT);
      
    REG_PMC_PCER1= 1<<7;  
    REG_DMAC_WPMR=DMAC_WPMR_WPKEY(0x444d4143);
    REG_DMAC_EN=1;
    REG_DMAC_GCFG=0x00;
    REG_DMAC_EBCIER=1<<4;
    REG_DMAC_DADDR4=(uint32_t)&REG_SPI0_TDR;  
    REG_DMAC_DSCR4=0;
    REG_DMAC_CTRLB4=0x20310000;
    REG_DMAC_CFG4=  0x01412210;    
    
    REG_PIOA_PDR  = (1<<25)|(1<<27)|(1<<28);
    REG_PIOA_PER  = 1<<26;
    REG_PIOA_ABSR&=~((1<<25)|(1<<27)|(1<<28));
    REG_PMC_PCER0= 1<<24;
    REG_SPI0_WPMR=0x53504900; 
    REG_SPI0_CR=0x1;
    REG_SPI0_MR=0x00000011;
    SPI0->SPI_CSR[0]=0x00000080 + (xscale << 8);  

}

void Vga::stopmono(){
  REG_DMAC_CHDR=1<<4;
  //while(REG_DMAC_CHSR&(1<<4));
  REG_DMAC_EBCIDR=1<<4;
  REG_SPI0_CR=0x0;
  REG_PIOA_PER  = 1<<26; 
}
void Vga::startcolour(){    
    
  // compile line output function
    uint16_t buf[3072]={0};
    uint16_t *bufp = buf;
    for(int i=0;i<xsize;i++){
      *bufp++=0xf813;
      *bufp++=0x0b01;        // ldrb.w  r0, [r3], #1
      *bufp++=0x0080;        // lsls    r0, r0, #2
      *bufp++=0x6008;        // str     r0, [r1, #0]
      for(int j=7;j<xscale;j++)
	*bufp++=0xbf00;      // nop
    }
    *bufp++=0x4770;
    
    uint32_t *a=(uint32_t *)buf;
    uint32_t *b = const_cast<uint32_t *> ((uint32_t *)colourfunc);
    
    // check if line output function has already been flashed
    int needsflash=0;
    for(int i=0;i<1536;i++)if(a[i]!=b[i])needsflash=1;

    if(needsflash)for(int i=0;i<24;i++){
      unsigned long FlashSectorNum = ((unsigned long)b - 0x80000) / 256; 
      Efc * efc = FlashSectorNum >= 1024 ? EFC1 : EFC0;
      FlashSectorNum &= 1023; 
      unsigned long flash_cmd = 0;
      unsigned long flash_status = 0;  
      flash_cmd = (0x5A << 24) | (FlashSectorNum << 8) | 0x03;

      noInterrupts();
      for(int j=0;j<64;j++)*b++ = *a++;      
      flash_status = _v_efc_perform_fcr (efc, flash_cmd);   
      interrupts();
      delay(1);
    }
    asm volatile ("isb \n\t");
         
    for(int i=34;i<=41;i++)pinMode(i,OUTPUT);
    REG_PIOC_OWER= 0x3fc;
    REG_PIOC_OER= 0x3fc;
}

void Vga::stopcolour()
{
    for(int i=34;i<=41;i++)pinMode(i,INPUT);
    REG_PIOC_OWDR= 0x3fc;
    REG_PIOC_ODR= 0x3fc;
}

int Vga::begin(int x, int y, int m)
{
  if(up)VGA.end();
  
  if(m!=VGA_MONO && m!=VGA_COLOUR) return -4;
  if(m==VGA_COLOUR && y>380)return -3;
  if(lfreqmin==0){lfreqmin=27000;lfreqmax=83000;ffreqmin=57;ffreqmax=70;}
  
  xsize=x;ysize=y;mode=m;
  tww=tw=xsize/8;twh=th=ysize/8;twx=twy=tx=ty=0;
  ink=255;paper=0;
  synced=0;framecount=0;
    
  int r;
  r=calcmodeline(); if(r)return r;
  r=allocvideomem(); if(r)return r;
  
  pinMode(_v_hsync,OUTPUT); 
  pinMode(_v_vsync,OUTPUT); 
  starttimers();

  if(mode == VGA_MONO)startmono();
  else if (mode == VGA_COLOUR)startcolour();
  startinterrupts();
  
  up=1;
  return 0;
}

void Vga::end()
{
  if(!up)return;
  up=0;
  
  stopinterrupts();
  if(mode == VGA_MONO)stopmono();
  else if (mode == VGA_COLOUR)stopcolour(); 
  
  stoptimers();
  pinMode(_v_hsync,INPUT); 
  pinMode(_v_vsync,INPUT); 
  
  freevideomem();
  
  pclock=xsize=xsyncstart=xsyncend=xtotal=ysize=ysyncstart=ysyncend=ytotal=0;
  mode=line=linedouble=synced=xclocks=xstart=xsyncwidth=xscale=yscale=0;
  lfreq=ffreq=ltot=0;

  return;
}



