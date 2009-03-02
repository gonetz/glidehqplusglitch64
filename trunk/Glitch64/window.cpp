#include <windows.h>
//#include <gl/gl.h>
#include "main.h"

#ifdef GCC
#include <stdio.h>
#endif

static HDC 			hDC=NULL;		// Private GDI Device Context
static HGLRC		hRC=NULL;		// Permanent Rendering Context
static HWND			hWnd=NULL;		// Holds Our Window Handle
static HINSTANCE	hInstance;		// Holds The Instance Of The Application

#ifdef DEBUG
void MSG_DBG(const char *szString)
{
	::OutputDebugString(szString);
}
#else
#define MSG_DBG(x) 
#endif

static LRESULT CALLBACK WndProc(	HWND	hWnd, 	// Handle For This Window
							UINT	uMsg, 			// Message For This Window
							WPARAM	wParam, 		// Additional Message Information
							LPARAM	lParam) 		// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0); 					// Send A Quit Message
			return 0;								// Jump Back
		}
	}
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


GLvoid KillGLWindow(GLvoid) 							
{
	wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);						
	hRC=NULL;										
	ReleaseDC(hWnd, hDC);
	hDC=NULL;										
	DestroyWindow(hWnd);
	hWnd=NULL;										
	UnregisterClass("GLTEST", hInstance);
	hInstance=NULL; 								

}
 
BOOL CreateGLWindow()
{
	PIXELFORMATDESCRIPTOR pfd; // A Simplified PFD
	memset(&pfd,0,sizeof(pfd)); // Done properly this time
    pfd.nSize = sizeof (pfd);
	GLuint		PixelFormat;				
	WNDCLASS	wc;
  memset(&wc, 0, sizeof(WNDCLASS));
  hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_OWNDC;	                            //Get own DC, needed 
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL; 								// No Background Required For GL
    wc.lpszClassName = "GLTEST";
    RegisterClass (&wc);

    if (!( hWnd=CreateWindowEx(	WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, 	// Extended Style For The Window
								wc.lpszClassName, 						// Class Name
								"GLTEST", 								// Window Title
								WS_OVERLAPPEDWINDOW |					// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN, 					// Required Window Style
								0, 0, 								// Window Position
								0, 	                                // We Don't Need Width
								0, 	// Calculate Window Height      // We Don't Need Height
								NULL, 								// No Parent Window
								NULL, 								// No Menu
								hInstance, 							// Instance
								NULL))) 
	{
		KillGLWindow(); 							// Reset The Display
		MSG_DBG("Window Creation Error.");
		return FALSE;								// Return FALSE
	}
	hDC=GetDC(hWnd);	
    PixelFormat=ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, PixelFormat, &pfd);
	hRC=wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
	return TRUE;									// Success
}
