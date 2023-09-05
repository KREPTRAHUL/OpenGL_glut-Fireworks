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
float ge = 0.5f; // Grid extra thin
int x1, y11, x2, y2, m;

void myInit(void)
{
	glClearColor(1.0, 1.0, 0.0, 0.0); // Yellow
	glColor3f(0.0f, 0.0f, 0.0f); // Black
	glPointSize(1.0); // 1 point = 4 pixels
	glMatrixMode(GL_PROJECTION); // Matrix projection mode
	glLoadIdentity(); // Identity matrix
	gluOrtho2D(-m / 2, m / 2, -m / 2, m / 2); // Determines the origin
}
void grid()
{
	glLineWidth(1.0);
	glColor3f(0.5, 0.0, 0.0);
	for (int i = -m / 2; i <= m / 2; ++i) {
		for (int j = -m / 2; j <= m / 2; ++j) {
			glBegin(GL_LINE_LOOP);
			glVertex2f(-ge + i, -ge + j);
			glVertex2f(ge + i, -ge + j);
			glVertex2f(ge + i, ge + j);
			glVertex2f(-ge + i, ge + j);
			glEnd();
		}
	}
	glLineWidth(1.5);
	glBegin(GL_LINES);
	glVertex2i(-m, 0);
	glVertex2i(m, 0); // X axis
	glVertex2i(0, -m);
	glVertex2i(0, m); // Y axis
	glEnd();
}
void display(void) {
	cout << "display";
	glClear(GL_COLOR_BUFFER_BIT);
	// Grid lines
	grid();

	// Swaps left and right ends if necessary
	glPointSize(WSx / m); // Setting vertex size as big as m
	glBegin(GL_POINTS);
	if (x1 > x2) {
		float temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y11;
		y11 = y2;
		y2 = temp;
	}
	// Calculating delta x,y
	float x, y;
	int dx = x2 - x1;
	int dy = y2 - y11;
	// Mid-point Line Drawing Algorithm
	if (dy <= dx) {
		int d = dy - dx / 2;
		x = x1;
		y = y11;
		while (x <= x2) {
			glVertex2i(x, y);
			if (d < 0) {
				cout << "1";
				d += dy;
			}
			else {
				cout << "2";
				d += (dy - dx);
				y++;
			}
			x++;
		}
	}
	else if(dx < dy) {
		int d = dx - dy / 2;
		x = x1;
		y = y11;
		while (y <= y2) {
			glVertex2i(x, y);
			if (d < 0) {
				cout << "1";
				d += dx;
			}
			else {
				cout << "2";
				d += (dx - dy);
				x++;
			}
			y++;
		}
	}
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

	cout << "Enter number of pixels: "; cin >> m;
	m = ((int)m - 2 % WSx) + 2;
	cout << "Enter x1(-400,400): "; cin >> x1;
	cout << "Enter y1(-400,400): "; cin >> y11;
	cout << "Enter x2(-400,400): "; cin >> x2;
	cout << "Enter y2(-400,400): "; cin >> y2;

	myInit(); // Initializing
	//glutMouseFunc(mouseHandler);
	glutDisplayFunc(display); // passes a func as argument
	glutMainLoop();

	return 0;
}

