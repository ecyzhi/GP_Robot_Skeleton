#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Robot"

// For Lighting
GLfloat fNormalX, fNormalY, fNormalZ = 0.0;

float lx = 8.0;
float ly = 8.0;
float lz = 8.0;
float lightPos[] = { lx, ly, lz };

float dif[] = { 1.0, 1.0, 1.0 };
float amb[] = { 1.0, 1.0, 1.0 };

float green[] = { 0.0, 1.0, 0.0 };
float yellow[] = { 1.0, 1.0, 0.0 };
float white[] = { 1.0, 1.0, 1.0 };

float lightSpeed = 0.5;
boolean lightOn = false;

// For Texture
GLuint texture = 0;

BITMAP BMP;
HBITMAP hBMP = NULL;


// For rotation
float rx, ry, rz = 0.0;
float rotateSpeed = 10;

// For part movement
int movePart = 0;

float moveSpeed = 5;

float mx, my, mz = 0.0;

float accmX[12] = { 0.0 };
float accmY[12] = { 0.0 };
float accmZ[12] = { 0.0 };

// TODO: Combine duplicating shapes Eg: upperlimbs, upperlimbs2, lowerlimbs, lowerlimbs2
// Improvement 1: Now when selecting both left and right part, the rotation will follow the left side due to getting value for mx,my,mz from accm[left]

//*******************************************************************
// Function: CalculateVectorNormal
// 
// Purpose: Given three points of a 3D plane, this function calculates
//          the normal vector of that plane.
// 
// Parameters:
//     fVert1[]   == array for 1st point (3 elements are x, y, and z).
//     fVert2[]   == array for 2nd point (3 elements are x, y, and z).
//     fVert3[]   == array for 3rd point (3 elements are x, y, and z).
// 
// Returns:
//     fNormalX   == X vector for the normal vector
//     fNormalY   == Y vector for the normal vector
//     fNormalZ   == Z vector for the normal vector
// 
// Comments:
// 
// History:  Date       Author        Reason
//           3/22/95     GGB           Created
//**********************************************************************
GLvoid CalculateVectorNormal(GLfloat fVert1[], GLfloat fVert2[], GLfloat fVert3[], GLfloat *fNormalX, GLfloat *fNormalY, GLfloat *fNormalZ) {
	GLfloat Qx, Qy, Qz, Px, Py, Pz;

	Qx = fVert2[0] - fVert1[0];
	Qy = fVert2[1] - fVert1[1];
	Qz = fVert2[2] - fVert1[2];
	Px = fVert3[0] - fVert1[0];
	Py = fVert3[1] - fVert1[1];
	Pz = fVert3[2] - fVert1[2];

	*fNormalX = Py * Qz - Pz * Qy;
	*fNormalY = Pz * Qx - Px * Qz;
	*fNormalZ = Px * Qy - Py * Qx;
}

// Easier to visualise the transformation matrix
void axisLine() {
	glPushAttrib(GL_CURRENT_BIT);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(3.0, 0.0, 0.0);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 3.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 3.0);
	glEnd();
	glPopAttrib();
}

// DEVELOPING BASIC SHAPES
void head() {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	//gluQuadricDrawStyle(sphere, GLU_LINE); //only draw line
	gluSphere(sphere, 1.5, 30, 10);
	gluDeleteQuadric(sphere);
}

void chest() {
	GLfloat chestVertices[8][3] = {
		2.5, 4.0, 1.0,
		2.5, 4.0, -1.0,
		-2.5, 4.0, -1.0,
		-2.5, 4.0, 1.0,
		1.5, 0.0, -0.8,
		1.5, 0.0, 0.8,
		-1.5, 0.0, 0.8,
		-1.5, 0.0, -0.8
	};

	glBegin(GL_QUADS);

	// Top
	CalculateVectorNormal(chestVertices[2], chestVertices[1], chestVertices[0], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(chestVertices[0]);	// 1
	glVertex3fv(chestVertices[1]); 	// 2
	glVertex3fv(chestVertices[2]);	// 3
	glVertex3fv(chestVertices[3]); 	// 4
			  
	// Front
	CalculateVectorNormal(chestVertices[3], chestVertices[0], chestVertices[5], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(chestVertices[5]);	// 6
	glVertex3fv(chestVertices[0]);	// 1
	glVertex3fv(chestVertices[3]); 	// 4
	glVertex3fv(chestVertices[6]);	// 7
			  
	// Right  
	CalculateVectorNormal(chestVertices[0], chestVertices[1], chestVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(chestVertices[4]);	// 5
	glVertex3fv(chestVertices[1]);	// 2
	glVertex3fv(chestVertices[0]);	// 1
	glVertex3fv(chestVertices[5]);	// 6
			  
	// Back	  
	CalculateVectorNormal(chestVertices[1], chestVertices[2], chestVertices[7], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(chestVertices[7]);	// 8
	glVertex3fv(chestVertices[2]);	// 3
	glVertex3fv(chestVertices[1]); 	// 2
	glVertex3fv(chestVertices[4]);	// 5
			  
	// Left	 
	CalculateVectorNormal(chestVertices[2], chestVertices[3], chestVertices[6], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(chestVertices[6]);	// 7
	glVertex3fv(chestVertices[3]);	// 4
	glVertex3fv(chestVertices[2]);	// 3
	glVertex3fv(chestVertices[7]);	// 8
			  
	// Bottom
	CalculateVectorNormal(chestVertices[6], chestVertices[5], chestVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(chestVertices[4]);	// 5
	glVertex3fv(chestVertices[5]);	// 6
	glVertex3fv(chestVertices[6]);	// 7
	glVertex3fv(chestVertices[7]);	// 8

	glEnd();
}

void hip() {
	GLfloat hipVertices[8][3] = {
		1.5, 0.0, 0.8,
		1.5, 0.0, -0.8,
		-1.5, 0.0, -0.8,
		-1.5, 0.0, 0.8,
		2.0, -1.0, -1.0,
		2.0, -1.0, 1.0,
		-2.0, -1.0, 1.0,
		-2.0, -1.0, -1.0
	};

	glBegin(GL_QUADS);
	// Top
	CalculateVectorNormal(hipVertices[2], hipVertices[1], hipVertices[0], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(hipVertices[0]);	// 1
	glVertex3fv(hipVertices[1]);	// 2
	glVertex3fv(hipVertices[2]);	// 3
	glVertex3fv(hipVertices[3]);	// 4

	// Front
	CalculateVectorNormal(hipVertices[3], hipVertices[0], hipVertices[5], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(hipVertices[5]);	// 6
	glVertex3fv(hipVertices[0]);	// 1
	glVertex3fv(hipVertices[3]);	// 4
	glVertex3fv(hipVertices[6]);	// 7

	// Right
	CalculateVectorNormal(hipVertices[0], hipVertices[1], hipVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(hipVertices[4]);	// 5
	glVertex3fv(hipVertices[1]);	// 2
	glVertex3fv(hipVertices[0]);	// 1
	glVertex3fv(hipVertices[5]);	// 6

	// Back
	CalculateVectorNormal(hipVertices[1], hipVertices[2], hipVertices[7], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(hipVertices[7]);	// 8
	glVertex3fv(hipVertices[2]);	// 3
	glVertex3fv(hipVertices[1]);	// 2
	glVertex3fv(hipVertices[4]);	// 5

	// Left
	CalculateVectorNormal(hipVertices[2], hipVertices[3], hipVertices[6], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(hipVertices[6]);	// 7
	glVertex3fv(hipVertices[3]);	// 4
	glVertex3fv(hipVertices[2]);	// 3
	glVertex3fv(hipVertices[7]);	// 8

	// Bottom
	CalculateVectorNormal(hipVertices[6], hipVertices[5], hipVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(hipVertices[4]);	// 5
	glVertex3fv(hipVertices[5]);	// 6
	glVertex3fv(hipVertices[6]);	// 7
	glVertex3fv(hipVertices[7]);	// 8

	glEnd();
}

void buttocks() {
	GLfloat buttocksVertices[8][3] = {
		2.0, 0.0, 1.0,
		2.0, 0.0, -1.0,
		-2.0, 0.0, -1.0,
		-2.0, 0.0, 1.0,
		0.5, -2.0, -0.8,
		0.5, -2.0, 0.8,
		-0.5, -2.0, 0.8,
		-0.5, -2.0, -0.8
	};

	glBegin(GL_QUADS);
	// Top
	CalculateVectorNormal(buttocksVertices[2], buttocksVertices[1], buttocksVertices[0], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(buttocksVertices[0]);	// 1
	glVertex3fv(buttocksVertices[1]);	// 2
	glVertex3fv(buttocksVertices[2]);	// 3
	glVertex3fv(buttocksVertices[3]);	// 4

	// Front
	CalculateVectorNormal(buttocksVertices[3], buttocksVertices[0], buttocksVertices[5], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(buttocksVertices[5]);	// 6
	glVertex3fv(buttocksVertices[0]);	// 1
	glVertex3fv(buttocksVertices[3]);	// 4
	glVertex3fv(buttocksVertices[6]);	// 7

	// Right
	CalculateVectorNormal(buttocksVertices[0], buttocksVertices[1], buttocksVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(buttocksVertices[4]);	// 5
	glVertex3fv(buttocksVertices[1]);	// 2
	glVertex3fv(buttocksVertices[0]);	// 1
	glVertex3fv(buttocksVertices[5]);	// 6

	// Back
	CalculateVectorNormal(buttocksVertices[1], buttocksVertices[2], buttocksVertices[7], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(buttocksVertices[7]);	// 8
	glVertex3fv(buttocksVertices[2]);	// 3
	glVertex3fv(buttocksVertices[1]);	// 2
	glVertex3fv(buttocksVertices[4]);	// 5

	// Left
	CalculateVectorNormal(buttocksVertices[2], buttocksVertices[3], buttocksVertices[6], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(buttocksVertices[6]);	// 7
	glVertex3fv(buttocksVertices[3]);	// 4
	glVertex3fv(buttocksVertices[2]);	// 3
	glVertex3fv(buttocksVertices[7]);	// 8

	// Bottom
	CalculateVectorNormal(buttocksVertices[6], buttocksVertices[5], buttocksVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(buttocksVertices[4]);	// 5
	glVertex3fv(buttocksVertices[5]);	// 6
	glVertex3fv(buttocksVertices[6]);	// 7
	glVertex3fv(buttocksVertices[7]);	// 8

	glEnd();
}

void neck() {
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();

	//gluQuadricDrawStyle(cylinder, GLU_LINE);
	gluCylinder(cylinder, 0.5, 0.7, 1.0, 10, 10);
	gluDeleteQuadric(cylinder);
}

void waist() {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	//gluQuadricDrawStyle(sphere, GLU_LINE); //only draw line
	gluSphere(sphere, 1.3, 30, 10);
	gluDeleteQuadric(sphere);
}

void joint() {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	//gluQuadricDrawStyle(sphere, GLU_LINE); //only draw line
	gluSphere(sphere, 0.5, 30, 10);
	gluDeleteQuadric(sphere);
}

void limb() {
	GLfloat limbVertices[8][3] = {
		{0.5, 0.0, 0.5},
		{0.5, 0.0, -0.5},
		{-0.5, 0.0, -0.5},
		{-0.5, 0.0, 0.5},
		{0.5, -2.5, -0.5},
		{0.5, -2.5, 0.5},
		{-0.5, -2.5, 0.5},
		{-0.5, -2.5, -0.5}
	};

	glBegin(GL_QUADS);

	// Top
	CalculateVectorNormal(limbVertices[2], limbVertices[1], limbVertices[0], &fNormalX,  &fNormalY,  &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(limbVertices[0]);		// 1
	glVertex3fv(limbVertices[1]); 		// 2
	glVertex3fv(limbVertices[2]);		// 3
	glVertex3fv(limbVertices[3]); 		// 4
			  
	// Front
	CalculateVectorNormal(limbVertices[3], limbVertices[0], limbVertices[5], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(limbVertices[5]);		// 6
	glVertex3fv(limbVertices[0]);		// 1
	glVertex3fv(limbVertices[3]); 		// 4
	glVertex3fv(limbVertices[6]);		// 7
			  
	// Right
	CalculateVectorNormal(limbVertices[0], limbVertices[1], limbVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(limbVertices[4]);		// 5
	glVertex3fv(limbVertices[1]); 		// 2
	glVertex3fv(limbVertices[0]);		// 1
	glVertex3fv(limbVertices[5]);		// 6
			  
	// Back
	CalculateVectorNormal(limbVertices[1], limbVertices[2], limbVertices[7], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(limbVertices[7]);		// 8
	glVertex3fv(limbVertices[2]);		// 3
	glVertex3fv(limbVertices[1]); 		// 2
	glVertex3fv(limbVertices[4]);		// 5
			  
	// Left
	CalculateVectorNormal(limbVertices[2], limbVertices[3], limbVertices[6], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(limbVertices[6]);		// 7
	glVertex3fv(limbVertices[3]); 		// 4
	glVertex3fv(limbVertices[2]);		// 3
	glVertex3fv(limbVertices[7]);		// 8
			  
	// Bottom 
	CalculateVectorNormal(limbVertices[6], limbVertices[5], limbVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(limbVertices[4]);		// 5
	glVertex3fv(limbVertices[5]);		// 6
	glVertex3fv(limbVertices[6]);		// 7
	glVertex3fv(limbVertices[7]);		// 8

	glEnd();
}

void palm() {
	GLfloat palmVertices[8][3] = {
		0.5, 0.0, 0.5,
		0.5, 0.0, -0.5,
		-0.5, 0.0, -0.5,
		-0.5, 0.0, 0.5,
		0.2, -1.5, -0.4,
		0.2, -1.5, 0.4,
		-0.2, -1.5, 0.4,
		-0.2, -1.5, -0.4
	};

	glBegin(GL_QUADS);
	// Top
	CalculateVectorNormal(palmVertices[2], palmVertices[1], palmVertices[0], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(palmVertices[0]);	// 1
	glVertex3fv(palmVertices[1]);	// 2
	glVertex3fv(palmVertices[2]);	// 3
	glVertex3fv(palmVertices[3]);	// 4

	// Front
	CalculateVectorNormal(palmVertices[3], palmVertices[0], palmVertices[5], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(palmVertices[5]);	// 6
	glVertex3fv(palmVertices[0]);	// 1
	glVertex3fv(palmVertices[3]);	// 4
	glVertex3fv(palmVertices[6]);	// 7

	// Right
	CalculateVectorNormal(palmVertices[0], palmVertices[1], palmVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(palmVertices[4]);	// 5
	glVertex3fv(palmVertices[1]);	// 2
	glVertex3fv(palmVertices[0]);	// 1
	glVertex3fv(palmVertices[5]);	// 6

	// Back
	CalculateVectorNormal(palmVertices[1], palmVertices[2], palmVertices[7], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(palmVertices[7]);	// 8
	glVertex3fv(palmVertices[2]);	// 3
	glVertex3fv(palmVertices[1]);	// 2
	glVertex3fv(palmVertices[4]);	// 5

	// Left
	CalculateVectorNormal(palmVertices[2], palmVertices[3], palmVertices[6], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(palmVertices[6]);	// 7
	glVertex3fv(palmVertices[3]);	// 4
	glVertex3fv(palmVertices[2]);	// 3
	glVertex3fv(palmVertices[7]);	// 8

	// Bottom
	CalculateVectorNormal(palmVertices[6], palmVertices[5], palmVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(palmVertices[4]);	// 5
	glVertex3fv(palmVertices[5]);	// 6
	glVertex3fv(palmVertices[6]);	// 7
	glVertex3fv(palmVertices[7]);	// 8

	glEnd();
}

void foot() {
	GLfloat footVertices[10][3] = {
		0.5, 0.0, 0.5,
		0.5, 0.0, -0.5,
		-0.5, 0.0, -0.5,
		-0.5, 0.0, 0.5,
		0.5, -0.8, 2.0,
		-0.5, -0.8, 2.0,
		0.5, -1.5, -0.5,
		0.5, -1.5, 2.0,
		-0.5, -1.5, 2.0,
		-0.5, -1.5, -0.5
	};

	glBegin(GL_QUADS);
	// Top
	CalculateVectorNormal(footVertices[2], footVertices[1], footVertices[0], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(footVertices[0]);	// 1
	glVertex3fv(footVertices[1]);	// 2
	glVertex3fv(footVertices[2]);	// 3
	glVertex3fv(footVertices[3]);	// 4

	// Top2
	CalculateVectorNormal(footVertices[3], footVertices[0], footVertices[4], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(footVertices[4]);	// 5
	glVertex3fv(footVertices[0]);	// 1
	glVertex3fv(footVertices[3]);	// 4
	glVertex3fv(footVertices[5]);	// 6

	// Front
	CalculateVectorNormal(footVertices[5], footVertices[4], footVertices[7], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(footVertices[7]);	// 8
	glVertex3fv(footVertices[4]);	// 5
	glVertex3fv(footVertices[5]);	// 6
	glVertex3fv(footVertices[8]);	// 9

	// Bottom
	CalculateVectorNormal(footVertices[9], footVertices[6], footVertices[7], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(footVertices[7]);	// 8
	glVertex3fv(footVertices[6]);	// 7
	glVertex3fv(footVertices[9]);	// 10
	glVertex3fv(footVertices[8]);	// 9

	// Back
	CalculateVectorNormal(footVertices[1], footVertices[2], footVertices[9], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(footVertices[9]);	// 10
	glVertex3fv(footVertices[2]);	// 3
	glVertex3fv(footVertices[1]);	// 2
	glVertex3fv(footVertices[6]);	// 7
	glEnd();

	glBegin(GL_POLYGON);
	// Right
	CalculateVectorNormal(footVertices[0], footVertices[1], footVertices[6], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(footVertices[6]);	// 7
	glVertex3fv(footVertices[1]);	// 2
	glVertex3fv(footVertices[0]);	// 1
	glVertex3fv(footVertices[4]);	// 5
	glVertex3fv(footVertices[7]);	// 8
	glEnd();

	glBegin(GL_POLYGON);
	// Left
	CalculateVectorNormal(footVertices[3], footVertices[5], footVertices[8], &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);
	glVertex3fv(footVertices[8]);	// 9
	glVertex3fv(footVertices[5]);	// 6
	glVertex3fv(footVertices[3]);	// 4
	glVertex3fv(footVertices[2]);	// 3
	glVertex3fv(footVertices[9]);	// 10
	glEnd();
}

// CONSTRUCTING HUMANOID
void headNeck() {
	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 1) {
		glColor3f(1.0, 0.0, 0.0);
	}


	glPushMatrix();
		glTranslatef(0.0, 6, 0.0);
		head();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, 4.8, 0.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		neck();
	glPopMatrix();

	glPopAttrib();
}

void upperBody() {
	glPushMatrix();
		chest();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, -0.5, 0.0);
		waist();
	glPopMatrix();
}

void upperLimbs() {
	glPushMatrix();
		joint();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, -0.2, 0.0);
		limb();
	glPopMatrix();
}

void upperLimbs2() {
	glPushMatrix();
		joint();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, -0.2, 0.0);
		limb();
	glPopMatrix();
}

void upperLimbs3() {
	glPushMatrix();
		joint();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, -0.2, 0.0);
		palm();
	glPopMatrix();
}

void leftULimb() {
	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 31 || movePart == 33) {
		glColor3f(1.0, 0.0, 0.0);
	}

	//glPushMatrix();
	//glRotatef(accmX[3], 1, 0, 0);
	//glRotatef(accmY[3], 0, 1, 0);
	//glRotatef(accmZ[3], 0, 0, 1);
	//if (movePart == 31 || movePart == 33) {
	//	accmX[3] = mx;
	//	accmY[3] = my;
	//	accmZ[3] = mz;

	//	axisLine();
	//}

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 41 || movePart == 43) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(2.5, 3, 0.0);
		glRotatef(accmX[3], 1, 0, 0);
		glRotatef(accmY[3], 0, 1, 0);
		glRotatef(accmZ[3], 0, 0, 1);
		if (movePart == 41 || movePart == 43) {
			accmX[3] = mx;
			accmY[3] = my;
			accmZ[3] = mz;

			axisLine();
		}
		upperLimbs();
	glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 51 || movePart == 53) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(2.5, 0.1, 0.0);
		glRotatef(accmX[5], 1, 0, 0);
		glRotatef(accmY[5], 0, 1, 0);
		glRotatef(accmZ[5], 0, 0, 1);
		if (movePart == 51 || movePart == 53) {
			accmX[5] = mx;
			accmY[5] = my;
			accmZ[5] = mz;

			axisLine();
		}
		upperLimbs2();
	glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 61 || movePart == 63) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(2.5, -2.8, 0.0);
		upperLimbs3();
	glPopMatrix();

	//glPopMatrix();
	glPopAttrib();

	glPopAttrib();
}

void rightULimb() {
	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 32 || movePart == 33) {
		glColor3f(1.0, 0.0, 0.0);
	}

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 42 || movePart == 43) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(-2.5, 3, 0.0);
		glRotatef(accmX[4], 1, 0, 0);
		glRotatef(accmY[4], 0, 1, 0);
		glRotatef(accmZ[4], 0, 0, 1);
		if (movePart == 42 || movePart == 43) {
			accmX[4] = mx;
			accmY[4] = my;
			accmZ[4] = mz;

			axisLine();
		}
		upperLimbs();
	glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 52 || movePart == 53) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(-2.5, 0.1, 0.0);
		glRotatef(accmX[6], 1, 0, 0);
		glRotatef(accmY[6], 0, 1, 0);
		glRotatef(accmZ[6], 0, 0, 1);
		if (movePart == 52 || movePart == 53) {
			accmX[6] = mx;
			accmY[6] = my;
			accmZ[6] = mz;

			axisLine();
		}
		upperLimbs2();
	glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 62 || movePart == 63) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(-2.5, -2.8, 0.0);
		upperLimbs3();
	glPopMatrix();
	glPopAttrib();

	glPopAttrib();
}

// This part will not rotate and only translate together with upperbody and headneck when walking
void lowerBody() {
	glPushMatrix();
	glTranslatef(0.0, -1, 0.0);
	hip();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -2, 0.0);
	buttocks();
	glPopMatrix();
}

void lowerLimbs() {
	glPushMatrix();
	joint();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.2, 0.0);
	limb();
	glPopMatrix();
}

void lowerLimbs2() {
	glPushMatrix();
	joint();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.2, 0.0);
	limb();
	glPopMatrix();
}

void lowerLimbs3() {
	glPushMatrix();
		joint();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, -0.2, 0.0);
		foot();
	glPopMatrix();
}

void leftDLimb() {
	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 71 || movePart == 73) {
		glColor3f(1.0, 0.0, 0.0);
	}

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 81 || movePart == 83) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(1.3, -3.3, 0.0);
		lowerLimbs();
	glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 91 || movePart == 93) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(1.3, -6.2, 0.0);
		lowerLimbs2();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.3, -9.0, 0.0);
		lowerLimbs3();
	glPopMatrix();
	glPopAttrib();

	glPopAttrib();
}

void rightDLimb() {
	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 72 || movePart == 73) {
		glColor3f(1.0, 0.0, 0.0);
	}

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 82 || movePart == 83) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(-1.3, -3.3, 0.0);
		lowerLimbs();
	glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 92 || movePart == 93) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(-1.3, -6.2, 0.0);
		lowerLimbs2();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.3, -9.0, 0.0);
		lowerLimbs3();
	glPopMatrix();
	glPopAttrib();

	glPopAttrib();
}

void robot() {
	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 2) {
		glColor3f(1.0, 0.0, 0.0);
	}
	// Head & Neck
	headNeck();

	// Upper Body
	upperBody();

	// Upper Limbs
	leftULimb();
	rightULimb();

	glPopAttrib();

	// Lower Body
	lowerBody();

	// Lower Limbs
	leftDLimb();
	rightDLimb();
}

// Map the light source 
void lightSource() {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	//gluQuadricDrawStyle(sphere, GLU_LINE); //only draw line
	gluSphere(sphere, 0.5, 10, 10);
	gluDeleteQuadric(sphere);
}


//--------------------------------------------------------------------
LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		if (wParam == 'W') { rx = -1.0; ry = 0.0; rz = 0.0; glRotatef(rotateSpeed, -1.0, 0.0, 0.0); }
		if (wParam == 'S') { rx = 1.0; ry = 0.0; rz = 0.0;	glRotatef(rotateSpeed, 1.0, 0.0, 0.0);  }
		if (wParam == 'A') { rx = 0.0; ry = -1.0; rz = 0.0; glRotatef(rotateSpeed, 0.0, -1.0, 0.0); }
		if (wParam == 'D') { rx = 0.0; ry = 1.0; rz = 0.0;	glRotatef(rotateSpeed, 0.0, 1.0, 0.0);	}
		if (wParam == 'Q') { rx = 0.0; ry = 0.0; rz = 1.0;	glRotatef(rotateSpeed, 0.0, 0.0, 1.0);	}
		if (wParam == 'E') { rx = 0.0; ry = 0.0; rz = -1.0; glRotatef(rotateSpeed, 0.0, 0.0, -1.0); }
		if (wParam == VK_SPACE) {
			rx = 0.0; 
			ry = 0.0; 
			rz = 0.0; 
			lx = 8.0;
			ly = 8.0;
			lz = 8.0;
			lightOn = false;
			lightPos[0] = lx;
			lightPos[1] = ly;
			lightPos[2] = lz;
			movePart = 0;
			mx = 0.0;
			my = 0.0;
			mz = 0.0;

			for (int i = 0; i < 12; i++) {
				accmX[i] = 0.0;
				accmY[i] = 0.0;
				accmZ[i] = 0.0;
			}

			glLoadIdentity(); 
		}
		if (wParam == 0x30) {
			movePart = 0;
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
		}
		if (wParam == 0x31) {
			movePart = 1;
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
		}
		if (wParam == 0x32) {
			movePart = 2;
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
		}
		if (wParam == 0x33) {
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
			if (movePart == 31) 
				movePart = 32;
			else if (movePart == 32)
				movePart = 33;
			else
				movePart = 31;
		}
		if (wParam == 0x34) {
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
			if (movePart == 41) {
				movePart = 42;
				mx = accmX[4];
				my = accmY[4];
				mz = accmZ[4];
			}
			else if (movePart == 42) {
				movePart = 43;
				mx = accmX[3];
				my = accmY[3];
				mz = accmZ[3];
			}
			else {
				movePart = 41;
				mx = accmX[3];
				my = accmY[3];
				mz = accmZ[3];
			}
		}
		if (wParam == 0x35) {
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
			if (movePart == 51) {
				movePart = 52;
				mx = accmX[6];
				my = accmY[6];
				mz = accmZ[6];
			}
			else if (movePart == 52) {
				movePart = 53;
				mx = accmX[5];
				my = accmY[5];
				mz = accmZ[5];
			}
			else {
				movePart = 51;
				mx = accmX[5];
				my = accmY[5];
				mz = accmZ[5];
			}
		}
		if (wParam == 0x36) {
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
			if (movePart == 61)
				movePart = 62;
			else if (movePart == 62)
				movePart = 63;
			else
				movePart = 61;
		}
		if (wParam == 0x37) {
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
			if (movePart == 71)
				movePart = 72;
			else if (movePart == 72)
				movePart = 73;
			else
				movePart = 71;
		}
		if (wParam == 0x38) {
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
			if (movePart == 81)
				movePart = 82;
			else if (movePart == 82)
				movePart = 83;
			else
				movePart = 81;
		}
		if (wParam == 0x39) {
			mx = 0.0;
			my = 0.0;
			mz = 0.0;
			if (movePart == 91)
				movePart = 92;
			else if (movePart == 92)
				movePart = 93;
			else
				movePart = 91;
		}
		if (wParam == 'I') {
			ly += lightSpeed;
			lightPos[1] = ly;
		}
		if (wParam == 'K') {
			ly -= lightSpeed;
			lightPos[1] = ly;
		}
		if (wParam == 'J') {
			lx -= lightSpeed;
			lightPos[0] = lx;
		}
		if (wParam == 'L') {
			lx += lightSpeed;
			lightPos[0] = lx;
		}
		if (wParam == 'O') {
			lz -= lightSpeed;
			lightPos[2] = lz;
		}
		if (wParam == 'U') {
			lz += lightSpeed;
			lightPos[2] = lz;
		}
		if (wParam == 'P') {
			if (lightOn)
				lightOn = false;
			else
				lightOn = true;
		}
		if (wParam == 'Z') {
			lx = 8.0;
			ly = 8.0;
			lz = 8.0;
			lightOn = true;
			lightPos[0] = lx;
			lightPos[1] = ly;
			lightPos[2] = lz;
		}
		if (wParam == 'T') { mx -= moveSpeed;	my += 0.0;			mz += 0.0;		}
		if (wParam == 'G') { mx += moveSpeed;	my += 0.0;			mz += 0.0;		}
		if (wParam == 'F') { mx += 0.0;			my += 0.0;			mz -= moveSpeed;}
		if (wParam == 'H') { mx += 0.0;			my += 0.0;			mz += moveSpeed;}
		if (wParam == 'Y') { mx += 0.0;			my += moveSpeed;	mz += 0.0;		}
		if (wParam == 'R') { mx += 0.0;			my -= moveSpeed;	mz += 0.0;		}
		
		

		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	if (lightOn)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);


	glMatrixMode(GL_MODELVIEW);

	// Set up lighting
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

	if (lightOn) {
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}

	glShadeModel(GL_SMOOTH);

	if (lightOn) {
		// White color for light source
		glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
		glMaterialfv(GL_BACK, GL_AMBIENT, white);

		// Indicate the light source
		glPushMatrix();
		glTranslatef(lx, ly, lz);
		lightSource();
		glPopMatrix();
	}

	// Whole model continuous rotation
	//if (rx != 0.0 || ry != 0.0 || rz != 0.0)
	//	glRotatef(0.05, rx, ry, rz);

	// Green color for robot
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_BACK, GL_AMBIENT, green);

	// vv Draw outline vv
	glDisable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	robot();

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	robot();
	// ^^ Draw outline ^^

	

}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 700, 700,
		NULL, NULL, wc.hInstance, NULL);

	// For full screen
	//SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0L);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// Set the camera before display (while loop), then only move the model in display
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	//glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);
	//gluPerspective(60.0, 1.0, -1.0, 1.0);
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 6.0);

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------