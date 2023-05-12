///////////////////////////////////////////////////////////////////////////////////////          
// hemisphere.cpp
//
// This program approximates a hemisphere with an array of latitudinal triangle strips.
//
// Interaction:
// Press P/p to increase/decrease the number of longitudinal slices.
// Press Q/q to increase/decrease the number of latitudinal slices.
// Press x, X, y, Y, z, Z to turn the hemisphere.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////////////////////// 

#define _USE_MATH_DEFINES 

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

// Globals.
static enum choice { sphere = 1, helix = 2 };
static float R = 5.0; // Radius of hemisphere.
// Sphere Options
static int p = 6; // Number of longitudinal slices.
static int q = 4; // Number of latitudinal slices.
static enum drawType {wireframe = 0, fill = 1};
static int drawMode = 0;
// Helix Options
static int r = 5.0;
static int h = 1;
static int n = 1;

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static float offset = -10;
static float spinSpeed = 5;
static float prev_time = 0;

int userChoice = 0;
// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{
	int  i, j;

	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	// Command to push the hemisphere, which is drawn centered at the origin, 
	// into the viewing frustum.

		glTranslatef(0.0, 0.0, offset);

		// Commands to turn the hemisphere.
		glRotatef(Zangle, 0.0, 0.0, 1.0);
		glRotatef(Yangle, 0.0, 1.0, 0.0);
		glRotatef(Xangle, 1.0, 0.0, 0.0);

		// Hemisphere properties.
		if (drawMode == wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (drawMode == fill)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		//glColor3f(0.0, 0.0, 0.0);
		switch (userChoice) 
		{
		case sphere:
		// Array of latitudinal triangle strips, each parallel to the equator, stacked one
		// above the other from the equator to the north pole.
			for (j = 0; j < q; j++)
			{
				// One latitudinal triangle strip.
				glBegin(GL_TRIANGLE_STRIP);
				for (i = 0; i <= p; i++)
				{
					glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
					glVertex3f(R * cos((float)(j + 1) / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
						R * sin((float)(j + 1) / q * M_PI / 2.0),
						-R * cos((float)(j + 1) / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
					glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
					glVertex3f(R * cos((float)j / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
						R * sin((float)j / q * M_PI / 2.0),
						-R * cos((float)j / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));

				}
				glEnd();

				glBegin(GL_TRIANGLE_STRIP);
				for (i = 0; i <= p; i++)
				{
					glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
					glVertex3f(R * cos((float)(j + 1) / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
						-R * sin((float)(j + 1) / q * M_PI / 2.0),
						-R * cos((float)(j + 1) / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
					glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
					glVertex3f(R * cos((float)j / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
						-R * sin((float)j / q * M_PI / 2.0),
						-R * cos((float)j / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));

				}
				glEnd();
			
			}
			break;
		case helix:
			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_LINE_STRIP);
			for (float t = -5 * M_PI; t <= 5 * M_PI; t += M_PI / n) {
				glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
				glVertex3f(r * cos(t), r * sin(t), h * t - 20.0);
			}
			glEnd();

			break;
		default:
			break;

	}


	glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void spinDisplay() {
	Yangle += spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
	prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
	if (Yangle > 360.0) Yangle -= 360.0;
	while (glutGet(GLUT_ELAPSED_TIME) - prev_time < 1) {}
	glutPostRedisplay();
}
void spinDisplayReverse() {

	Yangle -= spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
	prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
	if (Yangle > 360.0) Yangle -= 360.0;
	while(glutGet(GLUT_ELAPSED_TIME) - prev_time < 1) {}
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			prev_time = 0;
			glutIdleFunc(spinDisplay);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			prev_time = 0;
			glutIdleFunc(spinDisplayReverse);
		}

		break;

	default:
		break;
	}
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'P':
		if (userChoice == sphere) {
			p += 1;
			glutPostRedisplay();
		}
		break;
	case 'p':
		if (userChoice == sphere) {
			if (p > 3) p -= 1;
			glutPostRedisplay();
		}
		break;
	case 'Q':
		if (userChoice == sphere) {
			q += 1;
			glutPostRedisplay();
		}
		break;
	case 'q':
		if (userChoice == sphere) {
			if (q > 3) q -= 1;
			glutPostRedisplay();
		}
		break;
	case 'W':
		if (userChoice == sphere) {
			drawMode = wireframe;
			glutPostRedisplay();
		}
		break;
	case 'w':
		if (userChoice == sphere) {
			drawMode = fill;
			glutPostRedisplay();
		}
		break;
	case 'R':
		if (userChoice == helix) {
			r += 1;
			glutPostRedisplay();
		}
		break;
	case 'r':
		if (userChoice == helix) {
			if (r > 5) r -= 1;
			glutPostRedisplay();
		}
		break;
	case 'H':
		if (userChoice == helix) {
			h += 1;
			glutPostRedisplay();
		}
		break;
	case 'h':
		if (userChoice == helix) {
			if (h > 1) h -= 1;
			glutPostRedisplay();
		}
		break;
	case 'N':
		if (userChoice == helix) {
			n += 1;
			glutPostRedisplay();
		}
		break;
	case 'n':
		if (userChoice == helix) {
			if (n > 1) n -= 1;
			glutPostRedisplay();
		}
		break;
	case 'x':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	case 'O':
		offset += 1;
		glutPostRedisplay();
		break;
	case 'o':
		offset -= 1;
		glutPostRedisplay();
		break;
	case ' ':
		glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Sphere Interactions:" << std::endl;
	std::cout << "Press P/p to increase/decrease the number of longitudinal slices." << std::endl
		<< "Press Q/q to increase/decrease the number of latitudinal slices." << std::endl
		<< "Press W/w to choose between wireframe/fill." << std::endl;
	std::cout << std::endl;

	std::cout << "Helix Interactions:" << std::endl;
	std::cout << "Press R/r to increase/decrease the helix radius." << std::endl
		<< "Press H/h to increase/decrease the pitch of the helix." << std::endl
		<< "Press N/n to increase/decrease the number of points." << std::endl;
	std::cout << std::endl;
	std::cout << "General Interactions:" << std::endl;
	std::cout<< "Press x, X, y, Y, z, Z to turn the hemisphere." << std::endl
		<< "Press O/o to zoom in/out." << std::endl;
	std::cout << std::endl;

}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	std::cout << "Which shape do you want?\n1) Sphere\n2) Helix\n>> ";
	int input = 0;
	std::cin >> input;
	if (input == 1) {
		userChoice = sphere;
	}
	else if (input == 2) {
		userChoice = helix;
	}

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hemisphere.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouse);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
