#pragma once

#ifdef PWDNMLIB_EXPORTS
#define PWDNMLIB_API __declspec(dllexport)
#else
#define PWDNMLIB_API __declspec(dllimport)
#endif
#include<string>
extern "C" PWDNMLIB_API std::string genPwd(int nc, bool cap, bool num, bool sChar);
extern "C" PWDNMLIB_API std::string coutMessHdlr();
extern "C" PWDNMLIB_API bool logFace(const char* userName);