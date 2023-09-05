#include <iostream>
using namespace std;
#include <GL/glut.h>
const int WSx = 800; // Window size x
const int WSy = 800; // Window size y
const int WPx = 100; // Window position x
const int WPy = 100; // Window position y
const int SSx = 1536; // Screen size x
const int SSy = 894; // Screen size y
const int MAX = 1000; // Maximum number of vertices
int xo, yo, radius;

void myInit(void)
{
	glClearColor(1.0, 1.0, 0.0, 0.0); // Yellow
	glColor3f(0.0f, 0.0f, 0.0f); // Black
	glPointSize(1.0); // 1 point = 4 pixels
	glMatrixMode(GL_PROJECTION); // Matrix projection mode
	glLoadIdentity(); // Identity matrix
	gluOrtho2D(-WSx / 2, WSx / 2, -WSy / 2, WSy / 2); // Determines the origin
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
void circlePoints(int x, int y) {
	glVertex2i(xo + x, yo + y);
	glVertex2i(xo - x, yo + y);
	glVertex2i(xo + x, yo - y);
	glVertex2i(xo - x, yo - y);
	glVertex2i(xo + y, yo + x);
	glVertex2i(xo - y, yo + x);
	glVertex2i(xo + y, yo - x);
	glVertex2i(xo - y, yo - x);
}
void display(void) {
	cout << "display";
	glClear(GL_COLOR_BUFFER_BIT);
	// Grid lines
	grid();

	// Bresenham Circle Drawing Algorithm
	glBegin(GL_POINTS);
	int x = 0;
	int y = radius;
	double d = 3 - 2 * radius;
	glVertex2i(xo + x, yo + y);
	while (y > x) {
		if (d < 0) {
			d += 4 * x + 6;
		}
		else {
			d += 4 * (x - y) + 10;
			y--;
		}
		circlePoints(x, y);
		x++;
	}
	glEnd();
	// Plotting origin
	glPointSize(4.0);
	glBegin(GL_POINTS);
	glVertex2i(xo, yo);
	glEnd();
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv); // necessary
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Static | RGB
	glutInitWindowSize(WSx, WSy); // Win size
	// Win position (origin top_left)
	glutInitWindowPosition(SSx / 2 - WSx / 2 - 100, SSy / 2 - WSy / 2 - 100);
	glutCreateWindow("Assignment 2"); // Create win and title

	cout << "Enter radius of circle: "; cin >> radius;
	radius = ((int)radius - 2 % WSx) + 2;
	cout << "Enter coordinates of origin" << endl;
	cout << "Enter x: "; cin >> xo;
	cout << "Enter y: "; cin >> yo;

	myInit(); // Initializing
	//glutMouseFunc(mouseHandler);
	glutDisplayFunc(display); // passes a func as argument
	glutMainLoop();

	return 0;
}

