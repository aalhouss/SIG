#include <math.h>
#include "dsk6713.h"
#include "dsk6713_aic23.h"
#define pi 3.141592653589
#define NUM_TRACKS 3
#define MAX_TONES_PER_TRACK 64
#define LEN(arr) ((int) (sizeof (arr)/sizeof(arr)[0]))
int slotLen=9000; //num of samples of shortest tone/
struct tone{
	int bSlot;
	int eSlot;
	double freq;
	double amp;
};
struct tone to[NUM_TRACKS][MAX_TONES_PER_TRACK];

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
	int i,k,q,z,j,n,actSlot,s;
	int numToneSlots=128;
	double F[NUM_TRACKS];
	double x[NUM_TRACKS];
	double pF[NUM_TRACKS];
	double tA[NUM_TRACKS]={4000,5000,7000};
	double T=1.0/48000;
	double arg[NUM_TRACKS];
	arg[0]=0.0;
	arg[1]=0.0;
	double A[NUM_TRACKS];
	double da[NUM_TRACKS];
	double y;
	double outtmp;
	double xout;
	short out=0;
	DSK6713_AIC23_CodecHandle hCodec;
	// Initialize the board support library, must be called first
	DSK6713_init();
	// Start the codec
	hCodec = DSK6713_AIC23_openCodec(0, &config);
	actSlot=0;

	// initialize array of structures
	for ( q=0;q<3;q++){
		for( z=0;z<64;z++){
			to[q][z].bSlot=-1, to[q][z].eSlot=1, to[q][z].freq=0, to[q][z].amp=0; //e up1
		}
	}


	to[0][0].bSlot=0, to[0][0].eSlot=3, to[0][0].freq=329.627556, to[0][0].amp=tA[0]; //e up1
	to[0][1].bSlot=14, to[0][1].eSlot=14, to[0][1].freq=293.664768, to[0][1].amp=tA[0]; //d up1
	to[0][2].bSlot=15, to[0][2].eSlot=15, to[0][2].freq=261.625566, to[0][2].amp=tA[0]; //c up1
	to[0][3].bSlot=16, to[0][3].eSlot=19, to[0][3].freq=329.627556, to[0][3].amp=tA[0]; //e up1
	to[0][4].bSlot=30, to[0][4].eSlot=31, to[0][4].freq=329.627556, to[0][4].amp=tA[0]; //e up1
	to[0][5].bSlot=32, to[0][5].eSlot=33, to[0][5].freq=523.251132, to[0][5].amp=tA[0]; //c up2
	to[0][6].bSlot=34, to[0][6].eSlot=37, to[0][6].freq=523.251132, to[0][6].amp=tA[0]; //c up2
	to[0][7].bSlot=38, to[0][7].eSlot=39, to[0][7].freq=523.251132, to[0][7].amp=tA[0]; //c up2
	to[0][8].bSlot=40, to[0][8].eSlot=40, to[0][8].freq=493.883302, to[0][8].amp=tA[0]; //h up1
	to[0][9].bSlot=41, to[0][9].eSlot=41, to[0][9].freq=523.251132, to[0][9].amp=tA[0]; //c up2
	to[0][10].bSlot=42, to[0][10].eSlot=45, to[0][10].freq=493.883302, to[0][10].amp=tA[0]; //h up1
	to[0][11].bSlot=46, to[0][11].eSlot=47, to[0][11].freq=440, to[0][11].amp=tA[0]; //a up1
	to[0][12].bSlot=48, to[0][12].eSlot=55, to[0][12].freq=329.627556, to[0][12].amp=tA[0]; //e up1
	/**/
	to[0][13].bSlot=56, to[0][13].eSlot=57, to[0][13].freq=329.627556, to[0][13].amp=tA[0]; //e up1
	to[0][14].bSlot=58, to[0][14].eSlot=61, to[0][14].freq=311.126984, to[0][14].amp=tA[0]; //d up1
	to[0][15].bSlot=62, to[0][15].eSlot=63, to[0][15].freq=261.625565, to[0][15].amp=tA[0]; //c up1
	to[0][16].bSlot=64, to[0][16].eSlot=71, to[0][16].freq=311.126984, to[0][16].amp=tA[0]; //e up1
	to[0][17].bSlot=88, to[0][17].eSlot=89, to[0][17].freq=311.126984, to[0][17].amp=tA[0]; //e up1
	to[0][18].bSlot=90, to[0][18].eSlot=93, to[0][18].freq=277.182631, to[0][18].amp=tA[0]; //c up2
	to[0][19].bSlot=94, to[0][19].eSlot=95, to[0][19].freq=246.941651, to[0][19].amp=tA[0]; //c up2
	to[0][20].bSlot=96, to[0][20].eSlot=103, to[0][20].freq=329.627557, to[0][20].amp=tA[0]; //c up2








	to[1][0].bSlot=0, to[1][0].eSlot=13, to[1][0].freq=130.812783, to[1][0].amp=tA[1]; //c
	to[1][1].bSlot=15, to[1][1].eSlot=29, to[1][1].freq=130.812783, to[1][1].amp=tA[1]; //c
	to[1][2].bSlot=32, to[1][2].eSlot=33, to[1][2].freq=329.627556, to[1][2].amp=tA[1]; //e up1
	to[1][3].bSlot=34, to[1][3].eSlot=37, to[1][3].freq=329.627556, to[1][3].amp=tA[1]; //e up1
	to[1][4].bSlot=38, to[1][4].eSlot=39, to[1][4].freq=329.627556, to[1][4].amp=tA[1]; //e up1
	to[1][5].bSlot=42, to[1][5].eSlot=45, to[1][5].freq=293.664768, to[1][5].amp=tA[1]; //d up1
	to[1][6].bSlot=46, to[1][6].eSlot=47, to[1][6].freq=130.812783, to[1][6].amp=tA[1]; //c
	to[1][7].bSlot=48, to[1][7].eSlot=55, to[1][7].freq=130.812783, to[1][7].amp=tA[1]; //c

	to[1][8].bSlot=56, to[1][8].eSlot=61, to[1][8].freq=261.625565, to[1][8].amp=tA[1]; //c
	to[1][9].bSlot=64, to[1][9].eSlot=79, to[1][9].freq=246.941651, to[1][9].amp=tA[1]; //c
	to[1][10].bSlot=80, to[1][10].eSlot=87, to[1][10].freq=246.941651, to[1][10].amp=tA[1]; //e up1
	to[1][11].bSlot=88, to[1][11].eSlot=93, to[1][11].freq=246.941651, to[1][11].amp=tA[1]; //e up1
	to[1][12].bSlot=96, to[1][12].eSlot=111, to[1][12].freq=195.997718, to[1][12].amp=tA[1]; //e up1
	to[1][13].bSlot=112, to[1][13].eSlot=117, to[1][13].freq=195.997718, to[1][13].amp=tA[1]; //d up1


	to[2][0].bSlot=0, to[2][0].eSlot=1, to[2][0].freq=110, to[2][0].amp=tA[2]; //a big
	to[2][1].bSlot=2, to[2][1].eSlot=3, to[2][1].freq=164.813778, to[2][1].amp=tA[2]; //e
	to[2][2].bSlot=4, to[2][2].eSlot=5, to[2][2].freq=220, to[2][2].amp=tA[2]; //a
	to[2][3].bSlot=6, to[2][3].eSlot=7, to[2][3].freq=164.813778, to[2][3].amp=tA[2]; //e
	to[2][4].bSlot=8, to[2][4].eSlot=9, to[2][4].freq=110, to[2][4].amp=tA[2]; //a big
	to[2][5].bSlot=10, to[2][5].eSlot=11, to[2][5].freq=164.813778, to[2][5].amp=tA[2]; //e
	to[2][6].bSlot=12, to[2][6].eSlot=13, to[2][6].freq=220, to[2][6].amp=tA[2]; //a
	to[2][7].bSlot=14, to[2][7].eSlot=15, to[2][7].freq=164.813778, to[2][7].amp=tA[2]; //e
	to[2][8].bSlot=16, to[2][8].eSlot=17, to[2][8].freq=110, to[2][8].amp=tA[2]; //a big
	to[2][9].bSlot=18, to[2][9].eSlot=19, to[2][9].freq=164.813778, to[2][9].amp=tA[2]; //e
	to[2][10].bSlot=20, to[2][10].eSlot=21, to[2][10].freq=220, to[2][10].amp=tA[2]; //a
	to[2][11].bSlot=22, to[2][11].eSlot=23, to[2][11].freq=164.813778, to[2][11].amp=tA[2]; //e
	to[2][12].bSlot=24, to[2][12].eSlot=25, to[2][12].freq=110, to[2][12].amp=tA[2]; //a big
	to[2][13].bSlot=26, to[2][13].eSlot=27, to[2][13].freq=164.813778, to[2][13].amp=tA[2]; //e
	to[2][14].bSlot=28, to[2][14].eSlot=29, to[2][14].freq=220, to[2][14].amp=tA[2]; //a
	to[2][15].bSlot=30, to[2][15].eSlot=31, to[2][15].freq=164.813778, to[2][15].amp=tA[2]; //e
	to[2][16].bSlot=32, to[2][16].eSlot=33, to[2][16].freq=110, to[2][16].amp=tA[2]; //a big
	to[2][17].bSlot=34, to[2][17].eSlot=35, to[2][17].freq=164.813778, to[2][17].amp=tA[2]; //e
	to[2][18].bSlot=36, to[2][18].eSlot=37, to[2][18].freq=220, to[2][18].amp=tA[2]; //a
	to[2][19].bSlot=38, to[2][19].eSlot=39, to[2][19].freq=164.813778, to[2][19].amp=tA[2]; //e
	to[2][20].bSlot=40, to[2][20].eSlot=41, to[2][20].freq=110, to[2][20].amp=tA[2]; //a big
	to[2][21].bSlot=42, to[2][21].eSlot=43, to[2][21].freq=164.813778, to[2][21].amp=tA[2]; //e
	to[2][22].bSlot=44, to[2][22].eSlot=45, to[2][22].freq=220, to[2][22].amp=tA[2]; //a
	to[2][23].bSlot=46, to[2][23].eSlot=47, to[2][23].freq=164.813778, to[2][23].amp=tA[2]; //e
	to[2][24].bSlot=48, to[2][24].eSlot=49, to[2][24].freq=110, to[2][24].amp=tA[2]; //a big
	to[2][25].bSlot=50, to[2][25].eSlot=51, to[2][25].freq=164.813778, to[2][25].amp=tA[2]; //e
	to[2][26].bSlot=52, to[2][26].eSlot=53, to[2][26].freq=220, to[2][26].amp=tA[2]; //a
	to[2][27].bSlot=54, to[2][27].eSlot=55, to[2][27].freq=164.813778, to[2][27].amp=tA[2]; //e



	to[2][28].bSlot=56, to[2][28].eSlot=57, to[2][28].freq=110, to[2][28].amp=tA[2]; //a big
	to[2][29].bSlot=58, to[2][29].eSlot=59, to[2][29].freq=164.813778, to[2][29].amp=tA[2]; //e
	to[2][30].bSlot=60, to[2][30].eSlot=61, to[2][30].freq=220, to[2][30].amp=tA[2]; //a
	to[2][31].bSlot=62, to[2][31].eSlot=63, to[2][31].freq=164.813778, to[2][31].amp=tA[2]; //e

	to[2][32].bSlot=64, to[2][32].eSlot=65, to[2][32].freq=123.470825, to[2][32].amp=tA[2]; //a big
	to[2][33].bSlot=66, to[2][33].eSlot=67, to[2][33].freq=155.563492, to[2][33].amp=tA[2]; //e
	to[2][34].bSlot=68, to[2][34].eSlot=69, to[2][34].freq=174.614116, to[2][34].amp=tA[2]; //a
	to[2][35].bSlot=70, to[2][35].eSlot=71, to[2][35].freq=155.563492, to[2][35].amp=tA[2]; //e

	to[2][36].bSlot=72, to[2][36].eSlot=73, to[2][36].freq=123.470825, to[2][36].amp=tA[2]; //a big
	to[2][37].bSlot=74, to[2][37].eSlot=75, to[2][37].freq=155.563492, to[2][37].amp=tA[2]; //e
	to[2][38].bSlot=76, to[2][38].eSlot=77, to[2][38].freq=174.614116, to[2][38].amp=tA[2]; //a
	to[2][39].bSlot=78, to[2][39].eSlot=79, to[2][39].freq=155.563492, to[2][39].amp=tA[2]; //e

	to[2][40].bSlot=80, to[2][40].eSlot=81, to[2][40].freq=123.470825, to[2][40].amp=tA[2]; //e
	to[2][41].bSlot=82, to[2][41].eSlot=83, to[2][41].freq=155.563492, to[2][41].amp=tA[2]; //a
	to[2][42].bSlot=84, to[2][42].eSlot=85, to[2][42].freq=174.614116, to[2][42].amp=tA[2]; //e
	to[2][43].bSlot=86, to[2][43].eSlot=87, to[2][43].freq=155.563492, to[2][43].amp=tA[2]; //a


	to[2][44].bSlot=88, to[2][44].eSlot=89, to[2][44].freq=123.470825, to[2][44].amp=tA[2]; //e
	to[2][45].bSlot=90, to[2][45].eSlot=91, to[2][45].freq=155.563492, to[2][45].amp=tA[2]; //a
	to[2][46].bSlot=92, to[2][46].eSlot=93, to[2][46].freq=174.614116, to[2][46].amp=tA[2]; //e
	to[2][47].bSlot=94, to[2][47].eSlot=95; to[2][47].freq=155.563492, to[2][47].amp=tA[2]; //a

	to[2][48].bSlot=96, to[2][48].eSlot=97, to[2][48].freq=82.406889, to[2][48].amp=tA[2];
	to[2][49].bSlot=98, to[2][49].eSlot=99, to[2][49].freq=123.470825, to[2][49].amp=tA[2]; //a
	to[2][50].bSlot=100, to[2][50].eSlot=101, to[2][50].freq=164.813778, to[2][50].amp=tA[2]; //e
	to[2][51].bSlot=102, to[2][51].eSlot=103, to[2][51].freq=123.470825, to[2][51].amp=tA[2]; //a

	to[2][52].bSlot=104, to[2][52].eSlot=105, to[2][52].freq=82.406889, to[2][52].amp=tA[2];
	to[2][53].bSlot=106, to[2][53].eSlot=107, to[2][53].freq=123.470825, to[2][53].amp=tA[2];
	to[2][54].bSlot=108, to[2][54].eSlot=109, to[2][54].freq=164.813778, to[2][54].amp=tA[2]; //a
	to[2][55].bSlot=110, to[2][55].eSlot=111, to[2][55].freq=123.470825, to[2][55].amp=tA[2]; //e

	to[2][56].bSlot=112, to[2][56].eSlot=113, to[2][56].freq=82.406889, to[2][56].amp=tA[2]; //a
	to[2][57].bSlot=114, to[2][57].eSlot=115, to[2][57].freq=123.470825, to[2][57].amp=tA[2];
	to[2][58].bSlot=116, to[2][58].eSlot=117, to[2][58].freq=164.813778, to[2][58].amp=tA[2]; //a
	to[2][59].bSlot=118, to[2][59].eSlot=119, to[2][59].freq=123.470825, to[2][59].amp=tA[2]; //e

	to[2][60].bSlot=120, to[2][60].eSlot=121, to[2][60].freq=82.406889, to[2][60].amp=tA[2]; //a
	to[2][61].bSlot=122, to[2][61].eSlot=123, to[2][61].freq=123.470825, to[2][61].amp=tA[2];
	to[2][62].bSlot=124, to[2][62].eSlot=125, to[2][62].freq=164.813778, to[2][62].amp=tA[2]; //a
	to[2][63].bSlot=126, to[2][63].eSlot=127, to[2][63].freq=123.470825, to[2][63].amp=tA[2]; //e
	// to[2][51].bSlot=102, to[2][26].eSlot=103, to[2][26].freq=220, to[2][26].amp=tA[2]; //a

	// define the song




	// generate the song; at the end: repeat from start
	do{
		// initializations each time the song starts again (if any)

		// work through all tone slots of the song
		for (actSlot=0; actSlot<numToneSlots; actSlot++){
			// for each tone slot:
			// determine which tracks contribute a tone (frequency, amplitude)
			for (i=0; i<NUM_TRACKS; i++){
				// work through all samples of the tone slot
				for(s=0;s<64;s++){
					if(actSlot>=to[i][s].bSlot && actSlot <= to[i][s].eSlot){
						F[i]=to[i][s].freq;
						//pF[i]=F[i];
						A[i]=to[i][s].amp;
						break;
					}
					else{
						F[i]=0.0;
						A[i]=0.0;

					}
				}
				da[i]=2.0*pi*F[i]*T;
			}


			// work through all samples of the tone slot
			for (n=0; n<slotLen; n++){
				// for each sample: calculate sample value from all tracks
				out=0;
				xout=0;
				for  (j=0; j<NUM_TRACKS; j++){
					arg[j]=arg[j]+da[j];
					if(arg[j]>2.0*pi){

						arg[j]=arg[j]-2.0*pi;
					}
					x[j]=A[j]*sin(arg[j]);
				}
				for(k=0;k<NUM_TRACKS;k++){
					xout=xout+x[k];
				}
				out=(short)xout;


				// Send the sample to the left channel
				while (!DSK6713_AIC23_write(hCodec, out));
				// Send same sample to the right channel
				while (!DSK6713_AIC23_write(hCodec, out));

			}
		}
	}while(1);

	// Close the codec
	//DSK6713_AIC23_closeCodec(hCodec);
	return 0;
}

