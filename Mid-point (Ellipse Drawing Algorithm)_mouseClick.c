#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
const int WSx = 800; // Window size x
const int WSy = 800; // Window size y
const int WPx = 100; // Window position x
const int WPy = 100; // Window position y
const int SSx = 1536; // Screen size x
const int SSy = 894; // Screen size y
#define MAX 1000 // Maximum number of vertices
int xo, yo, a, b;

typedef struct vert {
	float x;
	float y;
}vert;
vert vertices0[MAX]; // Array of mouse button 0 vertices
int vertexCount0 = 0;
vert vertices2[MAX]; // Array of mouse button 2 vertices
int vertexCount2 = 0;

void myInit(void)
{
	glClearColor(1.0, 1.0, 0.0, 0.0); // Yellow
	glColor3f(0.0f, 0.0f, 0.0f); // Black
	glPointSize(1.0); // 1 point = 4 pixels
	glMatrixMode(GL_PROJECTION); // Matrix projection mode
	glLoadIdentity(); // Identity matrix
	gluOrtho2D(-WSx / 2, WSx / 2, -WSy / 2, WSy / 2); // Determines the origin
}
void mouseHandler(int btn, int state, int x, int y)
{
	/* btn = mouse button
	   state 0 = mouse down, state 1 = mouse up
	   x,y = click coordinates (origin bottom-left)*/
	if (state == 0 && btn == 0) {
		vertices0[vertexCount0].x = x - WSx / 2;
		vertices0[vertexCount0++].y = WSy / 2 - y;
		glutPostRedisplay(); // Refreshes window
	}
	if (state == 0 && btn == 2) {
		vertices2[vertexCount2].x = x - WSx / 2;
		vertices2[vertexCount2++].y = WSy / 2 - y;
		glutPostRedisplay(); // Refreshes window
	}
}

void grid()
{
	glLineWidth(1.5);
	glBegin(GL_LINES);
	glVertex2i(-WSx, 0);
	glVertex2i(WSx, 0); // X axis
	glVertex2i(0, -WSy);
	glVertex2i(0, WSy); // Y axis
	glEnd();
}
void ellipsePoints(int x, int y) {
	glVertex2i(xo + x, yo + y);
	glVertex2i(xo - x, yo + y);
	glVertex2i(xo + x, yo - y);
	glVertex2i(xo - x, yo - y);
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	// Grid lines
	grid();

	// Bresenham Circle Drawing Algorithm with mouseClick
	for (int i = 0; i < vertexCount0 - vertexCount0 % 2; i += 2) {
		glPointSize(1.0);
		glBegin(GL_POINTS);
		xo = vertices0[i].x;
		yo = vertices0[i].y;
		int x1 = vertices0[i + 1].x;
		int y1 = vertices0[i + 1].y;
		int a = abs(x1-xo);
		int b = abs(y1-yo);

		int x = 0;
		int y = b;
		double d1 = b * b - a * a * b + 0.25 * a * a;
		double d2 = b * b * (x + 0.5) * (x + 0.5) - a * a * (y - 1) * (y - 1) + a * a * b * b;
		glVertex2i(x + xo, y + yo);
		glVertex2i(xo, yo);
		// First Octant
		while (b * b * (x + 1) < a * a * (y - 0.5)) {
			if (d1 < 0) {
				d1 += b * b * (2 * x + 3);
			}
			else {
				d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
				y--;
			}
			x++;
			ellipsePoints(x, y);
		}
		// Second Octant
		while (y > 0) {
			if (d2 < 0) {
				d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
				x++;
			}
			else {
				d2 += a * a * (-2 * y + 3);
			}
			y--;
			ellipsePoints(x, y);
		}
		glEnd();
		// Plotting origin
		glPointSize(4.0);
		glBegin(GL_POINTS);
		glVertex2i(xo, yo);
		glEnd();
	}
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv); // necessary
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Static | RGB
	glutInitWindowSize(WSx, WSy); // Win size
	// Win position (origin top_left)
	glutInitWindowPosition(SSx / 2 - WSx / 2 - 100, SSy / 2 - WSy / 2 - 100);
	glutCreateWindow("Assignment 5"); // Create win and title

	myInit(); // Initializing
	glutMouseFunc(mouseHandler);
	glutDisplayFunc(display); // passes a func as argument
	glutMainLoop();

	return 0;
}

