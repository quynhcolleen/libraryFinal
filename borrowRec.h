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
    vector<BorrowRec> records;              // Tạo vector để lưu các bản ghi mượn sách
    ifstream fin("data/borrowed.txt");      // Mở file chứa dữ liệu mượn sách
    string line;

    // Đọc từng dòng trong file
    while (getline(fin, line))
    {
        stringstream ss(line);              // Tạo stringstream để tách các phần tử trong dòng
        string studentID, bookID, dateStr;

        // Tách mã sinh viên, mã sách và chuỗi ngày tháng
        getline(ss, studentID, ',');
        getline(ss, bookID, ',');
        getline(ss, dateStr);               // Lấy phần còn lại là chuỗi ngày mượn (dd/mm/yyyy)

        stringstream dateSS(dateStr);       // Tạo stringstream để tách ngày/tháng/năm
        string dayStr, monthStr, yearStr;

        // Tách ngày, tháng, năm từ chuỗi
        getline(dateSS, dayStr, '/');
        getline(dateSS, monthStr, '/');
        getline(dateSS, yearStr, '/');

        try {
            // Chuyển chuỗi ngày/tháng/năm sang kiểu số nguyên
            int d = stoi(dayStr);
            int m = stoi(monthStr);
            int y = stoi(yearStr);
            if (y < 100) y += 2000; // Nếu năm chỉ có 2 chữ số thì thêm 2000 (ví dụ: 23 -> 2023)

            // Kiểm tra ngày có hợp lệ không
            if (Date::isValidDate(d, m, y)) {
                // Nếu hợp lệ, thêm bản ghi vào vector
                records.push_back({studentID, bookID, Date(d, m, y)});
            } else {
                // Nếu không hợp lệ, in thông báo lỗi
                cout << "Invalid date in borrowed.txt: " << dateStr << endl;
            }

        } catch (const exception &e) {
            // Bắt lỗi khi chuyển đổi string -> int (ví dụ nếu ngày tháng không phải số)
            cout << "Error parsing date in borrowed.txt: " << dateStr << " — " << e.what() << endl;
        }
    }

    fin.close(); // Đóng file
    return records; // Trả về danh sách các bản ghi mượn sách
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