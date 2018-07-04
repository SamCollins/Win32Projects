/*Collins, Sam A00987689
Comp 3770 Assignment 4
*/

#include <windows.h>
#include <mmsystem.h>
#include <windowsx.h>

LRESULT CALLBACK raceWndProc(HWND, UINT, WPARAM, LPARAM);

CRITICAL_SECTION cs;

typedef struct Car
{
	char* message;
	BOOL* raceDone;
	BOOL* synchronized;
	HWND hwndButton;
	int yPos;

} Car;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCMLine, int iCmdShow) {
	static TCHAR szAppName[] = TEXT("HelloApplication");//name of app
	HWND	hwnd; //holds handle to the main window
	MSG		msg;//holds any message retrieved from the msg queue
	WNDCLASS wndclass;//wnd class for registration

	HANDLE showWindowPart2 = CreateEvent(NULL, FALSE, FALSE, TEXT("showWindowPart2"));
	HANDLE showWindowPart3 = CreateEvent(NULL, FALSE, FALSE, TEXT("showWindowPart3"));

	HANDLE hShowWindow[] = { showWindowPart2, showWindowPart3 };

	DWORD waitObject;

	InitializeCriticalSection(&cs);


					  //defn wndclass attributes for this application
	wndclass.style = CS_HREDRAW | CS_VREDRAW;//redraw on refresh both directions
	wndclass.lpfnWndProc = raceWndProc;//wnd proc to handle windows msgs/commands
	wndclass.cbClsExtra = 0;//class space for expansion/info carrying
	wndclass.cbWndExtra = 0;//wnd space for info carrying
	wndclass.hInstance = hInstance;//application instance handle
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//set icon for window
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//set cursor for window
	wndclass.hbrBackground = CreateSolidBrush(RGB(0, 255, 255));	//(HBRUSH)GetStockObject(WHITE_BRUSH);//set background
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
		TEXT("A4 Race"), // window caption
		WS_OVERLAPPEDWINDOW,	//window style
		450,		//initial x position	CW_USEDEFAULT
		50,		//initial y position
		800,		//initial x size
		600,		//initial y size
		NULL,				//parent window handle
		NULL,				//window menu handle
		hInstance,			//program instance handle
		NULL);				//creation parameters

	waitObject = WaitForMultipleObjects(2, hShowWindow, FALSE, INFINITE);

	if (waitObject == WAIT_OBJECT_0 + 0)
	{
		SetWindowText(hwnd, TEXT("Part 2"));
	}
	else if (waitObject == WAIT_OBJECT_0 + 1) 
	{
		SetWindowText(hwnd, TEXT("Part 3"));
	}

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

VOID StartCar(Car* car)
{
	for (int i = 10; i < 425; i++)
	{
		MoveWindow(car->hwndButton, i, car->yPos, 100, 25, TRUE);
	}

	if (*car->synchronized)
	{
		EnterCriticalSection(&cs);
		if (*car->raceDone == FALSE)
		{
			Sleep(100);
			MessageBox(car->hwndButton, TEXT(car->message), TEXT("Winner"), MB_OK);
			*car->raceDone = TRUE;
		}
		LeaveCriticalSection(&cs);
	}
	else
	{
		if (*car->raceDone == FALSE)
		{
			Sleep(100);
			MessageBox(car->hwndButton, TEXT(car->message), TEXT("Winner"), MB_OK);
			*car->raceDone = TRUE;
		}
	}
	
}

LRESULT CALLBACK raceWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC		hdc;
	PAINTSTRUCT ps;
	RECT	rect;
	static BOOL raceDone;
	static BOOL synchronized;
	static Car* car1 = (Car*)malloc(sizeof(Car));
	static Car* car2 = (Car*)malloc(sizeof(Car));
	static Car* car3 = (Car*)malloc(sizeof(Car));
	static Car* car4 = (Car*)malloc(sizeof(Car));
	static Car* car5 = (Car*)malloc(sizeof(Car));
	HANDLE hThread1, hThread2, hThread3, hThread4, hThread5;

	HANDLE closePart2 = CreateEvent(NULL, FALSE, FALSE, TEXT("closePart2"));
	HANDLE closePart3 = CreateEvent(NULL, FALSE, FALSE, TEXT("closePart3"));

	/*WM short for Window Message*/
	switch (message) {
	case WM_CREATE: //additional things to do when window is created

		raceDone = FALSE;
		synchronized = FALSE;

		car1->hwndButton = CreateWindow(
			TEXT("BUTTON"),  // Predefined class; Unicode assumed 
			TEXT("Red Car"),      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			10,         // x position 
			50,         // y position 
			100,        // Button width
			25,        // Button height
			hwnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		car1->message = TEXT("Red Car Wins");
		car1->raceDone = &raceDone;
		car1->synchronized = &synchronized;
		car1->yPos = 50;

		car2->hwndButton = CreateWindow(
			TEXT("BUTTON"),  // Predefined class; Unicode assumed 
			TEXT("Blue Car"),      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			10,         // x position 
			125,         // y position 
			100,        // Button width
			25,        // Button height
			hwnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		car2->message = TEXT("Blue Car Wins");
		car2->raceDone = &raceDone;
		car2->synchronized = &synchronized;
		car2->yPos = 125;

		car3->hwndButton = CreateWindow(
			TEXT("BUTTON"),  // Predefined class; Unicode assumed 
			TEXT("Green Car"),      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			10,         // x position 
			200,         // y position 
			100,        // Button width
			25,        // Button height
			hwnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		car3->message = TEXT("Green Car Wins");
		car3->raceDone = &raceDone;
		car3->synchronized = &synchronized;
		car3->yPos = 200;

		car4->hwndButton = CreateWindow(
			TEXT("BUTTON"),  // Predefined class; Unicode assumed 
			TEXT("Orange Car"),      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			10,         // x position 
			275,         // y position 
			100,        // Button width
			25,        // Button height
			hwnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		car4->message = TEXT("Orange Car Wins");
		car4->raceDone = &raceDone;
		car4->synchronized = &synchronized;
		car4->yPos = 275;

		car5->hwndButton = CreateWindow(
			TEXT("BUTTON"),  // Predefined class; Unicode assumed 
			TEXT("Black Car"),      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			10,         // x position 
			350,         // y position 
			100,        // Button width
			25,        // Button height
			hwnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		car5->message = TEXT("Black Car Wins");
		car5->raceDone = &raceDone;
		car5->synchronized = &synchronized;
		car5->yPos = 350;

		return 0;

	case WM_PAINT: //what to do when a paint msg occurs
				   //Must have BeginPaint or program will freeze
		hdc = BeginPaint(hwnd, &ps);//get a handle to a device context for drawing
		GetClientRect(hwnd, &rect);//define drawing area for clipping

		MoveToEx(hdc, rect.right/2, 0, NULL);
		LineTo(hdc, rect.right / 2, rect.bottom);

		EndPaint(hwnd, &ps);//release the device context
		return 0;

	case WM_LBUTTONDOWN:

		synchronized = FALSE;

		if (raceDone == FALSE)
		{
			hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car1, 0, NULL);
			hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car2, 0, NULL);
			hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car3, 0, NULL);
			hThread4 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car4, 0, NULL);
			hThread5 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car5, 0, NULL);

			HANDLE threads[] = { hThread1, hThread2, hThread3, hThread4, hThread5 };

			WaitForMultipleObjects(5, threads, TRUE, 100);
			SetWindowText(hwnd, TEXT("Winner Found"));
		}
		
		return 0;

	case WM_RBUTTONDOWN:

		synchronized = TRUE;

		if (raceDone == FALSE)
		{
			hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car1, 0, NULL); //Thread Id thing at end
			hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car2, 0, NULL); //Thread Id thing at end
			hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car3, 0, NULL); //Thread Id thing at end
			hThread4 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car4, 0, NULL); //Thread Id thing at end
			hThread5 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartCar, car5, 0, NULL); //Thread Id thing at end
			
			HANDLE threads[] = { hThread1, hThread2, hThread3, hThread4, hThread5 };

			WaitForMultipleObjects(5, threads, TRUE, 100);
			SetWindowText(hwnd, TEXT("Winner Found"));
		}

		return 0;

	case WM_DESTROY: //how to handle a destroy (close window app) msg

		SetEvent(closePart2);
		SetEvent(closePart3);

		PostQuitMessage(0);
		return 0;
	}
	//return the message to windows for further processing
	return DefWindowProc(hwnd, message, wParam, lParam);
}