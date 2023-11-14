
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

//---Audio Stuff---
#include "audio/sound.c"

#include "aiBrains.c"

int lastFr=0,FPS=0;                                        //for frames per second
int stateID=0;
int dir=0;

typedef struct                                             //player
{
 int x,y;
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



void buttons()                                             //buttons to press
{
 if(KEY_R ){P.x+=3; if(P.x>SW-4){ P.x=0; posX+=1;} dir=0;}             //move right
 if(KEY_L ){P.x-=3; if(P.x<   0){ P.x=SW-4; posX-=1;} dir=1;}             //move left
 if(KEY_U ){P.y-=3; if(P.y<   0){ P.y=SH-9; posY+=1;} dir=2;}             //move up
 if(KEY_D ){P.y+=3; if(P.y>SH-9){ P.y=0; posY-=1;} dir=3;}             //move down
 if(KEY_A ){} 
 if(KEY_B ){summonDemon();} 
 if(KEY_LS){} 
 if(KEY_RS){} 
 if(KEY_ST){} 
 if(KEY_SL){} 
 if(dir==0 && KEY_R){drawImage(P.rx,P.ry, P.x,P.y, P.map, 0);} 
 if(dir==1 && KEY_L){mirrorImage(P.rx,P.ry, P.map, 0,P.x,P.y);}
 if(dir==2 && KEY_U){if(P.frame==1){drawImage(P.rx,P.ry, P.x,P.y, P.map, 0);}else{mirrorImage(P.rx,P.ry, P.map, 0,P.x,P.y);}}
 if(dir==3 && KEY_D){if(P.frame==1){drawImage(P.rx,P.ry, P.x,P.y, P.map, 0);}else{mirrorImage(P.rx,P.ry, P.map, 0,P.x,P.y);}}
 if(!KEY_D && !KEY_L && !KEY_R && !KEY_U){if(dir!=1){drawImage(P.rx,P.ry, P.x,P.y, P.map, 0);} else{mirrorImage(P.rx,P.ry, P.map, 0,P.x,P.y);}}

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


const int demonPositionsX[] = {60, 43, 95, 70, 55, 40};
const int demonPositionsY[] = {40, 60, 50, 40, 50, 47};

//---PLAYER-STUFF-----
const int repeat=1;


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

	 if(stateID==0)
	{
			//Title Code Goes Here.
			drawImage(120,80, 0,0, title_Map, 0);
			 if(KEY_ST){ stateID=1; P.map=pDownIdl_Map;} 
	}
		
	if(stateID==1)
	{
		clearBackground(0, 0, 0);
		drawImage(120,80, 0,0, MainMenu_Map, 0);
		if(KEY_D){T.select+=1; if(T.select>1){T.select=1;}}
		if(KEY_U){T.select-=1; if(T.select<0){T.select=0;}}
		if(T.select==0){drawImage(6,8, 35,42, select_Map, 0); if(KEY_A ){ stateID=2; P.map=pDownIdl_Map;} }
		if(T.select==1){drawImage(6,8, 35,53, select_Map, 0); if(KEY_A ){ stateID=3; P.map=pDownIdl_Map;} }
		
	}
	
	if(stateID==2)
	{
	playerLoc(posX, posY);
    buttons();
    updatePlayer();
	}
	
	if(stateID==3)
	{
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
   }


   
   //swap buffers---------------------------------------------------------------
   while(*Scanline<120){}	                                         //wait all scanlines 
   if  ( DISPCNT&BACKB){ DISPCNT &= ~BACKB; VRAM=(u16*)VRAM_B;}      //back  buffer
   else{                 DISPCNT |=  BACKB; VRAM=(u16*)VRAM_F;}      //front buffer  
  }
 }


