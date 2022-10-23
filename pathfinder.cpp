#include ".\Framework\framework.h"

struct Node { 
    int val;
    Node *parent;
    Node *left;
    Node *right;
    int color;
};

typedef Node *NodePtr;

class GridSquare {
    public:
        int left;
        int top;
        int right;
        int bottom;
        GridSquare(){}
        GridSquare(int l, int t, int r, int b){
            left = l;
            top = t;
            right = r;
            bottom = b;
        }

};

class RBTree {
    private:
        NodePtr root;
        NodePtr nullNode;

        /*void initializeNULLNode(NodePtr node, NodePtr parent) {
            node->x = -1;
            node->parent = parent;
            node->left = nullptr;
            node->right = nullptr;
            node->color = 0;
        }*/

        int search(NodePtr node, int key, int delta_v) {
            if(node == nullNode){
                 return 0;
            }
            if(node->val < key){
                if(node->right == nullNode) {
                    return node->val + delta_v;
                }
                return search(node->right, key, delta_v);
            }
            if(node->val > key) {
                if(node->left == nullNode) {
                    return node->val;
                }
                return search(node->left, key, delta_v);
            }   
            /*if(key <= node->val) {
                return search(node->left, key, delta_v);
            }
            return search(node->right, key, delta_v);*/
            return 0;
        }

        void FixTree(NodePtr current) {
            NodePtr p;
            while(current->parent->color == 1) {
                if(current->parent == current->parent->parent->right) {
                    p = current->parent->parent->left;
                    if(p->color == 1) {
                        p->color = 0;
                        current->parent->color = 0;
                        current->parent->parent->color = 1;
                        current = current->parent->parent;
                    } else {
                        if(current == current->parent->left) {
                            current = current->parent;
                            rightRotate(current);        
                        }
                        current->parent->color = 0;
                        current->parent->parent->color = 1;
                        leftRotate(current->parent->parent);

                    }
                } else {
                    p = current->parent->parent->right;
                    if(p->color == 1) {
                        p->color = 0;
                        current->parent->color = 0;
                        current->parent->parent->color = 1;
                        current = current->parent->parent;
                    } else {
                        if(current = current->parent->right) {
                            current = current->parent;
                            leftRotate(current);
                        }
                        current->parent->color = 0;
                        current->parent->parent->color = 1;
                        rightRotate(current->parent->parent);
                    }
                }
                if(current == root) break;
            }
            root->color = 0;
        }

        void printHelper(NodePtr root, std::string indent, bool last) {
		// print the tree structure on the screen
	   	if (root != nullNode) {
		   std::cout<<indent;
		   if (last) {
		      std::cout<<"R----";
		      indent += "     ";
		   } else {
		      std::cout<<"L----";
		      indent += "|    ";
		   }
            
           std::string sColor = root->color?"RED":"BLACK";
		   std::cout<<root->val<<"("<<sColor<<")"<<std::endl;
		   printHelper(root->left, indent, false);
		   printHelper(root->right, indent, true);
		}
		
	}

    public:
        RBTree() {
            nullNode = new Node;
            nullNode->color = 0;
            nullNode->left = nullptr;
            nullNode->right = nullptr;
            root = nullNode;
        }

        int searchTree(int key, int delta_v) {
            return search(this->root, key, delta_v);
        }

        void leftRotate(NodePtr p) {
            NodePtr a = p->right;
            p->right = a->left;
            if(a->left != nullNode) {
                a->left->parent = p;
            }
            a->parent = p->parent;
            if(p->parent == nullptr) {
                this->root = a;
            } else if (p == p->parent->left) {
                p->parent->left = a;
            } else {
                p->parent->right = a;
            }
            a->left = p;
            p->parent = a;
        }

        void rightRotate(NodePtr p) {
            NodePtr a = p->left;
            p->left = a->right;
            if(a->right != nullNode) {
                a->right->parent = p;
            }
            a->parent = p->parent;
            if(p->parent == nullptr) {
                this->root = a;
            } else if (p == p->parent->right) {
                p->parent->right = a;
            } else {
                p->parent->left = a;
            }
            a->right = p;
            p->parent = a;
        }

        void rbInsert(int key) {
            NodePtr node = new Node;
            node->parent = nullptr;
            node->val = key;
            node->left = nullNode;
            node->right = nullNode;
            node->color = 1;

            NodePtr a = nullptr;
            NodePtr p =this->root;

            while (p != nullNode) {
                a = p;
                if(node->val < p->val) {
                    p = p->left;
                } else {
                    p = p->right;
                }
            }

            node->parent = a;
            if(a == nullptr) {
                root = node;
            } else if (node->val < a->val) {
                a->left = node;
            } else {
                a->right = node;
            }

            if(node->parent == nullptr) {
                node->color = 0;
                return;
            }

            if(node->parent->parent == nullptr) {
                return;
            }

            FixTree(node);
            
        }

        void prettyPrint() {
            if (root) {
                printHelper(this->root, "", true);
            }
	    }
};


class Grid {
    public:
        //GridSquare gridSquares[1500];
        int delta_x;
        int delta_y;
        Grid(){}
        Grid(RECT rect, Gdiplus::Graphics* gf, Gdiplus::Pen* pen, RBTree* bstX, RBTree* bstY) {
            delta_x = rect.right / 50.0;
            delta_y = (rect.bottom - 200) / 30.0;
            int top = 200;
            int left = delta_x;
            int right = left + delta_x;
            int bottom = delta_y + 200;
            bool once = true;
            GridSquare gs;
            for(int j = 0; j < 50; j++) {
                for(int i = 0; i < 30; i++) {           
                    if(right <= rect.right && bottom <= rect.bottom) {
                        gs = GridSquare(left, top, right, bottom);
                        gf->DrawRectangle(pen, gs.left, gs.top, delta_x, delta_y);
                        if(once) bstY->rbInsert(bottom);  
                        top = bottom;
                        bottom += delta_y;
                    }
                }
                bstX->rbInsert(right);
                left = right;
                right += delta_x;
                top = 200;
                bottom = delta_y + 200;
                once = false;             
            }
        }

        void setStart() {

        }

        void setGoal() {

        }

        void pathDraw() {

        }
};

Grid grid;
LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
bool createWindow();
void draw(HDC hdc);
void drawUI(HDC hdc);
void clickDownhandler(std::pair<int, int>* inItem, RBTree* bstX, RBTree* bstY, Grid* grid);
bool clickUphandler(std::pair<int, int>* inItem, Grid* grid);
int successfulClick(std::pair<int, int>* inItem);
void FindShortestPath();
HWND hWnd;
HINSTANCE hInst;
RECT rect;
RBTree bstX;
RBTree bstY;
std::pair<int, int> inItem;

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    createWindow(); 
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}

bool createWindow(){
    WNDCLASS windowClass;
    ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    hInst = GetModuleHandle(NULL);
    
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = WindowProcessMessages;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInst;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = _T("TestClass");

    if(!RegisterClass(&windowClass)) {
        return false;
    }

    hWnd = CreateWindowEx(dwExStyle, _T("TestClass"), _T("TestClass"), WS_OVERLAPPEDWINDOW, 
    CW_USEDEFAULT, 0, 1200, 1000, NULL, NULL, hInst, NULL);
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
    UpdateWindow(hWnd);
    return true;
}

LRESULT CALLBACK WindowProcessMessages(HWND hWnd, UINT msg, WPARAM param, LPARAM lparam) {
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
        drawUI(hdc);
		EndPaint(hWnd, &ps);
		return 0;
    case WM_LBUTTONDOWN:
        clickDownhandler(&inItem, &bstX, &bstY, &grid);
        std::cout << inItem.first << " " << inItem.second << std::endl;
        return 0;
    case WM_LBUTTONUP:
        if(clickUphandler(&inItem, &grid)) {
            successfulClick(&inItem);
        }
        return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, param, lparam);
	}
}

void drawUI(HDC hdc) {
    Gdiplus::Graphics gf(hdc);  
    GetClientRect(hWnd, &rect);
    Gdiplus::FontFamily fontfam(L"Arial");
    Gdiplus::Font font(&fontfam, 8, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
    Gdiplus::Pen pen(Gdiplus::Color(255, 255, 255));
    Gdiplus::SolidBrush sBrush(Gdiplus::Color(255, 255, 255));
    grid = Grid(rect, &gf, &pen, &bstX, &bstY);
    std::string spb = "Shortest Path Button";
    std::string ssp = "Set Start Point";
    Button findPathbtn(&gf, 20, 20, 120, 20, &spb, &pen, &sBrush, &font);
    Button setStartbtn(&gf, 160, 20, 120, 20, &ssp, &pen, &sBrush, &font);
    //bstX.prettyPrint();
    //bstY.prettyPrint();
}

void clickDownhandler(std::pair<int, int>* inItem, RBTree* bstX, RBTree* bstY, Grid* grid) {
    POINT dmP;
    GetCursorPos(&dmP);
    ScreenToClient(hWnd, &dmP);
    inItem->first = bstX->searchTree(dmP.x, grid->delta_x);
    inItem->second = bstY->searchTree(dmP.y, grid->delta_y);
}

bool clickUphandler(std::pair<int, int>* item, Grid* grid) {
    if(item->first == 0 || item->second == 0) return false;
    POINT umP;
    GetCursorPos(&umP);
    ScreenToClient(hWnd, &umP);
    if(umP.x < item->first && umP.x + grid->delta_x > item->first 
    && umP.y < item->second && umP.y + grid->delta_y > item->second) {
        return true;
    }
    inItem.first = 0;
    inItem.second = 0;
    return false;
}

int successfulClick(std::pair<int, int>* inItem) {
    /*switch(){  
        case 0:
            FindShortestPath();
            return 0;
        default:
            return 0;
    }*/
    
    
    std::cout << "successful click" << std::endl;
    inItem->first = 0;
    inItem->second = 0;
    return 0;
}

void FindShortestPath() {
    std::cout << "fsp btn clicked" << std::endl;
}
