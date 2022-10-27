#include "framework.h"

//setup gridsquare for start, goal, and path, so that there are finite instances of them

class Grid {
    public:
        int dX;
        int dY;
        int width;
        int height;
        std::pair<int, int> gridClick;
        RBTree bstX;
        RBTree bstY;
        Grid(){}
        Grid(RECT rect, Gdiplus::Graphics* gf, Gdiplus::Pen* pen) {
            bstX = bstX;
            bstY = bstY;
            dX = rect.right / 50.0;
            dY = (rect.bottom - 200) / 30.0;
            width = dX*50;
            height = dY*30;
            int top = 200;
            int left = dX;
            int right = left + dX;
            int bottom = top + dY;
            bool once = true;
            for(int j = 0; j < 50; j++) {
                for(int i = 0; i < 30; i++) {           
                    if(right <= rect.right && bottom <= rect.bottom) {
                        gf->DrawRectangle(pen, left, top, dX, dY);
                        if(once) Grid::bstY.rbInsert(bottom); 
                        top = bottom;
                        bottom += dY;
                    }
                }
                Grid::bstX.rbInsert(right);
                left = right;
                right += dX;
                top = 200;
                bottom = dY + 200;
                once = false;             
            }
        }

        void gridClickDownHandler(POINT* dmP) {
            Grid::gridClick.first = bstX.searchTree(dmP->x, Grid::dX);
            Grid::gridClick.second = bstY.searchTree(dmP->y, Grid::dY);
        }

        bool gridClickUphandler(POINT* umP) {
            if(Grid::gridClick.first == 0 || Grid::gridClick.second == 0) return false;;
            if(umP->x < Grid::gridClick.first && umP->x + Grid::dX > Grid::gridClick.first 
            && umP->y < Grid::gridClick.second && umP->y + Grid::dY > Grid::gridClick.second) {
                return true;
            }
            Grid::gridClick.first = 0;
            Grid::gridClick.second = 0;
            return false;
        }

        void setStart(HWND hWnd) {          
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            Gdiplus::SolidBrush sBrush(Gdiplus::Color(80, 200, 120));
            gf.FillRectangle(&sBrush, Grid::gridClick.first - (dX - 1), Grid::gridClick.second - (dY - 1), dX - 1, dY - 1);
            ReleaseDC(hWnd, hdc);
        }

        void setGoal(HWND hWnd) {
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            Gdiplus::SolidBrush sBrush(Gdiplus::Color(204, 0, 0));
            gf.FillRectangle(&sBrush, gridClick.first - (dX - 1), gridClick.second - (dY - 1), dX - 1, dY - 1);
            ReleaseDC(hWnd, hdc);
        }

        void pathDraw() {

        }
};

