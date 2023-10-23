#include "envmanage.h"

bool EnvManage::setEnvironmentVariable(string name, string value) {
    string cmd = name + '=' + value;
    return 1+putenv(cmd.c_str());
}

string EnvManage::getEnvironmentVariable(string name) {
    char* ptc = getenv(name.c_str());
    string value(ptc);
    return value;
}

errno_t EnvManage::s_getEnvironmentVariable(string name, string& value, size_t& len, size_t maxLen) {
    value.resize(maxLen);
    errno_t err = getenv_s(&len, value.data(), maxLen, name.c_str());
    return err;
}

int EnvManage::getGlobalEnvironmentVariableLarge(string name, string &value) {
    HKEY hkResult; int error = ERROR_SUCCESS;
    
    api_RegOpenKey(hkResult, error, KEY_READ);
    if(ERROR_SUCCESS != error) return error; 
    
    DWORD bufferSize = 8192, cbData, dwRet;
    string perfData;
    perfData.reserve(bufferSize);
    cbData = bufferSize;

    dwRet = RegQueryValueExA(hkResult, name.data(), NULL, NULL, (LPBYTE)perfData.data(), &cbData);
    while (dwRet == ERROR_MORE_DATA) {         
        bufferSize += 4096;          
        perfData.resize(bufferSize);
        cbData = bufferSize;                
        dwRet = RegQueryValueExA(hkResult, name.data(), NULL, NULL, (LPBYTE)perfData.data(), &cbData);      
    }       
    
    if (dwRet == ERROR_SUCCESS) {
        value = perfData;
    } else {
        return ERROR_IOFAILURE;
    }
    RegCloseKey(hkResult);
    return error;
}

int EnvManage::setGlobalEnvironmentVariable(string name, string value, ulong keyType = REG_SZ) {
    HKEY hkResult; int error = ERROR_SUCCESS;

    api_RegOpenKey(hkResult, error, KEY_SET_VALUE);
    if(ERROR_SUCCESS != error) return error; 

    long setResult = RegSetValueExA(hkResult, name.data(), 0, keyType, (LPBYTE) value.data(), value.size());

    SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, LPARAM(_T("Environment")), SMTO_ABORTIFHUNG, 5000, (PDWORD_PTR)&error);

    return error;
}

void EnvManage::api_RegOpenKey(HKEY &key, int& error, int keyPrivilage = KEY_SET_VALUE | KEY_READ, LPCTSTR envKey = environmentKey) {
    if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, environmentKey, 0, KEY_SET_VALUE , &key))  
        error = ERROR_PRIVILAGE;
}