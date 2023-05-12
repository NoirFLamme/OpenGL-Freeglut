// Interaction:
// Press +/- to add/erase random point ---> good practice to add interaction as a comment at the top of source code

#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <vector>
#include <glm/vec3.hpp>
#include <random>
#include <iostream>

//points: datastructure to store current points
std::vector<glm::vec3> points;
// window dimension
float windowWidth = 100.0;
float windowHeight = 100.0;
int accum = 0;

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(1.0); // Default line width.
	/*
	----------set point size below---------
	*/
	glPointSize(5);
/*
----------write points drawing logic below------------
*/
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	//code here
	for (glm::vec3 point : points) {
		glVertex3f(point.x, point.y, point.z);
	}
	glEnd();
	//----------------------------------------------------
	/*
	----------write lines drawing logic below------------
	*/
	glBegin(GL_LINES);
	//code here
	glColor3f(1.0f, 0.0f, 0.0f);
	for (glm::vec3 point : points) {
		glVertex3f(point.x, point.y, point.z);
	}
	glEnd();
	//----------------------------------------------------
	glFlush();


}
// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, windowWidth, 0.0, windowHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	accum = (accum + 1) % 10;
	std::srand(std::time(nullptr) + accum); // use current time as seed for random generator
	float generatedX = rand() % (int)windowWidth;
	float generatedY = rand() % (int)windowHeight;
	switch (key)
	{
	case 27:
		exit(0);
		break;
		/*
		------------- Add +/- cases handling below (handle corner cases)----------------
		*/
		//code here
	case 43:
		std::cout << "(" << generatedX << ", " << generatedY << ")\n";
		glm::vec3 generatedPoint = glm::vec3(generatedX, generatedY, 0);
		points.push_back(generatedPoint);
		glutPostRedisplay();
		break;
	case 45:
		if (points.size() != 0) {
			points.pop_back();
			glutPostRedisplay();
		}
		break;
	//-----------------------------------------------------------------------
	default:
		break;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press +/- to add/erase random point" << std::endl;
}

// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	printInteraction(); // good practice to print how to interact
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("RandomLines.cpp");
	glutDisplayFunc(drawScene);//drawing function

	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);//handle keyboard events
	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}