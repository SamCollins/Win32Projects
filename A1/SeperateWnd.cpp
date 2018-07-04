/*******************************************************************
Collins, Sam - A00987689
Comp 3770 Assignment 1
Class definition for the windows that have different messages and 
close seperately.
*******************************************************************/

#include "data.h"
#include "SeperateWnd.h"

/**
Purpose: To handle windows messages for specific cases including when
the window is first created, refreshing (painting), and closing
the window.

Returns: Long - any error message (see Win32 API for details of possible error messages)
Notes:	 CALLBACK is defined as __stdcall which defines a calling
convention for assembly (stack parameter passing)
**/
LRESULT CALLBACK seperateWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC		hdc;
	PAINTSTRUCT ps;
	RECT	rect;
	Data* windowData;
	Data* seperateClassData; 
	char wndCount[256];

	/*WM short for Window Message*/
	switch (message) {
		case WM_CREATE: {//additional things to do when window is created
			PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);

			seperateClassData = (Data*)GetClassLongPtr(hwnd, 0);

			if (lParam != NULL && seperateClassData == NULL) {
				CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
				seperateClassData = reinterpret_cast<Data*>(pCreate->lpCreateParams);
				SetClassLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)seperateClassData);
			}
			//Alternate way to do Create Struct thing
			//connectClassData = (Data*)(((LPCREATESTRUCT)lParam)->lpCreateParams);

			seperateClassData->count++;

			return 0;
		}

		case WM_PAINT: {//what to do when a paint msg occurs
					   //Must have BeginPaint or program will freeze
			hdc = BeginPaint(hwnd, &ps);//get a handle to a device context for drawing
			GetClientRect(hwnd, &rect);//define drawing area for clipping

			windowData = (Data*)GetWindowLongPtr(hwnd, 0);
			seperateClassData = (Data*)GetClassLongPtr(hwnd, 0);

			SetTextColor(hdc, windowData->color);
			DrawText(hdc, windowData->msg, -1, &rect,
				DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			wsprintf(wndCount, "Number of windows: %d", seperateClassData->count);

			SetTextColor(hdc, seperateClassData->color);
			DrawText(hdc, wndCount, -1, &rect,
				DT_SINGLELINE | DT_CENTER | DT_TOP);

			EndPaint(hwnd, &ps);//release the device context
			return 0;
		}

		case WM_DESTROY: {//how to handle a destroy (close window app) msg

			seperateClassData = (Data*)GetClassLongPtr(hwnd, 0);

			if (seperateClassData->count == 1) {
				PostQuitMessage(0);
			}
			seperateClassData->count--;

			return 0;
		}
	}
	//return the message to windows for further processing
	return DefWindowProc(hwnd, message, wParam, lParam);
}

