#include "Disparo.h"

extern int width;
extern int height;

Disparo::Disparo(int X, int Y,int S, bool friendly)
{
	x_pos = X;
	y_pos = Y;
	status = 0;
	speed = S;
	isFriendly = friendly;
}

Disparo::Disparo(){}

Disparo::~Disparo(){}

void Disparo::UpdateBulletPos(Nave &s)
{
	x_pos = s.x_pos;
	y_pos = s.y_pos + 35;
}

void Disparo::UpdatebulletPos(Enemigo &e)
{
	x_pos = e.x_pos + 30;
	y_pos = e.y_pos + 10;
}

void Disparo::Increment()
{
	if (isFriendly)
		x_pos += speed;
	else
		x_pos -= speed;

	if (x_pos<10 || x_pos >width +10)				//if bullet reaches top or bottom
		status = 0;
}