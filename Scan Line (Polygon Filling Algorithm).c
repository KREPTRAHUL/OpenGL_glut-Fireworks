#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
const int WSx = 800; // Window size x
int WSy = 800; // Window size y
const int WPx = 100; // Window position x
const int WPy = 100; // Window position y
const int SSx = 1536; // Screen size x
const int SSy = 894; // Screen size y
#define MAX 1000 // Maximum number of vertices
#define BLACK 48
#define RED 49
#define BLUE 50
#define GREEN 51
float r = 0.0, g = 0.0, b = 0.0;
float time_increment = 0.0;

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
	glClearColor(1.0, 1.0, 0.0, 0.0); // Yellow
	glColor3f(0.0f, 0.0f, 0.0f); // Black
	glPointSize(1.0); // 1 point = 4 pixels
	glMatrixMode(GL_PROJECTION); // Matrix projection mode
	glLoadIdentity(); // Identity matrix
	gluOrtho2D(-WSx / 2, WSx / 2, -WSy / 2, WSy / 2); // Determines the origin
}
void idle(void) {
	if (time_increment <= WSy*2) {
		time_increment += 0.1;
	}
	glutPostRedisplay();
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
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex2i(-WSx, 0);
	glVertex2i(WSx, 0); // X axis
	glVertex2i(0, -WSy);
	glVertex2i(0, WSy); // Y axis
	glEnd();
}
void edgeSort(int k) {
	float temp;
	for (int i = 0; i < k - 1; i++) {
		for (int j = 0; j < k - 1; j++) {
			if (xi[j] > xi[j + 1]) {
				temp = xi[j];
				xi[j] = xi[j + 1];
				xi[j + 1] = temp;
			}
		}
	}
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	// Grid lines
	grid();
	// printf("%f", time_increment);

	// Scan-line Polygon Filling Algorithm
	vertices0[vertexCount0].x = vertices0[0].x;
	vertices0[vertexCount0].y = vertices0[0].y;
	float dy, dx;
	int k = 0;
	for (int i = 1; i <= vertexCount0; i++) {
		dx = vertices0[i].x - vertices0[i - 1].x;
		dy = vertices0[i].y - vertices0[i - 1].y;
		if (dx == 0) slope[i] = 0.0;
		else if (dy == 0) slope[i] = 1.0;
		else if (dy != 0 && dy != 0) {
			slope[i] = (float)(dx / dy);
		}
	}
	for (int y = -WSy; y < -WSy + time_increment; y++) {
		k = 0;
		for (int j = 1; j <= vertexCount0; j++) {
			if ((vertices0[j - 1].y <= y && vertices0[j].y > y) ||
				(vertices0[j - 1].y > y && vertices0[j].y <= y)) {
				xi[k] = (float)(vertices0[j - 1].x + slope[j] * (y - vertices0[j - 1].y));
				k++;
			}
		}
		edgeSort(k);

		// Actual Coloring
		glColor3f(r, g, b);
		glBegin(GL_LINES);
		for (int i = 0; i < k; i += 2) {
			glVertex2i(xi[i], y);
			glVertex2i(xi[i + 1], y);
		}
		glEnd();
	}

	// Scan-line Polygon Drawing Algorithm with mouseClick
	glBegin(GL_LINES);
	for (int i = 1; i <= vertexCount0; i++) {
		glVertex2i(vertices0[i - 1].x, vertices0[i - 1].y);
		glVertex2i(vertices0[i].x, vertices0[i].y);
		// printf("(%f,%f)", vertices0[i - 1].x, vertices0[i - 1].y);
	}
	glEnd();

	glFlush();
	glutSwapBuffers();
}
// Function that exits from program
void keyboard(unsigned char key,
	int x, int y) {
	switch (key) {
	case BLACK:
		r = 0.0;
		b = 0.0;
		g = 0.0;
		glutPostRedisplay(); // Refreshes window
		break;
	case RED:
		r = 1.0;
		b = 0.0;
		g = 0.0;
		glutPostRedisplay(); // Refreshes window
		break;
	case BLUE:
		r = 0.0;
		b = 1.0;
		g = 0.0;
		glutPostRedisplay(); // Refreshes window
		break;
	case GREEN:
		r = 0.0;
		b = 0.0;
		g = 1.0;
		glutPostRedisplay(); // Refreshes window
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); // necessary
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Static | RGB
	glutInitWindowSize(WSx, WSy); // Win size
	// Win position (origin top_left)
	glutInitWindowPosition(SSx / 2 - WSx / 2 - 100, SSy / 2 - WSy / 2 - 100);
	glutCreateWindow("Assignment 4"); // Create win and title

	printf("\nPress 0. BLACK");
	printf("\nPress 1. RED");
	printf("\nPress 2. BLUE");
	printf("\nPress 3. GREEN");

	myInit(); // Initializing
	glutMouseFunc(mouseHandler);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display); // passes a func as argument
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}

