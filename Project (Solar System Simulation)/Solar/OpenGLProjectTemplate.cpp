///////////////////////////////////////////////////////////////////////////////////        
// spaceTravel.cpp
//
// This program draws a conical spacecraft that can travel and an array of 
// fixed spherical asteroids. The view in the left viewport is from a fixed
// camera; the view in the right viewport is from the spacecraft.
// There is approximate collision detection.
// 
// User-defined constants: 
// ROW is the number of rows of  asteroids.
// COLUMN is the number of columns of asteroids.
// FILL_PROBABILITY is the percentage probability that a particular row-column slot
// will be filled with an asteroid.
//
// Interaction:
// Press the left/right arrow keys to turn the craft.
// Press the up/down arrow keys to move the craft.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////////////////// 

#define _USE_MATH_DEFINES 

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

#define ROWS 1  // Number of rows of asteroids.
#define COLUMNS 11 // Number of columns of asteroids.
#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will be 
							 // filled with an asteroid. It should be an integer between 0 and 100.

// Globals.
enum planets { sun = 0, mercury = 1, venus = 2, earth = 3, mars = 4, jupiter = 5, saturn = 6, uranus = 7, neptune = 8, pluto = 9, moon = 10};
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 200; // Co-ordinates of the spacecraft.
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static unsigned int spacecraft; // Display lists base index.
static int frameCount = 0; // Number of frames

static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.


static float d = 1.0; // Diffuse and specular white light intensity.
static float m = 0.2; // Global ambient white light intensity.
static int localViewer = 1; // Local viewpoint?
static float p = 1.0; // Positional light?
static float t = 0.0; // Quadratic attenuation factor.

void animate(int);
// Routine to draw a bitmap character string.
void writeBitmapString(void* font, char* string)
{
	char* c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Asteroid class.
class Asteroid
{
public:
	Asteroid();
	Asteroid(float x, float y, float z, float r, unsigned char colorR,
		unsigned char colorG, unsigned char colorB);
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	float getRadius() { return radius; }
	void draw();

private:
	float centerX, centerY, centerZ, radius;
	unsigned char color[3];
};

// Asteroid default constructor.
Asteroid::Asteroid()
{
	centerX = 0.0;
	centerY = 0.0;
	centerZ = 0.0;
	radius = 0.0; // Indicates no asteroid exists in the position.
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(float x, float y, float z, float r, unsigned char colorR,
	unsigned char colorG, unsigned char colorB)
{
	centerX = x;
	centerY = y;
	centerZ = z;
	radius = r;
	color[0] = colorR;
	color[1] = colorG;
	color[2] = colorB;
}

// Function to draw asteroid.
void Asteroid::draw()
{
	if (radius > 0.0) // If asteroid exists.
	{
		glPushMatrix();
		glTranslatef(centerX, centerY, centerZ);
		glColor3ubv(color);
		glutSolidSphere(radius, (int)radius * 6, (int)radius * 6);
		glPopMatrix();
	}
}

Asteroid arrayAsteroids[ROWS][COLUMNS]; // Global array of asteroids.
float arrayRotation[COLUMNS];

// Routine to count the number of frames drawn every second.
void frameCounter(int value)
{
	if (value != 0) // No output the first time frameCounter() is called (from main()).
		std::cout << "FPS = " << frameCount << std::endl;
	frameCount = 0;
	glutTimerFunc(1000, frameCounter, 1);
}


// Initialization routine.
void setup(void)
{

	glEnable(GL_LIGHTING);

	// Material property vectors.
	

	// Material properties of ball.
	

	// Cull back faces.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int i, j;

	spacecraft = glGenLists(1);
	glNewList(spacecraft, GL_COMPILE);
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the $z$-axis initially.
	glColor3f(1.0, 1.0, 1.0);
	glutWireCone(5.0, 10.0, 10, 10);
	glPopMatrix();
	glEndList();

	arrayAsteroids[0][sun] = Asteroid(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0, 100.0,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][mercury] = Asteroid(30.0 * (-COLUMNS / 2 + 4), 0.0, -40.0 - 30.0 * 0, 3.0,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][venus] = Asteroid(30.0 * (-COLUMNS / 2 + 5), 0.0, -40.0 - 30.0 * 0, 6.0,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][earth] = Asteroid(30.0 * (-COLUMNS / 2 + 6), 0.0, -40.0 - 30.0 * 0, 7.0,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][moon] = Asteroid(30.0 * (-COLUMNS / 2 + 6) + 20, 0.0, -40.0 - 30.0 * 0, 1.0,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][mars] = Asteroid(30.0 * (-COLUMNS / 2 + 7), 0.0, -40.0 - 30.0 * 0, 3.5,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][jupiter] = Asteroid(30.0 * (-COLUMNS / 2 + 9), 0.0, -40.0 - 30.0 * 0, 40,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][saturn] = Asteroid(30.0 * (-COLUMNS / 2 + 12), 0.0, -40.0 - 30.0 * 0, 36,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][uranus] = Asteroid(30.0 * (-COLUMNS / 2 + 14), 0.0, -40.0 - 30.0 * 0, 15,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][neptune] = Asteroid(30.0 * (-COLUMNS / 2 + 16), 0.0, -40.0 - 30.0 * 0, 15,
		rand() % 256, rand() % 256, rand() % 256);
	arrayAsteroids[0][pluto] = Asteroid(30.0 * (-COLUMNS / 2 + 17), 0.0, -40.0 - 30.0 * 0, 1,
		rand() % 256, rand() % 256, rand() % 256);



	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glutTimerFunc(0, frameCounter, 0); // Initial call of frameCounter().
	animate(1);

}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
	float x2, float y2, float z2, float r2)
{
	return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) <= (r1 + r2) * (r1 + r2));
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a)
{
	int i, j;

	// Check for collision with each asteroid.
	for (j = 0; j < COLUMNS; j++)
		for (i = 0; i < ROWS; i++)
			if (arrayAsteroids[i][j].getRadius() > 0) // If asteroid exists.
				if (checkSpheresIntersection(x - 5 * sin((M_PI / 180.0) * a), 0.0,
					z - 5 * cos((M_PI / 180.0) * a), 7.072,
					arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterY(),
					arrayAsteroids[i][j].getCenterZ(), arrayAsteroids[i][j].getRadius()))
					return 1;
	return 0;
}

// Drawing routine.
void drawScene(void)
{
	frameCount++; // Increment number of frames every redraw.

	int i, j;
	

	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec0[] = { 0, 1, 0, 1.0 };
	float lightPos0[] = { 30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0, p }; //Demo: last value p define whether it is direction or positional
	float lightDifAndSpec1[] = { 1.0, 0.0, 0.0, 0 };
	float lightPos1[] = { 1.0, 2.0, 0.0, 1.0 };
	float globAmb[] = { m, m, m, 1.0 };

	// Light0 properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_EMISSION, lightDifAndSpec1);
	glLightfv(GL_LIGHT0, GL_SHININESS, lightDifAndSpec1);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.




	glEnable(GL_LIGHT0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Begin right viewport.
	glViewport(0, 0, width , height);//demo
	glLoadIdentity();

	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, t);

	glDisable(GL_LIGHTING);

	// Write text in isolated (i.e., before gluLookAt) translate block.
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(-28.0, 25.0, -30.0);
	if (isCollision) writeBitmapString((void*)font, (char*) "Cannot - will crash!");
	glPopMatrix();

	//// Draw a vertical line on the left of the viewport to separate the two viewports
	//glColor3f(1.0, 1.0, 1.0);
	//glLineWidth(2.0);
	//glBegin(GL_LINES);
	//glVertex3f(-5.0, -5.0, -5.0);
	//glVertex3f(-5.0, 5.0, -5.0);
	//glEnd();
	//glLineWidth(1.0);

	// Locate the camera at the tip of the cone and pointing in the direction of the cone.
	gluLookAt(xVal - 10 * sin((M_PI / 180.0) * angle) + 0.5,
		0.0,
		zVal - 10 * cos((M_PI / 180.0) * angle),
		xVal - 11 * sin((M_PI / 180.0) * angle),
		0.0,
		zVal - 11 * cos((M_PI / 180.0) * angle),
		0.0,
		1.0,
		0.0);

	// Draw all the asteroids in arrayAsteroids.

	float matAmbAndDif[] = { 0.91, 0.7, 0.05, 1.0 };
	float matSpec[] = {0, 0, 0, 1.0 };
	float matShine[] = { 30.0 };
	float matEmission[] = { 0.91, 0.7, 0.05, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
	arrayAsteroids[0][sun].draw();

	matEmission[0] = 0;
	matEmission[1] = 0;
	matEmission[2] = 0;

	matAmbAndDif[0] = 0.89;
	matAmbAndDif[1] = 0.75;
	matAmbAndDif[2] = 0.75;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);

	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[mercury], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][mercury].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();

	matAmbAndDif[0] = 0.9;
	matAmbAndDif[1] = 0.6;
	matAmbAndDif[2] = 0.1;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[venus], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][venus].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.43;
	matAmbAndDif[1] = 0.53;
	matAmbAndDif[2] = 0.95;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[earth], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][earth].draw();
	glPushMatrix();
		matAmbAndDif[0] = 1;
		matAmbAndDif[1] = 1;
		matAmbAndDif[2] = 1;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
		glTranslatef(30.0 * (-COLUMNS / 2 + 6), 0.0, -40.0 - 30.0 * 0);
		glRotatef(arrayRotation[moon], 0.0, 1.0, 0.0);
		glTranslatef(-30.0 * (-COLUMNS / 2 + 6), 0.0, 40.0 + 30.0 * 0);
		arrayAsteroids[0][moon].draw();
	glPopMatrix();
	glTranslatef(-10, 0, 0);
	glPopMatrix();

	matAmbAndDif[0] = 0.73;
	matAmbAndDif[1] = 0.3;
	matAmbAndDif[2] = 0.05;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[mars], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][mars].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.73;
	matAmbAndDif[1] = 0.60;
	matAmbAndDif[2] = 0.65;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[jupiter], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][jupiter].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.67;
	matAmbAndDif[1] = 0.37;
	matAmbAndDif[2] = 0.29;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[saturn], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][saturn].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.80;
	matAmbAndDif[1] = 0.98;
	matAmbAndDif[2] = 0.99;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[uranus], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][uranus].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.2;
	matAmbAndDif[1] = 0.4;
	matAmbAndDif[2] = 0.99;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[neptune], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][neptune].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();




	matAmbAndDif[0] = 1;
	matAmbAndDif[1] = 0.95;
	matAmbAndDif[2] = 0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[pluto], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][pluto].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();

	// End right viewport.

	glDisable(GL_LIGHTING);
	// Beg	in left viewport.
	glViewport(2 * width / 3.0, 0, width / 3.0, height / 3.0);
	glScissor(2 * width / 3.0, 0, width / 3.0, height / 3.0);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);



	glLoadIdentity();

	

	// Write text in isolated (i.e., before gluLookAt) translate block.
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos3f(-28.0, 25.0, -30.0);
	if (isCollision) writeBitmapString((void*)font, (char*)"Cannot - will crash!");
	glPopMatrix();

	// Fixed camera.
	gluLookAt(-140, 700, -20, 30.0 * (-COLUMNS / 2 + 0), 0, -20, 0.0, 1.0, 0.0);

	glColor3f(1, 0, 0);
	glLineWidth(500);
	glBegin(GL_LINES);
	glVertex3f(30.0 * (-COLUMNS / 2 + 0) - 800, 0, 670);
	glVertex3f(30.0 * (-COLUMNS / 2 + 0) + 800, 0, 650);
	glVertex3f(30.0 * (-COLUMNS / 2 + 0) - 700, 0, 670);
	glVertex3f(30.0 * (-COLUMNS / 2 + 0) - 700, 0, -730);
	glEnd();
	glLineWidth(2);


	// Draw all the asteroids in arrayAsteroids.
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	glEnable(GL_LIGHTING);
	matEmission[0] = 0.91;
	matEmission[1] = 0.7;
	matEmission[2] = 0.05;

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
	arrayAsteroids[0][sun].draw();

	
	matEmission[0] = 0;
	matEmission[1] = 0;
	matEmission[2] = 0;

	matAmbAndDif[0] = 0.89;
	matAmbAndDif[1] = 0.75;
	matAmbAndDif[2] = 0.75;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);

	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[mercury], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][mercury].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();

	matAmbAndDif[0] = 0.9;
	matAmbAndDif[1] = 0.6;
	matAmbAndDif[2] = 0.1;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[venus], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][venus].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.43;
	matAmbAndDif[1] = 0.53;
	matAmbAndDif[2] = 0.95;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[earth], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][earth].draw();
	glPushMatrix();
	matAmbAndDif[0] = 1;
	matAmbAndDif[1] = 1;
	matAmbAndDif[2] = 1;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glTranslatef(30.0 * (-COLUMNS / 2 + 6), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[moon], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 6), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][moon].draw();
	glPopMatrix();
	glTranslatef(-10, 0, 0);
	glPopMatrix();

	matAmbAndDif[0] = 0.73;
	matAmbAndDif[1] = 0.3;
	matAmbAndDif[2] = 0.05;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[mars], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][mars].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.73;
	matAmbAndDif[1] = 0.60;
	matAmbAndDif[2] = 0.65;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[jupiter], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][jupiter].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.67;
	matAmbAndDif[1] = 0.37;
	matAmbAndDif[2] = 0.29;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[saturn], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][saturn].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.80;
	matAmbAndDif[1] = 0.98;
	matAmbAndDif[2] = 0.99;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[uranus], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][uranus].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	matAmbAndDif[0] = 0.2;
	matAmbAndDif[1] = 0.4;
	matAmbAndDif[2] = 0.99;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[neptune], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][neptune].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();



	matAmbAndDif[0] = 1;
	matAmbAndDif[1] = 0.95;
	matAmbAndDif[2] = 0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glPushMatrix();
	glTranslatef(30.0 * (-COLUMNS / 2 + 0), 0.0, -40.0 - 30.0 * 0);
	glRotatef(arrayRotation[pluto], 0.0, 1.0, 0.0);
	glTranslatef(-30.0 * (-COLUMNS / 2 + 0), 0.0, 40.0 + 30.0 * 0);
	arrayAsteroids[0][pluto].draw();
	glTranslatef(-10, 0, 0);
	glPopMatrix();


	// Draw spacecraft.
	glPushMatrix();
	glTranslatef(xVal, 0.0, zVal);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glCallList(spacecraft);
	glPopMatrix();
	// End left viewport.

	glutSwapBuffers();
}

void animate(int value)
{

		arrayRotation[1] += 5;
		if (arrayRotation[1] > 360.0) arrayRotation[1] -= 360.0;
		arrayRotation[2] += 4;
		if (arrayRotation[2] > 360.0) arrayRotation[2] -= 360.0;
		arrayRotation[3] += 3;
		if (arrayRotation[3] > 360.0) arrayRotation[3] -= 360.0;
		arrayRotation[4] += 2;
		if (arrayRotation[4] > 360.0) arrayRotation[4] -= 360.0;
		arrayRotation[5] += 1;
		if (arrayRotation[5] > 360.0) arrayRotation[5] -= 360.0;
		arrayRotation[6] += 0.75;
		if (arrayRotation[6] > 360.0) arrayRotation[6] -= 360.0;
		arrayRotation[7] += 0.5;
		if (arrayRotation[7] > 360.0) arrayRotation[7] -= 360.0;
		arrayRotation[8] += 0.3;
		if (arrayRotation[8] > 360.0) arrayRotation[8] -= 360.0;
		arrayRotation[9] += 0.2;
		if (arrayRotation[8] > 360.0) arrayRotation[9] -= 360.0;
		arrayRotation[10] += 10;
		if (arrayRotation[10] > 360.0) arrayRotation[10] -= 360.0;


		glutPostRedisplay();
		glutTimerFunc(animationPeriod, animate, 1);

}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 1000);
	glMatrixMode(GL_MODELVIEW);

	// Pass the size of the OpenGL window.
	width = w;
	height = h;
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
		if (isAnimate) isAnimate = 0;
		else
		{
			isAnimate = 1;
			animate(1);
		}
		break;
	default:
		break;
	}
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

	// Compute next position.
	if (key == GLUT_KEY_LEFT) tempAngle = angle + 5.0;
	if (key == GLUT_KEY_RIGHT) tempAngle = angle - 5.0;
	if (key == GLUT_KEY_UP)
	{
		tempxVal = xVal - sin(angle * M_PI / 180.0);
		tempzVal = zVal - cos(angle * M_PI / 180.0);
	}
	if (key == GLUT_KEY_DOWN)
	{
		tempxVal = xVal + sin(angle * M_PI / 180.0);
		tempzVal = zVal + cos(angle * M_PI / 180.0);
	}

	// Angle correction.
	if (tempAngle > 360.0) tempAngle -= 360.0;
	if (tempAngle < 0.0) tempAngle += 360.0;

	// Move spacecraft to next position only if there will not be collision with an asteroid.
	if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle))
	{
		isCollision = 0;
		xVal = tempxVal;
		zVal = tempzVal;
		angle = tempAngle;
	}
	else isCollision = 1;

	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press the left/right arrow keys to turn the craft." << std::endl
		<< "Press the up/down arrow keys to move the craft." << std::endl;
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("spaceTravel.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
