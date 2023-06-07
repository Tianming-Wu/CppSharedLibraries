#pragma once

// There are some code templates below. You can copy them and paste them in your code.
// They will not be compiled.

#include <windows.h>
#include <string>
#include <string.h>

namespace windowhandler {

class hxWindowClass {
private:
    WNDCLASSEX wc;
public:
    hxWindowClass(std::string name);
    //hxWindowClass(std::string name);

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
    BOOL show(int showid) { return ShowWindow(_hwnd, showid); }
    BOOL close() {  }
    HWND hwnd() const { return _hwnd; }
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