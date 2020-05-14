#pragma once
#include "vars.h"

class Brick
{
public:
	bool active;
	short row, col, hit;
	float w, h;
	Brick(short r_, short c_, float w_, float h_, short hit_): row(r_), col(c_), hit(hit_), w(w_), h(h_), active(false){};
	Brick(): row(), col(), w(70), h(30), hit(1), active(false){};
};
