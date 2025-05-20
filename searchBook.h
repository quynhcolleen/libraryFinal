#include <iostream>
#include <vector>
#include "book.h"
#include "borrowableBook.h"
#include "bookManange.h"
using namespace std;

// Tìm sách theo ID, không dùng vector như các hàm khác vì 1 ID sách chỉ kèm 1 sách duy nhất
BorrowableBook *searchByID(vector<BorrowableBook> &books, const string &searchID)
{

    // Chuẩn hóa input vì ID đang để dạng 3 chữ số
    string inputID = searchID;
    while (inputID.length() < 3)
    {
        inputID = "0" + searchID;
    }

    for (BorrowableBook &book : books)
    {
        // Trả về con trỏ đến sách nếu tìm thấy
        if (book.getID() == searchID)
        {
            return &book;
        }
        else
        {
            return nullptr; // Trỏ rỗng nếu không tìm thấy
        }
    }
}
// Search theo keyword, nếu title chứa keyword => kết quả
vector<BorrowableBook *> searchByTitle(vector<BorrowableBook> &books, const string &keyword)
{

    // Chứa các kết quả tìm được trong vector
    vector<BorrowableBook *> results;
    // Chuẩn hóa keyword
    string inputKeyword = normalizeString(keyword);

    for (BorrowableBook &book : books)
    {
        string normalizedTitle = normalizeString(book.getTitle()); // Chuẩn hóa title nếu chưa làm
        if (normalizedTitle.find(inputKeyword) != string::npos)
        {
            results.push_back(&book);
        }
    }
}