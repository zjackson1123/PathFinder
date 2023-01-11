#include "framework.h"

class mouseClick {
    private:
        POINT umP;
        POINT dmP;
    public:
        void set_mUp() {
            GetCursorPos(&umP);
        }

        void set_mDown() {
            GetCursorPos(&dmP);
        }

        void checkClick(HWND *hwnd) {
            if(!Buttons::checkButtons(&umP, &dmP)) {
                if(Buttons::btnarr[1].Clicked || Buttons::btnarr[2].Clicked) {
                    HDC hdc = GetDC(*hwnd);
                    Gdiplus::Graphics gf(hdc);
                    std::string name;
                    if(Buttons::btnarr[1].Clicked){name = std::string("Start");}
                    else {name = std::string("Goal");}
                    if(Grid::legalClick(&dmP, &umP, name.c_str())) {
                        Buttons::resetButtons(&gf);
                    }
                }
                else if(Buttons::btnarr[3].Clicked) {
                Grid::legalClick(&dmP, &umP);
                }
                else if(Buttons::btnarr[4].Clicked) {
                    Grid::legalClick(&dmP, &umP, "Erase");
                }
            }  
        }
};
