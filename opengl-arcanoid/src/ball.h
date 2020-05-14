#pragma once
#include "vars.h"

class Ball
{
public:
	float x, y, dx, dy, r;
	bool active;
	Ball(float y_, float r_): x(), y(y_), r(r_), dx(), dy(), active(false){}
	void move();
};
