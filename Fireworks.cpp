#include <stdio.h>
#include <stdlib.h>
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

void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // Black Background
	glColor3f(0.0f, 0.0f, 0.0f); // Black
	glPointSize(1.0); // 1 point = 4 pixels
	glMatrixMode(GL_PROJECTION); // Matrix projection mode
	glLoadIdentity(); // Identity matrix
	gluOrtho2D(-WSx / 2, WSx / 2, -WSy / 2, WSy / 2); // Determines the origin
}
// ******************************************************************************


#define MAX 1000 // Maximum number of vertices
#define MAX_BOMBS 100 // MAX bombs at a time
#define MAX_STARS 1000 // MAX stars at a time
const int max_bombs = 100;
const int max_stars = 1000;
const int max_night_sky = 50;
const float pi = 3.14159265359;
const float millisec_per_frame = 1000 / 60; // 60 fps
const float millisec_per_creation = 100; // of bombs
const float bomb_speed_lvl = 5; // lvl 1-10
int temp = 1; // for wobble effect

typedef struct vert { // vertex
	float x;
	float y;
}vert;

typedef struct circle { // CIRCLE
	vert center;
	float initial_y;
	float radius;
	vert points[MAX];
	int size;
	float speed;
}circle;
circle circles[MAX_BOMBS]; // Array of circles
int circleFront = 0;
int circleRear = 0;
void init_circle(circle* C) { // Initialize a circle
	C->radius = 0.0;
	C->initial_y = 0.0;
	C->size = 0;
	C->speed = 0.0;
}
float xc[MAX]; // Stores clipped scan lines
float slope_c[MAX]; // Stores slopes 
vert trail[MAX][2]; // trail of stars
circle moon; // THE MOON
circle cresent; // THE MOON CRESENT
vert nightSky[MAX]; // TWINKLING STARS


// 8-way symmetry of circle
// x,y : point
// xo,yo : center
// *C : pointer to circle
void circlePoints(float x, float y, float xo, float yo, circle* C) {
	vert vertex;
	vertex.x = xo + x; vertex.y = yo + y;
	C->points[C->size++] = vertex;
	vertex.x = xo - x; vertex.y = yo + y;
	C->points[C->size++] = vertex;
	vertex.x = xo + x; vertex.y = yo - y;
	C->points[C->size++] = vertex;
	vertex.x = xo - x; vertex.y = yo - y;
	C->points[C->size++] = vertex;
	vertex.x = xo + y; vertex.y = yo + x;
	C->points[C->size++] = vertex;
	vertex.x = xo - y; vertex.y = yo + x;
	C->points[C->size++] = vertex;
	vertex.x = xo + y; vertex.y = yo - x;
	C->points[C->size++] = vertex;
	vertex.x = xo - y; vertex.y = yo - x;
	C->points[C->size++] = vertex;
}
// to create circle with center xo,yo and radius
void createCircle(float xo, float yo, float radius) {
	circle newCircle;
	init_circle(&newCircle);
	vert vertex;
	// Mid-point Circle Drawing Algorithm
	float x = 0, y = radius;
	double d = 5 / 4 - radius;
	vertex.x = xo; vertex.y = yo;
	newCircle.center = vertex;
	newCircle.initial_y = yo;
	newCircle.radius = radius;
	vertex.x = x + xo; vertex.y = y + yo;
	newCircle.points[newCircle.size++] = vertex;
	while (y >= x) {
		if (d < 0) {
			d += 2 * x + 3;
		}
		else {
			d += 2 * (x - y) + 5;
			y--;
		}
		circlePoints(x, y, xo, yo, &newCircle);
		x++;
	}
	vertex.x = xo;
	vertex.y = yo - radius;
	int temp = 1;
	// Bomb trail stick
	for (int i = 0; i < 50; i++) {
		if (i % 5 == 0) { temp *= -1; }
		else if (i % 7 == 0) {}
		else { vertex.x += temp; }
		vertex.y -= 1;
		newCircle.points[newCircle.size++] = vertex;
	}

	circles[circleRear] = newCircle;
	circleRear = (circleRear + 1) % max_bombs;
}
// Creates Moon outline with center xo,yo and radius
void createMoon(float xo, float yo, float radius) {
	init_circle(&moon);
	moon.radius = radius;
	// Mid-point Circle Drawing Algorithm
	vert vertex;
	// MOON outline points
	vertex.x = xo; vertex.y = yo;
	moon.center = vertex;
	float x = 0, y = radius;
	double d = 5 / 4 - radius;
	vertex.x = x + xo; vertex.y = y + yo;
	moon.points[moon.size++] = vertex;
	while (y >= x) {
		if (d < 0) {
			d += 2 * x + 3;
		}
		else {
			d += 2 * (x - y) + 5;
			y--;
		}
		circlePoints(x, y, xo, yo, &moon);
		x++;
	}
	// CRESENT outline points
	vertex.x = xo - radius / 2; vertex.y = yo + radius / 2;
	cresent.center = vertex;
	float x2 = 0, y2 = radius;
	double d2 = 5 / 4 - radius;
	vertex.x = x2 + xo - radius; vertex.y = y2 + yo + radius;
	cresent.points[cresent.size++] = vertex;
	while (y2 >= x2) {
		if (d2 < 0) {
			d2 += 2 * x2 + 3;
		}
		else {
			d2 += 2 * (x2 - y2) + 5;
			y2--;
		}
		circlePoints(x2, y2, cresent.center.x, cresent.center.y, &cresent);
		x2++;
	}
}
// Stores star position in nightSky[]
void createNightSky() {
	for (int i = 0; i < max_night_sky; i++) {
		nightSky[i].x = ((float)rand() / RAND_MAX * WSx) - WSx / 2;
		nightSky[i].y = (float)rand() / RAND_MAX * WSy / 2;
	}
}
// *****************************************************************************************

typedef struct star { // STAR
	vert center;
	float initial_y;
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
void init_star(star* S) { // Initialize star
	S->radius = 0.0;
	S->size = 0;
	S->t_amt = 0;
	S->to_inc = 0;
}
float xs[MAX]; // Stores clipped scan lines of sparks
float slope_s[MAX]; // Stores slope


void starPoints(float angle, star* S, int divide) { //Stores rotated & scaled point in *S
	angle = angle / 180 * pi;
	float tempx, tempy;
	vert vertex;
	tempx = (S->points[0].x - S->center.x) / divide;
	tempy = (S->points[0].y - S->center.y) / divide;
	vertex.x = tempx * cos(angle) - tempy * sin(angle) + S->center.x;
	vertex.y = tempx * sin(angle) + tempy * cos(angle) + S->center.y;
	S->points[S->size++] = vertex;
}
// createStar(center.x, center.y, center.x.add, center.y.add, radius,
// translation.x, translation.y, translation_amt.increase_per_frame,
// trail_offset.x, trail_offset.y)
void createStar(float xo, float yo, float xa, float ya,
	float radius, int tx, int ty, int to_inc, float otx, float oty) {
	xo += xa;
	yo += ya;
	star newStar;
	init_star(&newStar);
	vert vertex;
	vertex.x = xo; vertex.y = yo;
	newStar.center = vertex;
	newStar.initial_y = yo;
	newStar.radius = radius;
	newStar.tx = tx;
	newStar.ty = ty;
	newStar.to_inc = to_inc;
	vertex.x = xo; vertex.y = yo + radius;
	newStar.points[newStar.size++] = vertex;
	starPoints(36, &newStar, 3); // 36 degree
	starPoints(72, &newStar, 1); // 72 degree
	starPoints(108, &newStar, 3); // 72 + 36 degree
	starPoints(144, &newStar, 1); // 72 + 72 degree
	vertex.x = xo; vertex.y = yo - radius / 3;
	newStar.points[newStar.size++] = vertex;
	starPoints(-144, &newStar, 1); // -72 - 72 degree
	starPoints(-108, &newStar, 3); // -72 - 36 degree
	starPoints(-72, &newStar, 1); // -72 degree
	starPoints(-36, &newStar, 3); // -36 degree
	newStar.points[newStar.size++] = newStar.points[0];

	// Star Trail Centers
	vertex.x = xo - xa + otx;
	vertex.y = yo - ya + oty;
	trail[starRear][0] = vertex;
	trail[starRear][1] = newStar.center;
	// newStar addition
	stars[starRear] = newStar;
	starRear = (starRear + 1) % max_stars;
}
// ***********************************************COLORING***********************************************

// Linear Sort
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
// Displaying Text
void colorText() {
	glPushMatrix();
	glTranslatef(-170, -200, 0);
	glScalef(0.5, 0.5, 0.5);
	glLineWidth(4.0);
	char* i;
	char* text = (char*)"FIREWORKS";
	glColor3f(0, 0, 0.5); // Blue

	//void* font = GLUT_BITMAP_9_BY_15;
	for (i = text; *i; i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *i);
	}
	glPopMatrix();
}
// Colors the twinkling stars
void colorNightSky() {
	for (int i = 0; i < max_night_sky; i++) {
		glPointSize((rand() % 4) + 1); // MAKES THE TWINKLING EFFECT
		int x = rand() % 2;
		x = 1;
		glColor3f(x, x, x);
		glBegin(GL_POINTS);
		glVertex2i(nightSky[i].x, nightSky[i].y);
		glEnd();
	}
}
// Coloring The Moon
void colorMoon() {
	// The Moon
	glColor3f(0.7, 0.7, 0.7); // Whitish Grey
	glBegin(GL_LINES);
	for (int j = 1; j < moon.size; j++) {
		glVertex2f(moon.points[j - 1].x, moon.points[j - 1].y);
		glVertex2f(moon.points[j].x, moon.points[j].y);
	}
	glEnd();
	// The Cresent
	glColor3f(0,0,0); // Black
	glBegin(GL_LINES);
	for (int j = 1; j < cresent.size; j++) {
		glVertex2f(cresent.points[j - 1].x, cresent.points[j - 1].y);
		glVertex2f(cresent.points[j].x, cresent.points[j].y);
	}
	glEnd();
}
// Coloring the ground
void colorGround() {
	// Ground
	glColor3f(0.1, 0.1, 0.1); // Gray
	glBegin(GL_LINES);
	for (int y = -WSy; y <= -WSy / 3; y++) {
		glVertex2i(-WSx, y);
		glVertex2i(+WSx, y);
	}
	glEnd();
	// Ground Line
	glPointSize(4.0);
	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_POINTS);
	int temp = 1;
	int y = -WSy / 3;
	for (int x = -WSx; x <= WSx; x++) {
		if (x % 5 == 0) { temp *= -1; }
		else if (x % 17 == 0) {}
		else { y += temp; }
		glVertex2i(x, y);
	}
	glEnd();
}
// Colors the bombs
void colorCircle() {
	glLineWidth(2.0);
	glColor3f(0, 0.5, 0); // Green
	glBegin(GL_LINES);
	for (int i = circleFront; i != circleRear; i = (i + 1) % max_bombs) {
		for (int j = 1; j < circles[i].size; j++) {
			glVertex2f(circles[i].points[j - 1].x, circles[i].points[j - 1].y);
			glVertex2f(circles[i].points[j].x, circles[i].points[j].y);
		}
	}
	glEnd();
}
// Colors the star trail
void colorStarTrail() {
	// Star Trail
	glColor3f(1, 1, 1); // White
	glLineWidth(3.0);
	glBegin(GL_LINES);
	for (int i = starFront; i != starRear; i = (i + 1) % max_stars) {
		if (trail[i][0].x != 0) {
			glVertex2f(trail[i][0].x, trail[i][0].y);
			glVertex2f(trail[i][1].x, trail[i][1].y);
		}
	}
	glEnd();
}
// Colors the sparks(Star shaped)
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
		glColor3f(1, 0, 0); // Red
		glBegin(GL_LINES);
		for (int i = 0; i < k; i += 2) {
			glVertex2i(xs[i], y);
			glVertex2i(xs[i + 1], y);
		}
		glEnd();
	}
}
// **********************************************************************************

// Center grid
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
// To Move the sparks(Star shaped) in 8-ways
void translate_star(int t) {
	for (int i = starFront; i != starRear; i = (i + 1) % max_stars) {
		//printf("%d,%d) ", starFront, starRear);
		if (stars[i].t_amt <= 50) {
			trail[i][1] = stars[i].center; // trail lines update
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
// To Move the bomb(rocket) 700 pixels north from creation point
void translate_bomb(int t) {
	int change = 0; // for wobble
	for (int i = circleFront; i != circleRear; i = (i + 1) % max_bombs) {
		int counter = rand() % 100; // for wobble
		if (circles[i].center.y - circles[i].initial_y <= 700) { // Before reaching 700th pixel
			circles[i].center.y += bomb_speed_lvl;
			for (int j = 0; j < circles[i].size; j++) {
				circles[i].points[j].y += bomb_speed_lvl;
				circles[i].points[j].x += 6 * sin(counter * 0.001) * temp; // WOBBLE EFFECT
			}
			counter++; // for wobble
			if (i % 5 == 0) { temp *= -1; } // for wobble
		}
		else { // After reaching 700th pixel
			float x = circles[circleFront].center.x;
			float y = circles[circleFront].initial_y + 700;
			float radius = circles[circleFront].radius;

			// X and Y value for offset of Trail Center
			float otx = ((float)rand() / RAND_MAX * radius) - radius / 3;
			float oty = ((float)rand() / RAND_MAX * radius) - radius / 3;

			// 8 Star Octagram
			createStar(x, y, 0, +radius, radius / 2, 0, +1, 1, otx, oty);
			createStar(x, y, 0, -radius, radius / 2, 0, -1, 1, otx, oty);
			createStar(x, y, +radius, 0, radius / 2, +1, 0, 1, otx, oty);
			createStar(x, y, -radius, 0, radius / 2, -1, 0, 1, otx, oty);
			float y_45 = radius / pow(2, 0.5);
			createStar(x, y, +y_45, +y_45, radius / 2, +1, +1, 0.3, otx, oty);
			createStar(x, y, +y_45, -y_45, radius / 2, +1, -1, 0.3, otx, oty);
			createStar(x, y, -y_45, +y_45, radius / 2, -1, +1, 0.3, otx, oty);
			createStar(x, y, -y_45, -y_45, radius / 2, -1, -1, 0.3, otx, oty);

			// Deleting the current bomb from circles[] array
			circleFront = (circleFront + 1) % max_bombs;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(millisec_per_frame, translate_bomb, 0);
}
// Creates a bomb every 'millisec_per_creation' ms
void bomb_creation(int t) {
	int amt = rand() % 2;
	if (circleFront != (circleRear + 4) % max_bombs) {
		for (int i = 0; i < amt; i++) {
			createCircle((rand() % 800) - 400, (rand() % 400) - 750, rand() % 20 + 10);
		}
	}

	glutPostRedisplay();
	glutTimerFunc(millisec_per_creation, bomb_creation, 0);
}
// ***********************************************************************************

// display callback
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	// Grid lines
	//grid();
	colorText();
	colorMoon();
	colorNightSky();
	colorGround();
	colorStarTrail();
	colorCircle();
	colorStar();

	// Star Outline
	/*
	glColor3f(1, 0.6+0.4, 1);
	glBegin(GL_LINES);
	for (int i = starFront; i != starRear; i = (i + 1) % max_stars) {
		star S = stars[i];
		for (int j = 1; j < S.size; j++) {
			glVertex2f(S.points[j - 1].x, S.points[j - 1].y);
			glVertex2f(S.points[j].x, S.points[j].y);
		}
	}
	glEnd();

	// BOMBS Outline
	glColor3f(0.4+0.6, 1, 1);
	glPointSize(2.0);
	glBegin(GL_POINTS);
	for (int i = circleFront; i != circleRear; i = (i + 1) % max_bombs) {
		circle C = circles[i];
		for (int j = 0; j < C.size; j++) {
			glVertex2f(C.points[j].x, C.points[j].y);
		}
	}
	glEnd();
	*/

	glutSwapBuffers(); // for DOUBLE BUFFER
	glFlush(); // for SINGLE BUFFER
}
int main(int argc, char** argv) {
	glutInit(&argc, argv); // Initializing glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Dynamic | RGB
	glutInitWindowSize(WSx, WSy); // Window size
	glutInitWindowPosition(WPx, WPy); // Win position (origin top_left)
	glutCreateWindow("Assignment 6_- FIREWORKS IN THE NIGHT SKY"); // Create window and title

	srand(time(NULL)); // To create random no.s
	createNightSky(); // Night Sky Creation
	createMoon(250, 250, 50); // Moon Creation
	createCircle(0, -600, 100); // First Big Bomb

	myInit(); // Initializing
	//glutMouseFunc(mouseHandler);
	glutDisplayFunc(display); // passes a func as argument
	glutTimerFunc(millisec_per_creation, bomb_creation, 0); // Create bomb
	glutTimerFunc(millisec_per_frame, translate_bomb, 0); // Move bomb
	glutTimerFunc(millisec_per_frame, translate_star, 0); // Move stars(sparks)
	glutMainLoop();

	return 0;
}

