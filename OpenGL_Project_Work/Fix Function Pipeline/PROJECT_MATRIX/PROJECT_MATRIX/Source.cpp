//PROJECT_MATRIX

// header
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<time.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#define WIN_WIDTH 1366
#define WIN_HEIGHT 768

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL; // OpenGL Rendering Context
FILE* gpfile = NULL;
DWORD dwstyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
bool bDone = false;
bool gbActiveWindow = false;
bool gbFullScreen = false;

GLfloat x = -7.899996f;
GLfloat y = 3.600004;
GLfloat z = -12.200006f;
GLfloat incdec = 0.1f;
GLfloat xRightCorner = -0.9f;	//Of Quard
GLfloat xLeftCorner = -1.0f;	//Of Quard
GLfloat speed = 0.001f;
GLfloat f;
int i, j;
GLfloat stack[10][100][2];
int top = 0;
GLfloat increase = 0.0f;
GLfloat increaseLimit = 13.0f;
GLfloat increaseBy = 0.001f;
bool isLayer2Gen = false;
struct matrix
{
	GLfloat mPhase;
	GLfloat colorPhase;
	GLfloat fallSpeed;
	GLfloat mPhase2;
	GLfloat colorPhase2;
	GLfloat fallSpeed2;
	GLfloat fallSpeedIncBy;
}R[100];

// callback
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global function declarations
void ToggleFullScreen(void);
void resize(int, int);
void uninitialize();
void update(void);
void updateForLayer2(bool isLayer2Gen);

void rain(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
//Passing two member of 2DArray-stack to fuction === C in Depth Pg No.211
void generateRainCords(GLfloat(*stack)[100][2], int top);
void resetR();

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// fuction declarations
	int initialize(void);
	void display(void);

	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd = NULL;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");
	int iRet = 0;

	// code
	if (fopen_s(&gpfile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Log File Cannot be Created"), TEXT("Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpfile, "Log File Successfully Created\n");
		fprintf(gpfile, "By Rohan\n");
	}

	// initialization of WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register above class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
		TEXT("PROJECT MATRIX - ROHAN V. WAGHMODE"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		100,
		100,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	iRet = initialize();

	if (iRet == -1)
	{
		fprintf(gpfile, "Choose Pixel Format Failed\n");
	}
	else if (iRet == -2)
	{
		fprintf(gpfile, "Set Pixel Format Failed\n");
	}
	else if (iRet == -3)
	{
		fprintf(gpfile, "wglCreateContext failed\n");
	}
	else if (iRet == -4)
	{
		fprintf(gpfile, "wglMakeCurrent failed\n");
		DestroyWindow(hwnd);
	}
	else
	{
		fprintf(gpfile, "Initialization Successful\n");
	}

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);


	// Gameloop
	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == true)
			{
				// Here call update
				update();
				if (isLayer2Gen == true)
				{
					updateForLayer2(isLayer2Gen);
				}
			}
			display();
		}


	}
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// code
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActiveWindow = true;
		break;
	case WM_KILLFOCUS:
		gbActiveWindow = false;
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
	case WM_ERASEBKGND:
		return (0);
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		case 0x46:
			ToggleFullScreen();
			break;
		}
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'x':
			x = x - incdec;
			fprintf(gpfile, "-X = %f\n", x);
			break;
		case 'X':
			x = x + incdec;
			fprintf(gpfile, "+X = %f\n", x);
			break;
		case 'y':
			y = y - incdec;
			fprintf(gpfile, "-Y = %f\n", y);
			break;
		case 'Y':
			y = y + incdec;
			fprintf(gpfile, "+Y = %f\n", y);
			break;
		case 'z':
			z = z - incdec;
			fprintf(gpfile, "-Z = %f\n", z);
			break;
		case 'Z':
			z = z + incdec;
			fprintf(gpfile, "+Z = %f\n", z);
			break;
		case 'i':
		case 'I':
			for (int i = 0; i < 100; i++)
			{
				R[i].fallSpeedIncBy += 0.001f;
			}
			fprintf(gpfile, "incBy = %f\n", R[0].fallSpeedIncBy);
			break;
		case 'd':
		case 'D':
			for (int i = 0; i < 100; i++)
			{
				R[i].fallSpeedIncBy -= 0.001f;
			}
			fprintf(gpfile, "decBy = %f\n", R[0].fallSpeedIncBy);
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		uninitialize();
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

int initialize(void)
{
	srand(time(0));
	for (int i = 0; i < 100; i++)
	{
		R[i].mPhase = 0.0f;
		R[i].fallSpeed = 0.001f;
		R[i].colorPhase = 0.0f;
		if ((i % 10) == 1)
		{
			R[i].fallSpeedIncBy = 0.0001f;
		}
		else if ((i % 10) == 2)
		{
			R[i].fallSpeedIncBy = 0.00013f;
		}
		else if ((i % 10) == 3)
		{
			R[i].fallSpeedIncBy = 0.00016f;
		}
		else if ((i % 10) == 4)
		{
			R[i].fallSpeedIncBy = 0.00019f;
		}
		else if ((i % 10) == 5)
		{
			R[i].fallSpeedIncBy = 0.00022f;
		}
		else if ((i % 10) == 6)
		{
			R[i].fallSpeedIncBy = 0.00025f;
		}
		else if ((i % 10) == 7)
		{
			R[i].fallSpeedIncBy = 0.00028f;
		}
		else if ((i % 10) == 8)
		{
			R[i].fallSpeedIncBy = 0.00031f;
		}
		else if ((i % 10) == 9)
		{
			R[i].fallSpeedIncBy = 0.00034f;
		}
		else if ((i % 10) == 0)
		{
			R[i].fallSpeedIncBy = 0.0005f;
		}

	}

	generateRainCords(stack, top);
	resetR();

	// function declaration
	void resize(int, int);

	// variable declaration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	// code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	// initialize PDF structure / Filling Form
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	ghdc = GetDC(ghwnd);
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
	{
		return (-1);
	}
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == false)
	{
		return (-2);
	}

	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL)
	{
		return (-3);
	}
	if (wglMakeCurrent(ghdc, ghrc) == false)
	{
		return (-4);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	resize(WIN_WIDTH, WIN_HEIGHT);
	ToggleFullScreen();
	return (0);
}

void resize(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, GLfloat(width) / GLfloat(height), 0.1f, 100.0f);
}

void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, z);

	for (int i = 0; i < 100; i++)
	{
		rain(stack[top][i][0], stack[top][i][1], R[i].mPhase, R[i].colorPhase, R[i].fallSpeed);
	}

	if (R[0].mPhase >= 4.0f && increase > increaseLimit)
	{
		top++;
		generateRainCords(stack, top);
		resetR();
		isLayer2Gen = false;
		increase = 0.0f;
	}

	if (increase > 7.0f && isLayer2Gen == false)
	{
		generateRainCords(stack, top + 1);
		isLayer2Gen = true;
	}

	if (isLayer2Gen == true)
	{
		for (int i = 0; i < 100; i++)
		{
			rain(stack[top + 1][i][0], stack[top + 1][i][1], R[i].mPhase2, R[i].colorPhase2, R[i].fallSpeed2);
		}
	}


	increase += increaseBy;

	SwapBuffers(ghdc);
}

void generateRainCords(GLfloat(*stack)[100][2], int top)
{
	float f;
	srand(time(0));
	GLfloat iff = 0.0f;
	for (i = 0; i < 100; i++)
	{
		stack[top][i][0] = (GLfloat)iff;
		iff += 0.178f;
	}
	for (i = 0; i < 100; i++)
	{
		f = (float)(rand() % 9);
		stack[top][i][1] = f;
	}

}

void rain(float row, float col, GLfloat mPhase, GLfloat colorPhase, GLfloat fallSpeed)
{
	if (mPhase <= 2.0f)//Vantage Quard
	{
		//Vantage
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(xRightCorner + row, 1.0f - col);
		glVertex2f(xLeftCorner + row, 1.0f - col);
		glVertex2f(xLeftCorner + row, 0.9f - col);
		glVertex2f(xRightCorner + row, 0.9f - col);
		glEnd();
	}

	else if (mPhase >= 2.0f && mPhase < 3.0f)//Series of Quards
	{
		if (mPhase >= 2.0f && mPhase <= 2.25f)
		{
			glBegin(GL_QUADS);
			glColor3f(0.0f, 0.7f, 0.0f);//DG
			glVertex2f(xRightCorner + row, 1.0f - col);
			glVertex2f(xLeftCorner + row, 1.0f - col);
			glVertex2f(xLeftCorner + row, 0.9f - col);
			glVertex2f(xRightCorner + row, 0.9f - col);
			glEnd();
		}
		else if (mPhase >= 2.25f && mPhase <= 2.5f)
		{
			//Quard1
			glBegin(GL_QUADS);
			glColor3f(0.0f, 0.7f, 0.0f);//DG
			glVertex2f(xRightCorner + row, 1.0f - col);
			glVertex2f(xLeftCorner + row, 1.0f - col);
			glVertex2f(xLeftCorner + row, 0.9f - col);
			glVertex2f(xRightCorner + row, 0.9f - col);
			glEnd();
			//Quard2
			glBegin(GL_QUADS);
			glColor3f(0.0f, 0.3f, 0.0f);//EDG
			glVertex2f(xRightCorner + row, 0.9f - col);
			glVertex2f(xLeftCorner + row, 0.9f - col);
			glVertex2f(xLeftCorner + row, 0.8f - col);
			glVertex2f(xRightCorner + row, 0.8f - col);
			glEnd();
		}
		else if (mPhase >= 2.5f && mPhase <= 2.75f)
		{
			//Quard1
			glBegin(GL_QUADS);
			glColor3f(0.0f, 0.7f, 0.0f);//DG
			glVertex2f(xRightCorner + row, 1.0f - col);
			glVertex2f(xLeftCorner + row, 1.0f - col);
			glVertex2f(xLeftCorner + row, 0.9f - col);
			glVertex2f(xRightCorner + row, 0.9f - col);
			glEnd();
			//Quard2
			glBegin(GL_QUADS);
			glColor3f(0.0f, 0.3f, 0.0f);//EDG
			glVertex2f(xRightCorner + row, 0.9f - col);
			glVertex2f(xLeftCorner + row, 0.9f - col);
			glVertex2f(xLeftCorner + row, 0.8f - col);
			glVertex2f(xRightCorner + row, 0.8f - col);
			glEnd();
			//Quard3
			glBegin(GL_QUADS);
			glColor3f(0.0f, 1.0f, 0.0f);//LG
			glVertex2f(xRightCorner + row, 0.8f - col);
			glVertex2f(xLeftCorner + row, 0.8f - col);
			glVertex2f(xLeftCorner + row, 0.7f - col);
			glVertex2f(xRightCorner + row, 0.7f - col);
			glEnd();
		}
		else if (mPhase >= 2.75f && mPhase <= 3.0f)
		{
			//Quard1
			glBegin(GL_QUADS);
			glColor3f(0.0f, 0.7f, 0.0f);//DG
			glVertex2f(xRightCorner + row, 1.0f - col);
			glVertex2f(xLeftCorner + row, 1.0f - col);
			glVertex2f(xLeftCorner + row, 0.9f - col);
			glVertex2f(xRightCorner + row, 0.9f - col);
			glEnd();
			//Quard2
			glBegin(GL_QUADS);
			glColor3f(0.0f, 0.3f, 0.0f);//EDG
			glVertex2f(xRightCorner + row, 0.9f - col);
			glVertex2f(xLeftCorner + row, 0.9f - col);
			glVertex2f(xLeftCorner + row, 0.8f - col);
			glVertex2f(xRightCorner + row, 0.8f - col);
			glEnd();
			//Quard3
			glBegin(GL_QUADS);
			glColor3f(0.0f, 1.0f, 0.0f);//LG
			glVertex2f(xRightCorner + row, 0.8f - col);
			glVertex2f(xLeftCorner + row, 0.8f - col);
			glVertex2f(xLeftCorner + row, 0.7f - col);
			glVertex2f(xRightCorner + row, 0.7f - col);
			glEnd();
			//Quard4
			glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex2f(xRightCorner + row, 0.7f - col);
			glVertex2f(xLeftCorner + row, 0.7f - col);
			glVertex2f(xLeftCorner + row, 0.6f - col);
			glVertex2f(xRightCorner + row, 0.6f - col);
			glEnd();
		}
	}

	else if (mPhase >= 3.0f)
	{
		if (colorPhase < 1.0f)
		{
			glColor3f(0.0f, 1.0f, 0.0f);//LG
		}
		else if (colorPhase >= 1.0f && colorPhase < 2.0f)
		{
			glColor3f(0.0f, 0.7f, 0.0f);//DG
		}
		else if (colorPhase >= 2.0f && colorPhase < 3.0f)
		{
			glColor3f(0.0f, 0.3f, 0.0f);//EDG
		}
		//Quard1
		glBegin(GL_QUADS);
		glVertex2f(xRightCorner + row, 1.0f - col - fallSpeed);
		glVertex2f(xLeftCorner + row, 1.0f - col - fallSpeed);
		glVertex2f(xLeftCorner + row, 0.9f - col - fallSpeed);
		glVertex2f(xRightCorner + row, 0.9f - col - fallSpeed);
		glEnd();

		if (colorPhase < 1.0f)
		{
			glColor3f(0.0f, 1.0f, 0.0f);//LG
		}
		else if (colorPhase >= 1.0f && colorPhase < 2.0f)
		{
			glColor3f(0.0f, 0.3f, 0.0f);//EDG
		}
		else if (colorPhase >= 2.0f && colorPhase < 3.0f)
		{
			glColor3f(0.0f, 0.7f, 0.0f);//DG
		}
		//Quard2
		glBegin(GL_QUADS);
		glVertex2f(xRightCorner + row, 0.9f - col - fallSpeed);
		glVertex2f(xLeftCorner + row, 0.9f - col - fallSpeed);
		glVertex2f(xLeftCorner + row, 0.8f - col - fallSpeed);
		glVertex2f(xRightCorner + row, 0.8f - col - fallSpeed);
		glEnd();

		if (colorPhase < 1.0f)
		{
			glColor3f(0.0f, 0.3f, 0.0f);//EDG
		}
		else if (colorPhase >= 1.0f && colorPhase < 2.0f)
		{
			glColor3f(0.0f, 0.7f, 0.0f);//DG
		}
		else if (colorPhase >= 2.0f && colorPhase < 3.0f)
		{
			glColor3f(0.0f, 1.0f, 0.0f);//LG
		}
		//Quard3
		glBegin(GL_QUADS);
		glVertex2f(xRightCorner + row, 0.8f - col - fallSpeed);
		glVertex2f(xLeftCorner + row, 0.8f - col - fallSpeed);
		glVertex2f(xLeftCorner + row, 0.7f - col - fallSpeed);
		glVertex2f(xRightCorner + row, 0.7f - col - fallSpeed);
		glEnd();

		//Quard4
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(xRightCorner + row, 0.7f - col - fallSpeed);
		glVertex2f(xLeftCorner + row, 0.7f - col - fallSpeed);
		glVertex2f(xLeftCorner + row, 0.6f - col - fallSpeed);
		glVertex2f(xRightCorner + row, 0.6f - col - fallSpeed);
		glEnd();
	}
}

void update(void)
{
	for (int i = 0; i < 100; i++)
	{
		R[i].mPhase = R[i].mPhase + speed;
		R[i].colorPhase = R[i].colorPhase + speed + speed;
		if (R[i].mPhase >= 3.0f)
		{
			R[i].fallSpeed = R[i].fallSpeed + R[i].fallSpeedIncBy;
		}
		if (R[i].colorPhase >= 3.0f)
		{
			R[i].colorPhase = 0.0f;
		}
	}
}

void updateForLayer2(bool isLayer2Gen)
{
	if (isLayer2Gen == true)
	{
		for (int i = 0; i < 100; i++)
		{
			R[i].mPhase2 = R[i].mPhase2 + speed;
			R[i].colorPhase2 = R[i].colorPhase2 + speed + speed;
			if (R[i].mPhase2 >= 3.0f)
			{
				R[i].fallSpeed2 = R[i].fallSpeed2 + R[i].fallSpeedIncBy;
			}
			if (R[i].colorPhase2 >= 3.0f)
			{
				R[i].colorPhase2 = 0.0f;
			}
		}
	}
}


void resetR()
{
	for (int i = 0; i < 100; i++)
	{
		R[i].mPhase = 0.0f;
		R[i].colorPhase = 0.0f;
		R[i].fallSpeed = 0.001f;
	}
	for (int i = 0; i < 100; i++)
	{
		R[i].mPhase2 = 0.0f;
		R[i].colorPhase2 = 0.0f;
		R[i].fallSpeed2 = 0.001f;
	}
}

void uninitialize(void)
{
	if (gbFullScreen == true)
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwstyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

		ShowCursor(TRUE);
	}
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}
}

void ToggleFullScreen(void)
{
	MONITORINFO mi;
	if (gbFullScreen == false)
	{
		dwstyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwstyle && WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwstyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
		gbFullScreen = true;
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwstyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
		ShowCursor(TRUE);
		gbFullScreen = false;
	}
}
