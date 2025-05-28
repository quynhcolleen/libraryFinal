#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#pragma once
using namespace std;

// Struct để định dạng thứ ngày tháng
struct Date
{
    int day;
    int month;
    int year;

    Date(int d = 1, int m = 1, int y = 2000) : day(d), month(m), year(y) {}

    static Date getCurrentDate() // Lấy ngày giờ hiện tại
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm *local_time = std::localtime(&now_time);

        return Date(
            local_time->tm_mday,       // Ngày (1-31)
            local_time->tm_mon + 1,    // Tháng (1-12, do tm_mon bắt đầu từ 0)
            local_time->tm_year + 1900 // Năm (tm_year tính từ 1900)
        );
    }
    string toString() const // Chuẩn hóa ngày tháng
    {
        return to_string(day) + "/" +
               to_string(month) + "/" +
               to_string(year);
    }

    bool isDefault() const
    {
        return day == 1 && month == 1 && year == 2000;
    }
};
