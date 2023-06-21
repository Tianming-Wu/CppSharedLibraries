#pragma once

// There are some code templates below. You can copy them and paste them in your code.
// They will not be compiled.

#include <windows.h>
#include <string>
#include <string.h>

namespace windowhandler {

enum hxWindowFlags {
    HXC_TITLEBAR = 0x00000001,
    HXC_DRAGABLE = 0x00000002,
    HXC_RESIZABLE = 0x00000004,
    HXC_AUTODRAW = 0x00000008,
    HXC_,
    HXC_
};

#define HXC_USEDEFAULT HXC_AUTODRAW


#define HXC_NORMALWINDOW

class hxWindowClass {
private:
    WNDCLASSEX wc;
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);
    RECT wndRect, cliRect, desktopRect;

    void Sync();

public:
    hxWindowClass(std::string name);

    const WNDCLASSEX cclass() const { return wc; }
    WNDCLASSEX* rpclass() { return &wc; }
};

class hxWindowPos {
public:
    hxWindowPos(size_t _x, size_t _y, size_t _h, size_t _w):x(_x),y(_y),h(_h),w(_w) {}
    size_t x, y, h, w;
};

class base_hxBaseWindow {
protected:
    HWND _hwnd = nullptr;
    size_t x, y, height, width;
public:
    virtual base_hxBaseWindow();
    BOOL show(int showid) { return ShowWindow(_hwnd, showid); }
    BOOL close() {  }
    HWND hwnd() const { return _hwnd; }
    void Resize(size_t width, size_t hight);
    void Move(size_t x, size_t y);
};

class base_hxWindow : public base_hxBaseWindow
{
private:

    LRESULT CALLBACK WINAPI WindowProc(HWND hwnd,UINT umsg, WPARAM wParam, LPARAM lParam) {
        switch(umsg) {
        case WM_CREATE: {

        }
        case WM_CLOSE: {

        }
        default: return DefWindowProc(hwnd, umsg, wParam, lParam);
        }
    }

public:
    base_hxWindow(hxWindowClass windowclass, std::string name, hxWindowPos pos);

    

    

};

class base_hxChildWindow : public base_hxBaseWindow
{
private:
    HWND hwnd;

public:
    base_hxChildWindow(hxWindowClass windowclass) {}

};

class hxMainWindow : public base_hxWindow
{


};

} //namespace windowhander






#if false

// These are for you to copy.

// WNDCLASS REGIST
    WNDCLASSEX wc;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc 	 = MainWndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = TEXT("MyClassName");
	wc.hIcon		 = LoadIcon(NULL, TEXT("IDI_APPLICATION"));
	wc.hIconSm		 = LoadIcon(NULL, TEXT("IDI_APPLICATION"));

    RegisterClassEx(&wc);

#endif