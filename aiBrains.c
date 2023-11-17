//This is what controls the demons behavior. Im not sure if you would actually call this AI, but I don't care.
#include <math.h>
//---Texture Stuff---
#include "textures/Micheal.c"

int playerX=0;
int playerY=0;
extern int playerDir=0;
int speed=1;

typedef struct
{
	int x,y;
	int frame;
}Demon; Demon De;

void Fight(int Cast)
{
playerDir=Cast;
}


void Micheal()
{	
	De.frame += 1;
	if(De.x <= playerX){De.x +=speed;}else{De.x -=speed;}
	if(De.y <= playerY-1){De.y +=speed;}else if(De.y >= playerY+1){De.y -=speed;}
	if(De.x == playerX && De.y == playerY){dead(4);}
	if(KEY_A && playerDir==0){speed= -1;}
	if(De.x == 0 | De.x == 120){speed=1;}
	if(De.frame == 1)
	{
		mirrorImage(12, 10, Micheal_Map, 0, De.x, De.y);
	} else
	{
		drawImage(12, 10, De.x, De.y, Micheal_Map, 0);
		De.frame = 0;
	}
}


int summonDemon(int PX, int PY)
{
	Micheal();
	playerX = PX;
	playerY = PY;
}


