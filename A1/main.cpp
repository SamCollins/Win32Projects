/*******************************************************************
Collins, Sam - A00987689
Comp 3770 Assignment 1
The main file that creates the windows.
*******************************************************************/

#include <windows.h>
#include <mmsystem.h>

#include "data.h"
#include "SeperateWnd.h"
#include "ConnectWnd.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCMLine, int iCmdShow) {
	static TCHAR connectAppName[] = TEXT("ConnectApplication");//name of app
	static TCHAR seperateAppName[] = TEXT("SeperateApplication");//name of app
	HWND	hwnd;//holds handle to the main window
	MSG		msg;//holds any message retrieved from the msg queue
	WNDCLASS connectWndClass; //wnd class for registration
	WNDCLASS seperateWndClass; 
	Data* connectClassData = (Data*)malloc(sizeof(Data));
	Data* seperateClassData = (Data*)malloc(sizeof(Data));
	Data* windowData = (Data*)malloc(sizeof(Data));



	//defn wndclass attributes for this application
	connectWndClass.style = CS_HREDRAW | CS_VREDRAW;//redraw on refresh both directions
	connectWndClass.lpfnWndProc = connectWndProc;//wnd proc to handle windows msgs/commands
	connectWndClass.cbClsExtra = sizeof(Data);//class space for expansion/info carrying
	connectWndClass.cbWndExtra = 0;//wnd space for info carrying
	connectWndClass.hInstance = hInstance;//application instance handle
	connectWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//set icon for window
	connectWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);//set cursor for window
	connectWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//set background
	connectWndClass.lpszMenuName = NULL;//set menu
	connectWndClass.lpszClassName = connectAppName;//set application name

									   //register wndclass to O/S so approp. wnd msg are sent to application
	if (!RegisterClass(&connectWndClass)) {
		MessageBox(NULL, TEXT("This program requires Windows 95/98/NT"),
			connectAppName, MB_ICONERROR);//if unable to be registered
		return 0;
	}

	//defn wndclass attributes for this application
	seperateWndClass.style = CS_HREDRAW | CS_VREDRAW;//redraw on refresh both directions
	seperateWndClass.lpfnWndProc = seperateWndProc;//wnd proc to handle windows msgs/commands
	seperateWndClass.cbClsExtra = sizeof(Data);//class space for expansion/info carrying
	seperateWndClass.cbWndExtra = sizeof(Data);//wnd space for info carrying
	seperateWndClass.hInstance = hInstance;//application instance handle
	seperateWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//set icon for window
	seperateWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);//set cursor for window
	seperateWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//set background
	seperateWndClass.lpszMenuName = NULL;//set menu
	seperateWndClass.lpszClassName = seperateAppName;//set application name

									   //register wndclass to O/S so approp. wnd msg are sent to application
	if (!RegisterClass(&seperateWndClass)) {
		MessageBox(NULL, TEXT("This program requires Windows 95/98/NT"),
			seperateAppName, MB_ICONERROR);//if unable to be registered
		return 0;
	}

	connectClassData->count = 0;
	connectClassData->msg = TEXT("Connected Windows");
	connectClassData->color = PURPLE;
	//create the main window and get it's handle for future reference
	hwnd = CreateWindow(connectAppName,		//window class name
		TEXT("Connected"), // window caption
		WS_OVERLAPPEDWINDOW,	//window style
		WND_POS_1,		//initial x position
		WND_POS_1,		//initial y position
		WND_SIZE,		//initial x size
		WND_SIZE,		//initial y size
		NULL,				//parent window handle
		NULL,				//window menu handle
		hInstance,			//program instance handle
		connectClassData);				//creation parameters
	SetClassLongPtr(hwnd, 0, (LONG_PTR)connectClassData);
	ShowWindow(hwnd, iCmdShow);//set window to be shown
	UpdateWindow(hwnd);//force an update so window is drawn

					   //create the main window and get it's handle for future reference
	hwnd = CreateWindow(connectAppName,		//window class name
		TEXT("Connected"), // window caption
		WS_OVERLAPPEDWINDOW,	//window style
		WND_POS_2,		//initial x position
		WND_POS_1,		//initial y position
		WND_SIZE,		//initial x size
		WND_SIZE,		//initial y size
		NULL,				//parent window handle
		NULL,				//window menu handle
		hInstance,			//program instance handle
		NULL);				//creation parameters
	ShowWindow(hwnd, iCmdShow);//set window to be shown
	UpdateWindow(hwnd);//force an update so window is drawn

	seperateClassData->count = 0;
	seperateClassData->color = GREEN; //Sets class data

	windowData->msg = TEXT("Seperate");
	windowData->color = BLUE; //Sets data for first window

	hwnd = CreateWindow(seperateAppName,		//window class name
		TEXT("Seperate"), // window caption
		WS_OVERLAPPEDWINDOW,	//window style
		WND_POS_1,		//initial x position
		WND_POS_2,		//initial y position
		WND_SIZE,		//initial x size
		WND_SIZE,		//initial y size
		NULL,				//parent window handle
		NULL,				//window menu handle
		hInstance,			//program instance handle
		seperateClassData);				//creation parameters
	SetWindowLongPtr(hwnd, 0, (LONG_PTR)windowData);
	SetClassLongPtr(hwnd, 0, (LONG_PTR)seperateClassData);
	ShowWindow(hwnd, iCmdShow);//set window to be shown
	UpdateWindow(hwnd);//force an update so window is drawn

	windowData = (Data*)malloc(sizeof(Data)); //Clears the window pointer
	windowData->msg = TEXT("Windows");
	windowData->color = RED; //Sets the data for the second window

	hwnd = CreateWindow(seperateAppName,		//window class name
		TEXT("Seperate"), // window caption
		WS_OVERLAPPEDWINDOW,	//window style
		WND_POS_2,		//initial x position
		WND_POS_2,		//initial y position
		WND_SIZE,		//initial x size
		WND_SIZE,		//initial y size
		NULL,				//parent window handle
		NULL,				//window menu handle
		hInstance,			//program instance handle
		NULL);				//creation parameters
	SetWindowLongPtr(hwnd, 0, (LONG_PTR)windowData);
	ShowWindow(hwnd, iCmdShow);//set window to be shown
	UpdateWindow(hwnd);//force an update so window is drawn

					   //messgae loop
	while (GetMessage(&msg, NULL, 0, 0)) {//get message from queue
		TranslateMessage(&msg);//for keystroke translation
		DispatchMessage(&msg);//pass msg back to windows for processing
							  //note that this is to put windows o/s in control, rather than this app
	}

	return msg.wParam;
}