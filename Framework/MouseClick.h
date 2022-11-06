#include "framework.h"

class mouseClick {
    private:
        bool cleanClick = true;
        bool sClick = false;
        bool gClick = false;
        bool pClick = false;

    public:
        POINT umP;
        POINT dmP;

        void set_mUp() {
            GetCursorPos(&umP);
        }

        void set_mDown() {
            GetCursorPos(&dmP);
        }

        void checkClick(Buttons* btnArr, Grid* grid, HWND hWnd) {
            if(cleanClick) {
                int dI = btnArr->checkButtons(&dmP);
                int uI = btnArr->checkButtons(&umP);
                if(uI == dI) {
                    switch(uI) {
                        case 0:
                        //find path
                        break;
                        case 1:
                        sClick = true;
                        break;
                        case 2:
                        gClick = true;
                        break;
                        case 3:
                        if(!pClick) pClick = true;
                        else {
                            pClick = false;
                            cleanClick = true;
                        } 
                        break;
                    }
                    cleanClick = false;
                }
            }
            else if(grid->legalClick(&dmP, &umP)) {
                if(sClick){
                    grid->setStart(hWnd);
                    sClick = false;
                }
                if(gClick) {
                    grid->setGoal(hWnd);
                    gClick = false;
                }
                cleanClick = true;
            }
            else if(pClick) {
                grid->tryDraw(hWnd, &dmP, &umP);
            }

            
        }
};