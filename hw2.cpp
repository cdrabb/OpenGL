#ifdef _WIN32
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
//#include <GL/glut.h>
//#include <ctime>
//#include <iostream>
//using namespace std;

class wcPt2D{
	public:
		GLfloat x, y;
};

GLdouble theta = 1;
int stop = 0;
const int FPS = 30;
GLint nVerts = 3;
wcPt2D verts[3] = { { 0, 0 }, { 50.0, 100.0 }, { 100.0, 0 } };
wcPt2D * vertsRot = new wcPt2D[3];
void init(void);
void rotatePolygon(wcPt2D, GLint, wcPt2D, GLdouble);
void displayFcn(void);
void timer(int);
void keyboard(unsigned char, int, int);
void triangle(wcPt2D);


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("CS445 Christopher Rabb - Project 2");

	init();
	glutDisplayFunc(displayFcn);
	glutTimerFunc(100, timer, 0);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}
void rotatePolygon(wcPt2D * verts, GLint nVerts, wcPt2D pivPt, GLdouble theta)
{
	
	GLint k;
	if (!stop)
	for (k = 0; k < nVerts; k++)
	{
		vertsRot[k].x = pivPt.x + (verts[k].x - pivPt.x) * cos(theta) - (verts[k].y - pivPt.y) * sin(theta);
		vertsRot[k].y = pivPt.y + (verts[k].x - pivPt.x) * sin(theta) + (verts[k].y - pivPt.y) * cos(theta);
	}
	glBegin(GL_TRIANGLES);
	for (k = 0; k < nVerts; k++)
		glVertex2f(vertsRot[k].x, vertsRot[k].y);
	glEnd();
	verts = vertsRot;
}
void displayFcn(void)
{
	wcPt2D centroidPt;

	GLint k, xSum = 0, ySum = 0;
	for (k = 0; k < nVerts; k++)
	{
		xSum += verts[k].x;
		ySum += verts[k].y;
	}
	centroidPt.x = GLfloat(xSum) / GLfloat(nVerts);
	centroidPt.y = GLfloat(ySum) / GLfloat(nVerts);

	wcPt2D pivPt, fixedPt;
	pivPt = centroidPt;
	fixedPt = centroidPt;

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(50, 50, 0);
	rotatePolygon(verts, nVerts, pivPt, theta);
	glTranslatef(-50, -50, 0);
	glPopMatrix();

	glutSwapBuffers();

	glFlush();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key){
	case 'x':
		stop = !stop;
		break;
	case '1':
		theta = key;
		break;
	case '2':
		theta = key;
		break;
	case '3':
		theta = key;
		break;
	case '4':
		theta = key;
		break;
	case '5':
		theta = key;
		break;
	case '6':
		theta = key;
		break;
	case '7':
		theta = key;
		break;
	case '8':
		theta = key;
		break;
	case '9':
		theta = key;
		break;
	case '0':
		theta = key;
		break;
	default:
		break;
	}
}
void timer(int v)
{
	glutPostRedisplay();

	glutTimerFunc(1000 / FPS, timer, v); // re-arm timer....
}
void triangle(wcPt2D *verts)
{
	GLint k;

	glBegin(GL_TRIANGLES);
	for (k = 0; k < 3; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}
