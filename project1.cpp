#include <stdlib.h>
#include <GL/glut.h>
#include <ctime>
#include <iostream>

using namespace std;

int round(const float a);
void init(void);
void lineSegment(void);
void lineDDA(int x0, int y0, int xEnd, int yEnd);
void lineBres(int x0, int y0, int xEnd, int yEnd);
void setPixel(int x, int y);
void timerFunc(int v);
GLint FPS = 15;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("CS44 Christopher Rabb - Project 1");

	init();

	glutDisplayFunc(lineSegment);

	glutMainLoop();
}
void init(void)
{
	glClearColor (1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}
void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	clock_t start;
	float duration;

	start = clock();
	for(int i = 0; i < 10000; i++)
	{
		glBegin(GL_LINES);
			glVertex2i(10, 10);
			glVertex2i(180, 145);
		glEnd();
	}
	duration = float (clock() - start) / CLOCKS_PER_SEC;
	cout << "OpenGL Line Drawing Algorithm: " << duration <<  " seconds" << endl;

	start = clock();
	for(int i = 0; i < 10000; i++)
	{
		lineBres(10, 10, 180, 135);
	}
	duration = float(clock() - start) / CLOCKS_PER_SEC;
	cout << "Bresenham Algorithm: " << duration <<  " seconds" << endl;

	start = clock();
	for(int i = 0; i < 10000; i++)
	{
		lineDDA(10, 10, 180, 125);
	}
	duration = float(clock() - start) / CLOCKS_PER_SEC;
	cout << "Line DDA Algorithm: " << duration <<  " seconds" << endl;

	glFlush();
}
void lineDDA(int x0, int y0, int xEnd, int yEnd)
{
	int dx = xEnd - x0, dy = yEnd - y0, steps, k;
	float xIncrement, yIncrement, x = x0, y = y0;

	if(fabs(float (dx)) > fabs(float (dy)))
		steps = fabs(float (dx));
	else
		steps = fabs(float(dy));
	xIncrement = float (dx) / float (steps);
	yIncrement = float (dy) / float (steps);

	setPixel(round(x), round(y));
	for(k = 0; k < steps; k++)
	{
		x += xIncrement;
		y += yIncrement;
		setPixel(round(x), round(y));
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	int dx = fabs(float(xEnd - x0)), dy = fabs(float(yEnd - y0));
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
	int x, y;

	if(x0 > xEnd)
	{
		x = xEnd;
		y = yEnd;
		xEnd = x0;
	}
	else
	{
		x = x0;
		y = y0;
	}
	setPixel(x, y);

	while(x < xEnd)
	{
		x++;
		if(p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
void setPixel(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(x,y);
