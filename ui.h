#pragma once
#include <iostream>
#include <string>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

inline void setColor(int color)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
}

// Căn lề giữa
inline void printCentered(const string &text, int width = 73)
{
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding > 0)
        cout << string(padding, ' ');
    cout << text << endl;
}

inline void printLibraryHeader()
{
    setColor(12);
    cout << string(73, '=') << endl;
    setColor(14);
    printCentered("FINAL C/C++ - LIBRARY MANAGEMENT SYSTEM");
    setColor(12);
    cout << string(73, '=') << endl;
    setColor(7);
}

void drawTable()
{
    cout << "+------+----------------------+------------------------------+----------+\n";
    cout << "| " << setw(4) << left << "ID"
         << " | " << setw(20) << left << "Author"
         << " | " << setw(28) << left << "Title"
         << " | " << setw(8) << right << "Quantity" << " |\n";
    cout << "+------+----------------------+------------------------------+----------+\n";
}
void endTable()
{
    cout << "+------+----------------------+------------------------------+----------+\n";
}

void choices()
{
    cout << endl;
    setColor(240); cout << "1."; setColor(7); cout << " Manage Books\n";
    setColor(240); cout << "2."; setColor(7); cout << " Search Books\n";
    setColor(240); cout << "3."; setColor(7); cout << " Borrow / Return Books\n";
    setColor(240); cout << "4."; setColor(7); cout << " Manage Library Cards\n";
    setColor(240); cout << "5."; setColor(7); cout << " View Activity Logs\n";
    setColor(240); cout << "6."; setColor(7); cout << " Exit\n";
    cout << "\nEnter your choice: ";
}

void printSearchMenu() {
    setColor(240); cout << "1."; setColor(7); cout << " Search by ID\n";
    setColor(240); cout << "2."; setColor(7); cout << " Search by Title\n";
    setColor(240); cout << "3."; setColor(7); cout << " Search by Author\n";
    setColor(240); cout << "4."; setColor(7); cout << " Display available books\n";
    setColor(240); cout << "0."; setColor(7); cout << " Return to main menu\n";
}