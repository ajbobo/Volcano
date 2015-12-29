// Volcano.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <stdio.h>
#include "Volcano.h"
#include "DialogCode.h"
#include "GameData.h"
#include "VolcanoRules.h"
#include "VolcanoDraw.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

/********************************
 * A.J.'s Global Variables, etc *
 ********************************/
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

LRESULT CALLBACK SetupGame(HWND, UINT, WPARAM, LPARAM);

GameData *gdata = NULL;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_VOLCANO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_VOLCANO);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_VOLCANO);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_VOLCANO;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

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

	if (DialogBox(hInstance,(LPCTSTR)IDD_SETUPGAME,NULL,(DLGPROC)SetupGame) == IDOK)
	   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		   CW_USEDEFAULT, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	else
		hWnd = NULL;

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
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_NEWGAME:
			if (DialogBox(hInst,(LPCTSTR)IDD_SETUPGAME,hWnd,(DLGPROC)SetupGame) == IDOK)
			{
				SetupBoard();
			}
			InvalidateRect(hWnd,NULL,true);
			UpdateWindow(hWnd);
			break;
		case ID_FILE_SCORES:
			DialogBox(hInst,(LPCTSTR)IDD_SCORES,hWnd,(DLGPROC)Scores);
			break;
		case ID_HELP_RULES:
			MessageBox(hWnd,"Feature not implemented yet","Volcano",MB_OK);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
	{
		if (!gdata)// Hopefully this never happens
		{
			MessageBox(hWnd,"GameData not initialized","Error",MB_OK);
			PostQuitMessage(0);
		}
		SetupBoard();
		InitDraw();
		break;
	}
	case WM_PAINT:
	{
		char title[300];
		sprintf(title,"Volcano - %s's Turn",gdata->Players[gdata->CurrPlayer - 1]);
		SetWindowText(hWnd,title);
		hdc = BeginPaint(hWnd, &ps);
		MainDraw(hdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONUP:
	{
		HandleLClick(hWnd,LOWORD(lParam),HIWORD(lParam));
		InvalidateRect(hWnd,NULL,true);
		UpdateWindow(hWnd);
		break;
	}
	case WM_RBUTTONUP:
	{
		HandleRClick(hWnd,LOWORD(lParam),HIWORD(lParam));
		InvalidateRect(hWnd,NULL,true);
		UpdateWindow(hWnd);
		break;
	}
	case WM_DESTROY:
		DeinitDraw();
		if (gdata)
			delete gdata;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}