#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "book.h"
#pragma once

using namespace std;

// Hàm lưu danh sách sách hiện tại vào file "books.txt"
void saveBooksToFiles(const vector<BorrowableBook> &books)
{
    ofstream fout("data/books.txt"); // Mở file để ghi đè nội dung cũ

    if (!fout)
    {
        // Nếu mở file thất bại, báo lỗi
        cout << "Couldn't open \"books.txt\".";
    }
    else
    {
        // Ghi từng cuốn sách vào file theo định dạng: ID,Title,Author,Quantity
        for (const auto &book : books)
        {
            fout << book.getID() << ","         // ID sách
                 << book.getTitle() << ","      // Tên sách
                 << book.getAuthor() << ","     // Tác giả
                 << book.getQuantity() << endl; // Số lượng còn lại
        }
    }

    fout.close(); // Đóng file sau khi ghi xong
}

// Hàm đọc danh sách sách từ file "books.txt"
vector<BorrowableBook> loadBooksFromFiles()
{
    vector<BorrowableBook> books;   // Khởi tạo vector chứa các sách
    ifstream fin("data/books.txt"); // Mở file để đọc

    if (!fin)
    {
        // Nếu không mở được file, báo lỗi và trả về danh sách rỗng
        cout << "Couldn't open \"books.txt\".";
        return books;
    }

    string line;
    // Đọc từng dòng trong file
    while (getline(fin, line))
    {
        stringstream ss(line); // Dùng stringstream để tách dữ liệu theo dấu phẩy
        string id, title, author, quantity;

        getline(ss, id, ',');     // Lấy ID sách
        getline(ss, title, ',');  // Lấy tên sách
        getline(ss, author, ','); // Lấy tên tác giả
        getline(ss, quantity);    // Lấy số lượng (cuối dòng)

        int quantityToInt = stoi(quantity); // Chuyển đổi chuỗi thành số nguyên

        // Tạo đối tượng sách và thêm vào vector
        books.emplace_back(id, title, author, quantityToInt);
    }

    fin.close();  // Đóng file sau khi đọc xong
    return books; // Trả về danh sách sách đã đọc được
}
