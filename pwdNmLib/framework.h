#pragma once

/*#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>*/
#ifdef _WIN32
// Windows-specific code
#include <windows.h>
#elif __linux__
// Linux-specific code
#include <unistd.h>
#endif
