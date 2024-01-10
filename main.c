
//---#defines---
#include "gba.h"
#include "levelStuff.c"

//---Math functions---
#include <math.h> 

//---Global variables---
#define  GBA_SW 160                                        //actual gba screen width
#define  SW     120                                        //game screen width
#define  SH      80

#define FRONT 0

int posX = 0;                                        //Game world X;
int posY = 0;                                        //Game world Y;
#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10))                //15 bit, 0-31, 5bit=r, 5bit=g, 5bit=b 
//---Debug Textures---
#include "textures/Linear_RGB_color_wheel.c"

//---Texture Stuff---
#include "textures/title.c"
#include "textures/pDownIdl.c"
#include "textures/pDown.c"
#include "textures/pDownAtk.c"
#include "textures/pUpIdle.c"
#include "textures/pUp.c"
#include "textures/pUpAtk.c"
#include "textures/pVertIdle.c"
#include "textures/pVert1.c"
#include "textures/pVert2.c"
#include "textures/pVertAtk.c"
#include "textures/apple.c"
#include "textures/DemonTut.c"
#include "textures/menu.c"
#include "textures/select.c"
#include "textures/catechismusNoChange.c"
#include "textures/catechismusChange.c"
#include "textures/mortis.c"
#include "levelAssets/Key.c"
#include "End-Demo.c"
#include "logos/AirDolf.c"
#include "logos/VSink.c"


//---SFX---
#include "audio/sound.c"


//---Demon Stuff---
#include "aiBrains.c"

int sound = 6;
int titleLoad;
int lastFr=0,FPS=0;                                        //for frames per second
int stateID=0;
int dir=0;
int counter;
int haskey = 0;

typedef struct                                             //player
{
 int x,y;
 int colXmin, colXmax;
 int colYmin, colYmax;
 int posX, posY;
 int frame;
 const u16* map;
 int rx,ry;
}Player; Player P;



void clearBackground(int r, int g, int b)
{int x,y;
 for(x=0;x<SW;x++)
 {                                                         //rgb values 0-31
  for(y=0;y<SH;y++){ VRAM[y*GBA_SW+x]=RGB(r,g,b);}       //clear all 120x80 pixels
 }
}
//Sound stuff (IN DEVELOPMENT!!!!)
int _myTimer;
u16 _soundLength[] = { 8, 46, 131, 65, 3, 0, 0, 0};// Length of all sound in file sound.h. Sound.h has 4 variable: Sound1Data, Sound2Data, Sound3Data, Sound4Data.
u16 _soundLengthInSec;//Length of sound
u16 _soundPlayedInSec; //Number of second played the sound


void PlaySound1() {
	//Turn on sound circuit - Master sound enable and enable Sound 1-4
	REG_SOUNDCNT_X = 0x80;

	//Sound 1-4 Output level and Stereo control
	//Full volume, enable sound 1 to both left and right
	REG_SOUNDCNT_L = 0x1177;

	// Overall: output ratio
	REG_SOUNDCNT_H = 2;

	// Configure for sound channel 1
	REG_SOUND1CNT_L = 0x0046; //Sound 1 Sweep control //Low
	REG_SOUND1CNT_H = 0xfAF7; //Sound 1 Length, wave duty and envelope control //High
	REG_SOUND1CNT_X = 0xCEAD; //Sound 1 Frequency, reset and loop control //Frequency
}

//****
//Modified from: http://belogic.com/gba/ - Direct sound
//****
//soundNum: 4 songs
void PlaySoundDMA(u8 soundNum) {
	_soundPlayedInSec = 1; // number of seconds has passed
	_soundLengthInSec = _soundLength[soundNum - 1]; // total length in seconds of this song

	//Play a mono sound at 16khz
	//Use timer 0 as sampling rate source
	
	REG_SOUNDCNT_H = 0x0B0F; //0x0b0F; //enable DS A&B + FIFO reset + use timer0 + max volume to L and R
	REG_SOUNDCNT_X = 0x0080; //turn sound chip on
	switch (soundNum) {
	case 1:
		REG_DMA1SAD = (u32) _Mortis; // src
		break;
	case 2:
		REG_DMA1SAD = (u32) _MainTheme;
		break;
	case 3:REG_DMA1SAD = (u32) _Michael;
		break;
	case 4:
		REG_DMA1SAD = (u32) _House;
		break;
	case 5:
		REG_DMA1SAD = (u32) _locked;
		break;
	case 6:
		REG_DMA1SAD = (u32) _Forest;
		break;
	case 7:
		REG_DMA1SAD = (u32) _End_Demo;
		break;
	case 8:
		REG_DMA1SAD = (u32) _Start_Demo;
		break;
	case 9:
		REG_DMA1SAD = (u32) _Radio;
		break;
		
	dPrintValue(REG_SOUND1CNT_X);
	}

	//DMA1 destination
	REG_DMA1DAD = 0x040000a0; //write to FIFO A address 
	REG_DMA1CNT_H = 0xb600; //DMA control: DMA enabled+ start on FIFO+32bit+repeat+increment source&dest

	//REG_TM0D
	//Formula for playback frequency is: 0xFFFF-round(cpuFreq/playbackFreq) 
	REG_TM0CNT_L =  0xFBE6; //16khz playback frequency
	REG_TM0CNT_H = 0x0080; //enable timer
}

///Stop timer0 & DMA1
void StopDMASound() {
	REG_TM0CNT_H = 0; //disable timer 0
	REG_DMA1CNT_H = 0x00; //stop DMA
	dprint("Stop DMA Sound");
}





typedef struct
{
	int select;
}Title; Title T;


void drawImage(int w,int h, int xo,int yo, const u16* map, int to)
{int x,y,c;          //image w/h, position offset,  texture name, texture offset
 for(x=0;x<w;x++) 
 {                
  for(y=0;y<h;y++){ c=map[(y+to*h)*w+x]; if(c>0){ VRAM[(y+yo)*GBA_SW+x+xo]=c;}} 
 }
}

void mirrorImage(int w, int h, const u16* map, int to, int xo, int yo) {
    int x, y, c;
    for (x = 0; x < w; x++) {
        for (y = 0; y < h; y++) {
            c = map[(y + to * h) * w + (w - x - 1)]; // Mirroring the image horizontally
            if (c > 0) {
                VRAM[(y + yo) * GBA_SW + x + xo] = c; // Using xo and yo for positioning
            }
        }
    }
}

int setColision(int minX, int minY, int maxX, int maxY)
{
	P.colXmin = minX;
	P.colXmax = maxX;
	P.colYmin = minY;
	P.colYmax = maxY;
}

int Speak = 0;
int testFor = 1;

void resetMicheal()
{

	sound = 6; 
	counter=0;
	sound = 6; 
	testFor = 1;
	Speak = 0;
	StopDMASound();
	sound = 6;
}

void buttons()                                             //buttons to press
{
 if(KEY_R ){if(P.colXmax <= P.x){P.x-=3;} P.x+=3; if(P.x>SW-9){ P.x=0; posX+=1; } dir=0;}             //move right
 if(KEY_L ){if(P.colXmin >= P.x){P.x+=3;} P.x-=3; if(P.x<   5){ P.x=SW-4; posX-=1;} dir=1;}             //move left
 if(KEY_U ){if(P.colYmin >= P.y){P.y+=3;} P.y-=3; if(P.y<   5){ P.y=SH-9; posY+=1;} dir=2; counter += 1;}             //move up
 if(KEY_D ){if(P.colYmax <= P.y){P.y+=3;} P.y+=3; if(P.y>SH-14){ P.y=0; posY-=1;} dir=3;}             //move down
 if(KEY_A ){Fight(dir);} 
 if(KEY_B ){posX = 30;} 
 if(KEY_LS){} 
 if(KEY_RS){} 
 if(KEY_ST){} 
 if(KEY_SL){dead();} 
 if(dir==0 && KEY_R){drawImage(P.rx,P.ry, P.x,P.y, P.map, 0);} 
 if(dir==1 && KEY_L){mirrorImage(P.rx,P.ry, P.map, 0,P.x,P.y);}
 if(dir==2 && KEY_U){if(P.frame==1){drawImage(P.rx,P.ry, P.x,P.y, P.map, 0);}else{mirrorImage(P.rx,P.ry, P.map, 0,P.x,P.y);}}
 if(dir==3 && KEY_D){if(P.frame==1){drawImage(P.rx,P.ry, P.x,P.y, P.map, 0);}else{mirrorImage(P.rx,P.ry, P.map, 0,P.x,P.y);}}
 if(!KEY_D && !KEY_L && !KEY_R && !KEY_U){if(dir!=1){drawImage(P.rx,P.ry, P.x,P.y, P.map, 0);} else{mirrorImage(P.rx,P.ry, P.map, 0,P.x,P.y);}}
 if(counter>=45 && posX != 40 | posX != 30){summonDemon(P.x, P.y, 1); Speak = 1;  sound = 3;}
 if(Speak == 1 && testFor == 1){ StopDMASound();  Speak = 0; testFor = 0;}
}







void updatePlayer()
{
	//player frame manager
	P.frame+=1; if(P.frame==2){P.frame=0;}
	
	//player direction manager
	if(KEY_R){if(P.frame==0){P.map=pVert1_Map;}if(P.frame==1){P.map=pVert2_Map;} P.rx=6; P.ry=10;} else if (dir==0){P.map=pVertIdle_Map; P.rx=5; P.ry=10;}
	if(dir==0 && !KEY_R && KEY_A){P.map=pVertAtk_Map; P.rx=6; P.ry=10;}
	
	if(KEY_L){if(P.frame==0){P.map=pVert1_Map;}if(P.frame==1){P.map=pVert2_Map;} P.rx=6; P.ry=10;} else if (dir==1){P.map=pVertIdle_Map; P.rx=5; P.ry=10;}
	if(dir==1 && !KEY_L && KEY_A){P.map=pVertAtk_Map; P.rx=6; P.ry=10;}
	
	if(KEY_U){P.map=pUp_Map; P.rx=6; P.ry=10;} else if (dir==2){P.map=pUpIdle_Map; P.rx=5; P.ry=10;}
	if(dir==2 && !KEY_U && KEY_A){P.map=pUpAtk_Map; P.rx=5; P.ry=10;}
	
	if(KEY_D){P.map=pDown_Map; P.rx=6; P.ry=10;} else if (dir==3){P.map=pDownIdl_Map; P.rx=5; P.ry=10;}
	if(dir==3 && !KEY_D && KEY_A){P.map=pDownAtk_Map; P.rx=7; P.ry=10;}
	

}
void dead()
{
	StopDMASound();
	stateID = 4;
}



void init()
{

 P.x=70; P.y=35;
}

int died;
typedef struct
{
	int demon;
	int ID;
}Item; Item D;

void tutDemon(int x, int y)
{
	if(died==2){D.demon+=1; died=0;}
	if(KEY_A){
	if(dir==0 && x>P.x){drawImage(7,10, x,y, DemonTut_Map, 1); died=1; }
	if(dir==1 && x<P.x){drawImage(7,10, x,y, DemonTut_Map, 1); died=1; }
	if(dir==2 && y<P.y){drawImage(7,10, x,y, DemonTut_Map, 1); died=1; }
	if(dir==3 && y>P.y){drawImage(7,10, x,y, DemonTut_Map, 1); died=1; }
	}
	if(died==1){drawImage(7,10, x,y, DemonTut_Map, 2); died=2; } else{drawImage(7,10, x,y, DemonTut_Map, 0);}
}

void apple(int x, int y)
{
	if(P.x>x-4 && P.x<x && P.y>y-10 && P.y<y+6){D.demon+=1; return;}
	drawImage(4,5, x,y, apple_Map, 0);
}

int key(int x, int y)
{
	if(haskey == 0)
	{
		if(P.x>x-4 && P.x<x && P.y>y-10 && P.y<y+6){haskey = 1; return;}
		drawImage(3, 7, x,y, key_Map, 0);
	}
}

const int demonPositionsX[] = {60, 43, 95, 70, 55, 40};
const int demonPositionsY[] = {40, 60, 50, 40, 50, 47};

//---PLAYER-STUFF-----
int repeat=1;



int main()
{int x,y; 
 //Init mode 5------------------------------------------------------------------
 *(u16*)0x4000000 = 0x405;                                           //mode 5 background 2



 //scale small mode 5 screen to full screen-------------------------------------
 REG_BG2PA=256/2;                                                    //256=normal 128=scale 
 REG_BG2PD=256/2;                                                    //256=normal 128=scale 
  
 init();                                                             //init game variables

 while(repeat) 
 {

switch(stateID){
	case 0:
	PlaySoundDMA(7);
	if(KEY_A){titleLoad += 1;}
	switch(titleLoad){
		case 0:
		drawImage(120,80, 0,0, AirDolf, 0);
		break;
		case 1:
		drawImage(120,80, 0,0, VSink, 0);
		break;
		case 2:
		drawImage(120,80, 0,0, title_Map, 0);
		break;
		case 3:
		StopDMASound();
		stateID = 1;
		break;
	}
	break;
	
	case 1:
		PlaySoundDMA(2);
		setColision(0, 0, 120, 80);
		drawImage(120,80, 0,0, MainMenu_Map, 0);
		if(KEY_D){T.select+=1; if(T.select>1){T.select=1;}}
		if(KEY_U){T.select-=1; if(T.select<0){T.select=0;}}
		if(T.select==0){drawImage(6,8, 35,42, select_Map, 0); if(KEY_A ){ stateID=2; P.map=pDownIdl_Map; StopDMASound();} }
		if(T.select==1){drawImage(6,8, 35,53, select_Map, 0); if(KEY_A ){ stateID=3; P.map=pDownIdl_Map; StopDMASound();} }
		if(KEY_B){drawImage(80,80, 0,0, RGBwheel_Map, 0);} //Debug Image
	break;
	
	case 2:
	PlaySoundDMA(sound);	
	playerLoc(posX, posY);
    	buttons();
    	updatePlayer();
		if(posX==3 && posY==6 && P.x>=53 && P.x<=60 && P.y>=49)
		{
			//outside key
			posX=30;
		}
		if(posX==30 && P.x<=55 && P.y>=62)
		{
			//inside key
			posX = 3;
			posY = 6;
			P.x = 60;
			P.y = 50;
		}

		if(posX==0 && posY==6 && P.x<=60 && P.x>=48 && P.y<=54)
		{
			//House outside
			if(haskey == 1)
			{
			StopDMASound();
			sound = 4; 
			posX=40;
			posY=40;
			}
			else
			{
			StopDMASound();
			P.y = 65;
			sound = 5; 
			}
		}
		if(posX==39)
		{
			posX = 0;
			posY = 6;
			P.x = 47;
			P.y = 55;
		}
	if(posX==41 || posY == 41)
	{
	StopDMASound();
	stateID = 5;
	}
	break;
	
	case 3:
		PlaySoundDMA(2);
		drawImage(120, 80, 0, 0, Catechrismus_Map, 0);
		if(P.y<   34){ P.y=   34;}
		buttons();
		updatePlayer();
	if (D.demon >= 0 && D.demon <= 2) {
		apple(demonPositionsX[D.demon], demonPositionsY[D.demon]);
	}
   if(D.demon>=3){drawImage(120, 17, 0, 14, CastOut_Map, 0);}
	if (D.demon >= 3 && D.demon <= 6) {
		tutDemon(demonPositionsX[D.demon], demonPositionsY[D.demon]);
	}
   if(D.demon==6){stateID=1; D.demon=0;}
   T.select=0;
   break;
   
   case 4:
   //mortis
		PlaySoundDMA(1);
		drawImage(120,80, 0,0, Mortis_Map, 0);
		if(KEY_ST){stateID=1;} 
	break;
	
	case 5:
	//end of demo
	PlaySoundDMA(8);
	drawImage(120,80, 0,0, Ending_Map, 0);
	break;
}

   
   //swap buffers---------------------------------------------------------------
   while(*Scanline<120){}	                         //wait all scanlines 
   if  ( DISPCNT&BACKB){ DISPCNT &= ~BACKB; VRAM=(u16*)VRAM_B;}      //back  buffer
   else{                 DISPCNT |=  BACKB; VRAM=(u16*)VRAM_F;}      //front buffer  
  }
 }


