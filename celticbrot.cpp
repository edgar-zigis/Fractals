//
//  main.cpp
//  Mandelbrot & Julia Sets
//
//  Created by Edgar Žigis on 29/11/13.
//  Copyright (c) 2013 Bio:Matic. All rights reserved.
//

#define GLUT_DISABLE_ATEXIT_HACK

#include "stdafx.h" // uncomment if you use Visual Studio
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <QuartzCore/QuartzCore.h>
#else
#include <gl/glut.h>
#include "wtypes.h"
#endif
#include <cmath>
#include <time.h>
#include <string>

int currScreenWidth, currScreenHeight, c = 0;

GLdouble zoomIntensityMandelbrot = 0.1; // speed of the zoom in Mandelbrot set
GLdouble maxXMandelbrot = 2.25f;
GLdouble minXMandelbrot = -0.75f;
GLdouble maxYMandelbrot = 1.25f;
GLdouble minYMaldenbrot = -1.25f;
GLdouble imaginaryXMandelbrot;
GLdouble imaginaryYMandelbrot;

GLdouble r, g, b, tr, tg, tb; // coloring parameters

typedef struct
{
	GLdouble x;
	GLdouble y;
}
complex;    // definition of our complex number


void renderCelticbrot(void)
{
	int x, y, cnt, maxIter;
	complex z, t, t2;
	r = g = b = 256.0;
	maxIter = 256.0;
	imaginaryXMandelbrot = (maxXMandelbrot - minXMandelbrot) / (GLdouble)currScreenWidth;
	imaginaryYMandelbrot = (maxYMandelbrot - minYMaldenbrot) / (GLdouble)currScreenHeight;

	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	glBegin(GL_POINTS);

	for (x = 0, z.x = minXMandelbrot; x < currScreenWidth; x++, z.x += imaginaryXMandelbrot)
	{                                                                                       // z = x + i*y;
		for (y = 0, z.y = minYMaldenbrot; y < currScreenHeight; y++, z.y += imaginaryYMandelbrot)
		{
			t2 = z;
			cnt = 0;

			while ((t2.x * t2.x + t2.y * t2.y <= 4) && (cnt < maxIter))
			{

				t.x = fabs(t2.x*t2.x - t2.y*t2.y) - z.x;
				t2.y = 2 * t2.x*t2.y - z.y;
				t2.x = t.x;

				cnt++;
			}

			if (cnt != maxIter)     // color generator, may be freely modified
			{
				if (cnt >= 0 && cnt <= 31) { b = cnt * 4; g = cnt * 8; r = 0; }
				else if (cnt >= 32 && cnt <= 63) { b = 200; g = 500 - cnt * 8; r = 0; }
				else if (cnt >= 64 && cnt <= 95) { b = 200; g = 0; r = (cnt - 64) * 4; }
				else if (cnt >= 96 && cnt <= 127) { r = 200; g = 0; b = 1000 - cnt * 8; }
				else if (cnt >= 128 && cnt <= 159) { r = 200; g = (cnt - 128) * 8; b = 0; }
				else if (cnt >= 160 && cnt <= 191) { g = 200; r = 1500 - cnt * 8; b = 0; }
				else if (cnt >= 192 && cnt <= 223) { g = 200; r = 0; b = (cnt - 192) * 8; }
				else if (cnt >= 224 && cnt <= 255) { g = 230; r = (cnt - 224) * 8; b = 256; }

				tr = r; tb = b; tg = g;
				switch (c)
				{
				case 0:
					break;
				case 1: r = tb; b = tr;
					break;
				case 2: r = tg; g = tr;
					break;
				case 3: b = tg; g = tb;
					break;
				case 4: r = tg; g = tb; b = tr;
					break;
				case 5: r = tb; g = tr; b = tg;
					break;
				}
				glColor3f(r / 256, g / 256, b / 256);
				glVertex3d(x - currScreenWidth / 2, y - currScreenHeight / 2, 0.0f);
			}
		}
	}

	glEnd();

	glPopMatrix();
	glutSwapBuffers();
}
void mandelbrotSpecKeyboardControl(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		maxYMandelbrot += zoomIntensityMandelbrot;
		minYMaldenbrot += zoomIntensityMandelbrot;
	}
	if (key == GLUT_KEY_DOWN)
	{
		maxYMandelbrot -= zoomIntensityMandelbrot;
		minYMaldenbrot -= zoomIntensityMandelbrot;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		maxXMandelbrot += zoomIntensityMandelbrot;
		minXMandelbrot += zoomIntensityMandelbrot;
	}
	if (key == GLUT_KEY_LEFT)
	{
		maxXMandelbrot -= zoomIntensityMandelbrot;
		minXMandelbrot -= zoomIntensityMandelbrot;
	}

	glutPostRedisplay();
}

void mandelbrotKeyboardControl(unsigned char key, int x, int y)
{

	GLdouble zoomCoef = 0.833333333333333333333333;

	if (key == 'q' || key == 'Q')
	{
		if (maxXMandelbrot - minXMandelbrot>3 * zoomIntensityMandelbrot)
		{
			maxXMandelbrot -= zoomIntensityMandelbrot;
			minXMandelbrot += zoomIntensityMandelbrot;
			maxYMandelbrot -= zoomIntensityMandelbrot * zoomCoef;
			minYMaldenbrot += zoomIntensityMandelbrot * zoomCoef;
		}
		else
			zoomIntensityMandelbrot = zoomIntensityMandelbrot / 10;
	}
	if (key == 'a' || key == 'A')
	{
		maxXMandelbrot += zoomIntensityMandelbrot;
		minXMandelbrot -= zoomIntensityMandelbrot;
		maxYMandelbrot += zoomIntensityMandelbrot * zoomCoef;
		minYMaldenbrot -= zoomIntensityMandelbrot * zoomCoef;
	}
	if (key == 'c' || key == 'C')
	{
		c++;
		if (c == 6)
			c = 0;
	}
	if (key == 'w')
	{
		zoomIntensityMandelbrot /= 10.0f;
	}
	if (key == 's')
	{
		zoomIntensityMandelbrot *= 10.0f;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	currScreenWidth = w;
	currScreenHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -w / 2, w / 2, -w / 2, w / 2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 1.0f, 0.0f);
}

void GetDesktopResolution(int& horizontal, int& vertical)   // uncomment if you use Windows OS
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

/*void GetDesktopResolution(int& horizontal, int& vertical)   // uncomment if you use Mac OS
{
	CGRect mainMonitor = CGDisplayBounds(CGMainDisplayID());
	horizontal = CGRectGetWidth(mainMonitor);
	vertical = CGRectGetHeight(mainMonitor);
}
*/

int main(int argc, char* argv[])
{

	int width;
	int height;
	GetDesktopResolution(width, height);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);

	glutCreateWindow("Edgaro Žigio Mandelbroto aibė");
	glutPositionWindow((width - 700) / 2, (height - 700) / 2);
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(renderCelticbrot);
	glutSpecialFunc(mandelbrotSpecKeyboardControl);
	glutKeyboardFunc(mandelbrotKeyboardControl);
	glutMainLoop();

	return 0;
}