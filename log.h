#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

// Dùng inline để tăng tốc độ gọi hàm "ngắn"
inline string getTimestamp()
{
    time_t now = time(0);                                 // Lấy thời gian hiện tại (số giây kể từ 1/1/1970)
    tm *ltm = localtime(&now);                            // Chuyển sang giờ địa phương
    char buf[20];                                         // Tạo bộ đệm chứa chuỗi thời gian
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", ltm); // Định dạng thời gian
    return string(buf);                                   // Trả về chuỗi thời gian
}

inline void writeLog(const string &action, const string &detail)
{
    ofstream fout("data/logs.txt", ios::app); // Mở file để ghi thêm (append)
    if (!fout)
    {                                         // Nếu không mở được file
        cout << "Unable to open log file.\n"; // Thông báo lỗi
        return;
    }
    fout << "[" << getTimestamp() << "] "     // Ghi thời gian hiện tại
         << action << ": " << detail << endl; // Ghi nội dung hành động và chi tiết
    fout.close();                             // Đóng file
}
