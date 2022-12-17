#include <Windows.h>

int toGo = 0;

RECT snowBalls[10][10];
//for ellipse
int x = 0;
int y = 0;
int a = 50;
int b = 50;

void moveSun() {
    x += 5;
    a += 5;
    if (a > 450) {
        a = 50;
        x = 0;
    }
}


void move() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            snowBalls[i][j].top += 10;
            snowBalls[i][j].bottom += 10;

            if (snowBalls[i][j].top > 400) {
                snowBalls[i][j].top = 0;
                snowBalls[i][j].bottom = snowBalls[i][j].top + 10;
            }
        }
    }
}


void draw(HDC hdc) {
    SaveDC(hdc);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(50, 255, 0));
    Rectangle(hdc, 0, 150, 600, 600);
    SetDCBrushColor(hdc, RGB(100, 100, 250));
    Rectangle(hdc, 0, 0, 600, 150);
    SetDCBrushColor(hdc, RGB(0, 0, 255));

    SetDCBrushColor(hdc, RGB(255, 255, 0));
    //draw sun
    Ellipse(hdc, x, y, a, b);
    SetDCBrushColor(hdc, RGB(90, 0, 255));
    Rectangle(hdc, 0, 0, x, 150);

    //draw home
    //set brown
    SetDCBrushColor(hdc, RGB(150, 75, 0));
    Rectangle(hdc, 100, 100, 200, 200);
    Rectangle(hdc, 150, 50, 150, 100);
    //draw roof
    POINT roof[3];
    roof[0].x = 100;
    roof[0].y = 100;
    roof[1].x = 200;
    roof[1].y = 100;
    roof[2].x = 150;
    roof[2].y = 50;
    //set
    Polygon(hdc, roof, 3);
    //draw door
    //set black
    SetDCBrushColor(hdc, RGB(0, 0, 0));
    Rectangle(hdc, 150, 150, 170, 200);
    //draw window
    //set white
    SetDCBrushColor(hdc, RGB(255, 255, 255));
    Rectangle(hdc, 120, 120, 140, 140);

    SetDCBrushColor(hdc, RGB(0, 0, 255));
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            Rectangle(hdc, snowBalls[i][j].left, snowBalls[i][j].top,
                      snowBalls[i][j].right, snowBalls[i][j].bottom);
        }
    }
    if (toGo) { move(); moveSun(); }
    RestoreDC(hdc, -1);
}

LRESULT CALLBACK mainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            if (wParam == VK_SPACE) {
                toGo = !toGo;
            }
            break;
        default:
            DefWindowProcA(hWnd, uMsg, wParam, lParam);
            break;
    }
}

int main() {
    WNDCLASSEX wcl;
    HWND hWnd;
    MSG msg;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            snowBalls[i][j].left = 20 * (i % 2) + j * 40;
            snowBalls[i][j].top = -300 + 40 * i;
            snowBalls[i][j].right = snowBalls[i][j].left + 10;
            snowBalls[i][j].bottom = snowBalls[i][j].top + 10;

        }
    }

    memset(&wcl, 0, sizeof(wcl));
    wcl.cbSize = sizeof(WNDCLASSEX);
    wcl.lpfnWndProc = mainWndProc;
    wcl.hCursor = LoadCursor(0, IDC_ARROW);
    wcl.hbrBackground = (HBRUSH) (COLOR_3DSHADOW + 1);
    wcl.lpszClassName = "Test";

    RegisterClassEx(&wcl);

    hWnd = CreateWindowEx(0, "Test", "Home", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                          500, 200, 400, 400, 0, 0, 0, 0);

    while (1) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) break;
        } else {
            draw(GetDC(hWnd));
            Sleep(30);
        }
    }
    return (int) msg.wParam;
}