#include "framework.h"

class Button {          
    public:
        Gdiplus::RectF btnRect;
        Gdiplus::RectF btnBorder;
        std::string btnName = "";
        int btnID = 0;
        bool btnActive = true;
        Button(){}
        Button(Gdiplus::Graphics* gf, int x, int y, int width, int height){      
            btnRect = Gdiplus::RectF(x, y, width, height);
            btnBorder = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
            drawButton(gf, this);
        }
        Button(Gdiplus::Graphics* gf, int x, int y, int width, int height, std::string* text){
            btnRect = Gdiplus::RectF(x, y, width, height);
            btnBorder = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
            btnName = *text;
            drawButton(gf, this);
        }
        Button(Gdiplus::Graphics* gf, int x, int y, int width, int height, std::string* text, bool btnactive){
            btnRect = Gdiplus::RectF(x, y, width, height);
            btnBorder = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
            btnName = *text;
            if(btnactive) {
                drawButton(gf, this);
            }           
        }

        void drawButton(Gdiplus::Graphics* gf, Button* btn) {
            Gdiplus::Pen pen(Gdiplus::Color(0, 0, 0));
            Gdiplus::SolidBrush sbrush(Gdiplus::Color(114,137,218));
            Gdiplus::FontFamily fontfam(L"Arial");
            Gdiplus::Font font(&fontfam, 8, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
            gf->FillRectangle(&sbrush, btn->btnRect);
            sbrush.SetColor(Gdiplus::Color(0, 0, 0));
            gf->DrawRectangle(&pen, btn->btnBorder);
            if(btn->btnName != "") {
                std::wstring btnString(btn->btnName.begin(), btn->btnName.end());
                Gdiplus::StringFormat strF = new Gdiplus::StringFormat();
                strF.SetAlignment(Gdiplus::StringAlignmentCenter);
                strF.SetLineAlignment(Gdiplus::StringAlignmentCenter);        
                gf->DrawString(btnString.c_str(), -1, &font, btn->btnBorder, &strF, &sbrush);
            }
        }

        void hideButton(Gdiplus::Graphics* gf, Button* btn) {
            Gdiplus::SolidBrush bgBrush(Gdiplus::Color(66,69,73));
            gf->FillRectangle(&bgBrush, btn->btnRect);           
        }
};

class Buttons {
    private:
        int btnCount = 0;

    public:
        Button Buttons[8];
        void addBtn(Button button) {
            if(btnCount < 8){
                button.btnID = btnCount;
                Buttons[btnCount++] = button;
            }
        }

        void disableBtn(Button button, HWND hWnd) {
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            button.hideButton(&gf, &button);
            button.btnActive = false;
            ReleaseDC(hWnd, hdc);    
        }

        void enableBtn(Button button, HWND hWnd) {
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            button.drawButton(&gf, &button);
            button.btnActive = true;
            ReleaseDC(hWnd, hdc); 
        }

        int checkButtons(POINT* mPos) {
            for(int i = 0; i < 8; i++) {
                if(mPos->x > Buttons[i].btnRect.X && mPos->x < (Buttons[i].btnRect.X + Buttons[i].btnRect.Width)
                && mPos->y > Buttons[i].btnRect.Y && mPos->y < (Buttons[i].btnRect.Y + Buttons[i].btnRect.Height + 20)) {
                    return i;
                }
            }
            return -1;
        }
};