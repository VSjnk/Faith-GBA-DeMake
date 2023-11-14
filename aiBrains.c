//This is what controls the demons behavior. Im not sure if you would actually call this AI, but I don't care.
#include <math.h>
//---Texture Stuff---
#include "textures/Micheal.c"

typedef struct
{
	int x,y;
	int frame;
}Demon; Demon De;

void Micheal()
{	
	De.frame += 1;
	De.x = 0;
	De.y = 0;
	
	if(De.frame == 1)
	{
		mirrorImage(12, 10, Micheal_Map, 0, De.x, De.y);
	} else
	{
		drawImage(12, 10, De.x, De.y, Micheal_Map, 0);
		De.frame = 0;
	}
}


int summonDemon()
{
	Micheal();
}


