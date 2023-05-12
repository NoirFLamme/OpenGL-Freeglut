///////////////////////////////////////////////////////////        
// helixList.cpp
//
// This program draws several helixes using a display list.
// 
// Sumanta Guha.
///////////////////////////////////////////////////////////

#define _USE_MATH_DEFINES 

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

// Globals.

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.

enum drawingType {wireframe = GLU_LINE, fill = GLU_FILL};
int choice = 0;
static unsigned int seat; // List index.
static unsigned int leg;
GLUquadricObj* objCylinder1 = gluNewQuadric();
GLUquadricObj* objCylinder2 = gluNewQuadric();

GLUquadricObj* objDisk1 = gluNewQuadric();
GLUquadricObj* objDisk2 = gluNewQuadric();


// Initialization routine.
void setup(void)
{
	glEnable(GL_DEPTH_TEST);

	float t; // Angle parameter.

	seat = glGenLists(1); // Return a list index.
	leg = glGenLists(1);

	// Begin create a display list.

	glNewList(seat, GL_COMPILE);
	// Draw a helix.
	glColor3f(0.5f, 0.35f, 0.05f);
	gluDisk(objDisk1, 0, 10, 20, 20);
	glColor3f(0.6f, 0.35f, 0.05f);
	gluCylinder(objCylinder1, 10, 10, 5, 10, 10);
	glTranslatef(0, 0, 5);
	glColor3f(0.7f, 0.35f, 0.05f);
	gluDisk(objDisk2, 0, 10, 20, 20);
	glEndList();

	glNewList(leg, GL_COMPILE);
	// Draw a helix
	glColor3f(0.7f, 0.35f, 0.05f);
	gluCylinder(objCylinder2, 1, 1, 20, 10, 10);
	glEndList();
	// End create a display list.

	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	if (choice == wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glLoadIdentity();
	glPushMatrix();
		glTranslatef(0.0, 0.0, -40);
		glRotatef(90, 1, 0, 0);


		glColor3f(1.0, 0.0, 0.0);
		glPushMatrix();
		glRotatef(Zangle, 0.0, 0.0, 1.0);
		glRotatef(Yangle, 0.0, 1.0, 0.0);
		glRotatef(Xangle, 1.0, 0.0, 0.0);
		glCallList(seat); // Execute display list.
		glPopMatrix();

		glColor3f(1.0, 0.0, 0.0);
		glPushMatrix();
		glRotatef(Zangle, 0.0, 0.0, 1.0);
		glRotatef(Yangle, 0.0, 1.0, 0.0);
		glRotatef(Xangle, 1.0, 0.0, 0.0);
		glTranslatef(0, 7.5, 5);
		
		glRotatef(-15, 1, 0, 0);
		glCallList(leg); // Execute display list.
		glPopMatrix();

		glPushMatrix();
		glRotatef(Zangle, 0.0, 0.0, 1.0);
		glRotatef(Yangle, 0.0, 1.0, 0.0);
		glRotatef(Xangle, 1.0, 0.0, 0.0);
		glTranslatef(0, -7.5, 5);

		glRotatef(15, 1, 0, 0);
		glCallList(leg); // Execute display list.
		glPopMatrix();



		glColor3f(1.0, 0.0, 0.0);
		glPushMatrix();
		glRotatef(Zangle, 0.0, 0.0, 1.0);
		glRotatef(Yangle, 0.0, 1.0, 0.0);
		glRotatef(Xangle, 1.0, 0.0, 0.0);
		glTranslatef(-7.5, -1, 5);
		glRotatef(-15, 0, 1, 0);
		glCallList(leg); // Execute display list.
		glPopMatrix();



		glColor3f(1.0, 0.0, 0.0);
		glPushMatrix();
		glRotatef(Zangle, 0.0, 0.0, 1.0);
		glRotatef(Yangle, 0.0, 1.0, 0.0);
		glRotatef(Xangle, 1.0, 0.0, 0.0);
		glTranslatef(7.5, -1, 5);
		
		glRotatef(15, 0, 1, 0);
		glCallList(leg); // Execute display list.
		glPopMatrix();


	glPopMatrix();


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
	case ' ':
		if (choice == wireframe)
			choice = fill;
		else
			choice = wireframe;
		glutPostRedisplay();
		break;
	case 'x':
		Xangle += 5;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Xangle -= 5;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void instructions() {
	printf("Press x/X to rotate around the x-axis\n");
	printf("Press y/Y to rotate around the y-axis\n");
	printf("Press z/Z to rotate around the z-axis\n");
	printf("Press space to toggle between wireframe and fill\n");
}

// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	instructions();
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("helixList.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}