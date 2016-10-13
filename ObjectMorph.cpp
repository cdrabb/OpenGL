// 
// Name: Rabb, Christopher
// Homework: # 3
// Due: Thursday, May 22, 2014 
// Course: cs-445-01-sp14 
// 
// Description: 
// This program has an 1 of 5 various polyhedra rotating in the middle of the screen. 
//Right clicking the screen freezes the animation and gives you the option to change the color, polyhedra, or state of the shape.
//Holding s or S will scale the shape and holding x, X, y, Y, z, or Z will change the perspective of the viewport.

#ifdef _WIN32
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

GLint winWidth = 600, winHeight = 600;
GLfloat x0 = 11.0, Y0 = 10.0, z0 = 10.0, scale = 2.0, rot = 0.0, red = 1.0, green = 0.0, blue = 0.0;
GLfloat xwMin = -40.0, ywMin = -60.0, xwMax = 40.0, ywMax = 60.0, dnear = 25.0, dfar = 200.0;
bool idle = false, wireframe = true, first = true;
GLuint cube, dodeca, tetra, octa, icosa, selected = cube;
const int FPS = 30;

enum MENU_TYPE
{
	MENU_FRONT,
	MENU_SPOT,
	MENU_BACK,
};

inline void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	gluLookAt(x0, Y0, z0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);
	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 50.0);
}
void animate(void)
{
	if (!idle) //This is how animation is toggled
	{
		rot += .01;
		glutPostRedisplay();
	}
}
inline void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(x0, Y0, z0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //x0, y0, and z0 can be changed
	glColor3f(red, green, blue);	//rgb values can be changed
	glScalef(scale, scale, scale);	//scale can be changed
	glutIdleFunc(animate);			//This must be called in order to animate

	glNewList(cube, GL_COMPILE);	//Lists for all of the polyhedra included,
	if (wireframe)					//either a polyhedra or solid object is drawn.
		glutWireCube(1.0);
	else
		glutSolidCube(1.0);
	glEndList();

	glNewList(dodeca, GL_COMPILE);
	if (wireframe)
		glutWireDodecahedron();
	else
		glutSolidDodecahedron();
	glEndList();
	
	glNewList(tetra, GL_COMPILE);
	if (wireframe)
		glutWireTetrahedron();
	else
		glutSolidTetrahedron();
	glEndList();
	
	glNewList(octa, GL_COMPILE);
	if (wireframe)
		glutWireOctahedron();
	else
		glutSolidOctahedron();
	glEndList();
	
	glNewList(icosa, GL_COMPILE);
	if (wireframe)
		glutWireIcosahedron();
	else
		glutSolidIcosahedron();
	glEndList();

	if (first)						//This makes sure there is a cube on screen initially
		selected = cube;
	first = false;

	glPushMatrix();
	glRotatef(rot, 0.0, 0.0, 1.0);
	glCallList(selected);			//Selected list is called
	glPopMatrix();

	glFlush();
}

inline void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
	winWidth = newWidth;
	winHeight = newHeight;
}

inline void menu(int item)
{
	glutPostRedisplay();		//For popup menu
	return;
}
inline void colorSubMenu(GLint colorOption)	//popup menu options for colors
{
	switch (colorOption)
	{
	case 1:
		red = 0.0; green = 0.0; blue = 1.0;
		break;
	case 2:
		red = 0.0; green = 1.0; blue = 0.0;
		break;
	case 3:
		red = 1.0; green = 0.0; blue = 0.0;
		break;
	case 4:
		red = 0.0; green = 0.0, blue = 0.0;
	}
	glutPostRedisplay();
}
inline void stateSubMenu(GLint stateOption)	//popup menu options for state
{
	switch (stateOption)
	{
	case 1:
		wireframe = false;
		break;
	case 2:
		wireframe = true;
	}
	glutPostRedisplay();
}
inline void polySubMenu(GLint polyOption)	//popup menu options for polyhedra
{
	switch (polyOption)
	{
	case 1:
		selected = cube;
		break;
	case 2:
		selected = dodeca;
		break;
	case 3:
		selected = tetra;
		break;
	case 4:
		selected = octa;
		break;
	case 5:
		selected = icosa;
	}
	glutPostRedisplay();
}

inline void mouseFcn(GLint button, GLint action, GLint x, GLint y)	//Toggles animation by left clicking
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (action == GLUT_DOWN && idle == true)
			idle = false;
		else if (action == GLUT_DOWN && idle == false)
			idle = true;
		break;
	}
	glutPostRedisplay();
}
inline void keyboard(unsigned char key, int x, int y)	//Keyboard input
{
	switch (key) 
	{
	case's':
		if (scale > 0)
			scale -= .3;
		break;
	case 'S':
		scale += .3;
		break;
	case 'x':
		x0 -= .2;
		break;
	case 'X':
		x0 += .2;
		break;
	case 'y':
		Y0 -= .2;
		break;
	case 'Y':
		Y0 += .2;
		break;
	case 'z':
		z0 -= .2;
		break;
	case 'Z':
		z0 += .2;
		break;
	case 'q':
		exit(0);
	}
	glutPostRedisplay();
}
inline void timer(int v)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, v);
}

void main(int argc, char** argv)
{
	GLint colorSub, stateSub, polySub;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("CS445 Christopher Rabb - Project 3");

	colorSub = glutCreateMenu(colorSubMenu);	//Menus and submenus are created
	glutAddMenuEntry("Blue", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Red", 3);
	glutAddMenuEntry("Black", 4);

	stateSub = glutCreateMenu(stateSubMenu);
	glutAddMenuEntry("Solid", 1);
	glutAddMenuEntry("Wireframe", 2);

	polySub = glutCreateMenu(polySubMenu);
	glutAddMenuEntry("Cube", 1);
	glutAddMenuEntry("Dodecahedron", 2);
	glutAddMenuEntry("Tetrahedron", 3);
	glutAddMenuEntry("Octahedron", 4);
	glutAddMenuEntry("Icosahedron", 5);

	glutCreateMenu(menu);
	glutAddSubMenu("Polyhedra", polySub);
	glutAddSubMenu("Color", colorSub);
	glutAddSubMenu("State", stateSub);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	cube = glGenLists(1);						//Generate lists for each polyhedra
	dodeca = glGenLists(1);
	tetra = glGenLists(1);
	octa = glGenLists(1);
	icosa = glGenLists(1);

	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(reshapeFcn);
	glutTimerFunc(100, timer, 0);
	glutMouseFunc(mouseFcn);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}
