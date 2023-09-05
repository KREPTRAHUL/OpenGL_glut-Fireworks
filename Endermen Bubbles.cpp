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


#define MAX 1000 // Maximum number of vertices
#define MAX_BUBBLES 50 // MAX bombs at a time
#define MAX_STARS 500 // MAX stars at a time
int max_bubbles = 6;
const int max_stars = 500;
const float pi = 3.14159265359;
const float millisec_per_frame = 1000 / 60;
const float bubble_speed_lvl = 1;

typedef struct vert {
	float x;
	float y;
}vert;

typedef struct bubble {
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
bubble bubbles[MAX_BUBBLES];
int bubbleFront = 0;
int bubbleRear = 0;
void init_circle(bubble& B) {
	B.radius = 50.0;
	B.size = 0;
}
float xc[MAX];
float slope_c[MAX];


void circlePoints(float x, float y, float xo, float yo, bubble& B) {
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
	printf("doing");
	bubble B;
	init_circle(B);
	vert vertex;
	vertex.x = ((float)rand() / RAND_MAX * 600) - 300;
	vertex.y = ((float)rand() / RAND_MAX * 600) - 300;
	B.center = vertex;
	B.radius = ((float)rand() / RAND_MAX * 70) + 30;
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

	bubbles[bubbleRear] = B;
	bubbleRear = (bubbleRear + 1) % max_bubbles;
}
typedef struct star {
	vert center;
	float radius;
	vert points[12];
	int size;
	float tx;
	float ty;
	float t_amt;
	float to_inc;
}star;
star stars[MAX_STARS];
int starFront = 0;
int starRear = 0;
void init_star(star& S) {
	S.radius = 10.0;
	S.size = 0;
	S.t_amt = 0;
	S.to_inc = 0;
}
float xs[MAX];
float slope_s[MAX];


void starPoints(float angle, star& S, int divide) {
	angle = angle / 180 * pi;
	float tempx, tempy;
	vert vertex;
	tempx = (S.points[0].x - S.center.x) / divide;
	tempy = (S.points[0].y - S.center.y) / divide;
	vertex.x = tempx * cos(angle) - tempy * sin(angle) + S.center.x;
	vertex.y = tempx * sin(angle) + tempy * cos(angle) + S.center.y;
	S.points[S.size++] = vertex;
}
void createStar(float xo, float yo, int tx, int ty, int to_inc) {
	star newStar;
	init_star(newStar);
	vert vertex;
	vertex.x = xo; vertex.y = yo;
	newStar.center = vertex;
	newStar.tx = tx;
	newStar.ty = ty;
	newStar.to_inc = to_inc;
	vertex.x = xo; vertex.y = yo + newStar.radius;
	newStar.points[newStar.size++] = vertex;
	starPoints(36, newStar, 3); // 36 degree
	starPoints(72, newStar, 1); // 72 degree
	starPoints(108, newStar, 3); // 72 + 36 degree
	starPoints(144, newStar, 1); // 72 + 72 degree
	vertex.x = xo; vertex.y = yo - newStar.radius / 3;
	newStar.points[newStar.size++] = vertex;
	starPoints(-144, newStar, 1); // -72 - 72 degree
	starPoints(-108, newStar, 3); // -72 - 36 degree
	starPoints(-72, newStar, 1); // -72 degree
	starPoints(-36, newStar, 3); // -36 degree
	newStar.points[newStar.size++] = newStar.points[0];

	stars[starRear] = newStar;
	starRear = (starRear + 1) % max_stars;
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
void colorBubble() {
	for (int i = bubbleFront; i != bubbleRear; i = (i + 1) % max_bubbles) {
		bubble& B = bubbles[i];
		glColor3f(B.r, B.g, B.b);
		glBegin(GL_LINES);
		for (int j = 1; j < B.size; j++) {
			glVertex2f(B.points[j - 1].x, B.points[j - 1].y);
			glVertex2f(B.points[j].x, B.points[j].y);
		}
		glEnd();
	}
}
void colorStar() {
	// Scan-line Polygon Filling Algorithm
	float dy, dx;
	int k = 0;
	for (int i = starFront; i != starRear; i = (i + 1) % max_stars) {
		for (int j = 1; j < stars[i].size; j++) {
			dx = stars[i].points[j].x - stars[i].points[j - 1].x;
			dy = stars[i].points[j].y - stars[i].points[j - 1].y;
			if (dx == 0) slope_s[j] = 0.0;
			else if (dy == 0) slope_s[j] = 1.0;
			else if (dy != 0 && dy != 0) {
				slope_s[j] = (float)(dx / dy);
			}
		}
	}
	for (int y = -WSy; y < WSy; y++) {
		k = 0;
		for (int i = starFront; i != starRear; i = (i + 1) % max_stars) {
			for (int j = 1; j < stars[i].size; j++) {
				if ((stars[i].points[j - 1].y <= y && stars[i].points[j].y > y) ||
					(stars[i].points[j - 1].y > y && stars[i].points[j].y <= y)) {
					xs[k] = (float)(stars[i].points[j - 1].x + slope_s[j] * (y - stars[i].points[j - 1].y));
					k++;
				}
			}
		}
		edgeSort(xs, k);

		// Actual Coloring
		glColor3f(0, 0.5, 0);
		glBegin(GL_LINES);
		for (int i = 0; i < k; i += 2) {
			glVertex2i(xs[i], y);
			glVertex2i(xs[i + 1], y);
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
void translate_star(int t) {
	for (int i = starFront; i != starRear; i = (i + 1) % max_stars) {
		//printf("%d,%d) ", starFront, starRear);
		if (stars[i].t_amt <= 50) {
			stars[i].t_amt += stars[i].to_inc;
			stars[i].center.x += stars[i].tx;
			stars[i].center.y += stars[i].ty;
			for (int j = 0; j < stars[i].size; j++) {
				stars[i].points[j].x += stars[i].tx;
				stars[i].points[j].y += stars[i].ty;
			}
		}
		else {
			starFront = (starFront + 8) % max_stars;
			i = (i + 7) % max_stars;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(millisec_per_frame, translate_star, 0);
}
void translate_bubble(int t) {
	for (int i = bubbleFront; i != bubbleRear; i = (i + 1) % max_bubbles) {
		bubble& B = bubbles[i];
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

		// Collision
		for (int k = bubbleFront; k != bubbleRear; k = (k + 1) % max_bubbles) {
			bubble& B2 = bubbles[k];
			float d = pow(pow(B2.center.x - B.center.x, 2) + pow(B2.center.y - B.center.y, 2), 0.5);
			if (k != i && d <= B2.radius + B.radius) {
				float x = (B2.center.x + B.center.x) / 2;
				float y = (B2.center.y + B.center.y) / 2;

				float radius = B2.radius;
				createStar(x, y + radius, 0, +1, 1);
				createStar(x, y - radius, 0, -1, 1);
				createStar(x + radius, y, +1, 0, 1);
				createStar(x - radius, y, -1, 0, 1);
				float y_45 = pow(pow(radius, 2) / 2, 0.5);
				createStar(x + y_45, y + y_45, +1, +1, 0.3);
				createStar(x + y_45, y - y_45, +1, -1, 0.3);
				createStar(x - y_45, y + y_45, -1, +1, 0.3);
				createStar(x - y_45, y - y_45, -1, -1, 0.3);


				vert vertex;
				vertex.x = ((float)rand() / RAND_MAX * 600) - 300;
				vertex.y = ((float)rand() / RAND_MAX * 600) - 300;
				vertex.x = (int)(vertex.x - B2.center.x) % 400;
				vertex.y = (int)(vertex.y - B2.center.y) % 400;
				B2.center.x += vertex.x;
				B2.center.y += vertex.y;
				for (int l = 0; l < B2.size; l++) {
					B2.points[l].x += vertex.x;
					B2.points[l].y += vertex.y;
				}
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(millisec_per_frame, translate_bubble, 0);
}
void bubble_creation(int t) {
	int diff;
	if (bubbleFront <= bubbleRear) { diff = bubbleRear - bubbleFront; }
	else { diff = bubbleFront - bubbleRear; }
	for (int i = 0; i < max_bubbles - diff; i++) {
		createBubble();
	}

	glutPostRedisplay();
	glutTimerFunc(millisec_per_frame, bubble_creation, 0);
}
// ********************************************************************************************


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	// Grid lines
	grid();

	// Star Outline
	glColor3f(0, 0.6, 0);
	glBegin(GL_LINES);
	for (int i = starFront; i != starRear; i = (i + 1) % max_stars) {
		star S = stars[i];
		for (int j = 1; j < S.size; j++) {
			glVertex2f(S.points[j - 1].x, S.points[j - 1].y);
			glVertex2f(S.points[j].x, S.points[j].y);
		}
	}
	glEnd();
	colorStar();

	// BUBBLE Outline
	glColor3f(0, 0, 0);
	glPointSize(2);
	glBegin(GL_POINTS);
	for (int i = bubbleFront; i != bubbleRear; i = (i + 1) % max_bubbles) {
		bubble B = bubbles[i];
		for (int j = 0; j < B.size; j++) {
			glVertex2f(B.points[j].x, B.points[j].y);
		}
	}
	glEnd();
	// BUBBLE Color
	colorBubble();

	glutSwapBuffers();
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv); // necessary
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Static | RGB
	glutInitWindowSize(WSx, WSy); // Win size
	// Win position (origin top_left)
	glutInitWindowPosition(WPx, WPy);
	glutCreateWindow("Assignment 6_- ENDERMAN"); // Create win and title

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
	//glutTimerFunc(millisec_per_frame, bubble_creation, 0);
	glutTimerFunc(millisec_per_frame, translate_bubble, 0);
	glutTimerFunc(millisec_per_frame, translate_star, 0);
	glutMainLoop();

	return 0;
}


