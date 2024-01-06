//this controlls the map stuff
#include <math.h>

#include "levelAssets/tree1.c"
#include "levelAssets/tree2.c"
#include "levelAssets/Road.c"
#include "levelAssets/keyOutside.c"
#include "levelAssets/House.c"
#include "levelAssets/InsideKey.c"
//#include "levelAssets/HouseInside/X1Y1.c"


int playerLoc(int X, int Y) {
	clearBackground(0, 0, 0);
	switch(Y)
	{
		case 0:
		drawImage(120, 15, 0, 55, road_Map, 0);
		break;
		
		case 6:
		if(X == 0)
		{
		drawImage(120, 80, 0, 0, House_Map, 0);
		}
		if(X == 3)
		{
		drawImage(120, 80, 0, 0, keyOutside_Map, 0);
		}
		break;
	}
	if(X==30)
	{
	drawImage(120, 80, 0, 0, InsideKey_Map, 0);
	}
	if(X==40 && Y==40)
	{
	drawImage(120, 80, 0, 0, InsideKey_Map, 0);
	}
}
