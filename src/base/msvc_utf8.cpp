#include "msvc_utf8.h"

#if defined(OS_WIN)

#include <windows.h>

std::string narrow(const wchar_t *s)
{
    size_t size = wcslen(s);
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, s, (int)size, NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, s, (int)size, &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring widen(const char *s)
{
    size_t size = strlen(s);
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, s, (int)size, NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, s, (int)size, &wstrTo[0], size_needed);
    return wstrTo;
}

std::string narrow(const std::wstring &str)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring widen(const std::string &str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void fix_args(int &argc, char **&argv)
{
    int wargc;
    wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(), &wargc);
    if (!wargv) {
        argc = 0;
        static char *dummy = 0;
        argv = &dummy;
        return;
    }

    char** newargv = new char*[wargc + 1];
    newargv[wargc] = 0;
    try {
        for (int i = 0; i<wargc; i++) {
            std::string narrowArg = narrow(wargv[i]);
            char* str = new char[narrowArg.size() + 1];
            memcpy(str, narrowArg.data(), narrowArg.size() + 1);
            newargv[i] = str;
        }
        argc = wargc;
        argv = newargv;
    }
    catch (...) {
        LocalFree(wargv);
        throw;
    }
    LocalFree(wargv);
}

void free_fix_args(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
        delete[] argv[i];
    delete[] argv;
}

#endif
