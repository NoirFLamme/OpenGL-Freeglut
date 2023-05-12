#include <cmath>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h> 

#define VERTICES 0
#define TRIANGLE 1

enum userChoice{dda = 0, bersen = 1};
std::vector<float> points;

int size = 0;
int userInput = 0;

// Corner points of letters
static std::vector<float> vertices2 =
{
	20.0, 40.0, 0.0,
	20.0, 60.0, 0.0,
	25.0, 60.0, 0.0,
	25.0, 50.0, 0.0,
	20.0, 50.0, 0.0,
	25.0, 40.0, 0.0,

	30.0, 40.0, 0.0,
	35.0, 60.0, 0.0,
	40.0, 40.0, 0.0,

	32.5, 50.0, 0.0,
	37.5, 50.0, 0.0,

	45.0, 40.0, 0.0,
	50.0, 60.0, 0.0,
	55.0, 50.0, 0.0,
	60.0, 60.0, 0.0,
	65.0, 40.0, 0.0,

	70.0, 60.0, 0.0,
	75.0, 50.0, 0.0,
	80.0, 60.0, 0.0,
	75.0, 50.0, 0.0,
	75.0, 40.0, 0.0
};

// Vertex color vectors for the triangle.
std::vector<float> colors2;


static unsigned int buffer[2]; // Array of buffer ids.

static unsigned int vao[2]; // Array of VAO ids.
// End globals.

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Get a pointer to the vertex buffer.
	float* bufferData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// Randomly change the color values.
	for (int i = 0; i < size  * sizeof(float) / sizeof(float); i++)
		bufferData[ size * sizeof(float) / sizeof(float) + i] = (float)rand() / (float)RAND_MAX;

	glPointSize(5);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindVertexArray(vao[TRIANGLE]);
	glDrawArrays(GL_POINTS, 0, size);

	glutSwapBuffers();
	glFlush();
}

void animate(int someValue)
{
	glutPostRedisplay();
	glutTimerFunc(500, animate, 1);
}

void DDA(int X1, int Y1, int X2, int Y2) {
	int I;
	float Length;
	float X, Y, Xinc, Yinc;
	Length = abs(X2 - X1);
	if (abs(Y2 - Y1) > Length)
		Length = abs(Y2 - Y1);
	Xinc = (X2 - X1) / (Length);
	Yinc = (Y2 - Y1) / (Length);
	X = X1;
	Y = Y1;
	for (I = 0; I < Length; I++) {
		points.push_back(X);
		points.push_back(Y);
		points.push_back(0.0);
		colors2.push_back(1.0);
		colors2.push_back(0.0);
		colors2.push_back(0.0);
		size += 3;
		X = X + Xinc;
		Y = Y + Yinc;
	}
}

void Bersenham(int x1, int y1, int x2, int y2) {
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	dx = x2 - x1;
	dy = y2 - y1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1;
	if (x2 < x1) incx = -1;
	else if (x2 == x1) incx = 0;
	incy = 1;
	if (y2 < y1) incy = -1;
	else if (y2 == y1) incy = 0;
	x = x1; y = y1;
	if (dx > dy) {
		points.push_back(x);
		points.push_back(y);
		points.push_back(0.0);
		colors2.push_back(1.0);
		colors2.push_back(0.0);
		colors2.push_back(0.0);
		size += 3;
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx; i++) {
			if (e >= 0) {
				y += incy;
				e += inc1;
			}
			else
				e += inc2;
			x += incx;
			points.push_back(x);
			points.push_back(y);
			points.push_back(0.0);
			colors2.push_back(1.0);
			colors2.push_back(0.0);
			colors2.push_back(0.0);
			size += 3;
		}

	}
	else {
		points.push_back(x);
		points.push_back(y);
		points.push_back(0.0);
		colors2.push_back(1.0);
		colors2.push_back(0.0);
		colors2.push_back(0.0);
		size += 3;
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy; i++) {
			if (e >= 0) {
				x += incx;
				e += inc1;
			}
			else
				e += inc2;
			y += incy;
			points.push_back(x);
			points.push_back(y);
			points.push_back(0.0);
			colors2.push_back(1.0);
			colors2.push_back(0.0);
			colors2.push_back(0.0);
			size += 3;
		}
	}
}

// Initialization routine.
void setup(void)
{
	for (int i = 0; i < 60; i += 3) {
		if (i == 15 || i == 24 ||  i == 30 || i == 45) { continue; }
		if(userInput == dda)
			DDA(vertices2[i], vertices2[i + 1], vertices2[i + 3], vertices2[i + 4]);
		else if(userInput == bersen)
			Bersenham(vertices2[i], vertices2[i + 1], vertices2[i + 3], vertices2[i + 4]);
	}
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glGenVertexArrays(2, vao); // Generate VAO ids.

	glBindVertexArray(vao[TRIANGLE]);

	glGenBuffers(1, buffer);

	// Bind vertex buffer and reserve space.
	glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float) + size * sizeof(float), NULL, GL_STATIC_DRAW);

	// Copy vertex coordinates data into first half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), (float *) &points[0]);

	// Copy vertex color data into second half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, size * sizeof(float), size * sizeof(float), (float*) &colors2[0]);

	// Enable two vertex arrays: co-ordinates and color.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Specify vertex and color pointers to the start of the respective data.
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColorPointer(3, GL_FLOAT, 0, (void*)(size * sizeof(float)));
	// END bind VAO id vao[TRIANGLE].
	glutTimerFunc(5, animate, 1);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	std::cout << "Please choose the drawing algorithm" << std::endl;
	std::cout << "1: DDA" << std::endl;
	std::cout << "2: Bersenham" << std::endl;
	int input = 0;
	std::cin >> input;
	if(input == 1){
		userInput = dda;
	}
	else if(input == 2){
		userInput = bersen;
	}
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("squareAnnulusAndTriangleVAO.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}