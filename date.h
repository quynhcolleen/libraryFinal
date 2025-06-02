#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
using namespace std;

// Struct để định dạng thứ ngày tháng
struct Date
{
    int day;
    int month;
    int year;

    Date(int d = 1, int m = 1, int y = 2000) : day(d), month(m), year(y) {}

    // Kiểm tra năm nhuận
    static bool isLeapYear(int y)
    {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    // Kiểm tra ngày hợp lệ
    static bool isValidDate(int d, int m, int y)
    {
        if (y < 1900 || y > 2100)
            return false;
        if (m < 1 || m > 12)
            return false;

        int daysInMonth[] = {31, 28, 31, 30, 31, 30,
                             31, 31, 30, 31, 30, 31};

        if (m == 2 && isLeapYear(y))
            daysInMonth[1] = 29;

        return d >= 1 && d <= daysInMonth[m - 1];
    }

    // Lấy ngày giờ hiện tại
    static Date getCurrentDate()
    {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm *local_time = localtime(&now_time);

        return Date(
            local_time->tm_mday,
            local_time->tm_mon + 1,
            local_time->tm_year + 1900);
    }

    string toString() const
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
