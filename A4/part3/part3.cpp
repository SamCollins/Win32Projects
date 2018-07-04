/*Collins, Sam A00987689
Comp 3770 Assignment 4
*/

#include <windows.h>
#include <mmsystem.h>

LRESULT CALLBACK part3WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCMLine, int iCmdShow) {
	static TCHAR szAppName[] = TEXT("HelloApplication");//name of app
	HWND	hwnd, hwndButton;//holds handle to the main window
	MSG		msg;//holds any message retrieved from the msg queue
	WNDCLASS wndclass;//wnd class for registration

	HANDLE closePart3 = CreateEvent(NULL, FALSE, FALSE, TEXT("closePart3"));

					  //defn wndclass attributes for this application
	wndclass.style = CS_HREDRAW | CS_VREDRAW;//redraw on refresh both directions
	wndclass.lpfnWndProc = part3WndProc;//wnd proc to handle windows msgs/commands
	wndclass.cbClsExtra = 0;//class space for expansion/info carrying
	wndclass.cbWndExtra = 0;//wnd space for info carrying
	wndclass.hInstance = hInstance;//application instance handle
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//set icon for window
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//set cursor for window
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//set background
	wndclass.lpszMenuName = NULL;//set menu
	wndclass.lpszClassName = szAppName;//set application name

									   //register wndclass to O/S so approp. wnd msg are sent to application
	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, TEXT("This program requires Windows 95/98/NT"),
			szAppName, MB_ICONERROR);//if unable to be registered
		return 0;
	}
	//create the main window and get it's handle for future reference
	hwnd = CreateWindow(szAppName,		//window class name
		TEXT("A4 Part 3"), // window caption
		WS_OVERLAPPEDWINDOW,	//window style
		50,		//initial x position	CW_USEDEFAULT
		300,		//initial y position
		400,		//initial x size
		200,		//initial y size
		NULL,				//parent window handle
		NULL,				//window menu handle
		hInstance,			//program instance handle
		NULL);				//creation parameters
	ShowWindow(hwnd, iCmdShow);//set window to be shown
	UpdateWindow(hwnd);//force an update so window is drawn

	hwndButton = CreateWindow(
		TEXT("BUTTON"),  // Predefined class; Unicode assumed 
		TEXT("Press Me"),      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		10,         // y position 
		100,        // Button width
		25,        // Button height
		hwnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.
	ShowWindow(hwnd, iCmdShow);//set window to be shown
	UpdateWindow(hwnd);//force an update so window is drawn

					   //messgae loop
	while (GetMessage(&msg, NULL, 0, 0)) {//get message from queue
		TranslateMessage(&msg);//for keystroke translation
		DispatchMessage(&msg);//pass msg back to windows for processing
							  //note that this is to put windows o/s in control, rather than this app

		if (WaitForSingleObject(closePart3, 0) == WAIT_OBJECT_0)
		{
			PostQuitMessage(0);
		}
	}

	return msg.wParam;
}

LRESULT CALLBACK part3WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC		hdc;
	PAINTSTRUCT ps;
	RECT	rect;
	HANDLE showWindowPart3;

	/*WM short for Window Message*/
	switch (message) {
	case WM_CREATE: //additional things to do when window is created
		return 0;

	case WM_PAINT: //what to do when a paint msg occurs
				   //Must have BeginPaint or program will freeze
		hdc = BeginPaint(hwnd, &ps);//get a handle to a device context for drawing
		GetClientRect(hwnd, &rect);//define drawing area for clipping

		DrawText(hdc, TEXT("Part 3"), -1, &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);//write text to the context

		EndPaint(hwnd, &ps);//release the device context
		return 0;

	case WM_COMMAND:

		switch (wParam) {

		case (BN_CLICKED):

			showWindowPart3 = CreateEvent(NULL, FALSE, FALSE, TEXT("showWindowPart3"));
			SetEvent(showWindowPart3);

			//MessageBox(hwnd, TEXT("Button"), TEXT("Button"), MB_OK);
		}
		return 0;

	case WM_DESTROY: //how to handle a destroy (close window app) msg
		PostQuitMessage(0);
		return 0;
	}
	//return the message to windows for further processing
	return DefWindowProc(hwnd, message, wParam, lParam);
}