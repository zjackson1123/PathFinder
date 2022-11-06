#include "framework.h"

class GridSquare {
    public:
        bool Drawn;
        int X;
        int Y;
        int Width;
        int Height;
        GridSquare(){
            X = 0;
            Y = 0;
            Width = 0;
            Height = 0;
            Drawn = false;
        };
};




class Grid {
    private:
        GridSquare Start;
        GridSquare Goal; 
        std::pair<int, int> gridDraw;
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

        bool legalClick(POINT* dmP, POINT* umP) {
            int x = bstX.searchTree(umP->x, Grid::dX);
            int y = bstY.searchTree(umP->y, Grid::dY);
            if(bstX.searchTree(dmP->x, Grid::dX) == x
            && bstY.searchTree(dmP->y, Grid::dY) == y) {    
                Grid::gridClick.first = x;
                Grid::gridClick.second = y;     
                return true;
            }
            Grid::gridClick.first = 0;
            Grid::gridClick.second = 0;  
            return false;
        }

        void tryDraw(HWND hWnd, POINT* dmP, POINT* umP) {
            int x1 = bstX.searchTree(umP->x, dX);
            int y1 = bstY.searchTree(umP->y, dY);
            int x2 = bstX.searchTree(dmP->x, dX);
            int y2 = bstY.searchTree(dmP->y, dY);
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            Gdiplus::SolidBrush sBrush(Gdiplus::Color(255, 255, 255));
            if(x1 == x2) {
                int i = y1 > y2 ? y2 - (2*dY - 1) : y1 - (2*dY - 1);
                int j = y1 < y2 ? y2 - (2*dY - 1) : y1- (2*dY - 1);
                for(i; i <= j; i += dY) {
                    gf.FillRectangle(&sBrush, x1 - (dX - 1), i, dX - 1, dY - 1);
                }   
            }
            else if (y1 == y2) {
                int i = x1 > x2 ? x2 - (dX - 1): x1 - (dX - 1);
                int j = x1 < x2 ? x2 - (dX - 1): x1 - (dX - 1);
                for(i; i <= j; i += dX) {
                    gf.FillRectangle(&sBrush, i, y1 - (2*dY - 1), dX - 1, dY - 1);
                }
            }
            ReleaseDC(hWnd, hdc);
        }

        void setStart(HWND hWnd) {          
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            Gdiplus::SolidBrush sBrush(Gdiplus::Color(80, 200, 120));
            Gdiplus::SolidBrush brBrush(Gdiplus::Color(66,69,73));
            gf.FillRectangle(&sBrush, Grid::gridClick.first - (dX - 1), Grid::gridClick.second - (2*dY - 1), dX - 1, dY - 1);
            if(Start.Drawn == true) {
                gf.FillRectangle(&brBrush, Start.X, Start.Y, Start.Width, Start.Height);            
            }

            Start.Drawn = true;
            Start.X = gridClick.first - (dX - 1);
            Start.Y = gridClick.second - (2*dY - 1);
            Start.Width = dX - 1;
            Start.Height = dY - 1;                    
            ReleaseDC(hWnd, hdc);
        }

        void setGoal(HWND hWnd) {
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            Gdiplus::SolidBrush sBrush(Gdiplus::Color(204, 0, 0));
            Gdiplus::SolidBrush brBrush(Gdiplus::Color(66,69,73));
            gf.FillRectangle(&sBrush, gridClick.first - (dX - 1), gridClick.second - (2*dY - 1), dX - 1, dY - 1);
            if(Goal.Drawn) {
                gf.FillRectangle(&brBrush, Goal.X, Goal.Y, Goal.Width, Goal.Height);
            }
       
            Goal.Drawn = true;
            Goal.X = gridClick.first - (dX - 1);
            Goal.Y = gridClick.second - (2*dY - 1);
            Goal.Width = dX - 1;
            Goal.Height = dY - 1;            
            ReleaseDC(hWnd, hdc);
        }
};

