#include "Enemigo.h"

extern int width;
extern int height;

Enemigo::Enemigo()
{
}

Enemigo::~Enemigo()
{
}

Enemigo::Enemigo(int x, int y)
{
	x_pos = x;
	y_pos = y;

	active = true;
	speed = 3;

	boxright = 60;
	boxleft= 10;
	boxheight = 15;
}

void Enemigo::set(int x, int y)
{
	x_pos = x + 450;
	y_pos = y + 150;

	active = true;
	speed = 1;
	boxright = 60;
	boxleft = 10;
	boxheight = 15;
}

void Enemigo::MoverUp(bool check)
{
	x_pos -= speed;
	if (y_pos < height + 70)
		y_pos -=  2;

}

void Enemigo::MoverDOWN(bool check)
{

	x_pos -= speed;
	if (y_pos < height - 70)
		y_pos +=  2;

}
