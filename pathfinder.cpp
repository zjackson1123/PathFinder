#include ".\Framework\framework.h"

HWND hWnd;
HINSTANCE hInst;
RECT rect;
Buttons btnArr;
mouseClick mClick;
Grid grid;
RBTree bstX;
RBTree bstY;
LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
bool createWindow();
void drawUI(HDC hdc);

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    createWindow(); 
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}

bool createWindow(){
    WNDCLASSEX windowClass;
    ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    hInst = GetModuleHandle(NULL);
    //windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.cbSize = sizeof(windowClass);
    windowClass.lpfnWndProc = WindowProcessMessages;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInst;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(66,69,73));
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = _T("Path Finder");
    RegisterClassEx(&windowClass);
    hWnd = CreateWindowEx(dwExStyle, _T("Path Finder"), _T("Path Finder"), WS_OVERLAPPEDWINDOW, 
    CW_USEDEFAULT, 0, 1200, 1000, NULL, NULL, hInst, NULL);
    Buttons::setHWND(&hWnd);
    Grid::setHWND(&hWnd);
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
    UpdateWindow(hWnd);
    return true;

}

LRESULT CALLBACK WindowProcessMessages(HWND hWnd, UINT msg, WPARAM param, LPARAM lparam) {
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg) {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            drawUI(hdc);
            EndPaint(hWnd, &ps);
            return 0;
        case WM_LBUTTONDOWN:
            mClick.set_mDown();
            return 0;
        case WM_LBUTTONUP:
            mClick.set_mUp();
            mClick.checkClick(&btnArr, &grid, hWnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, msg, param, lparam);
	}
}

void drawUI(HDC hdc) {
    Gdiplus::Graphics gf(hdc);  
    GetClientRect(hWnd, &rect);
    Gdiplus::Pen pen(Gdiplus::Color(114,137,218));
    Gdiplus::SolidBrush sBrush(Gdiplus::Color(114,137,218));
    grid = Grid(rect, &gf, &pen);
    btnArr.initBtns();
}
