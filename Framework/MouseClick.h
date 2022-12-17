#include "framework.h"

class mouseClick {
    private:
        static inline bool settingStart = false;
        static inline bool settingGoal = false;
        static inline bool drawingPath = false;
        static inline bool erasingPath = false;

        void btnClicked(const char* name, Buttons *btnarr) {
            if(name == "start") {
                if(settingStart) {settingStart = false;}
                else{settingStart = true;}
                settingGoal = false;
                drawingPath = false;
                erasingPath = false;
                btnarr->stopDraw();
                btnarr->stopErase();
            }
            else if(name == "goal") {
                if(settingGoal) {settingGoal = false;}
                else{settingGoal = true;}
                settingStart = false;
                drawingPath = false;
                erasingPath = false;
                btnarr->stopDraw();
                btnarr->stopErase();
            }
            else if(name == "draw") {
                if(drawingPath) {drawingPath = false;}
                else{drawingPath = true;}
                settingStart = false;
                settingGoal = false;
                erasingPath = false;
                btnarr->toggleDraw();
            }
            else if(name == "erase") {
                if(erasingPath) {erasingPath = false;}
                else{erasingPath = true;}
                settingStart = false;
                settingGoal = false;
                drawingPath = false;
                btnarr->toggleErase();
            }
            else if(name == "clear") {
                settingStart = false;
                settingGoal = false;
                drawingPath = false;
                erasingPath = false;
                btnarr->stopDraw();
                btnarr->stopErase();
            }
        }

    public:
        POINT umP;
        POINT dmP;

        void set_mUp() {
            GetCursorPos(&umP);
        }

        void set_mDown() {
            GetCursorPos(&dmP);
        }

        void checkClick(Buttons *btnArr, Grid* grid, HWND hWnd) {
            int dI = btnArr->checkButtons(&dmP);
            int uI = btnArr->checkButtons(&umP);
            if(uI != -1 && uI == dI) { 
                
                Button btn = btnArr->btnarr[uI];
                if(btn.btnActive){
                    switch(uI) {
                        case 0:
                        grid->Astar();
                        break;
                        case 1:
                        btnClicked("start", btnArr);
                        break;
                        case 2:
                        btnClicked("goal", btnArr);
                        break;
                        case 3:
                        btnClicked("draw", btnArr);
                        break;
                        case 4:
                        btnClicked("erase", btnArr);
                        break;
                        case 5:
                        btnClicked("clear", btnArr);
                        grid->clearGrid(grid);
                        break;
                        case 6:
                        btnClicked("draw", btnArr);
                        break;
                        case 7:
                        btnClicked("erase", btnArr);
                        break;
                    }
                }
                return;
            }

            if(settingStart) {
                grid->legalClick(&dmP, &umP, "Start");
                settingStart = false;
            }

            if(settingGoal) {
                grid->legalClick(&dmP, &umP, "Goal");
                settingGoal = false;
            }

            if(erasingPath) {
                grid->legalClick(&dmP, &umP, "Erase");
            }

            if(drawingPath) {
                grid->legalClick(&dmP, &umP);
            }                
        }
};