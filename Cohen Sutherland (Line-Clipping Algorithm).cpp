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
#define ESC 27
int xmin, xmax, ymin, ymax;
const int LEFT = 1, RIGHT = 2, TOP = 4, BOTTOM = 8;

typedef struct vert {
	float x;
	float y;
}vert;
vert vertices0[MAX]; // Array of mouse button 0 vertices
int vertexCount0 = 0;
vert vertices2[MAX]; // Array of mouse button 2 vertices
int vertexCount2 = 0;

float xi[MAX];
int edgeCount = 0;
float slope[MAX];

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
int getcode(int x, int y) {
	int code = 0;
	if (y > ymax) code = TOP;
	if (y < ymin) code = BOTTOM;
	if (x < xmin) code = LEFT;
	if (x > xmax) code = RIGHT;
	return code;
}
void window() {
	glVertex2i(xmin, ymin);
	glVertex2i(xmin, ymax);

	glVertex2i(xmin, ymax);
	glVertex2i(xmax, ymax);

	glVertex2i(xmax, ymax);
	glVertex2i(xmax, ymin);

	glVertex2i(xmax, ymin);
	glVertex2i(xmin, ymin);
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	// Grid lines
	grid();

	// Cohen Sutherland Line-Clipping Algorithm
	glBegin(GL_LINES);
	int outcode1 = 0, outcode2 = 0, accept = 0;
	float slope;
	if (vertices0[0].x > vertices0[1].x) {
		xmax = vertices0[0].x;
		xmin = vertices0[1].x;
	}
	else {
		xmax = vertices0[1].x;
		xmin = vertices0[0].x;
	}
	if (vertices0[0].y > vertices0[1].y) {
		ymax = vertices0[0].y;
		ymin = vertices0[1].y;
	}
	else {
		ymax = vertices0[1].y;
		ymin = vertices0[0].y;
	}
	// Clipping Window
	if(vertexCount0 >= 2)
		window();

	// Drawing Clipped Lines
	for (int i = 2; i < vertexCount0 - vertexCount0 % 2; i += 2) {
		//glVertex2i(vertices0[i].x, vertices0[i].y);
		//glVertex2i(vertices0[i + 1].x, vertices0[i + 1].y);

		outcode1 = getcode(vertices0[i].x, vertices0[i].y);
		outcode2 = getcode(vertices0[i + 1].x, vertices0[i + 1].y);
		accept = 0;
		while (1) {
			slope = (float)(vertices0[i + 1].y - vertices0[i].y) /
				(vertices0[i + 1].x - vertices0[i].x);
			if (outcode1 == 0 && outcode2 == 0) {
				accept = 1;
				break;
			}
			else if ((outcode1 & outcode2) != 0) {
				vertexCount0 -= 2;
				break;
			}
			else {
				int x, y, temp;
				if (outcode1 == 0) temp = outcode2;
				else temp = outcode1;
				if (temp & TOP) {
					x = vertices0[i].x + (ymax - vertices0[i].y) / slope;
					y = ymax;
				}
				else if (temp & BOTTOM) {
					x = vertices0[i].x + (ymin - vertices0[i].y) / slope;
					y = ymin;
				}
				else if (temp & LEFT) {
					x = xmin;
					y = vertices0[i].y + (xmin - vertices0[i].x) * slope;
				}
				else if (temp & RIGHT) {
					x = xmax;
					y = vertices0[i].y + (xmax - vertices0[i].x) * slope;
				}
				if (temp == outcode1) {
					vertices0[i].x = x;
					vertices0[i].y = y;
					outcode1 = getcode(x, y);
				}
				else {
					vertices0[i + 1].x = x;
					vertices0[i + 1].y = y;
					outcode2 = getcode(x, y);
				}
			}
		}
		if (accept) {
			glVertex2i(vertices0[i].x, vertices0[i].y);
			glVertex2i(vertices0[i + 1].x, vertices0[i + 1].y);
		}
	}
	glEnd();
	glFlush();
}
// Function that exits from program
void keyboard(unsigned char key,
	int x, int y) {
	switch (key) {
	case ESC:
		vertices0[vertexCount0].x = vertices0[0].x;
		vertices0[vertexCount0++].y = vertices0[0].y;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); // necessary
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Static | RGB
	glutInitWindowSize(WSx, WSy); // Win size
	// Win position (origin top_left)
	glutInitWindowPosition(SSx / 2 - WSx / 2 - 100, SSy / 2 - WSy / 2 - 100);
	glutCreateWindow("Assignment 4"); // Create win and title

	myInit(); // Initializing
	glutMouseFunc(mouseHandler);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display); // passes a func as argument
	glutMainLoop();

	return 0;
}

