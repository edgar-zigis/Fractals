//
//  main.m
//  IFS Crystal
//
//  Created by ZET on 29/11/13.
//  Copyright (c) 2013 ZET. All rights reserved.
//
#define GLUT_DISABLE_ATEXIT_HACK

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <QuartzCore/QuartzCore.h>
#else
#include <GL/glut.h>
#include "wtypes.h"
#endif

#define POINTS_TO_ITERATE 2000000
#define WIN_WIDTH 700
#define WIN_HEIGHT 700

void display()
{
	int i;
	double param_table[][7] = {
		{ 0.255, 0.0, 0.0, 0.255, 0.3726, 0.6714, 0.2 },
		{ 0.255, 0.0, 0.0, 0.255, 0.1146, 0.2232, 0.15 },
		{ 0.255, 0.0, 0.0, 0.255, 0.6306, 0.2232, 0.15 },
		{ 0.370, -0.642, 0.642, 0.370, 0.6356, -0.0061, 0.75 } };


	float colors[][3] = { { 0.9, 0.1, 0.3 },
	{ 0.1, 0.4, 0.8 },
	{ 0.3, 0.75, 0.1 },
	{ 0.2, 0.3, 0.2 } };

	const int equations_to_iterate = sizeof(param_table) / sizeof(param_table[0]);
	double p[2] = { 0.0, 0.0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (i = 0; i<POINTS_TO_ITERATE; i++) {
		double old_p0 = p[0];
		double j = rand() / ((double)RAND_MAX + 1);
		double temp_prob_sum;
		int i;
		for (i = 0, temp_prob_sum = param_table[0][6];;) {
			if (j < temp_prob_sum) {
				glVertex2dv(p);
				p[0] = p[0] * param_table[i][0] + p[1] * param_table[i][1] + param_table[i][4];
				p[1] = old_p0*param_table[i][2] + p[1] * param_table[i][3] + param_table[i][5];
				glColor3fv(colors[i]);
				break;
			}
			if (i < equations_to_iterate - 1)
				temp_prob_sum += param_table[++i][6];
			else
				temp_prob_sum = 1.1;
		}
	}
	glEnd();
	glFlush();
}

void myinit() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPointSize(WIN_WIDTH*WIN_HEIGHT / POINTS_TO_ITERATE);
	glColor3f(0.0, 0.0, 1.0);
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

int main(int argc, char **argv)
{
	int width;
	int height;
	GetDesktopResolution(width, height);

	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition((width - 700) / 2, (height - 700) / 2);
	glutCreateWindow("Edgaro Žigio Kristalas");
	glutDisplayFunc(display);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	myinit();
	glutMainLoop();
	return 0;
}