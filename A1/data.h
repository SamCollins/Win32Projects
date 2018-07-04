/*******************************************************************
Collins, Sam - A00987689
Comp 3770 Assignment 1
Has the defined constants and colours used for the windows, aswell
as the definition of the data struct.
*******************************************************************/

#pragma once

#include <windows.h>

#define WND_SIZE 350
#define WND_POS_1 0
#define WND_POS_2 350

#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define BLUE RGB(0, 0, 255)
#define PURPLE RGB(255, 0, 255)

//Add colours to the constants

typedef struct Data {
	int count;
	TCHAR* msg;
	COLORREF color;
} Data;

