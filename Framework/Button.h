#include "framework.h"

class Button {
    private:       
        void drawButton(Gdiplus::Graphics* gf, Button* btn, Gdiplus::Font* font, int Buttons[3][4]) {
            for(int i = 0; i < 3; i++){
                if(Buttons[i] == NULL){
                    Buttons[i][0] = (int)btn->btnRect.X;
                    Buttons[i][1] = (int)btn->btnRect.Y;
                    Buttons[i][2] = (int)btn->btnRect.Width - btn->btnRect.X;
                    Buttons[i][2] = (int)btn->btnRect.Height - btn->btnRect.Y;
                    break;
                }
            }
            Gdiplus::Pen pen(Gdiplus::Color(114,137,218));
            Gdiplus::SolidBrush sbrush(Gdiplus::Color(114,137,218));
            gf->DrawRectangle(&pen, btn->btnRect);
            gf->FillRectangle(&sbrush, btn->btnRect);
            pen.SetColor(Gdiplus::Color(0, 0, 0));
            sbrush.SetColor(Gdiplus::Color(0, 0, 0));
            gf->DrawRectangle(&pen, btn->btnBorder);
            if(btn->btnText != "") {
                std::wstring btnString(btn->btnText.begin(), btn->btnText.end());
                Gdiplus::StringFormat strF = new Gdiplus::StringFormat();
                strF.SetAlignment(Gdiplus::StringAlignmentCenter);
                strF.SetLineAlignment(Gdiplus::StringAlignmentCenter);        
                gf->DrawString(btnString.c_str(), -1, font, btn->btnBorder, &strF, &sbrush);
            }
        }
    
    public:
        Gdiplus::RectF btnRect;
        Gdiplus::RectF btnBorder;
        std::string btnText = "";
        Button(){}
        Button(Gdiplus::Graphics* gf, int x, int y, int width, int height, Gdiplus::Font* font, int Buttons[3][4]){      
            btnRect = Gdiplus::RectF(x, y, width, height);
            btnBorder = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
            drawButton(gf, this, font, Buttons);
        }
        Button(Gdiplus::Graphics* gf, int x, int y, int width, int height, std::string* text, Gdiplus::Font* font, int Buttons[3][4]){
            btnRect = Gdiplus::RectF(x, y, width, height);
            btnBorder = Gdiplus::RectF(x + 2.5, y + 2.5, width - 5, height - 5);
            btnText = *text;
            drawButton(gf, this, font, Buttons);
        }       
};