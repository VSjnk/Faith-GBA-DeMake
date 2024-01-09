//this controlls the map stuff
#include <math.h>

#include "levelAssets/tree1.c"
#include "levelAssets/tree2.c"
#include "levelAssets/Road.c"
#include "levelAssets/keyOutside.c"
#include "levelAssets/House.c"
#include "levelAssets/InsideKey.c"
#include "levelAssets/HouseInside/X1Y1.c"
#include "levelAssets/outsidePreset.c"
#include "levelAssets/outsidePreset2.c"




int playerLoc(int X, int Y) {
	
int out = 1;	
	
	
	
	switch(Y)
	{
		case 0:
		drawImage(120, 80, 0, 0, road_Map, 0);
		setColision(0, 0, 120, 81);
		out = 0;
		break;
		
		case 6:
		if(X == 0)
		{
		drawImage(120, 80, 0, 0, House_Map, 0);
		setColision(20, 0, 120, 80);
		out = 0;
		}
		if(X == 3)
		{
		out = 0;
		drawImage(120, 80, 0, 0, keyOutside_Map, 0);
		setColision(20, 0, 120, 80);
		}
		break;
	}
	
	if(X==30)
	{
	out = 0;
	drawImage(120, 80, 0, 0, InsideKey_Map, 0);
	setColision(46, 24, 67, 57);
	key(64, 32);
	}
	if(X==40)
	{
	out = 0;
	drawImage(120, 80, 0, 0, X1Y1, 0);
	setColision(0, 0, 120, 80);
	}
	
	if(out==1)
	{
	drawImage(120, 80, 0, 0, Outside_Map, 0);
	
		if(Y != 5)
		{
		setColision(42, 0, 75, 81);
		}
		else
		{
		if(X<=2)
		{
		drawImage(60, 30, 60, 15, outsidePath_Map, 0);
		}
		if(X>=1)
		{
		drawImage(60, 30, 0, 15, outsidePath_Map, 0);
		}
		setColision(0, 0, 120, 81);
		}
	}
}
