#pragma once
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h> 
#endif

using namespace std;

// Đổi màu chữ

inline void setColor(int color) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
}

// In chữ ra giữa
inline void printCentered(const string& text, int width = 73) {
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding > 0)
        cout << string(padding, ' ');
    cout << text << endl;
}

// Kẻ lề bảng
inline void printLibraryHeader() {
    setColor(12);
    cout << string(73, '=') << endl;
    setColor(14);
    printCentered("FINAL C/C++ - LIBRARY MANAGEMENT SYSTEM");
    setColor(12);
    cout << string(73, '=') << endl;
    setColor(7);

}
