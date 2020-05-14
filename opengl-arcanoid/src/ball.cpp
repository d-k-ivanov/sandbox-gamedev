#include "ball.h"

void Ball::move() {
	x+=dx;
	y+=dy;
	if(x-r<=0)
	{
		x=r;
		dx*=-1;
	}
	else if(y-r<=0)
	{
		y=r;
		dy*=-1;
	}
	else if(x+r>=width)
	{
		x=width-r;
		dx*=-1;
	}
	else if(y+r>=r_y && y+r<=r_y+r_h && x>r_x && x<r_x+r_w)
	{
		y=r_y-r;
		dy*=-1;
	}
	else if(y>height)
		active=false;
}