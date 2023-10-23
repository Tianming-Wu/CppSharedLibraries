#ifndef ENVMANAGE_H
#define ENVMANAGE_H
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <tchar.h>

using namespace std;

typedef unsigned long ulong;

namespace EnvManage {

LPCTSTR environmentKey = _T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");

void api_RegOpenKey(HKEY &key, int& error, int keyPrivilage = KEY_SET_VALUE | KEY_READ, LPCTSTR envKey = environmentKey);

bool setEnvironmentVariable(string name, string value);
string getEnvironmentVariable(string name);

errno_t s_getEnvironmentVariable(string name, string& value, size_t& len, size_t maxLen = 256);

int getGlobalEnvironmentVariableLarge(string name, string &value);

int setGlobalEnvironmentVariable(string name, string value, ulong keyType);
int getGlobalEnvironmentVariableLarge(string name, string &value);

};

#define ERROR_PRIVILAGE 5
#define ERROR_IOFAILURE 6

#endif