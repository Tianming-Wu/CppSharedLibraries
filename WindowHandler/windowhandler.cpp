#include "windowhandler.h"

using namespace windowhandler;

hxWindowClass::hxWindowClass(std::string name) {
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
}