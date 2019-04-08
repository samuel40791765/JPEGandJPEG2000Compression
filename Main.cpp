//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Creates a white RGB image with a black line and displays it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Code used by students as a starter code to display and modify images
//
//*****************************************************************************


// Include class files
#include "Image.h"
#include <iostream>
#include <chrono>
#include <thread>
#define MAX_LOADSTRING 100

// Global Variables:
MyImage			inImage;						// image object
unsigned char*			jpg_uncompressed;
unsigned char*			jpg2000_uncompressed;
unsigned char*			jpg_progressive[64];
unsigned char*			jpg2000_progressive[64];
int coefficients;
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Read in a parameter from the command line
	char filepath[100];
	char temp[10];
	LPWSTR *szArglist;
	int nArgs;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (NULL == szArglist)
	{
		wprintf(L"CommandLineToArgvW failed\n");
		return 0;
	}
	else for (int i = 0; i < nArgs; i++) printf("%d: %ws\n", i, szArglist[i]);
	//filepath
	wcstombs(filepath, szArglist[1], sizeof(filepath));
	//number of coefficients
	wcstombs(temp, szArglist[2], sizeof(temp));
	coefficients = atof(temp);
	// Create a separate console window to display output to stdout
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	printf("The first parameter was: %s\n", filepath);


	// Set up the image
	int w = 512;
	int h = 512;
	inImage.setWidth(w);
	inImage.setHeight(h);

	inImage.setImagePath(filepath);
	inImage.ReadImage();
	
	//jpg_uncompressed = inImage.getImageData();
	inImage.compressJPGImage();
	inImage.compressJPG2000();
	if (0 < coefficients) {
		//jpg_uncompressed = inImage.decompressJPG(262144);
		//jpg_uncompressed = inImage.decompressJPG(229376);
		jpg_uncompressed = inImage.decompressJPG(coefficients);
		//jpg_uncompressed = inImage.decompressJPG(16384);
		jpg2000_uncompressed = inImage.decompressJPG2000(coefficients);
	}
	else {
		for (int i = 1; i < 65; i++) {
			jpg_progressive[i - 1] = inImage.decompressJPG((w*h) / 64 * i);
			jpg2000_progressive[i - 1] = inImage.decompressJPG2000((w*h) / 64 * i);
		}
	}
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1700, 800, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// TO DO: part useful to render video frames, may place your own code here in this function
// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case ID_MODIFY_IMAGE:
				   InvalidateRect(hWnd, &rt, false); 
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				// TO DO: Add any drawing code here...
				SetTextAlign(hdc, TA_CENTER);
				RECT rect;
				rect.left = inImage.getWidth() / 2;
				rect.top = inImage.getHeight() + 10;
				DrawText(hdc, TEXT("Raw Image"), -1, &rect, DT_SINGLELINE | DT_NOCLIP);
				rect.left = inImage.getWidth() + inImage.getWidth() / 2 + 1;
				DrawText(hdc, TEXT("DCT Image"), -1, &rect, DT_SINGLELINE | DT_NOCLIP);
				rect.left = inImage.getWidth() * 2 + inImage.getWidth() / 2 + 2;
				DrawText(hdc, TEXT("DWT Image"), -1, &rect, DT_SINGLELINE | DT_NOCLIP);
				
				BITMAPINFO bmi;
				CBitmap bitmap;
				memset(&bmi,0,sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biWidth = inImage.getWidth();
				bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 32;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = inImage.getWidth()*inImage.getHeight();

				SetDIBitsToDevice(hdc,
								  0,0,inImage.getWidth(),inImage.getHeight(),
								  0,0,0,inImage.getHeight(),
								  inImage.getImageData(),&bmi,DIB_RGB_COLORS);

				BITMAPINFO dct_bmi;
				memset(&dct_bmi, 0, sizeof(dct_bmi));
				dct_bmi.bmiHeader.biSize = sizeof(dct_bmi.bmiHeader);
				dct_bmi.bmiHeader.biWidth = inImage.getWidth();
				dct_bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
				dct_bmi.bmiHeader.biPlanes = 1;
				dct_bmi.bmiHeader.biBitCount = 32;
				dct_bmi.bmiHeader.biCompression = BI_RGB;
				dct_bmi.bmiHeader.biSizeImage = inImage.getWidth()*inImage.getHeight();
			

				BITMAPINFO dwt_bmi;
				memset(&dwt_bmi, 0, sizeof(dwt_bmi));
				dwt_bmi.bmiHeader.biSize = sizeof(dwt_bmi.bmiHeader);
				dwt_bmi.bmiHeader.biWidth = inImage.getWidth();
				dwt_bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
				dwt_bmi.bmiHeader.biPlanes = 1;
				dwt_bmi.bmiHeader.biBitCount = 32;
				dwt_bmi.bmiHeader.biCompression = BI_RGB;
				dwt_bmi.bmiHeader.biSizeImage = inImage.getWidth()*inImage.getHeight();

				SetTextColor(hdc, RGB(200, 0, 0));
				CString t;
				rect.left = inImage.getWidth() + inImage.getWidth() / 2 + 1;
				rect.top = rect.top + 30;
				if (0 < coefficients) {
					t.Format(_T("%d / %d"), coefficients, inImage.getWidth()*inImage.getHeight());
					DrawText(hdc, TEXT("Coefficients: ") + t, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
					SetDIBitsToDevice(hdc,
						inImage.getWidth() + 1, 0, inImage.getWidth(), inImage.getHeight(),
						0, 0, 0, inImage.getHeight(),
						jpg_uncompressed, &dct_bmi, DIB_RGB_COLORS);
					SetDIBitsToDevice(hdc,
						inImage.getWidth() * 2 + 2, 0, inImage.getWidth(), inImage.getHeight(),
						0, 0, 0, inImage.getHeight(),
						jpg2000_uncompressed, &dwt_bmi, DIB_RGB_COLORS);
				}
				else {
					rect.left = inImage.getWidth() + inImage.getWidth() / 2 + 1;
					rect.top = rect.top + 30;
					for (int i = 0; i < 64; i++) {
						t.Format(_T("%d / %d"), (i + 1) * 4096, inImage.getWidth()*inImage.getHeight());
						DrawText(hdc, TEXT("Coefficients: ") + t, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
						SetDIBitsToDevice(hdc,
							inImage.getWidth() + 1, 0, inImage.getWidth(), inImage.getHeight(),
							0, 0, 0, inImage.getHeight(),
							jpg_progressive[i], &dct_bmi, DIB_RGB_COLORS);
						SetDIBitsToDevice(hdc,
							inImage.getWidth() * 2 + 2, 0, inImage.getWidth(), inImage.getHeight(),
							0, 0, 0, inImage.getHeight(),
							jpg2000_progressive[i], &dct_bmi, DIB_RGB_COLORS);
						std::this_thread::sleep_for(std::chrono::milliseconds(500));
					}
				}

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}




// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


