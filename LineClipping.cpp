// 
// Name: Rabb, Christopher 
// Project: #2 
// Due: Thursday, May 15, 2014 
// Course: cs-445-01-sp14 
// 
// Description: 
// The program creates two viewports and draws a line while implementing
// two line clipping algorithms. Boxes are drawn to show where clipping
// is happening. The left side is OpenGL and the right is Cohen Sutherland.
#ifdef _WIN32
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

class wcPt2D
{
public:
	GLfloat x, y;
};

inline GLint Myround(const GLfloat a)
{
	return GLint(a + .05);
}

const GLint winLeftBitCode = 0x1;
const GLint winRightBitCode = 0x2;
const GLint winBottomBitCode = 0x4;
const GLint winTopBitCode = 0x8;

inline GLint inside(GLint code) { return GLint(!code); }
inline GLint reject(GLint code1, GLint code2) { return GLint(code1 & code2); }
inline GLint accept(GLint code1, GLint code2){ return GLint(!(code1 | code2)); }
inline void setPixel(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}
inline float fabsolute(float a)
{
	if (a < 0)
		return a *= -1.0;
	return a;
}
inline void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	int dx = fabsolute(float(xEnd - x0)), dy = fabsolute(float(yEnd - y0));
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
	int x, y;

	if (x0 > xEnd)
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

	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}
GLubyte encode(wcPt2D pt, wcPt2D winMin, wcPt2D winMax)
{
	GLubyte code = 0x00;

	if (pt.x < winMin.x)
		code = code | winLeftBitCode;
	if (pt.x > winMax.x)
		code = code | winRightBitCode;
	if (pt.y < winMin.y)
		code = code | winTopBitCode;
	return (code);
}
void swapPts(wcPt2D * p1, wcPt2D * p2)
{
	wcPt2D tmp;

	tmp = *p1; *p1 = *p2; *p2 = tmp;
}
void swapCodes(GLubyte * c1, GLubyte *c2)
{
	GLubyte tmp;

	tmp = *c1; *c1 = *c2; *c2 = tmp;
}
void lineClipCohSuth(wcPt2D winMin, wcPt2D winMax, wcPt2D p1, wcPt2D p2)
{
	GLubyte code1, code2;
	GLint done = false, plotLine = false;
	GLfloat m;

	while (!done)
	{
		code1 = encode(p1, winMin, winMax);
		code2 = encode(p2, winMin, winMax);
		if (accept(code1, code2))
		{
			done = true;
			plotLine = true;
		}
		else
		{
			if (reject(code1, code2))
				done = true;
			else
			{	//Label the endpoint outside the display window as p1
				if (inside(code1))
				{
					swapPts(&p1, &p2);
					swapCodes(&code1, &code2);
				}
				//Use slope m to find line-clipEdge intersection
				if (p2.x != p1.x)
					m = (p2.y - p1.y) / (p2.x - p1.x);

				if (code1 & winLeftBitCode)
				{
					p1.y += (winMin.x - p1.x) * m;
					p1.x = winMin.x;
				}
				else
				{
					if (code1 & winRightBitCode)
					{
						p1.y += (winMax.x - p1.x) * m;
						p1.x = winMax.x;
					}
					else
					{
						if (code1 & winBottomBitCode)
						{
							//Need to update p1.x for nonvertical lines only
							if (p2.x != p1.x)
								p1.x += (winMin.y - p1.y) / m;
							p1.y = winMin.y;
						}
						else
						{
							if (code1 & winTopBitCode)
							{
								if (p2.x != p1.x)
									p1.x += (winMax.y - p1.y) / m;
								p1.y = winMax.y;
							}
						}
					}
				}
			}
		}
	}
	if (plotLine)
	{
		lineBres(Myround(p1.x), Myround(p1.y), Myround(p2.x), Myround(p2.y));
		glColor3f(0, 0, 1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(winMin.x, winMin.y);
		glVertex2f(winMax.x, winMin.y);
		glVertex2f(winMax.x, winMax.y);
		glVertex2f(winMin.x, winMax.y);
		glEnd();
	}

}
inline void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 300.0, 0, 300.0);
}
inline void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(50, 100, 200, 400);
	glMatrixMode(GL_PROJECTION);
	//gluOrtho2D(1.0, 1.0, 1.0, 1.0);
	glColor3f(5, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(-1000, -1000);
	glVertex2i(1000, 1000);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(1, 1);
	glVertex2f(299, 1);
	glVertex2f(299, 299);
	glVertex2f(1, 299);
	glEnd();

	wcPt2D winMin, winMax, pt1, pt2;
	winMin.x = 50;
	winMin.y = 50;
	winMax.x = 250;
	winMax.y = 250;
	pt1.x = -1000;
	pt1.y = -1000;
	pt2.y = 1000;
	pt2.x = 1000;


	glViewport(300, 0, 300, 600);
	glColor3f(0.0, 0.0, 0.0);
	lineClipCohSuth(winMin, winMax, pt1, pt2);

	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("CS44 Christopher Rabb - Project 2");

	init();
	glutDisplayFunc(lineSegment);

	glutMainLoop();
}
