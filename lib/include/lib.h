#ifndef LIBFUNCTIONS_H
#define LIBFUNCTIONS_H
#include <string>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

EXPORT void processString(std::string& str);
EXPORT int calculateSum(const std::string& str);
EXPORT std::string checkString(const std::string& str);

#endif