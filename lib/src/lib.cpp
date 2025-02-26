// lib/src/libfunctions.cpp
#include "lib.h"
#include <algorithm>
#include <cctype>

void processString(std::string& str) {
    std::sort(str.begin(), str.end(), std::greater<char>());
    std::string result;
    for (char c : str) {
        if (std::isdigit(c) && (c - '0') % 2 == 0) {
            result += "KB";
        }
        else {
            result += c;
        }
    }
    str = result;
}

int calculateSum(const std::string& str) {
    int sum = 0;
    for (char c : str) {
        if (std::isdigit(c)) {
            sum += c - '0';
        }
    }
    return sum;
}

std::string checkString(const std::string& str) {
    return (str.size() > 2 && (str.size() % 32) == 0) ? "истина" : "ложь";
}