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
float ge = 0.5f; // Grid extra pixel
int x1, y11, x2, y2, m, x, y;

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
	for (int i = -m; i < m; ++i) {
		for (int j = -m; j < m; ++j) {
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
void low()
{
    y = y11;
    int yi = 1;
    int dx = x2 - x1;
    int dy = y2 - y11;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    int d = (2 * dy) - dx;
    for (x = x1; x <= x2; x++)
    {
        //printf("%d %d\n", x, y);
        glRectd(x - 0.5, y - 0.5, x + 0.5, y + 0.5);
        if (d > 0)
        {
            y += yi;
            d += (2 * (dy - dx));
        }
        else
            d += (2 * dy);
    }
}
void high()
{
    x = x1;
    int xi = 1;
    int dx = x2 - x1;
    int dy = y2 - y11;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }
    int d = (2 * dx) - dy;
    for (y = y11; y <= y2; ++y)
    {
        //printf("%d %d\n", x, y);
        glRectd(x - 0.5, y - 0.5, x + 0.5, y + 0.5);
        if (d > 0)
        {
            x += xi;
            d += (2 * (dx - dy));
        }
        else
            d += (2 * dx);
    }
}
void bresenham()
{
    if (abs(y2 - y11) > abs(x2 - x1))
    {
        if (y11 > y2)
        {
            int temp1 = x1;
            int temp2 = y11;
            x1 = x2;
            y11 = y2;
            x2 = temp1;
            y2 = temp2;
        }
        high();
    }
    else
    {
        if (x1 > x2)
        {
            int temp1 = x1;
            int temp2 = y11;
            x1 = x2;
            y11 = y2;
            x2 = temp1;
            y2 = temp2;
        }
        low();
    }
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    grid();
    bresenham();
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
    m = ((int)m % (WSx - 2)) + 2;
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

