#include <math.h>
#include "dsk6713.h"
#include "dsk6713_aic23.h"

/* Codec configuration settings */
/* C A U T I O N:
 * 'config' must not be the first variable, otherwise it will be placed at address
 * 0x0000, and this causes a compare config==NULL somewhere down in the BSL to
 * become true and then load default settings !!!!
 */
volatile int dummy[10];
static DSK6713_AIC23_Config config = { \
  0x0017,  /* 0 DSK6713_AIC23_LEFTINVOL  Left line input channel volume 0dB*/ \
  0x0017,  /* 1 DSK6713_AIC23_RIGHTINVOL Right line input channel volume 0dB*/\
  0x00f9,  /* 2 DSK6713_AIC23_LEFTHPVOL  Left channel headphone volume 0dB*/  \
  0x00f9,  /* 3 DSK6713_AIC23_RIGHTHPVOL Right channel headphone volume 0dB*/ \
  0x0011,  /* 4 DSK6713_AIC23_ANAPATH    Analog audio path control    */      \
  0x0000,  /* 5 DSK6713_AIC23_DIGPATH    Digital audio path control    */     \
  0x0000,  /* 6 DSK6713_AIC23_POWERDOWN  Power down control    */             \
  0x0043,  /* 7 DSK6713_AIC23_DIGIF      Digital audio interface format    */ \
  0x0081,  /* 8 DSK6713_AIC23_SAMPLERATE Sample rate control    */            \
  0x0001   /* 9 DSK6713_AIC23_DIGACT     Digital interface activation    */   \
};

/*
 * main.c
 */
int main(void) {
  int i=0;
  DSK6713_AIC23_CodecHandle hCodec;
  // Initialize the board support library, must be called first
  DSK6713_init();
  // Start the codec
  hCodec = DSK6713_AIC23_openCodec(0, &config);

  // define the filter
  short input=0;
  short output=0;
  double a[10];
  double b[10];
  double fkt=2.5;
  double x[10];
  double y[10];
  Uint32 in = 0;

  int j=1;

  b[0]= 2.83790522512316e-008;
  b[1]= 0.0;
  b[2]= -1.13516209004927e-007;
  b[3]= -3.97046694025453e-023;
  b[4]= 1.7027431350739e-007 ;
  b[5]= -3.97046694025453e-023;
  b[6]=-1.13516209004927e-007;
  b[7]= 0.0 ;
  b[8]=2.83790522512316e-008;
  b[9]=0.0;

  a[0]=0.0;
  a[1]= -7.92139814864085;
  a[2]=27.4628690203029;
  a[3]=-54.4271942671112;
  a[4]=67.4419254163037;
  a[5]=-53.5042025245989;
  a[6]=26.5393236487168;
  a[7]=-7.52519713333643;
  a[8]= 0.933873988406161;
  a[9]=0.0;


  /* now process one sample after the other */
  //forever
  for (;;){
  // Get a sample from the left channel (connector mid)
      while(!DSK6713_AIC23_read(hCodec,&in));
  // Process the sample
      input=(short)(in & 0x0000FFFF);
      x[0]=(double) input;
      y[0] = x[0]*b[0] + x[1]*b[1] + x[2]*b[2] + x[3]*b[3] + x[4]*b[4] + x[5]*b[5] + x[6]*b[6] + x[7]*b[7]+x[8]*b[8]+x[9]*b[9]  - y[1]*a[1] - y[2]*a[2] - y[3]*a[3]-y[4]*a[4] - y[5]*a[5] - y[6]*a[6]-y[7]*a[7]-y[8]*a[8]-y[9]*a[9];



      x[9] = x[8];
      x[8] = x[7];
      x[7] = x[6];
      x[6] = x[5];
      x[5] = x[4];
      x[4] = x[3];
      x[3] = x[2];
      x[2] = x[1];
      x[1] = x[0];

      y[9] = y[8];
      y[8] = y[7];
      y[7] = y[6];
      y[6] = y[5];
      y[5] = y[4];
      y[4] = y[3];
      y[3] = y[2];
      y[2] = y[1];
      y[1] = y[0];


      y[0]=y[0]*fkt;
      output=(short) y[0];
  // Send a sample to the left channel
      while(!DSK6713_AIC23_write(hCodec,output));
  // Get a sample from the right channel (connector end)
      while(!DSK6713_AIC23_read(hCodec,&in));
  // Process the sample
  // Send a sample to the right channel
    while(!DSK6713_AIC23_write(hCodec,output));
  }
  // Close the codec
  DSK6713_AIC23_closeCodec(hCodec);
  return 0;
  }
