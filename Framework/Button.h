#include "framework.h"

class Button {     
    public:
        Gdiplus::RectF Rect;
        Gdiplus::RectF BorderRect;
        static inline std::string NamesArr[8] = {"Find Shortest Path", "Set Start", "Set Goal", "Draw Path", 
                            "Erase Path", "Clear Grid", "Stop Drawing", "Stop Erasing"};
        std::string Name = "";
        int ID = 0;
        bool Drawn = true;
        bool Clicked = false;
        Button(){}
        Button(int x, int y, int width, int height){      
            Rect = Gdiplus::RectF(x, y, width, height);
            BorderRect = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
        }
        Button(int x, int y, int width, int height, bool btnactive){
            Rect = Gdiplus::RectF(x, y, width, height);
            BorderRect = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
            Drawn = btnactive;          
        }

        void drawButton(Gdiplus::Graphics *gf, Gdiplus::Color btnColor = Gdiplus::Color(114,137,218)) {
            Gdiplus::Pen pen(Gdiplus::Color(0, 0, 0));
            Gdiplus::SolidBrush sbrush(btnColor);
            Gdiplus::FontFamily fontfam(L"Arial");
            Gdiplus::Font font(&fontfam, 8, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
            gf->FillRectangle(&sbrush, this->Rect);
            sbrush.SetColor(Gdiplus::Color(0, 0, 0));
            gf->DrawRectangle(&pen, this->BorderRect);
            if(this->Name != "") {
                std::wstring btnString(this->Name.begin(), this->Name.end());
                Gdiplus::StringFormat strF = new Gdiplus::StringFormat();
                strF.SetAlignment(Gdiplus::StringAlignmentCenter);
                strF.SetLineAlignment(Gdiplus::StringAlignmentCenter);        
                gf->DrawString(btnString.c_str(), -1, &font, this->BorderRect, &strF, &sbrush);
            }
            this->Drawn = true;
        }

        void hideButton(Gdiplus::Graphics *gf) {
            Gdiplus::SolidBrush bgBrush(Gdiplus::Color(66,69,73));
            gf->FillRectangle(&bgBrush, this->Rect);         
            this->Drawn = false;  
        }
};

class Buttons {
    private:
        static const int btnCount = 8;
        static inline HWND hWnd = NULL;

        static void btnColor(Gdiplus::Graphics *gf, Button *btn) {
            if(!btn->Clicked){
                btn->drawButton(gf, Gdiplus::Color(255, 255, 255));
                btn->Clicked = true;
            }
            else {
                btn->drawButton(gf);
                btn->Clicked = false;
            }
        }

        static void Erasing(Gdiplus::Graphics *gf) {
            btnarr[4].hideButton(gf);
            btnarr[4].Clicked = true;
            btnarr[7].drawButton(gf);
        }

        static void Drawing(Gdiplus::Graphics *gf) {
            btnarr[3].hideButton(gf);
            btnarr[3].Clicked = true;
            btnarr[6].drawButton(gf);
        }

    public:
        static inline Button btnarr[8] = {};
        static void setHWND(HWND *hwnd) {
            hWnd = *hwnd;
        }
        
        void initBtns() {
            int x = 20;
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            for(int i = 0; i < btnCount - 2; i++) {
                addBtn(Button(x, 20, 120, 20), i, &gf);
                x += 140;
            }
            addBtn(Button(440, 60, 120, 20, false), btnCount - 2, &gf);
            addBtn(Button(580, 60, 120, 20, false), btnCount - 1, &gf);
            ReleaseDC(hWnd, hdc);

        }
        
        void addBtn(Button button, int i, Gdiplus::Graphics *gf) {
            if(i < btnCount) {
                button.ID = i;
                button.Name = button.NamesArr[i];
                btnarr[i] = button;
            }
            if(button.Drawn) {button.drawButton(gf);}            
        }

        static void resetButtons(Gdiplus::Graphics *gf) {
            Gdiplus::SolidBrush sbrush(Gdiplus::Color(66,69,73));
            gf->FillRectangle(&sbrush, 0, 0, 940, 120);
            for(int i = 0; i < btnCount - 2; i++) {            
                btnarr[i].Clicked = false;
                btnarr[i].Drawn = true;
                btnarr[i].drawButton(gf);
            }

            btnarr[btnCount - 1].Drawn = false;
            btnarr[btnCount].Drawn = false;
        }

        static bool checkButtons(POINT *ump, POINT *dmp) {
            for(int i = 0; i < btnCount; i++) {
                //the following overly-complex if statement just checks to make sure the mouse's down press and up press are both
                //in a button, and that its the same button. 
                if(ump->x > btnarr[i].Rect.X && ump->x < (btnarr[i].Rect.X + btnarr[i].Rect.Width)
                && ump->y > btnarr[i].Rect.Y && ump->y < (btnarr[i].Rect.Y + btnarr[i].Rect.Height + 20)
                && dmp->x > btnarr[i].Rect.X && dmp->x < (btnarr[i].Rect.X + btnarr[i].Rect.Width)
                && dmp->y > btnarr[i].Rect.Y && dmp->y < (btnarr[i].Rect.Y + btnarr[i].Rect.Height + 20)) {
                    handleClick(i);
                    return true;
                }
            }
            return false;
        }

        static void handleClick(int i) {
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            resetButtons(&gf);
            Button *btn = &btnarr[i];
            switch(i) {
                case 0:
                Grid::Astar();
                break;
                case 1:
                btnColor(&gf, btn);
                break;
                case 2:
                btnColor(&gf, btn);   
                break;
                case 3:
                Drawing(&gf);
                break;
                case 4:
                Erasing(&gf);
                break;
                case 5:
                Grid::clearGrid();
                break;
                case 6:
                break;
                case 7:
                break;
            }
            ReleaseDC(hWnd, hdc);
        }
};
