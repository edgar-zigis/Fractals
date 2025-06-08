//
//  main.cpp
//  IFS Tree
//
//  Created by Edgar Žigis on 29/11/13.
//  Copyright (c) 2013 Bio:Matic. All rights reserved.
//

#include "stdafx.h"
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <QuartzCore/QuartzCore.h>
#else
#include <GL/glut.h>
#include "wtypes.h"
#endif
#include <vector>

#define INIT_X -1.00
#define INIT_Y -1.00
#define FINAL_X 1.00
#define FINAL_Y 1.50

struct FractalPoint
{
public:
	float x;
	float y;
};

FractalPoint p;

std::vector<FractalPoint> _points;
std::vector<short> _colors;

float _slope_x, _slope_y, _intercept_x, _intercept_y;

void reshapeFunc(int w, int h)
{
	if (h == 0)
		h = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, ((float)w) / ((float)h), 1, 200);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0);
}

void displayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
	for (int i = 0; i < _points.size(); i++)
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(_points[i].x*_slope_x + _intercept_x,
			_points[i].y*_slope_y + _intercept_y);
		glEnd();

	}

	glFlush();
	glutSwapBuffers();
}

void init()
{
	glDisable(GLUT_DEPTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	p.x = 0.0f;
	p.y = 0.0f;
	_points.push_back(p);

	_slope_x = 700 / (FINAL_X - INIT_X);
	_slope_y = 700 / (FINAL_Y - INIT_Y);

	_intercept_x = -700 / 2 - (_slope_x)*INIT_X;
	_intercept_y = -700 / 2 - (_slope_y)*INIT_Y;

}

void update(int value)
{
	int _rand = rand() % 100;
	float _temp_x = p.x, _temp_y = p.y;


	if (_rand >= 0 && _rand <= 4)
	{
		p.x = 0.0f*_temp_x + 0.0f*_temp_y + 0.0f;
		p.y = 0.0f*_temp_x + 0.5f*_temp_y + 0.0f;
	}
	if (_rand >= 5 && _rand <= 19)
	{
		p.x = 0.1f*_temp_x + 0.0f*_temp_y + 0.0f;
		p.y = 0.0f*_temp_x + 0.1f*_temp_y + 0.2f;
	}
	if (_rand >= 20 && _rand <= 59)
	{
		p.x = 0.42f*_temp_x - 0.42f*_temp_y + 0.0f;
		p.y = 0.42f*_temp_x + 0.42f*_temp_y + 0.2f;
	}
	if (_rand >= 60 && _rand <= 99)
	{
		p.x = 0.42f*_temp_x + 0.42f*_temp_y + 0.0f;
		p.y = -0.42f*_temp_x + 0.42f*_temp_y + 0.2f;
	}

	_points.push_back(p);

	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

void GetDesktopResolution(int& horizontal, int& vertical)   // uncomment if you use Windows OS
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
/*
void GetDesktopResolution(int& horizontal, int& vertical)   // uncomment if you use Mac OS
{
	CGRect mainMonitor = CGDisplayBounds(CGMainDisplayID());
	horizontal = CGRectGetWidth(mainMonitor);
	vertical = CGRectGetHeight(mainMonitor);
}
*/
int main(int argc, char** argv)
{
	int width;
	int height;
	GetDesktopResolution(width, height);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition((width - 700) / 2, (height - 700) / 2);
	glutCreateWindow("Edgaro Žigio Medis");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	init();
	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutTimerFunc(5, update, 0);
	glutMainLoop();

	return 0;
}
