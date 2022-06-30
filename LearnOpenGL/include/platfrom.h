#pragma once

#if defined(__GNUC__)
#define GCC
#elif defined(_MSC_VER)
#define MSVC
#endif

#if defined(WINDOWS)
#define DLL_EXPORT __declspec(dllexport)
#elif defined(POSIX)
#define DLL_EXPORT __attribute__((visibility("default")))
#endif
