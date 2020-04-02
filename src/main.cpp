#define WIN32_LEAN_AND_MEAN

#include "window.h"
#include "nk_gui.h"
#include "filehandling.h"


int main(void)
{
	WINDOW_DATA windowData;
    unsigned int initialWindowWidth=360;
    unsigned int initialWindowHeight=180;
    SetupWindow(windowData,initialWindowWidth,initialWindowHeight,L"YGO LOTD LE YDC Converter v0.1");

    SetupGui(windowData,initialWindowWidth,initialWindowHeight);

    while (windowData.running)
    {
        HandleInput(windowData);

        HandleGui();

        RenderGui();
    }

    CleanupGui();
    CleanupWindow(windowData);
    return 0;
}

