#include "framework.h"

class Grid {
    public:
        int dX;
        int dY;
        int width;
        int height;
        Grid(){}
        Grid(RECT rect, Gdiplus::Graphics* gf, Gdiplus::Pen* pen, RBTree* bstX, RBTree* bstY) {
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
                        if(once) bstY->rbInsert(bottom); 
                        top = bottom;
                        bottom += dY;
                    }
                }
                bstX->rbInsert(right);
                left = right;
                right += dX;
                top = 200;
                bottom = dY + 200;
                once = false;             
            }
        }

        void setStart(std::pair<int, int>* inItem, HWND hWnd) {          
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            Gdiplus::SolidBrush sBrush(Gdiplus::Color(80, 200, 120));
            gf.FillRectangle(&sBrush, inItem->first - (dX - 1), inItem->second - (dY - 1), dX - 1, dY - 1);
            ReleaseDC(hWnd, hdc);
        }

        void setGoal() {

        }

        void pathDraw() {

        }
};

