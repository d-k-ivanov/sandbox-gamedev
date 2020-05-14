//#include "GL\glew.h"
#include <iostream>
#include <GL/freeglut.h>
#include <math.h>
#include "ball.h"
#include "brick.h"
#include "vars.h"

#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#define PI 3.1415926535897932384626433832795

int width = 1400;
int height = 900;
float r_x=(width/2)-75.0, r_y=900-50, r_w=150, r_h=10.0;
Ball ball(r_y, 10);
const int rows=12;
const int cols=20;
Brick brick[rows][cols];

void Init ()
{
	glClearColor( 0.2, 0.2, 0.2, 0.5 );
	glMatrixMode( GL_PROJECTION );
//	glMatrixMode(GL_MODELVIEW);

	glOrtho( 0, width, height, 0, 1, 0 );
}

void Render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glColor3f( 0.5, 1.0, 0.3 );
	glRectf( r_x, r_y, r_x+r_w, r_y+r_h );
	glColor3f( 0.6, 0.7, 1.0 );
	glBegin( GL_POLYGON );
		for( double i=0; i<2*PI; i+=PI/16 )
		{
			glVertex2f( ball.x+ball.r*cos(i), ball.y+ball.r*sin(i) );
		}
	glEnd();
	glBegin( GL_QUADS );
		for( int i=0; i<rows; i++ )
		{
			for( int j=0; j<cols; j++ )
			{
				Brick &b=brick[i][j];
				if( b.active )
				{
					glColor3f( 1.0, 0.0, 0.0 );
					glVertex2f( b.col*b.w+2, b.row*b.h+2 );
					glColor3f( 0.0, 1.0, 0.0 );
					glVertex2f( b.col*b.w+b.w-2, b.row*b.h+2 );
					glColor3f( 1.0, 0.0, 0.0 );
					glVertex2f( b.col*b.w+b.w-2, b.row*b.h+b.h-2 );
					glColor3f( 0.0, 0.0, 1.0 );
					glVertex2f( b.col*b.w+2, b.row*b.h+b.h-2 );
				}
			}
		}
	glEnd();
	glutSwapBuffers();
}

void Mouse( int ax, int ay )
{
	r_x=ax-r_w/2;
	if( !ball.active )
	{
		ball.x=r_x+r_w/2;
		ball.y=r_y-ball.r;
	}
}

void MousePressed( int button, int state, int x, int y )
{
	if( button==GLUT_LEFT_BUTTON && !ball.active ) {
		ball.active=true;
		ball.dx=10;
		ball.dy=-10;

	}
}

bool isCollision( Ball &b, Brick &br )
{
	float x=b.x, y=b.y, r=b.r, c=br.col, w=br.w, row=br.row, h=br.h;
	if( br.active )
	{
		if( abs(x-c*w-w/2-2)<=r+w/2 && abs(y-row*h-h/2-2)<=r+h/2 )
		{
			if( sqrt((c*w+w/2-x)*(c*w+w/2-x) + (row*h+h/2-y)*(row*h+h/2-y))-(w/2-1)*sqrt(2.0) - r > r*(sqrt(2.0)-1) )
				return 0;
			else
				return 1;
		}
	}
	return 0;
}

void Timer( int )
{
	if( ball.active )
	{
		ball.move();
		for( int i=0; i<rows; i++ )
		{
			for( int j=0; j<cols; j++ )
			{
				Brick &b=brick[i][j];
				if( isCollision(ball, b) )
				{
					if(abs(ball.x-b.col*b.w-b.w/2) < abs(ball.y-b.row*b.h-b.h/2)) {ball.dy*=-1;}
					else if(abs(ball.x-b.col*b.w-b.w/2) > abs(ball.y-b.row*b.h-b.h/2)) {ball.dx*=-1;}
					else {
						if(ball.dx>0) {if(ball.x < b.col*b.w+1) ball.dx*=-1;}
						else if(ball.x > (b.col+1)*b.w-1) ball.dx*=-1;
						if(ball.dy>0) {if(ball.y < b.row*b.h+1) ball.dy*=-1;}
						else if(ball.y > (b.row+1)*b.h-1) ball.dy*=-1;
					}
					if( --b.hit==0 )
					{
						b.active=false;
					}
					goto o;
				}
			}
		}
	}
o:	Render();
	glutTimerFunc(30, Timer, 0);
}

int main( int argc, char* argv[] )
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Arcanoid");
	glutDisplayFunc(Render);
	glutTimerFunc(30, Timer, 0);
	glutPassiveMotionFunc(Mouse);
	glutMouseFunc(MousePressed);
	Init();

	for( int i=0; i<rows; i++ )
	{
		for( int j=0; j<cols; j++ )
		{
			if( i!=0 && i!=1 && j!=0 && j!=cols-1 )
			{
			brick[i][j].active=true;
			brick[i][j].col=j;
			brick[i][j].row=i;
			}
		}
	}

	glutMainLoop();
	return 0;
}
