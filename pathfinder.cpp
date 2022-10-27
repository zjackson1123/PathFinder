#include ".\Framework\framework.h"

HWND hWnd;
HINSTANCE hInst;
RECT rect;
std::pair<int, int> inItem;
int Buttons[3][4];
std::pair<int, int> cursor;
Grid grid;
RBTree bstX;
RBTree bstY;
int bClicked = -1;
bool gridDrawing = false;
LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
bool createWindow();
void drawUI(HDC hdc);
void clickDownhandler(POINT* dmP, int* bClicked);
void clickUphandler(POINT* umP, int* bClicked, bool* gridDrawing);
void gridClickDownhandler(POINT* dmP);
void gridClickUphandler(POINT* umP);
int successfulClick(Grid* grid, HWND hWnd, int* bClicked);
void FindShortestPath();


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
    windowClass.lpszClassName = _T("TestClass");

    RegisterClassEx(&windowClass);

    hWnd = CreateWindowEx(dwExStyle, _T("TestClass"), _T("TestClass"), WS_OVERLAPPEDWINDOW, 
    CW_USEDEFAULT, 0, 1200, 1000, NULL, NULL, hInst, NULL);
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
            POINT dmP;
            GetCursorPos(&dmP);
            if(gridDrawing) {  
                grid.gridClickDownHandler(&dmP);
                return 0;
            }
            clickDownhandler(&dmP, &bClicked);
            return 0;
        case WM_LBUTTONUP:
            POINT umP;
            GetCursorPos(&umP);
            if(gridDrawing && grid.gridClickUphandler(&umP)) {
                successfulClick(&grid, hWnd, &bClicked);
                gridDrawing = false;
                return 0;
            }
            clickUphandler(&umP, &bClicked, &gridDrawing);
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
    Gdiplus::FontFamily fontfam(L"Arial");
    Gdiplus::Font font(&fontfam, 8, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
    Gdiplus::Pen pen(Gdiplus::Color(114,137,218));
    Gdiplus::SolidBrush sBrush(Gdiplus::Color(114,137,218));
    grid = Grid(rect, &gf, &pen);
    std::string spb = "Shortest Path Button";
    std::string ssp = "Set Start Point";
    Button findPathbtn(&gf, 20, 20, 120, 20, &spb, &font, Buttons);
    Button setStartbtn(&gf, 160, 20, 120, 20, &ssp, &font, Buttons);

}

void clickDownhandler(POINT* dmP, int* bClicked) {
    if(dmP->y < 200) {
        for(int i = 0; i < 3; i++){
            if(dmP->x > Buttons[i][0] && dmP->x < Buttons[i][2]
            && dmP->y > Buttons[i][1] && dmP->y < Buttons[i][3]) {
                *bClicked = i;
            }
        }
    }

}

void clickUphandler(POINT* umP, int* bClicked, bool* gridDrawing) {
    if(*bClicked == -1) {
        *gridDrawing = false;
        return;
    } 
    if(umP->x > Buttons[*bClicked][0] && umP->x < Buttons[*bClicked][2]
    && umP->y > Buttons[*bClicked][1] && umP->y < Buttons[*bClicked][3]) {
        *gridDrawing = true;
        return;
    }
    *bClicked = -1;
    *gridDrawing = false;
    return;
}

int successfulClick(Grid* grid, HWND hWnd, int* bClicked) {
    switch(*bClicked) {
        case 0:
        FindShortestPath();
        break;
        case 1:
        grid->setStart(hWnd);
        break;
        case 2:
        grid->setGoal(hWnd);
        break;
        case 3:
        grid->pathDraw();
        break;
    }   
    return 0;
}

void FindShortestPath() {
    std::cout << "fsp btn clicked" << std::endl;
}
