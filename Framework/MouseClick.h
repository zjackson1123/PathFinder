class mouseClick {
    private:
        bool sClick = false;
        bool gClick = false;
        bool pClick = false;
        bool eClick = false;

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
            int dI = btnArr->checkButtons(&dmP);
            int uI = btnArr->checkButtons(&umP);
            if(uI != -1 && uI == dI) { 
                Button btn = btnArr->Buttons[uI];
                if(btn.btnActive){
                    switch(uI) {
                        case 0:
                        grid->Astar(hWnd);
                        break;
                        case 1:
                        sClick = true;
                        break;
                        case 2:
                        gClick = true;
                        break;
                        case 3:
                        pClick = true;
                        eClick = false;
                        btnArr->disableBtn(btn, hWnd);
                        btnArr->enableBtn(btnArr->Buttons[uI + 1], hWnd);
                        break;
                        case 4:
                        pClick = false;           
                        btnArr->disableBtn(btn, hWnd);
                        btnArr->enableBtn(btnArr->Buttons[uI - 1], hWnd);
                        break;
                        case 5:
                        eClick = true;
                        pClick = false;
                        btnArr->disableBtn(btn, hWnd);
                        btnArr->enableBtn(btnArr->Buttons[uI + 1], hWnd);
                        break;
                        case 6:
                        eClick = false;
                        btnArr->disableBtn(btn, hWnd);
                        btnArr->enableBtn(btnArr->Buttons[uI -1], hWnd);
                        break;
                        case 7:
                        RECT rect = {0, 200, grid->width + grid->dX, grid->height + 200};
                        InvalidateRect(hWnd, &rect, true);
                        break;
                    }
                }
                return;
            }

            if(sClick) {
                grid->legalClick(&dmP, &umP, hWnd, "Start");
                sClick = false;
            }

            if(gClick) {
                grid->legalClick(&dmP, &umP, hWnd, "Goal");
                gClick = false;
            }

            if(eClick) {
                grid->legalClick(&dmP, &umP, hWnd, "Erase");
            }

            if(pClick) {
                grid->legalClick(&dmP, &umP, hWnd);
            }                
        }
};