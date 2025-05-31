#include "ApplicationManager.h"
#include "EngineManager.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    ApplicationManager appManager;
    EngineManager engineManager;

    // EngineManager��HWND���Z�b�g������A������
    return appManager.Run(&engineManager, hInstance, nCmdShow);
}
