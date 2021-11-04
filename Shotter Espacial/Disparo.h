#pragma once
#include "Nave.h"
#include "Enemigo.h"



class Disparo {

public:
	int x_pos;
	int y_pos;
	int radius;
	int status;
	int speed;
	bool isFriendly;
	Disparo(int , int , int , bool );
	Disparo();
	~Disparo();
	void Increment();
	void UpdateBulletPos(Nave&);
	void UpdatebulletPos(Enemigo&);
};