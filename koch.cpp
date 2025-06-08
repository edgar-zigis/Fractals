//
//  main.m
//  Koch's Snowflake
//
//  Created by ZET on 29/11/13.
//  Copyright (c) 2013 ZET. All rights reserved.
//
//#define GLUT_DISABLE_ATEXIT_HACK

#include "stdafx.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <QuartzCore/QuartzCore.h>
#else
#include <GL/glut.h>
#include "wtypes.h"
#endif
#include <stdlib.h>

using namespace std;

static int iteration = 0;
static long font = (long)GLUT_BITMAP_8_BY_13;
vector<float> starCoords;

class Source;

void writeData(void)
{
	char buffer[33];

	glColor3f(0.0, 1.0, 0.0);
	sprintf_s(buffer, "%d", iteration);
	glRasterPos3f(-70.0, -310.0, 0.0);

	char *c;

	for (c = (char*) "Kocho iteracija: "; *c != '\0'; c++)
		glutBitmapCharacter((void *)font, *c);

	for (c = (char*)buffer; *c != '\0'; c++)
		glutBitmapCharacter((void *)font, *c);
}

class Segment
{
public:
	Segment() {
		coords.clear();
		v.clear();
	}
	void drawKochCurve();

	friend class Source;

private:
	vector<float> coords;
	vector<Source> v;
};

void Segment::drawKochCurve()
{
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 5; i++)
		glVertex2f(coords[2 * i], coords[2 * i + 1]);
	glEnd();
}

class Source
{
public:
	Source(){ }
	Source(float coordsVal[4])
	{
		for (int i = 0; i < 4; i++)
			coords[i] = coordsVal[i];
	}

	void draw();
	Segment produceSegment();
	void produceKoch(int level);

	friend class Segment;

private:
	float coords[4];
};

void Source::draw()
{
	glBegin(GL_LINES);
	for (int i = 0; i < 2; i++)
		glVertex2f(coords[2 * i], coords[2 * i + 1]);
	glEnd();
}

Segment Source::produceSegment()
{
	float x0, y0, x1, y1, coordsVal[10], coordsVal1[4];
	int i, j;
	Source s;
	Segment seq = *new Segment();

	x0 = coords[0];
	y0 = coords[1];
	x1 = coords[2];
	y1 = coords[3];

	coordsVal[0] = x0;
	coordsVal[1] = y0;
	coordsVal[2] = 0.66666667*x0 + 0.33333333*x1;
	coordsVal[3] = 0.66666667*y0 + 0.33333333*y1;
	coordsVal[4] = 0.5*(x0 + x1) - 0.5*(1 / sqrt(3))*(y1 - y0);
	coordsVal[5] = 0.5*(y0 + y1) + 0.5*(1 / sqrt(3))*(x1 - x0);
	coordsVal[6] = 0.33333333*x0 + 0.66666667*x1;
	coordsVal[7] = 0.33333333*y0 + 0.66666667*y1;
	coordsVal[8] = x1;
	coordsVal[9] = y1;

	if (iteration == 5) {
		starCoords.push_back(x0);
		starCoords.push_back(y0);
	}

	for (i = 0; i < 10; i++)
	{
		seq.coords.push_back(coordsVal[i]);
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			coordsVal1[j] = coordsVal[2 * i + j];
		}
		s = *new Source(coordsVal1);
		seq.v.push_back(s);
	}

	return seq;
}

void Source::produceKoch(int level)
{
	glColor3f(0.0, 1.0, 0.0);
	if (iteration == 0)
		this->draw();
	else if (iteration == 1)
		this->produceSegment().drawKochCurve();
	else if (level < iteration - 1)
	for (int i = 0; i < 4; i++)
		this->produceSegment().v[i].produceKoch(level + 1);
	else
		this->produceSegment().drawKochCurve();
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);

	float coordsVal1[4] = { -210.0, -105.0, 0.0, static_cast<float>(-105.0 + sqrt(3)*0.5*420.0) };
	float coordsVal2[4] = { 0.0, static_cast<float>(-105.0 + sqrt(3)*0.5*420.0), 210.0, -105.0 };
	float coordsVal3[4] = { 210.0, -105.0, -210.0, -105.0 };

	Source src1 = *new Source(coordsVal1);
	Source src2 = *new Source(coordsVal2);
	Source src3 = *new Source(coordsVal3);

	writeData();

	src1.produceKoch(0);
	src2.produceKoch(0);
	src3.produceKoch(0);

	glFlush();
}

void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-350.0, 350.0, 350.0, -350.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		iteration++;
	if (key == GLUT_KEY_DOWN)
	if (iteration > 0) iteration--;
	glutPostRedisplay();
}

void onMouseButton(int button, int state, int x, int y)
{
	if (iteration == 5) {
		x -= 350;
		y -= 350;
		int distance = 700;
		int tempSum = 0;
		int targetX;
		int targetY;
		if (state == GLUT_DOWN) {
			for (int i = 0; i < starCoords.size() / 2 - 2; i++) {
				if ((int)starCoords.at(2 * i) != 0 && (int)starCoords.at(2 * i + 1) != 0) {

					if (x >= starCoords[2 * i] && y >= starCoords[2 * i + 1])
						tempSum = (x - starCoords[2 * i] + y - starCoords[2 * i + 1]);
					else if (x >= starCoords[2 * i] && y < starCoords[2 * i + 1])
						tempSum = (x - starCoords[2 * i] + starCoords[2 * i + 1] - y);
					else if (x < starCoords[2 * i] && y >= starCoords[2 * i + 1])
						tempSum = (starCoords[2 * i] - x + y - starCoords[2 * i + 1]);
					else
						tempSum = (starCoords[2 * i] - x + starCoords[2 * i + 1] - y);

					if (distance > tempSum && tempSum >= 0) {
						distance = tempSum;
						targetX = starCoords[2 * i];
						targetY = starCoords[2 * i + 1];
					}
				}
			}
			glColor3f(1.0, 0.0, 0.0);

			glBegin(GL_LINES);
			glVertex2f(x, y);
			glVertex2f(targetX, targetY);
			glEnd();
			glFlush();

			printf("Artimiausias atstumas: %d \n", distance);
		}
	}
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

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition((width - 700) / 2, (height - 700) / 2);
	glutCreateWindow("Edgaro Žigio Žvaigždė");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutMouseFunc(onMouseButton);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutMainLoop();

	return 0;
}
