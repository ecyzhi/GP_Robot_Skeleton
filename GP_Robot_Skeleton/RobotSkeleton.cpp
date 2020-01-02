#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Robot"

// For Lighting
GLfloat fNormalX, fNormalY, fNormalZ = 0.0;
GLfloat pointArray[] = { 0.0, 0.0, 0.0 };

float lx = 0.0;
float ly = 0.8;
float lz = 0.8;
float lightPos[] = { lx, ly, lz };

float dif[] = { 1.0, 1.0, 1.0 };
float green[] = { 0.0, 1.0, 0.0 };

float lightSpeed = 0.1;
boolean lightOn = false;

// For Texture
GLuint texture = 0;

BITMAP BMP;
HBITMAP hBMP = NULL;


// For rotation
float rx, ry, rz = 0.0;

// For part movement
int movePart = 0;
float mx, my, mz = 0.0;
float moveSpeed = 0.1;

// TODO: Combine duplicating shapes Eg: upperlimbs, upperlimbs2, lowerlimbs, lowerlimbs2

// DEVELOPING BASIC SHAPES
void head() {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();

	//gluQuadricDrawStyle(sphere, GLU_LINE); //only draw line
	gluSphere(sphere, 1.5, 30, 10);
	gluDeleteQuadric(sphere);
}

void chest() {
	glBegin(GL_POLYGON);
	// Top
	glVertex3f(2.5, 4, 1);		// 1
	glVertex3f(2.5, 4, -1); 		// 2
	glVertex3f(-2.5, 4, -1);		// 3
	glVertex3f(-2.5, 4, 1); 		// 4

	// Front
	glVertex3f(1.5, 0, 0.8);	// 6
	glVertex3f(2.5, 4, 1);		// 1
	glVertex3f(-2.5, 4, 1); 		// 4
	glVertex3f(-1.5, 0, 0.8);	// 7

	// Right
	glVertex3f(1.5, 0, -0.8);	// 5
	glVertex3f(2.5, 4, -1); 		// 2
	glVertex3f(2.5, 4, 1);		// 1
	glVertex3f(1.5, 0, 0.8);	// 6

	// Back
	glVertex3f(-1.5, 0, -0.8);	// 8
	glVertex3f(-2.5, 4, -1);		// 3
	glVertex3f(2.5, 4, -1); 		// 2
	glVertex3f(1.5, 0, -0.8);	// 5

	// Left
	glVertex3f(-1.5, 0, 0.8);	// 7
	glVertex3f(-2.5, 4, 1); 		// 4
	glVertex3f(-2.5, 4, -1);		// 3
	glVertex3f(-1.5, 0, -0.8);	// 8

	// Bottom
	glVertex3f(1.5, 0, -0.8);	// 5
	glVertex3f(1.5, 0, 0.8);	// 6
	glVertex3f(-1.5, 0, 0.8);	// 7
	glVertex3f(-1.5, 0, -0.8);	// 8

	glEnd();
}

void hip() {
	glBegin(GL_POLYGON);
	// Top
	glVertex3f(1.5, 0, 0.8);	// 1
	glVertex3f(1.5, 0, -0.8);	// 2
	glVertex3f(-1.5, 0, -0.8);	// 3
	glVertex3f(-1.5, 0, 0.8);	// 4
	
	// Front
	glVertex3f(2, -1, 1);		// 6
	glVertex3f(1.5, 0, 0.8);	// 1
	glVertex3f(-1.5, 0, 0.8);	// 4
	glVertex3f(-2, -1, 1);		// 7

	// Right
	glVertex3f(2, -1, -1);		// 5
	glVertex3f(1.5, 0, -0.8);	// 2
	glVertex3f(1.5, 0, 0.8);	// 1
	glVertex3f(2, -1, 1);		// 6

	// Back
	glVertex3f(-2, -1, -1);		// 8
	glVertex3f(-1.5, 0, -0.8);	// 3
	glVertex3f(1.5, 0, -0.8);	// 2
	glVertex3f(2, -1, -1);		// 5

	// Left
	glVertex3f(-2, -1, 1);		// 7
	glVertex3f(-1.5, 0, 0.8);	// 4
	glVertex3f(-1.5, 0, -0.8);	// 3
	glVertex3f(-2, -1, -1);		// 8

	// Bottom
	glVertex3f(2, -1, -1);		// 5
	glVertex3f(2, -1, 1);		// 6
	glVertex3f(-2, -1, 1);		// 7
	glVertex3f(-2, -1, -1);		// 8

	glEnd();
}

void buttocks() {
	glBegin(GL_POLYGON);
	// Top
	glVertex3f(2, 0, 1);		// 1
	glVertex3f(2, 0, -1);		// 2
	glVertex3f(-2, 0, -1);		// 3
	glVertex3f(-2, 0, 1);		// 4

	// Front
	glVertex3f(0.5, -2, 0.8);	// 6
	glVertex3f(2, 0, 1);		// 1
	glVertex3f(-2, 0, 1);		// 4
	glVertex3f(-0.5, -2, 0.8);	// 7

	// Right
	glVertex3f(0.5, -2, -0.8);	// 5
	glVertex3f(2, 0, -1);		// 2
	glVertex3f(2, 0, 1);		// 1
	glVertex3f(0.5, -2, 0.8);	// 6

	// Back
	glVertex3f(-0.5, -2, -0.8);	// 8
	glVertex3f(-2, 0, -1);		// 3
	glVertex3f(2, 0, -1);		// 2
	glVertex3f(0.5, -2, -0.8);	// 5

	// Left
	glVertex3f(-0.5, -2, 0.8);	// 7
	glVertex3f(-2, 0, 1);		// 4
	glVertex3f(-2, 0, -1);		// 3
	glVertex3f(-0.5, -2, -0.8);	// 8

	// Bottom
	glVertex3f(0.5, -2, -0.8);	// 5
	glVertex3f(0.5, -2, 0.8);	// 6
	glVertex3f(-0.5, -2, 0.8);	// 7
	glVertex3f(-0.5, -2, -0.8);	// 8

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
	glBegin(GL_POLYGON);
	// Top
	glVertex3f(0.5, 0, 0.5);		// 1
	glVertex3f(0.5, 0, -0.5); 		// 2
	glVertex3f(-0.5, 0, -0.5);		// 3
	glVertex3f(-0.5, 0, 0.5); 		// 4

	// Front
	glVertex3f(0.5, -2.5, 0.5);		// 6
	glVertex3f(0.5, 0, 0.5);		// 1
	glVertex3f(-0.5, 0, 0.5); 		// 4
	glVertex3f(-0.5, -2.5, 0.5);	// 7

	// Right
	glVertex3f(0.5, -2.5, -0.5);	// 5
	glVertex3f(0.5, 0, -0.5); 		// 2
	glVertex3f(0.5, 0, 0.5);		// 1
	glVertex3f(0.5, -2.5, 0.5);		// 6

	// Back
	glVertex3f(-0.5, -2.5, -0.5);	// 8
	glVertex3f(-0.5, 0, -0.5);		// 3
	glVertex3f(0.5, 0, -0.5); 		// 2
	glVertex3f(0.5, -2.5, -0.5);	// 5

	// Left
	glVertex3f(-0.5, -2.5, 0.5);	// 7
	glVertex3f(-0.5, 0, 0.5); 		// 4
	glVertex3f(-0.5, 0, -0.5);		// 3
	glVertex3f(-0.5, -2.5, -0.5);	// 8

	// Bottom
	glVertex3f(0.5, -2.5, -0.5);	// 5
	glVertex3f(0.5, -2.5, 0.5);		// 6
	glVertex3f(-0.5, -2.5, 0.5);	// 7
	glVertex3f(-0.5, -2.5, -0.5);	// 8

	glEnd();
}

void palm() {
	glBegin(GL_POLYGON);
	// Top
	glVertex3f(0.5, 0, 0.5);		// 1
	glVertex3f(0.5, 0, -0.5); 		// 2
	glVertex3f(-0.5, 0, -0.5);		// 3
	glVertex3f(-0.5, 0, 0.5); 		// 4

	// Front
	glVertex3f(0.2, -1.5, 0.4);		// 6
	glVertex3f(0.5, 0, 0.5);		// 1
	glVertex3f(-0.5, 0, 0.5); 		// 4
	glVertex3f(-0.2, -1.5, 0.4);	// 7

	// Right
	glVertex3f(0.2, -1.5, -0.4);	// 5
	glVertex3f(0.5, 0, -0.5); 		// 2
	glVertex3f(0.5, 0, 0.5);		// 1
	glVertex3f(0.2, -1.5, 0.4);		// 6

	// Back
	glVertex3f(-0.2, -1.5, -0.4);	// 8
	glVertex3f(-0.5, 0, -0.5);		// 3
	glVertex3f(0.5, 0, -0.5); 		// 2
	glVertex3f(0.2, -1.5, -0.4);	// 5

	// Left
	glVertex3f(-0.2, -1.5, 0.4);	// 7
	glVertex3f(-0.5, 0, 0.5); 		// 4
	glVertex3f(-0.5, 0, -0.5);		// 3
	glVertex3f(-0.2, -1.5, -0.4);	// 8

	// Bottom
	glVertex3f(0.2, -1.5, -0.4);	// 5
	glVertex3f(0.2, -1.5, 0.4);		// 6
	glVertex3f(-0.2, -1.5, 0.4);	// 7
	glVertex3f(-0.2, -1.5, -0.4);	// 8

	glEnd();
}

void foot() {
	glBegin(GL_QUADS);
	// Top
	glVertex3f(0.5, 0, 0.5);		// 1
	glVertex3f(0.5, 0, -0.5); 		// 2
	glVertex3f(-0.5, 0, -0.5);		// 3
	glVertex3f(-0.5, 0, 0.5); 		// 4

	// Top2
	glVertex3f(0.5, -0.8, 2.0);		// 5
	glVertex3f(0.5, 0, 0.5);		// 1
	glVertex3f(-0.5, 0, 0.5); 		// 4
	glVertex3f(-0.5, -0.8, 2.0);	// 6

	// Front
	glVertex3f(0.5, -1.5, 2.0);		// 8
	glVertex3f(0.5, -0.8, 2.0);		// 5
	glVertex3f(-0.5, -0.8, 2.0);	// 6
	glVertex3f(-0.5, -1.5, 2.0);	// 9

	// Bottom
	glVertex3f(0.5, -1.5, 2.0);		// 8
	glVertex3f(0.5, -1.5, -0.5);	// 7
	glVertex3f(-0.5, -1.5, -0.5);	// 10
	glVertex3f(-0.5, -1.5, 2.0);	// 9

	// Back
	glVertex3f(-0.5, -1.5, -0.5);	// 10
	glVertex3f(-0.5, 0, -0.5);		// 3
	glVertex3f(0.5, 0, -0.5); 		// 2
	glVertex3f(0.5, -1.5, -0.5);	// 7
	glEnd();

	glBegin(GL_POLYGON);
	// Right
	glVertex3f(0.5, -1.5, -0.5);	// 7
	glVertex3f(0.5, 0, -0.5); 		// 2
	glVertex3f(0.5, 0, 0.5);		// 1
	glVertex3f(0.5, -0.8, 2.0);		// 5
	glVertex3f(0.5, -1.5, 2.0);		// 8
	glEnd();

	glBegin(GL_POLYGON);
	// Left
	glVertex3f(-0.5, -1.5, 2.0);	// 9
	glVertex3f(-0.5, -0.8, 2.0);	// 6
	glVertex3f(-0.5, 0, 0.5); 		// 4
	glVertex3f(-0.5, 0, -0.5);		// 3
	glVertex3f(-0.5, -1.5, -0.5);	// 10
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

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 41 || movePart == 43) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(2.5, 3, 0.0);
		upperLimbs();
	glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 51 || movePart == 53) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(2.5, 0.1, 0.0);
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
		upperLimbs();
	glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_CURRENT_BIT);
	if (movePart == 52 || movePart == 53) {
		glColor3f(1.0, 0.0, 0.0);
	}
	glPushMatrix();
		glTranslatef(-2.5, 0.1, 0.0);
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

// To convert floating points to array for vertices
GLfloat *pointArrConv(GLfloat pointX, GLfloat pointY, GLfloat pointZ, GLfloat *pointArray) {
	pointArray[0] = pointX;
	pointArray[1] = pointY;
	pointArray[2] = pointZ;

	return pointArray;
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
		if (wParam == 'W') { rx = -1.0; ry = 0.0; rz = 0.0; }
		if (wParam == 'S') { rx = 1.0; ry = 0.0; rz = 0.0; }
		if (wParam == 'A') { rx = 0.0; ry = -1.0; rz = 0.0; }
		if (wParam == 'D') { rx = 0.0; ry = 1.0; rz = 0.0; }
		if (wParam == 'Q') { rx = 0.0; ry = 0.0; rz = 1.0; }
		if (wParam == 'E') { rx = 0.0; ry = 0.0; rz = -1.0; }
		if (wParam == VK_SPACE) { rx = 0.0; ry = 0.0; rz = 0.0; glLoadIdentity(); }
		if (wParam == 0x31) {
			movePart = 1;
		}
		if (wParam == 0x32) {
			movePart = 2;
		}
		if (wParam == 0x33) {
			if (movePart == 31) 
				movePart = 32;
			else if (movePart == 32)
				movePart = 33;
			else
				movePart = 31;
		}
		if (wParam == 0x34) {
			if (movePart == 41)
				movePart = 42;
			else if (movePart == 42)
				movePart = 43;
			else
				movePart = 41;
		}
		if (wParam == 0x35) {
			if (movePart == 51)
				movePart = 52;
			else if (movePart == 52)
				movePart = 53;
			else
				movePart = 51;
		}
		if (wParam == 0x36) {
			if (movePart == 61)
				movePart = 62;
			else if (movePart == 62)
				movePart = 63;
			else
				movePart = 61;
		}
		if (wParam == 0x37) {
			if (movePart == 71)
				movePart = 72;
			else if (movePart == 72)
				movePart = 73;
			else
				movePart = 71;
		}
		if (wParam == 0x38) {
			if (movePart == 81)
				movePart = 82;
			else if (movePart == 82)
				movePart = 83;
			else
				movePart = 81;
		}
		if (wParam == 0x39) {
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
			lx = 0.0;
			ly = 0.8;
			lz = 0.8;
			lightOn = false;
			lightPos[0] = lx;
			lightPos[1] = ly;
			lightPos[2] = lz;
		}

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
	if (lightOn)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_BACK, GL_AMBIENT, green);

	// Indicate the light source
	glPointSize(20);
	glBegin(GL_POINTS);
	glVertex3f(lx, ly, lz);
	glEnd();

	// Whole model rotation
	if (rx != 0.0 || ry != 0.0 || rz != 0.0)
		glRotatef(0.05, rx, ry, rz);

	// vv Draw outline vv
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	robot();

	glDisable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	// ^^ Draw outline ^^

	robot();

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