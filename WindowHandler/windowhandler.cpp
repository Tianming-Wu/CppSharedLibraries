#include "windowhandler.h"

using namespace windowhandler;

/// @brief Register a window class.
/// @param name The name to be registered.
hxWindowClass::hxWindowClass(std::string name) {
    memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc 	 = (WNDPROC)&WindowProc;
	wc.hInstance	 = NULL; ///TODO: Check if this works.
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = TEXT("MyClassName");
	wc.hIcon		 = LoadIcon(NULL, TEXT("IDI_APPLICATION"));
	wc.hIconSm		 = LoadIcon(NULL, TEXT("IDI_APPLICATION"));

    RegisterClassEx(&wc);
}

LRESULT CALLBACK hxWindowClass::WindowProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) {
	switch(umsg) {
	case WM_CREATE: {
		
	}
	case WM_SIZE: {

	}

	
	}
	return DefWindowProc(hwnd, umsg, wParam, lParam);
}

void hxWindowClass::Sync() {
	


}