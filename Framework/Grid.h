#include "framework.h"

struct GS {
    bool Drawn;
    int X;
    int Y;
    int parentX;
    int parentY;
    float hCost;
    float gCost;
    float fCost;    
    GS(){
        X = 0;
        Y = 0;
        parentX = -1;
        parentY = -1;
        hCost = __FLT_MAX__;
        gCost = __FLT_MAX__;
        fCost = __FLT_MAX__;
        Drawn = false;
    };
};

inline bool operator < (const GS& left, const GS& right) {
    return left.fCost < right.fCost;
}
 

class Grid {
    private:
        static inline HWND hWnd = NULL;
        static inline GS *Start = new GS;
        static inline GS *Goal = new GS;  
    public:
        static inline int dX = 0;
        static inline int dY = 0;
        static inline int width = 0;
        static inline int height = 0;
        static inline RBTree bstX;
        static inline RBTree bstY;
        static inline GS GridMap[ROWS][COLUMNS];
        Grid(){}
        Grid(RECT rect, Gdiplus::Graphics* gf, Gdiplus::Pen* pen) {
            dX = rect.right / 50.0;
            dY = (rect.bottom - 200) / 30.0;
            width = dX*50;
            height = dY*30;
            int top = 200;
            int left = dX;
            int right = left + dX;
            int bottom = top + dY;
            bool once = true;
            for(int j = 0; j < ROWS; j++) {
                for(int i = 0; i < COLUMNS; i++) {
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

        static void setHWND(HWND *hwnd) {
            hWnd = *hwnd;
        }

        static void clearGrid() {
            RECT rect = {0, 200, width + dX, height + 200};
            Grid::Start = new GS;
            Grid::Goal = new GS;
            InvalidateRect(hWnd, &rect, true);
        }

        static bool legalClick(POINT* dmP, POINT* umP, const char* sqType = "") {
            int x1 = bstX.searchTree(umP->x, Grid::dX);
            int y1 = bstY.searchTree(umP->y, Grid::dY);
            int x2 = bstX.searchTree(dmP->x, Grid::dX);
            int y2 = bstY.searchTree(dmP->y, Grid::dY);
            if(x1 == 0 || x2 == 0 || y1 == 0 || y2 == 0) return false;    
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            if(x1 == x2 && y1 == y2) {     
                drawGS(&gf, x1, y1, sqType);
                ReleaseDC(hWnd, hdc); 
                return true;
            }    
            while(x2 != x1 || y2 != y1) {
                drawGS(&gf, x2, y2, sqType);
                if(x2 > x1) x2 -= dX;
                if(x1 > x2) x2 += dX;
                if(y2 > y1) y2 -= dY;
                if(y1 > y2) y2 += dY;
            }
            drawGS(&gf, x1, y2, sqType);
            ReleaseDC(hWnd, hdc);
            return true;
        }

        static void drawGS(Gdiplus::Graphics* gf, int x, int y, const char* sqType) {
            Gdiplus::SolidBrush sqBrush(Gdiplus::Color(255, 255, 255)); 
            Gdiplus::SolidBrush bgBrush(Gdiplus::Color(66,69,73));
            if(strcmp(sqType, "bPath") == 0) {
                sqBrush.SetColor(Gdiplus::Color(80, 200, 120));
                int gridX = (x * dX) + 1;
                int gridY = (y * dY) + 175;
                if(!(gridX == Goal->X && gridY == Goal->Y)) {
                    gf->FillRectangle(&sqBrush, gridX, gridY, dX - 1, dY - 1);
                    return;
                }
            }

            if(x <= dX || x >= width || y <= dY + 200 || y >= height + 200) {
                return;
            }

            if(strcmp(sqType, "Erase") == 0) {
                HDC hdc = GetDC(hWnd);
                COLORREF clr = GetPixel(hdc, (x + 1) - dX , (y + 1) - (2*dY));
                if(clr == RGB(204, 0, 0)) {Grid::Goal = nullptr;} 
                if(clr == RGB(80, 200, 120)) {Grid::Start = nullptr;}
                gf->FillRectangle(&bgBrush, x - (dX - 1), y - (2*dY - 1), dX - 1, dY - 1);
                ReleaseDC(hWnd, hdc);
                return;
            }
            else if(strcmp(sqType, "Start") == 0) {
                sqBrush.SetColor(Gdiplus::Color(80, 200, 120));
                if(Start->Drawn) {
                    gf->FillRectangle(&bgBrush, Start->X, Start->Y, dX - 1, dY - 1);            
                }
                Start->Drawn = true;
                Start->X = x - (dX - 1);
                Start->Y = y - (2*dY - 1);
            }
            else if(strcmp(sqType, "Goal") == 0) {
                sqBrush.SetColor(Gdiplus::Color(204, 0, 0));
                if(Goal->Drawn) {
                    gf->FillRectangle(&bgBrush, Goal->X, Goal->Y, dX - 1, dY - 1);
                }
                Goal->Drawn = true;
                Goal->X = x - (dX - 1);
                Goal->Y = y - (2*dY - 1);
            }
            gf->FillRectangle(&sqBrush, x - (dX - 1), y - (2*dY - 1), dX - 1, dY - 1);
        }

        static bool IsValid(int x, int y, HDC *hdc) {
            COLORREF clr = GetPixel(*hdc, (x * dX) + 37, (y * dY) + 199);
            COLORREF white = RGB(255, 255, 255);
            COLORREF red = RGB(204, 0, 0);
            if(x < 0 || y < 0 || x >= ROWS || y >= COLUMNS || (clr != white && clr != red)) {
                std::cout << "valid sq found" << std::endl;
                return false;
            }
            return true;
        }
        
        static bool IsDest(int x, int y) {
            if(x == ((Goal->X - 1) / dX) && y == ((Goal->Y - 175) / dY)) {
                std::cout << "destination found" << std::endl;
                return true;
            }
            return false;
        }

        static double Hcalc(int x, int y) {
            int destX = (Goal->X - 1) / dX;
            int destY = (Goal->Y + (dY - 1) - 200) / dY;
            return (sqrt((x - destX)*(x - destX) + (y - destY)*(y - destY)));
        }

        static void MakePath(HDC *hdc) {
            int x = (Goal->X + 1) / dX;
            int y = ((Goal->Y + (dY - 1)) - 200) / dY;
            std::stack<GS> path;
            Gdiplus::Graphics gf(*hdc);
            try {
                while(!(GridMap[x][y].parentX == x && GridMap[x][y].parentY == y)
                && GridMap[x][y].parentX != -1 && GridMap[x][y].parentY != -1) {
                    path.push(GridMap[x][y]);
                    int parX = GridMap[x][y].parentX;
                    int parY = GridMap[x][y].parentY;
                    x = parX;
                    y = parY;
                }
                path.push(GridMap[x][y]);
                while(!path.empty()) {
                    GS top = path.top();
                    path.pop();
                    drawGS(&gf, top.X, top.Y, "bPath");
                    std::cout << top.X << std::endl;
                }
            }
            catch(...){
                //exception
            }
        }


        static void Astar() {
            if(!Goal->Drawn || !Start->Drawn) {
                return;
            }   
            bool cList[ROWS][COLUMNS];
            for(int i = 0; i < ROWS; i++) {
                for(int j = 0; j < COLUMNS; j++) {
                    GridMap[i][j].X = i;  
                    GridMap[i][j].Y = j;
                    GridMap[i][j].hCost = __FLT_MAX__;
                    GridMap[i][j].fCost = __FLT_MAX__;
                    GridMap[i][j].gCost = __FLT_MAX__;
                    cList[i][j] = false;
                }
            }
            int x = (Start->X + 1) / dX;
            int y = ((Start->Y + (dY - 1)) - 200) / dY;
            GridMap[x][y].fCost = 0.0;
            GridMap[x][y].gCost = 0.0;
            GridMap[x][y].hCost = 0.0;
            GridMap[x][y].X = x;
            GridMap[x][y].Y = y;
            bool foundDest = false;
            std::set<GS> oList;
            oList.emplace(GridMap[x][y]);
            HDC hdc = GetDC(hWnd);
            while(!oList.empty()) {
                std::set<GS>::iterator it = oList.begin();
                GS gs = *it;
                oList.erase(it);
                x = gs.X;
                y = gs.Y;
                cList[x][y] = true;
                for(int nX = -1; nX <= 1; nX++){
                    for(int nY = -1; nY <= 1; nY++){
                        double nG, nH, nF;
                        if(IsValid(x + nX, y + nY, &hdc)){                   
                            if(IsDest(x + nX, y + nY)){
                                GridMap[x + nX][y + nY].parentX = x;
                                GridMap[x + nX][y + nY].parentY = y;
                                foundDest = true;                   
                                MakePath(&hdc);
                                ReleaseDC(hWnd, hdc);
                            }
                            else if (cList[x + nX][y + nY] != true){
                                nG = gs.gCost + 1.0;
                                nH = Hcalc(x + nX, y + nY);
                                nF = nH + nG;

                                if(GridMap[x + nX][y + nY].fCost == __FLT_MAX__ || GridMap[x + nX][y + nY].fCost > nF){
                                    GridMap[x + nX][y + nY].fCost = nF;
                                    GridMap[x + nX][y + nY].gCost = nG;
                                    GridMap[x + nX][y + nY].hCost = nH;
                                    GridMap[x + nX][y + nY].parentX = x;
                                    GridMap[x + nX][y + nY].parentY = y;
                                    oList.emplace(GridMap[x + nX][y + nY]);      
                                     
                                }
                            }
                        }

                    }
                }    
            }
                ReleaseDC(hWnd, hdc);       
        }
};
