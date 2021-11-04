#pragma once

#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include "Global.h"	

class Nave
{
public:
	int x_pos;
	int y_pos;
	int boxright;
	int boxleft;
	int boxheight;
	int health;
	bool active;

	Nave(int, int);
	Nave();

	void MoverNaveIzq();

	void MoverNaveDer();

	void MoverNaveArriba();

	void MoverNaveAbajo();
};