#pragma once
#include <winsock.h>
#include <intrin.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "Advapi32.lib")

namespace infocenter {
    class infostructure {
    public:
        bool IPHostSucceed, cpuVendorSucceed, cpuBrandSucceed, usernameSucceed;
        std::string cpuVendor, cpuBrand, cpuID, username, pcIP, pcHostname;
        char _cpuvendor[12], _cpubrand[48], _username[256];
    };

    void Refresh(infostructure &infoStructure) {

        WORD wVersionRequested; WSADATA wsaData; PHOSTENT hostinfo;
        char name[255]; wVersionRequested = MAKEWORD(2, 0);
        if (WSAStartup(wVersionRequested, &wsaData) == 0) {
            if (gethostname(name, sizeof(name)) == 0) {
                infoStructure.IPHostSucceed = true;
                if ((hostinfo = gethostbyname(name)) != NULL) {
                    LPCSTR ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
                    infoStructure.IPHostSucceed = true;
                    infoStructure.pcIP = ip; infoStructure.pcHostname = name;
                }
            }
            WSACleanup();
        }

        INT32 dwBuf[4]; __cpuid(dwBuf, 0);
        memset(infoStructure._cpuvendor, 0, sizeof(char[12]));
        *(INT32*)&infoStructure._cpuvendor[0] = dwBuf[1];
        *(INT32*)&infoStructure._cpuvendor[4] = dwBuf[3];    // edx: 中间四个字符
        *(INT32*)&infoStructure._cpuvendor[8] = dwBuf[2];    // ecx: 最后四个字符
        infoStructure._cpuvendor[12] = '\0';
        infoStructure.cpuVendorSucceed = true;
        infoStructure.cpuVendor = infoStructure._cpuvendor;

        __cpuid(dwBuf, 0x80000000);
        memset(infoStructure._cpubrand, 0, sizeof(char[48]));
        if (! dwBuf[0] < 0x80000004) {
            __cpuid((INT32*)&infoStructure._cpubrand[0], 0x80000002);
            __cpuid((INT32*)&infoStructure._cpubrand[16], 0x80000003);
            __cpuid((INT32*)&infoStructure._cpubrand[32], 0x80000004);
            infoStructure._cpubrand[48] = '\0';
            infoStructure.cpuBrandSucceed = true;
            infoStructure.cpuBrand = infoStructure._cpubrand;
        }
        
        char cpuidbuf[32] = { 0 }; __cpuidex(dwBuf, 1, 1);
        memset(cpuidbuf, 0, 32); sprintf_s(cpuidbuf, 32, "%08X", dwBuf[3]); infoStructure.cpuID += cpuidbuf;
        memset(cpuidbuf, 0, 32); sprintf_s(cpuidbuf, 32, "%08X", dwBuf[0]); infoStructure.cpuID += cpuidbuf;

        DWORD dwSize = 256;
        GetUserNameA(infoStructure._username, &dwSize);
        infoStructure.usernameSucceed = true;
        infoStructure.username = infoStructure._username;
    }


} //namespace infocenter
