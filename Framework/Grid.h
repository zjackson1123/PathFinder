#include "framework.h"
struct GridSquare {
    bool Drawn;
    int X;
    int Y;
    GridSquare(){
        X = 0;
        Y = 0;
        Drawn = false;
    };
};

class Grid {
    private:
        GridSquare Start;
        GridSquare Goal; 

    public:
        int dX;
        int dY;
        int width;
        int height;
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

        void legalClick(POINT* dmP, POINT* umP, HWND hWnd, const char* sqType = "") {
            int x1 = bstX.searchTree(umP->x, Grid::dX);
            int y1 = bstY.searchTree(umP->y, Grid::dY);
            int x2 = bstX.searchTree(dmP->x, Grid::dX);
            int y2 = bstY.searchTree(dmP->y, Grid::dY);
            if(x1 == 0 || x2 == 0) return;    
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            
            if(x1 == x2 && y1 == y2) {     
                drawGsquare(&gf, x1, y1, sqType);
                ReleaseDC(hWnd, hdc);
                return;
            }

            while(x1 != x2 || y1 != y2) {
                if(x2 > x1) {
                    drawGsquare(&gf, x2, y2, sqType);
                    x2 -= dX;
                }
                if(x1 > x2) {
                    drawGsquare(&gf, x2, y2, sqType);
                    x2 += dX;
                }
                if(y2 > y1) {
                    drawGsquare(&gf, x2, y2, sqType);
                    y2 -= dY;
                }          
                if(y1 > y2) {
                    drawGsquare(&gf, x2, y2, sqType);
                    y2 += dY;
                } 
            }
            drawGsquare(&gf, x1, y2, sqType);
            ReleaseDC(hWnd, hdc);
        }

        void drawGsquare(Gdiplus::Graphics* gf, int x, int y, const char* sqType) {
            Gdiplus::SolidBrush sqBrush(Gdiplus::Color(255, 255, 255));
            Gdiplus::SolidBrush bgBrush(Gdiplus::Color(66,69,73));
            if(strcmp(sqType, "Erase") == 0) {
                gf->FillRectangle(&bgBrush, x - (dX - 1), y - (2*dY - 1), dX - 1, dY - 1);
                return;
            }
            if(strcmp(sqType, "Start") == 0) {
                sqBrush.SetColor(Gdiplus::Color(80, 200, 120));
                if(Start.Drawn == true) {
                    gf->FillRectangle(&bgBrush, Start.X, Start.Y, dX - 1, dY - 1);            
                }
                Start.Drawn = true;
                Start.X = x - (dX - 1);
                Start.Y = y - (2*dY - 1);
            }
            else if(strcmp(sqType, "Goal") == 0) {
                sqBrush.SetColor(Gdiplus::Color(204, 0, 0));
                if(Goal.Drawn) {
                    gf->FillRectangle(&bgBrush, Goal.X, Goal.Y, dX - 1, dY - 1);
                }
                Goal.Drawn = true;
                Goal.X = x - (dX - 1);
                Goal.Y = y - (2*dY - 1);
            }
            gf->FillRectangle(&sqBrush, x - (dX - 1), y - (2*dY - 1), dX - 1, dY - 1);
        }
};
