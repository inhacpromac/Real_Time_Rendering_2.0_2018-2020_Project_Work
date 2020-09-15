//Header Files
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <gl/glew.h> // IMPORTANT : THIS FILE MUST BE INCLUDED BEFORE <gl/GL.h >  & <gl/GLU.h>
#include <gl/GL.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "uf/vmath.h"
// following sequence is important
#include "uf/utilheader.h"
#include "uf/loadshader.h"
#include "uf/timer.h"
#include "uf/ktxloader.h"
#include "uf/textoverlay.h"
#include "uf/ddsloader.h"
#include "uf/objloader.h"
#include "scenes/sceneparams.h"
#include "scenes/animation.h"

//Linking with Opengl Library (This is for linker)
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "cudart.lib")

// global Variable declarations
bool gbFullscreen 		= false;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev 	= { sizeof(WINDOWPLACEMENT) };
HWND ghwnd 				= NULL;
HDC ghdc 				= NULL;
HGLRC ghrc 				= NULL;
bool gbActiveWindow 	= false;
bool gbAnimation = true;
GLenum result;

static GLfloat time;

//Function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Main Function - WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function declarations
	int initialize(void);
	void display(void);
	void update(float);
	void uninitialize();
	
	// variable declarations
	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");
	bool bDone = false;
	int iRet = 0;

	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, 
			TEXT("Log File Can not be Created"), 
			TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log file is Sucessfully created \n");
	}

	// code
	// initialization of WNDCLASSEX
	wndClass.cbSize 		= sizeof(WNDCLASSEX);
	wndClass.style 			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbClsExtra 	= 0;
	wndClass.cbWndExtra 	= 0;
	wndClass.lpfnWndProc 	= WndProc;
	wndClass.hInstance 		= hInstance;
	wndClass.hIcon 			= LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor 		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground 	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName 	= szAppName;
	wndClass.lpszMenuName 	= NULL;
	wndClass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	// register above class
	RegisterClassEx(&wndClass);

	// get mid point dimensions
	int screenX = ((GetSystemMetrics(SM_CXSCREEN)/2) - (WIN_WIDTH  / 2));
	int screenY = ((GetSystemMetrics(SM_CYSCREEN)/2) - (WIN_HEIGHT / 2));

	// create windowex
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Fragment : Test Scene"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		screenX,
		screenY,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;


	iRet = initialize(); // here we should check the value of iRet for errors

	if (iRet == -1)
	{
		fprintf(gpFile, "ChoosePixelFormat() failed\n");
		DestroyWindow(hwnd);
		exit(0);
	}
	else if (iRet == -2)
	{
		fprintf(gpFile, "SetPixelFormat() failed\n");
		DestroyWindow(hwnd);
		exit(0);
	}
	else if (iRet == -3)
	{
		fprintf(gpFile, "wglCreateContext() failed\n");
		DestroyWindow(hwnd);
		exit(0);
	}
	else if (iRet == -4)
	{
		fprintf(gpFile, "wglMakeCurrentContext() failed\n");
		DestroyWindow(hwnd);
		exit(0);
	}
	else if (iRet == -5)
	{
		fprintf(gpFile, "cudaInit() failed\n");
		DestroyWindow(hwnd);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Initialization Sucessfull !n");
	}

	// Show Window
	ShowWindow(hwnd, iCmdShow);

	//Here instead of calling UpdateWindow(), call below 2 new functions
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// initialize timer
	TimerInitialize();
	TimerReset();

	// Game loop
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
			//Game Loop - Here the rendering happens
			if (gbActiveWindow == true)
			{
				// Here call update()
				TimerTick();
				update(TimerDeltaTime());
				// render function
				display();
			}
		}
	}
	fflush(gpFile);
	uninitialize();
	
	return((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//Function declarations
	void ToggleFullscreen(void);
	void resize(int, int);
	void uninitialize(void);

	static GLfloat xPos, xMove, yPos, yMove, zPos;
	GLint viewport[4];

	// code
	switch (iMsg)
	{
	case WM_SETFOCUS:
		TimerStart();
		gbActiveWindow = true;
		break;

	case WM_KILLFOCUS:
		TimerStop();
		gbActiveWindow = false;
		break;

	case WM_SIZE:
		// to be used for picking ray
		WindowWidth  = LOWORD(lParam);
		WindowHeight = HIWORD(lParam);
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ENTERSIZEMOVE:
		TimerStop();
		break;

	case WM_EXITSIZEMOVE:
		TimerStart();
		break;

	case WM_ERASEBKGND:
		return (0);

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_MOUSEWHEEL:

		break;

	case WM_MOUSEMOVE:

		break;

	case WM_LBUTTONDOWN:

		break;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
			case VK_ESCAPE:
				DestroyWindow(hwnd);
				break;
			case 0x46:
				ToggleFullscreen();
				break;
			default:
				break;
		}
		break;
	case WM_CHAR:
		switch (wParam)
		{
			case 'r':
			case 'R':
				//gbReset = true;
				break;
			case 'a':
			case 'A':
				gbAnimation = !gbAnimation;
			break;
		}
		break;
	case WM_DESTROY:
		//uninitialize();
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	//code
	MONITORINFO mi;

	if (gbFullscreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev)
				&& GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

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
		gbFullscreen = true;
	}
	else
	{
		SetWindowLong(ghwnd,
			GWL_STYLE,
			dwStyle | WS_OVERLAPPEDWINDOW);

		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

		ShowCursor(TRUE);
		gbFullscreen = false;
	}
}

int initialize(void)
{
	//function declarations
	void resize(int, int);
	void uninitialize(void);
	int cudaInit();

	// variable declarations
	static PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	// code
	// Initialize pfd structure
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize 		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion 	= 1;
	pfd.dwFlags 	= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType 	= PFD_TYPE_RGBA;
	pfd.cColorBits 	= 32;
	pfd.cRedBits 	= 8;
	pfd.cGreenBits 	= 8;
	pfd.cBlueBits 	= 8;
	pfd.cAlphaBits 	= 8;
	pfd.cDepthBits 	= 24;

	ghdc = GetDC(ghwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	//return index is always 1 based means it zero get return is failure
	if (iPixelFormatIndex == 0)
	{
		return (-1);
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		return (-2);
	}

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		return(-3);
	}
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		return(-4);
	}

	if (cudaInit() < 0)
	{
		return(-5);
	}

	//GLEW intialization code for GLSL
	// IMPORTANT : it must be here.means after creating OpenGL Context
	// before using any opengl function
	result = glewInit();
	if (result != GLEW_OK)
	{
		fprintf(gpFile, "glewInit error.Exiting now\n");
		uninitialize();
	}

	if (PrepareAnimation() < 0)
	{
		fprintf(gpFile, "Prepare Animation Failed.Exitting Now...\n");
		uninitialize();
		exit(0);
	}


	// clear the screen by opengl color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	PerspectiveProjectionMatrix = mat4::identity();

	// INITIALIZE VALUES
	WindowWidth  = WIN_WIDTH;
	WindowHeight = WIN_HEIGHT;


	// initialize the values
    text_overlay_init(100, 100, NULL);

	//warm up call to resize
	resize(WIN_WIDTH, WIN_HEIGHT); //function calls

	return(0);
}

void resize(int width, int height)
{
	// code
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// set perspective projection
	PerspectiveProjectionMatrix = perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}


void display(void)
{
	void updateOverlay(void);
	void uninitialize(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//if(gbAnimation)
	{
		RenderAnimation(PerspectiveProjectionMatrix, time);
	}
	updateOverlay();

	SwapBuffers(ghdc);
}

void update(float dt)
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.

	static int frameCnt = 0;
	static float t_base = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	time = TimerTotalTime();
	
	// debug
	//fprintf(gpFile, "Total Elapsed Time %f\n", time);

	if ((time - t_base) >= 1.0f)
	{
		fps = (float)frameCnt; // fps = frameCnt / 1
		mspf = 1000.0f / fps;

		// Reset for next average.
		frameCnt = 0;
		t_base += 1.0f;
	}
}

char buffer[256];

void updateOverlay()
{
    text_overlay_clear();
 	sprintf(buffer, "fps: %f    Frame: %f ms", fps, mspf);
	text_overlay_drawText(buffer, 0, 0);
    text_overlay_draw();
}

void uninitialize(void)
{
	//code
	//check whether fullscreen or not if it is, then restore to
	// normal size & then proceed for uninitialization
	if (gbFullscreen == true)
	{
		// code
		SetWindowLong(ghwnd,
			GWL_STYLE,
			dwStyle | WS_OVERLAPPEDWINDOW);

		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);


		ShowCursor(TRUE);
	}

	//break the current context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	DeallocateAnimation();

	if (gpFile)
	{
		fprintf(gpFile, "Log file closed Succesfully");
		fclose(gpFile);
		gpFile = NULL;
	}
} // END OF UNINITIALIZE

int cudaInit()
{
	int iDevCnt = 0;
	cudaError_t error;

	error = cudaGetDeviceCount(&iDevCnt);
	if (error != cudaSuccess)
	{
		return -1;
	}
	else if (iDevCnt == 0)
	{
		return -2;
	}
	else
	{
		cudaSetDevice(0);
	}
	return 0;
}

