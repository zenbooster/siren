#pragma once
#include <sstream>
#include <string>
#include <tchar.h>

namespace std {
#ifdef UNICODE
#define tstringstream wstringstream
#define tstring wstring
#define to_tstring to_wstring
#else
#define tstringstream stringstream
#define tstring string
#define to_tstring to_string
#endif
}