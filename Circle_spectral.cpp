#include <stdio.h>
#include <iostream>
using namespace std;
#include <math.h>
#include <time.h>
#include <GL/glut.h>
const int WSx = 800; // Window size x
const int WSy = 800; // Window size y
const int WPx = 700; // Window position x
const int WPy = 0; // Window position y
const int SSx = 1536; // Screen size x
const int SSy = 894; // Screen size y
#define MAX 1000 // Maximum number of vertices
float xo, yo, radius;
float r, g, b;
float theta = 5;
const float sec_per_frame = 1000 / 8;

typedef struct vert {
	float x;
	float y;
}vert;
vert vertices0[MAX]; // Array of mouse button 0 vertices
int vertexCount0 = 0;
vert vertices2[MAX]; // Array of mouse button 2 vertices
int vertexCount2 = 0;

float xi[MAX];
float slope[MAX];

void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // Black
	glColor3f(0.0f, 0.0f, 0.0f); // Black
	glPointSize(8.0); // 1 point = 4 pixels
	glMatrixMode(GL_PROJECTION); // Matrix projection mode
	glLoadIdentity(); // Identity matrix
	gluOrtho2D(-WSx / 2, WSx / 2, -WSy / 2, WSy / 2); // Determines the origin
}
void grid()
{
	glLineWidth(1.5);
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2i(-WSx, 0);
	glVertex2i(WSx, 0); // X axis
	glVertex2i(0, -WSy);
	glVertex2i(0, WSy); // Y axis
	glEnd();
}
void addToVertices0(float x, float y, int i) {
	vertices0[i].x = x;
	vertices0[i].y = y;
}
void circlePoints(float x, float y) {
	addToVertices0(xo + x, yo + y, vertexCount0++);
	addToVertices0(xo - x, yo + y, vertexCount0++);
	addToVertices0(xo + x, yo - y, vertexCount0++);
	addToVertices0(xo - x, yo - y, vertexCount0++);
	addToVertices0(xo + y, yo + x, vertexCount0++);
	addToVertices0(xo - y, yo + x, vertexCount0++);
	addToVertices0(xo + y, yo - x, vertexCount0++);
	addToVertices0(xo - y, yo - x, vertexCount0++);
}
void drawCircle() {
	// Mid-point Circle Drawing Algorithm
	float x = 0;
	float y = radius;
	double d = 5 / 4 - radius;
	addToVertices0(x + xo, y + yo, vertexCount0++);
	while (y >= x) {
		if (d < 0) {
			d += 2 * x + 3;
		}
		else {
			d += 2 * (x - y) + 5;
			y--;
		}
		circlePoints(x, y);
		x++;
	}
}
void timer(int t) {
	// Mid-Point Circle Drawing Algorithm
	// 2-D Rotation
	float tempx, tempy, temp_xo;
	for (int i = 0; i < vertexCount0; i++) {
		tempx = vertices0[i].x;
		tempy = vertices0[i].y;
		vertices0[i].x = tempx * cos(theta) - tempy * sin(theta);
		vertices0[i].y = tempx * sin(theta) + tempy * cos(theta);
		temp_xo = xo;
		xo = xo * cos(theta) - yo * sin(theta);
		xo = temp_xo * sin(theta) + yo * cos(theta);
		theta += 0.01;
	}
	r = (float)rand() / (float)(RAND_MAX) * 0.5 + 0.5;
	g = (float)rand() / (float)(RAND_MAX) * 0.5 + 0.5;
	b = (float)rand() / (float)(RAND_MAX) * 0.5 + 0.5;

	glutPostRedisplay();
	glutTimerFunc(sec_per_frame, timer, 0);
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	// Grid lines
	grid();

	for (int i = 0; i < vertexCount0; i++) {
		glColor3f(r, g, b);
		glBegin(GL_POINTS);
		glVertex2i(vertices0[i].x, vertices0[i].y);
		glEnd();
	}

	glutSwapBuffers();
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv); // necessary
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Static | RGB
	glutInitWindowSize(WSx, WSy); // Win size
	// Win position (origin top_left)
	glutInitWindowPosition(WPx, WPy);
	glutCreateWindow("Assignment 6_- SPECTRAL"); // Create win and title

	radius = 150;
	xo = 100;
	yo = 100;

	srand((unsigned int)time(NULL));
	drawCircle();
	myInit(); // Initializing
	//glutMouseFunc(mouseHandler);
	glutDisplayFunc(display); // passes a func as argument
	glutTimerFunc(sec_per_frame, timer, 0);
	glutMainLoop();

	return 0;
}

