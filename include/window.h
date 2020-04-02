#pragma once
#include <windows.h>
#include <string>

struct WINDOW_DATA{
	WNDCLASSW wc;
	ATOM atom;
	RECT rect;
	DWORD style;
    DWORD exstyle;
    HWND wnd;
    HDC dc;
    int running;
    int needs_refresh; 
};

struct EVENT_DATA{
    HWND wnd;
    UINT msg;
    WPARAM wparam;
    LPARAM lparam;
};

void SetupWindow(WINDOW_DATA &windowData, unsigned int width, unsigned int height, LPCWSTR windowName);
void CleanupWindow(WINDOW_DATA &windowData);
std::string OpenFilename(const char *filter = "All Files (*.*)\0*.*\0");
std::string SaveFilename(const char *filter = "All Files (*.*)\0*.*\0");