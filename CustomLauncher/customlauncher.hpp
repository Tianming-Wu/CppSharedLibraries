#pragma once

#include <windows.h>

template<typename _cw>
size_t cl_strlen(const _cw* str) {
#if _cw==char
#define _text(quote) quote
#elif _cw==wchar_t
#define _text(quote) L##quote
#endif
    for(size_t i = 0; i < MAX_PATH; i++) if str[i]==_text('\0') return i;
    return -1;
}

template<typename _cw>
BOOL CustomLauncher(const _cw* File, const _cw* CommandLine, size_t lengthFile = -1, size_t lengthCommandLine = -1) {
    _cw *s1 = (File==NULL)?NULL:(new _cw[(lengthFile==-1)?cl_strlen(File):lengthFile]),
        *s2 = (CommandLine==NULL)?NULL:(new _cw[(lengthCommandLine==-1)?cl_strlen(CommandLine):lengthCommandLine]);
#if _cw==char
#define _CreateProcess CreateProcessA
#define _STARTUPINFO STARTUPINFOA
#elif _cw==wchar_t
#define _CreateProcess CreateProcessW
#define _STARTUPINFO STARTUPINFOW
#endif
    _STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;
    return _CreateProcess(s1,s2,NULL,NULL,FALSE,DETACHED_PROCESS,NULL,NULL,&si,&pi);
}