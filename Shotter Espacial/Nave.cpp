#include "Nave.h"

extern int width;
extern int height;

Nave::Nave(int x, int y)
{
	x_pos = x;
	y_pos = y;	
	boxright = 60;
	boxleft = 60;
	boxheight = 50;
	health = 60;
	active = true;
}

Nave::Nave() {}


void Nave::MoverNaveIzq()
{
	if (x_pos >= 50)
		x_pos = x_pos - 7;
}

void Nave::MoverNaveDer()
{
	if (x_pos<width - 50)
		x_pos = x_pos + 7;
}

void Nave::MoverNaveArriba()
{
	if (y_pos < height + 70)
		y_pos = y_pos - 7;
}

void Nave::MoverNaveAbajo()
{
	if (y_pos < height - 70)
		y_pos = y_pos + 7;
}