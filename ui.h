#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <limits.h>

#ifdef _WIN32
#include <windows.h> // Thư viện hỗ trợ màu console cho Windows
#endif

using namespace std;

// Thiết lập màu chữ cho console (chỉ hỗ trợ trên Windows)
inline void setColor(int color)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
}

// Hàm in chuỗi ra console với căn giữa theo chiều ngang
inline void printCentered(const string &text, int width = 73)
{
    int padding = (width - static_cast<int>(text.length())) / 2; // Tính số khoảng trắng để căn giữa
    if (padding > 0)
        cout << string(padding, ' ');
    cout << text << endl;
}

// In phần tiêu đề của chương trình thư viện
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

// In tiêu đề bảng thông tin sách
void drawTable()
{
    cout << "+------+----------------------+------------------------------+----------+\n";
    cout << "| " << setw(4) << left; setColor(11); cout << "ID"; setColor(7);
    cout << " | " << setw(20) << left; setColor(11); cout << "Author"; setColor(7);
    cout << " | " << setw(28) << left; setColor(11); cout << "Title"; setColor(7);
    cout << " | " << setw(8) << right; setColor(11); cout << "Quantity"; setColor(7); cout << " |\n";
    cout << "+------+----------------------+------------------------------+----------+\n";
}

// In dòng kết thúc bảng
void endTable()
{
    cout << "+------+----------------------+------------------------------+----------+\n";
}

// Menu chính
void choices()
{
    cout << endl;
    setColor(31); cout << " MAIN MENU "; setColor(7); cout << "\n\n"; 
    setColor(240); cout << "[1]"; setColor(7); cout << " Manage Books\n";
    setColor(240); cout << "[2]"; setColor(7); cout << " Search Books\n";
    setColor(240); cout << "[3]"; setColor(7); cout << " Borrow / Return Books\n";
    setColor(240); cout << "[4]"; setColor(7); cout << " Manage Library Cards\n";
    setColor(240); cout << "[5]"; setColor(7); cout << " View Activity Logs\n";
    setColor(240); cout << "[6]"; setColor(7); cout << " Exit\n\n";
    setColor(31); cout << " Enter your choice: "; setColor(7); cout << " ";
}

// Menu tìm kiếm
void printSearchMenu() {
    setColor(240); cout << "[1]"; setColor(7); cout << " Search by ID\n";
    setColor(240); cout << "[2]"; setColor(7); cout << " Search by Title\n";
    setColor(240); cout << "[3]"; setColor(7); cout << " Search by Author\n";
    setColor(240); cout << "[4]"; setColor(7); cout << " Display available books\n";
    setColor(240); cout << "[0]"; setColor(7); cout << " Return to main menu\n";
}

// Kiểm tra đầu vào
int validateInput(int min = INT_MIN, int max = INT_MAX) {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl;
            setColor(206);
            cout << " ! ";
            setColor(240);
            cout << " Invalid input. Please enter a number: "; setColor(7); cout << " ";
        } else if (input < min || input > max) {
            cout << endl;
            setColor(206);
            cout << " ! ";
            setColor(240);
            cout << " Input must be between " << min << " and " << max << ". Try again: "; setColor(7); cout << " ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return input;
        }
    }
}


inline void setLogColor(const string &line)
{
    if (line.find("ADD BOOK") != string::npos)
        setColor(10); // Xanh lá
    else if (line.find("DELETE BOOK") != string::npos || line.find("DELETE CARD") != string::npos)
        setColor(12); // Đỏ
    else if (line.find("BORROW") != string::npos || line.find("RETURN") != string::npos)
        setColor(11); // Cyan
    else if (line.find("CREATE CARD") != string::npos)
        setColor(14); // Vàng
    else
        setColor(7); // Mặc định
}
