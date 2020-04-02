#include "window.h"
#include "nk_gui.h"

static LRESULT CALLBACK
WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
	
    if (HandleEvent({wnd,msg,wparam,lparam}))
        return 0;

    return DefWindowProcW(wnd, msg, wparam, lparam);
}

void SetupWindow(WINDOW_DATA &windowData, unsigned int width, unsigned int height, LPCWSTR windowName){
    windowData.rect = { 0, 0, (long int)width, (long int)height };
    windowData.style = WS_OVERLAPPEDWINDOW;
    windowData.exstyle = WS_EX_APPWINDOW;
    windowData.running = 1;
    windowData.needs_refresh = 1; 

    /* Win32 */
    memset(&(windowData.wc), 0, sizeof(windowData.wc));
    windowData.wc.style = CS_DBLCLKS;
    windowData.wc.lpfnWndProc = WindowProc;
    windowData.wc.hInstance = GetModuleHandleW(0);
    windowData.wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowData.wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowData.wc.lpszClassName = L"NuklearWindowClass";
    windowData.atom = RegisterClassW(&(windowData.wc));

    AdjustWindowRectEx(&(windowData.rect), windowData.style, FALSE, windowData.exstyle);
    windowData.wnd = CreateWindowExW(windowData.exstyle, windowData.wc.lpszClassName, windowName,
        windowData.style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        windowData.rect.right - windowData.rect.left, windowData.rect.bottom - windowData.rect.top,
        NULL, NULL, windowData.wc.hInstance, NULL);
    windowData.dc = GetDC(windowData.wnd);
}

void CleanupWindow(WINDOW_DATA &windowData){
    ReleaseDC(windowData.wnd, windowData.dc);
    UnregisterClassW(windowData.wc.lpszClassName, windowData.wc.hInstance);
}

// Returns an empty string if dialog is canceled
std::string OpenFilename(const char *filter) {
  OPENFILENAMEA ofn;
  char fileName[MAX_PATH] = "";
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter = filter;
  ofn.lpstrFile = fileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = "";
  std::string fileNameStr;
  if ( GetOpenFileNameA(&ofn) )
    fileNameStr = fileName;
  return fileNameStr;
}

std::string SaveFilename(const char *filter) {
  OPENFILENAMEA ofn;
  char fileName[MAX_PATH] = "";
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter = filter;
  ofn.lpstrFile = fileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = "";
  std::string fileNameStr;
  if ( GetSaveFileNameA(&ofn) )
    fileNameStr = fileName;
  return fileNameStr;
}