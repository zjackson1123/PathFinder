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
LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
bool createWindow();
void drawUI(HDC hdc);
void clickDownhandler(std::pair<int, int>* inItem, RBTree* bstX, RBTree* bstY, Grid* grid);
void clickUphandler(std::pair<int, int>* inItem, Grid* grid);
int successfulClick(std::pair<int, int>* inItem, Grid* grid, POINT* cursor);
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
            clickDownhandler(&inItem, &bstX, &bstY, &grid);
            return 0;
        case WM_LBUTTONUP:
            clickUphandler(&inItem, &grid);
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
    grid = Grid(rect, &gf, &pen, &bstX, &bstY);
    std::string spb = "Shortest Path Button";
    std::string ssp = "Set Start Point";
    Button findPathbtn(&gf, 20, 20, 120, 20, &spb, &font, Buttons);
    Button setStartbtn(&gf, 160, 20, 120, 20, &ssp, &font, Buttons);
}

void clickDownhandler(std::pair<int, int>* inItem, RBTree* bstX, RBTree* bstY, Grid* grid) {
    POINT dmP;
    GetCursorPos(&dmP);
    ScreenToClient(hWnd, &dmP);
    if(dmP.y > 200) {
        for(int i = 0; i < 3; i++){
            if(dmP.x > Buttons[i][0] && dmP.x < Buttons[i][3]
            && dmP.y > Buttons[i][1] && dmP.y < Buttons[i][3]) {

            }
        }
    }
    inItem->first = bstX->searchTree(dmP.x, grid->dX);
    inItem->second = bstY->searchTree(dmP.y, grid->dY);
}

void clickUphandler(std::pair<int, int>* inItem, Grid* grid) {
    if(inItem->first == 0 || inItem->second == 0) return;
    POINT umP;
    GetCursorPos(&umP);
    ScreenToClient(hWnd, &umP);
    if(umP.x < inItem->first && umP.x + grid->dX > inItem->first 
    && umP.y < inItem->second && umP.y + grid->dY > inItem->second) {
        successfulClick(inItem, grid, &umP);
        return;
    }
    inItem->first = 0;
    inItem->second = 0;
}

int successfulClick(std::pair<int, int>* inItem, Grid* grid, POINT* cursor) {
    grid->setStart(inItem, hWnd);
    inItem->first = 0;
    inItem->second = 0;
    return 0;
}

void FindShortestPath() {
    std::cout << "fsp btn clicked" << std::endl;
}
