//this controlls the map stuff
#include <math.h>

#include "levelAssets/tree1.c"
#include "levelAssets/tree2.c"
#include "levelAssets/Road.c"


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
		drawImage(16, 18, 0, 0, tree2_Map, 0);
		drawImage(16, 18, 0, 16, tree2_Map, 0);
		drawImage(16, 18, 0, 32, tree2_Map, 0);
		drawImage(16, 18, 0, 48, tree2_Map, 0);
		drawImage(16, 18, 0, 64, tree2_Map, 0);
		drawImage(16, 18, 102, 0, tree2_Map, 0);
		drawImage(16, 18, 102, 16, tree2_Map, 0);
		drawImage(16, 18, 102, 32, tree2_Map, 0);
		drawImage(16, 18, 102, 48, tree2_Map, 0);
		drawImage(16, 18, 102, 64, tree2_Map, 0);
		}
		break;
	}
}
