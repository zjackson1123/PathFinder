#include "framework.h"

class Button {
    private:       
        void drawButton(Gdiplus::Graphics* gf, Button* btn, Gdiplus::Pen* pen, Gdiplus::SolidBrush* sbrush, Gdiplus::Font* font) {
            gf->DrawRectangle(pen, btn->btnRect);
            gf->FillRectangle(sbrush, btn->btnRect);
            pen->SetColor(Gdiplus::Color(0, 0, 0));
            gf->DrawRectangle(pen, btn->btnBorder);
            pen->SetColor(Gdiplus::Color(255, 255, 255));
            if(btn->btnText != "") {
                std::wstring btnString(btn->btnText.begin(), btn->btnText.end());
                Gdiplus::StringFormat strF = new Gdiplus::StringFormat();
                strF.SetAlignment(Gdiplus::StringAlignmentCenter);
                strF.SetLineAlignment(Gdiplus::StringAlignmentCenter);
                sbrush->SetColor(Gdiplus::Color(0, 0, 0));           
                gf->DrawString(btnString.c_str(), -1, font, btn->btnBorder, &strF, sbrush);
                sbrush->SetColor(Gdiplus::Color(255, 255, 255));
            }
        }
    
    public:
        Gdiplus::RectF btnRect;
        Gdiplus::RectF btnBorder;
        std::string btnText = "";
        Button(){}
        Button(Gdiplus::Graphics* gf, int x, int y, int width, int height, Gdiplus::Pen* pen, Gdiplus::SolidBrush* sbrush, Gdiplus::Font* font){
            btnRect = Gdiplus::RectF(x, y, width, height);
            btnBorder = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
            drawButton(gf, this, pen, sbrush, font);
        }
        Button(Gdiplus::Graphics* gf, int x, int y, int width, int height, std::string* text, Gdiplus::Pen* pen, Gdiplus::SolidBrush* sbrush, Gdiplus::Font* font){
            btnRect = Gdiplus::RectF(x, y, width, height);
            btnBorder = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
            btnText = *text;
            drawButton(gf, this, pen, sbrush, font);
        }       
};