#pragma once

#include "base/platform.h"

#if defined(OS_WIN)

#include <string>

std::string narrow(const wchar_t *s);
std::wstring widen(const char *s);
std::string narrow(const std::wstring &s);
std::wstring widen(const std::string &s);

void fix_args(int &argc, char **&argv);
void free_fix_args(int argc, char **argv);

#endif
