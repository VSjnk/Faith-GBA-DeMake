//this controlls the map stuff
#include <math.h>

#include "levelAssets/tree1.c"
#include "levelAssets/Road.c"


int playerLoc(int X, int Y) {
	clearBackground(0, 0, 0);
	if(Y == 0)
	{
	drawImage(120, 15, 0, 55, road_Map, 0);
	}
	drawImage(32, 39, 0, 35, tree1_Map, 0);
}
