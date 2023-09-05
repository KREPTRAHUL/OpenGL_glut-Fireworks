#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
using namespace std;
#include <math.h>
#include <GL/glut.h>
const int WSx = 800; // Window size x
const int WSy = 800; // Window size y
const int WPx = 700; // Window position x
const int WPy = 0; // Window position y
const int SSx = 1536; // Screen size x
const int SSy = 894; // Screen size y

void myInit(void)
{
	glClearColor(1.0, 1.0, 0.0, 0.0); // Yellow
	glColor3f(0.0f, 0.0f, 0.0f); // Black
	glPointSize(1.0); // 1 point = 4 pixels
	glMatrixMode(GL_PROJECTION); // Matrix projection mode
	glLoadIdentity(); // Identity matrix
	gluOrtho2D(-WSx / 2, WSx / 2, -WSy / 2, WSy / 2); // Determines the origin
}
// *******************************************************************************************************


#define MAX 10000 // Maximum number of vertices
#define MAX_BUBBLES 5000 // MAX bombs at a time
int max_bubbles = 10;
const float millisec_per_frame = 1000 / 60;
const float bubble_speed_lvl = 1;

typedef struct vert {
	float x;
	float y;
}vert;

typedef struct circle {
	vert center;
	float radius;
	vert points[MAX];
	int size;
	float velocity_x;
	float velocity_y;
	float r;
	float g;
	float b;
}bubble;
circle bubbles[MAX_BUBBLES];
int circleFront = 0;
int circleRear = 0;
void init_circle(circle& B) {
	B.radius = 50.0;
	B.size = 0;
}
float xc[MAX];
float slope_c[MAX];


void circlePoints(float x, float y, float xo, float yo, circle& B) {
	vert vertex;
	vertex.x = xo + x; vertex.y = yo + y;
	B.points[B.size++] = vertex;
	vertex.x = xo - x; vertex.y = yo + y;
	B.points[B.size++] = vertex;
	vertex.x = xo + x; vertex.y = yo - y;
	B.points[B.size++] = vertex;
	vertex.x = xo - x; vertex.y = yo - y;
	B.points[B.size++] = vertex;
	vertex.x = xo + y; vertex.y = yo + x;
	B.points[B.size++] = vertex;
	vertex.x = xo - y; vertex.y = yo + x;
	B.points[B.size++] = vertex;
	vertex.x = xo + y; vertex.y = yo - x;
	B.points[B.size++] = vertex;
	vertex.x = xo - y; vertex.y = yo - x;
	B.points[B.size++] = vertex;
}
void createBubble() {
	circle B;
	init_circle(B);
	vert vertex;
	vertex.x = ((float)rand() / RAND_MAX * 600) - 300;
	vertex.y = ((float)rand() / RAND_MAX * 600) - 300;
	B.center = vertex;
	B.radius = (float)rand() / RAND_MAX * 30 + 70;
	B.velocity_x = ((float)rand() / RAND_MAX * 10) - 5;
	B.velocity_y = ((float)rand() / RAND_MAX * 10) - 5;
	//printf("(%f,%f) ", B.velocity_x, B.velocity_y);
	B.r = (float)rand() / RAND_MAX * 0.6;
	B.g = (float)rand() / RAND_MAX * 0.6;
	B.b = (float)rand() / RAND_MAX * 0.6;
	// Mid-point Circle Drawing Algorithm
	float x = 0, y = B.radius;
	double d = 5 / 4 - B.radius;
	vertex.x += x; vertex.y += y;
	B.points[B.size++] = vertex;
	while (y >= x) {
		if (d < 0) {
			d += 2 * x + 3;
		}
		else {
			d += 2 * (x - y) + 5;
			y--;
		}
		circlePoints(x, y, B.center.x, B.center.y, B);
		x++;
	}

	bubbles[circleRear] = B;
	circleRear = (circleRear + 1) % max_bubbles;
}
// ***********************************************COLORING***********************************************


void edgeSort(float x[], int k) {
	float temp;
	for (int i = 0; i < k - 1; i++) {
		for (int j = 0; j < k - 1; j++) {
			if (x[j] > x[j + 1]) {
				temp = x[j];
				x[j] = x[j + 1];
				x[j + 1] = temp;
			}
		}
	}
}
void colorCircle() {
	for (int i = circleFront; i != circleRear; i = (i + 1) % max_bubbles) {
		circle& B = bubbles[i];
		glColor3f(B.r, B.g, B.b);
		glBegin(GL_LINES);
		for (int j = 1; j < B.size; j++) {
			glVertex2f(B.points[j - 1].x, B.points[j - 1].y);
			glVertex2f(B.points[j].x, B.points[j].y);
		}
		glEnd();
	}
}
// **********************************************************************************


void grid()
{
	glLineWidth(1.5);
	glColor3i(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(-WSx, 0);
	glVertex2i(WSx, 0); // X axis
	glVertex2i(0, -WSy);
	glVertex2i(0, WSy); // Y axis
	glEnd();
}
void translate_bubble(int t) {
	for (int i = circleFront; i != circleRear; i = (i + 1) % max_bubbles) {
		circle& B = bubbles[i];
		B.center.x += B.velocity_x * bubble_speed_lvl;
		B.center.y += B.velocity_y * bubble_speed_lvl;
		for (int j = 0; j < B.size; j++) {
			B.points[j].x += B.velocity_x * bubble_speed_lvl;
			B.points[j].y += B.velocity_y * bubble_speed_lvl;
		}

		if ((B.center.x > WSx / 2 - B.radius) || (B.center.x < -WSx / 2 + B.radius)) {
			B.velocity_x *= -1;
		}
		if ((B.center.y > WSy / 2 - B.radius) || (B.center.y < -WSy / 2 + B.radius)) {
			B.velocity_y *= -1;
		}

		//printf("(%f,) ", B.center.x);
		//printf("(%f,%f) ", B.velocity_x, B.velocity_y);
		/*
		// Collision
		for (int k = bubbleFront; k != bubbleRear; k = (k + 1) % max_bubbles) {
			bubble& B2 = bubbles[k];
			float d = pow(pow(B2.center.x - B.center.x, 2) + pow(B2.center.y - B.center.y, 2), 0.5);
			if (d <= B2.radius + B.radius) {
				float nx = (B2.center.x - B.center.x) / 1;
				float ny = (B2.center.y - B.center.y) / 1;
				float p = 2 * (B.velocity_x * nx + B.velocity_y * ny - B2.velocity_x * nx - B2.velocity_y * ny)
					/ (pow(B.radius, 3) + pow(B2.radius, 3));
				B.velocity_x = B.velocity_x - p * pow(B.radius,3) * nx;
				B.velocity_y = B.velocity_y - p * pow(B.radius, 3) * ny;
				B2.velocity_x = B2.velocity_x + p * pow(B2.radius, 3) * nx;
				B2.velocity_y = B2.velocity_y + p * pow(B2.radius, 3) * ny;


				B.center.x += B.velocity_x * bubble_speed_lvl;
				B.center.y += B.velocity_y * bubble_speed_lvl;
				B2.center.x += B2.velocity_x * bubble_speed_lvl;
				B2.center.y += B2.velocity_y * bubble_speed_lvl;

				for (int j = 0; j < B.size; j++) {
					B.points[j].x += B.velocity_x * bubble_speed_lvl;
					B.points[j].y += B.velocity_y * bubble_speed_lvl;
				}
				for (int j = 0; j < B2.size; j++) {
					B2.points[j].x += B2.velocity_x * bubble_speed_lvl;
					B2.points[j].y += B2.velocity_y * bubble_speed_lvl;
				}
				
			}
		}/*
				

				//printf("(%f,%f) ", B.velocity_x, B.velocity_y);
				//printf("(%f,%f) ", B2.velocity_x, B2.velocity_y);
				B.velocity_x = B.velocity_x - p * B.radius/10 * nx;
				B.velocity_y = B.velocity_y - p * B.radius/10 * ny;
				bubbles[k].velocity_x = B2.velocity_x + p * B2.radius / 10 * nx;
				bubbles[k].velocity_y = B2.velocity_y + p * B2.radius / 10 * ny;

				for (int j = 0; j < B.size; j++) {
					B.points[j].x += B.velocity_x * bubble_speed_lvl;
					B.points[j].y += B.velocity_y * bubble_speed_lvl;
				}
				for (int j = 0; j < B2.size; j++) {
					B2.points[j].x += B2.velocity_x * bubble_speed_lvl;
					B2.points[j].y += B2.velocity_y * bubble_speed_lvl;
				}
			}
		}*/
	}
	glutPostRedisplay();
	glutTimerFunc(millisec_per_frame, translate_bubble, 0);
}
// ********************************************************************************************


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	// Grid lines
	grid();

	// BUBBLE Outline
	glColor3f(0, 0, 0);
	glPointSize(2);
	glBegin(GL_POINTS);
	for (int i = circleFront; i != circleRear; i = (i + 1) % max_bubbles) {
		circle B = bubbles[i];
		for (int j = 0; j < B.size; j++) {
			glVertex2f(B.points[j].x, B.points[j].y);
		}
	}
	glEnd();
	// BUBBLE Color
	colorCircle();

	glutSwapBuffers();
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv); // necessary
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Static | RGB
	glutInitWindowSize(WSx, WSy); // Win size
	// Win position (origin top_left)
	glutInitWindowPosition(WPx, WPy);
	glutCreateWindow("Assignment 6_- BUBBLES"); // Create win and title

	max_bubbles++;
	srand(time(NULL)); // Generate random numbers w.r.t. time
	// Creating BUBBLES
	for (int i = 1; i < max_bubbles; i++) {
		createBubble();
	}
	//createBubble(200, 200);
	//createBubble(-100, -100);
	myInit(); // Initializing glut
	//glutMouseFunc(mouseHandler);
	glutDisplayFunc(display); // passes a func as argument
	//glutTimerFunc(millisec_per_creation, bubble_creation, 0);
	glutTimerFunc(millisec_per_frame, translate_bubble, 0);
	glutMainLoop();

	return 0;
}


