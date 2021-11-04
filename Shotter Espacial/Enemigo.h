#pragma once
#include "Global.h"

using namespace std;


class Enemigo
{
public:
	Enemigo();
	~Enemigo();
	Enemigo(int, int);
	void set(int, int);
	void MoverUp(bool);
	void MoverDOWN(bool);

	int x_pos;
	int y_pos;

	bool active;
	int speed;

	//collision variables
	int boxright;
	int boxleft;
	int boxheight;
};

