#include "framework.h"

class Button {     
    public:
        Gdiplus::RectF btnRect;
        Gdiplus::RectF btnBorder;
        static inline std::string btnNames[8] = {"Find Shortest Path", "Set Start Point", "Set Goal", "Draw Path", 
                            "Erase Path", "Clear Grid", "Stop Drawing", "Stop Erasing"};
        std::string btnName = "";
        int btnID = 0;
        bool btnActive = true;
        Button(){}
        Button(int x, int y, int width, int height){      
            btnRect = Gdiplus::RectF(x, y, width, height);
            btnBorder = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
        }
        Button(int x, int y, int width, int height, bool btnactive){
            btnRect = Gdiplus::RectF(x, y, width, height);
            btnBorder = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
            btnActive = btnactive;          
        }

        void drawButton(Gdiplus::Graphics *gf) {
            Gdiplus::Pen pen(Gdiplus::Color(0, 0, 0));
            Gdiplus::SolidBrush sbrush(Gdiplus::Color(114,137,218));
            Gdiplus::FontFamily fontfam(L"Arial");
            Gdiplus::Font font(&fontfam, 8, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
            gf->FillRectangle(&sbrush, this->btnRect);
            sbrush.SetColor(Gdiplus::Color(0, 0, 0));
            gf->DrawRectangle(&pen, this->btnBorder);
            if(this->btnName != "") {
                std::wstring btnString(this->btnName.begin(), this->btnName.end());
                Gdiplus::StringFormat strF = new Gdiplus::StringFormat();
                strF.SetAlignment(Gdiplus::StringAlignmentCenter);
                strF.SetLineAlignment(Gdiplus::StringAlignmentCenter);        
                gf->DrawString(btnString.c_str(), -1, &font, this->btnBorder, &strF, &sbrush);
            }
            this->btnActive = true;
        }

        void hideButton(Gdiplus::Graphics *gf) {
            Gdiplus::SolidBrush bgBrush(Gdiplus::Color(66,69,73));
            gf->FillRectangle(&bgBrush, this->btnRect);         
            this->btnActive = false;  
        }
};

class Buttons {
    private:
        int btnCount = 8;
        static inline HWND hWnd = NULL;
    public:
        Button btnarr[8];
        static void setHWND(HWND *hwnd) {
            hWnd = *hwnd;
        }

        void addBtn(Button button, int i, Gdiplus::Graphics *gf) {
            if(i < btnCount) {
                button.btnID = i;
                button.btnName = button.btnNames[i];
                if(button.btnActive)
                    button.drawButton(gf);

                btnarr[i] = button;
            }
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

        void toggleErase() {
            Button *stopErase = &btnarr[7];
            Button *Erase = &btnarr[4];
            Button *stopDraw = &btnarr[6];
            Button *Draw = &btnarr[3];
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            if(!Erase->btnActive && stopErase->btnActive) {
                stopErase->hideButton(&gf);
                Erase->drawButton(&gf);
            }
            else {
                Erase->hideButton(&gf);
                stopErase->drawButton(&gf);
                if(stopDraw->btnActive) {
                    stopDraw->hideButton(&gf);
                    Draw->drawButton(&gf);
                }

            }
            ReleaseDC(hWnd, hdc);
        }

        void toggleDraw() {
            Button *stopDraw = &btnarr[6];
            Button *Draw = &btnarr[3];
            Button *Erase = &btnarr[4];
            Button *stopErase = &btnarr[7];
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            if(!Draw->btnActive && stopDraw->btnActive) {
                stopDraw->hideButton(&gf);
                Draw->drawButton(&gf);
            }
            else {
                Draw->hideButton(&gf);
                stopDraw->drawButton(&gf);
                if(stopErase->btnActive) {
                    stopErase->hideButton(&gf);
                    Erase->drawButton(&gf);
                }
            }
            ReleaseDC(hWnd, hdc);
        }

        void stopDraw() {
            Button *Draw = &btnarr[3];
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            if(!Draw->btnActive) {
                Button *stopDraw = &btnarr[6];
                Draw->drawButton(&gf);
                stopDraw->hideButton(&gf);
            }
            ReleaseDC(hWnd, hdc);
        }

        void stopErase() {
            Button *Erase = &btnarr[4];
            HDC hdc = GetDC(hWnd);
            Gdiplus::Graphics gf(hdc);
            if(!Erase->btnActive) {
                Button *stopErase = &btnarr[7];
                stopErase->hideButton(&gf);
                Erase->drawButton(&gf);
            }
            ReleaseDC(hWnd, hdc);
        }

        int checkButtons(POINT* mPos) {
            for(int i = 0; i < btnCount; i++) {
                if(mPos->x > btnarr[i].btnRect.X && mPos->x < (btnarr[i].btnRect.X + btnarr[i].btnRect.Width)
                && mPos->y > btnarr[i].btnRect.Y && mPos->y < (btnarr[i].btnRect.Y + btnarr[i].btnRect.Height + 20)) {
                    return i;
                }
            }
            return -1;
        }
};