#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "date.h"
#include <sstream>

using namespace std;

// Cấu trúc BorrowRec dùng để lưu thông tin mượn sách
struct BorrowRec
{
    string studentID; // Mã số sinh viên mượn sách
    string bookID;    // Mã sách đã mượn
    Date borrowDate;  // Ngày mượn sách

    // Chuyển thông tin mượn sách thành chuỗi để lưu file
    string toString() const
    {
        return studentID + "," + bookID + "," + borrowDate.toString();
    }
};

// Ghi một bản ghi mượn sách vào file "borrowed.txt"
void saveBR(const BorrowRec &record)
{
    ofstream fout("data/borrowed.txt", ios::app); // Mở file ở chế độ ghi nối tiếp (append)
    if (!fout)
    {
        cout << "Couldn't open \"borrowed.txt\"" << endl;
    }
    fout << record.toString() << endl; // Ghi bản ghi vào file
    fout.close();                      // Đóng file
}

// Đọc toàn bộ danh sách mượn sách từ file "borrowed.txt"
vector<BorrowRec> loadBorrowRec()
{
    vector<BorrowRec> records;         // Danh sách các bản ghi mượn sách
    ifstream fin("data/borrowed.txt"); // Mở file để đọc
    string line;

    // Đọc từng dòng trong file
    while (getline(fin, line))
    {
        stringstream ss(line);
        string studentID, bookID, dateStr;

        // Tách các trường dữ liệu theo dấu phẩy
        getline(ss, studentID, ',');
        getline(ss, bookID, ',');
        getline(ss, dateStr); // Phần còn lại là chuỗi ngày (dd/mm/yyyy)

        // Tách ngày, tháng, năm từ chuỗi dateStr
        stringstream dateSS(dateStr);
        string dayStr, monthStr, yearStr;

        getline(dateSS, dayStr, '/');
        getline(dateSS, monthStr, '/');
        getline(dateSS, yearStr, '/');

        int d = stoi(dayStr);
        int m = stoi(monthStr);
        int y = stoi(yearStr);
        if (y < 100)
            y += 2000; // Nếu người dùng chỉ nhập năm dạng ngắn như "25", chuyển thành "2025"

        // Thêm bản ghi mới vào vector
        records.push_back({studentID, bookID, Date(d, m, y)});
    }

    fin.close(); // Đóng file sau khi đọc xong
    return records;
}

// Đếm số lần một sinh viên đã mượn một cuốn sách nhất định
int countBorrowed(const string &studentID, const string &bookID)
{
    int count = 0;
    vector<BorrowRec> records = loadBorrowRec(); // Tải tất cả bản ghi

    // Duyệt qua từng bản ghi để đếm số lần trùng khớp cả studentID và bookID
    for (auto &rec : records)
    {
        if (rec.studentID == studentID && rec.bookID == bookID)
        {
            count++;
        }
    }
    return count; // Trả về số lần mượn
}