#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Robot"

GLuint texture = 0;

BITMAP BMP;
HBITMAP hBMP = NULL;

float rx, ry, rz = 0.0;

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
	glPushMatrix();
	glTranslatef(0.0, 6, 0.0);
	head();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 4.8, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	neck();
	glPopMatrix();
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
	glPushMatrix();
		glTranslatef(2.5, 3, 0.0);
		upperLimbs();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(2.5, 0.1, 0.0);
		upperLimbs2();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(2.5, -2.8, 0.0);
		upperLimbs3();
	glPopMatrix();
}

void rightULimb() {
	glPushMatrix();
		glTranslatef(-2.5, 3, 0.0);
		upperLimbs();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-2.5, 0.1, 0.0);
		upperLimbs2();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-2.5, -2.8, 0.0);
		upperLimbs3();
	glPopMatrix();
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
	glPushMatrix();
		glTranslatef(1.3, -3.3, 0.0);
		lowerLimbs();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.3, -6.2, 0.0);
		lowerLimbs2();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.3, -9.0, 0.0);
		lowerLimbs3();
	glPopMatrix();
}

void rightDLimb() {
	glPushMatrix();
		glTranslatef(-1.3, -3.3, 0.0);
		lowerLimbs();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.3, -6.2, 0.0);
		lowerLimbs2();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.3, -9.0, 0.0);
		lowerLimbs3();
	glPopMatrix();
}

void robot() {
	// Head & Neck
	headNeck();

	// Upper Body
	upperBody();

	// Upper Limbs
	leftULimb();
	rightULimb();

	// Lower Body
	lowerBody();

	// Lower Limbs
	leftDLimb();
	rightDLimb();
}


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

	glMatrixMode(GL_MODELVIEW);

	if (rx != 0.0 || ry != 0.0 || rz != 0.0)
		glRotatef(0.05, rx, ry, rz);



	// vv Draw outline vv
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0, 0.0);
	robot();

	glDisable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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