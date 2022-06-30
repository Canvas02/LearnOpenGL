#pragma once

#if defined(_WIN32) && !defined(UNIX)
#	define WINDOWS
#elif !defined(_WIN32) && defined(UNIX)
#	define POSIX
#endif

#if defined(__GNUC__)
#	define GCC
#elif defined(_MSC_VER)
#	define MSVC
#endif // defined(__GNUC__)


#if defined(WINDOWS)
#	define DLL_EXPORT __declspec(dllexport)
#elif defined(POSIX)
#	define DLL_EXPORT __attribute__((visibility("default")))
#endif
 