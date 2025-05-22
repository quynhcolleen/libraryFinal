#pragma once
#include "book.h"
#include "ui.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctype.h>
using namespace std;

static int bookCounter = 1;

// Chuẩn hóa title, author
string normalizeString(const string &s)
{
    stringstream ss(s);
    string res, word;
    while (ss >> word)
    {
        res += toupper(word[0]);
        for (size_t j = 1; j < word.size(); j++)
        {
            res += tolower(word[j]);
        }
        res += " ";
    }
    if (!res.empty())
        res.pop_back();
    return res;
}

// Thêm sách vào thư viện
void addBook(vector<BorrowableBook> &books)
{
    static int bookCounter = 3;

    string title, author;
    int quantity;

    cin.ignore();
    cout << "Enter title: ";
    getline(cin, title);

    cout << "Enter author: ";
    getline(cin, author);

    cout << "Enter quantity: ";
    cin >> quantity;

    string normTitle = normalizeString(title);
    string normAuthor = normalizeString(author);

    for (BorrowableBook &b : books)
    {
        if (normalizeString(b.getTitle()) == normalizeString(title) &&
            normalizeString(b.getAuthor()) == normalizeString(author))
        {
            b.setQuantity(b.getQuantity() + quantity);
            cout << "\nBook already exists. Quantity updated.\n";
            cout << "ID: " << b.getID() << endl;
            cout << title << " by " << author << endl;
            cout << "Quantity added: " << quantity << ". Total: " << b.getQuantity() << "\n";
            return;
        }
    }

    stringstream idStream;
    idStream << setw(3) << setfill('0') << bookCounter++;
    string newID = idStream.str();

    books.emplace_back(newID, title, author, quantity);

    cout << "\nNew book added:\n";
    cout << "ID: " << newID << endl;
    cout << title << " by " << author << endl;
    cout << "Quantity: " << quantity << ". Total: " << quantity << "\n";
}

// Tìm sách theo ID, không dùng vector như các hàm khác vì 1 ID sách chỉ kèm 1 sách duy nhất
BorrowableBook *searchByID(vector<BorrowableBook> &books, const string &searchID)
{

    // Chuẩn hóa input vì ID đang để dạng 3 chữ số
    string inputID = searchID;
    while (inputID.length() < 3)
    {
        inputID = "0" + inputID;
    }

    for (BorrowableBook &book : books)
    {
        // Trả về con trỏ đến sách nếu tìm thấy
        if (book.getID() == inputID)
        {
            return &book;
        }
    }
    return nullptr; // Trỏ rỗng nếu không tìm thấy
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
    return results;
}

// Search theo tác giả, nếu trùng hoặc đúng 1 phần tên tác giả => kết quả
vector<BorrowableBook *> searchByAuthor(vector<BorrowableBook> &books, const string &keyword)
{

    // Chứa các kết quả tìm được trong vector
    vector<BorrowableBook *> results;
    // Chuẩn hóa keyword
    string inputName = normalizeString(keyword);

    for (BorrowableBook &book : books)
    {
        string normalizedTitle = normalizeString(book.getAuthor()); // Chuẩn hóa title nếu chưa làm
        if (normalizedTitle.find(inputName) != string::npos)
        {
            results.push_back(&book);
        }
    }
    return results;
}

// Search theo quantity, trả về những sách còn cho thuê
vector<BorrowableBook *> searchAvailable(vector<BorrowableBook> &books)
{

    // Chứa các kết quả tìm được trong vector
    vector<BorrowableBook *> results;
    for (BorrowableBook &book : books)
    {
        if (book.getQuantity() > 0) // Sách mà còn thì thêm vào
        {
            results.push_back(&book);
        }
    }
    return results;
}

void searchMenu(vector<BorrowableBook> &books)
{
    int option;
    do
    {
        cout << "\n========== SEARCH BOOK ==========\n";
        cout << "1. Search by ID\n";
        cout << "2. Search by Title\n";
        cout << "3. Search by Author\n";
        cout << "4. View Available Books (quantity > 0)\n";
        cout << "0. Back to main menu\n";
        cout << "=================================\n";
        cout << "Enter your choice: ";
        cin >> option;

        switch (option)
        {
        case 1:
        {
            string id;
            setColor(11);
            cout << "\nEnter book ID: ";
            setColor(7);
            cin >> id;
            BorrowableBook *result = searchByID(books, id);

            if (result != nullptr)
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(39);
                cout << "\nBook with ID " << id << " found:\n";
                setColor(7);
                drawTable();
                result->display();
                endTable();
            }
            else
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(12);
                cout << "\nNo book found with that ID.\n";
                setColor(7);
            }
            break;
        }

        case 2:
        {
            cin.ignore();
            string keyword;
            cout << "\nEnter title: ";
            getline(cin, keyword);
            auto results = searchByTitle(books, keyword);

            if (!results.empty())
            {
                system("cls");
                printLibraryHeader();
                cout << "\nFound " << results.size() << " book(s):\n";
                drawTable();
                for (BorrowableBook *b : results)
                {
                    cout << endl;
                    b->display();
                }
                endTable();
            }
            else
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                cout << "\nNo matching title found.\n";
            }
            break;
        }

        case 3:
        {
            cin.ignore();
            string keyword;
            cout << "\nEnter author name: ";
            getline(cin, keyword);
            auto results = searchByAuthor(books, keyword);

            if (!results.empty())
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                cout << "\nFound " << results.size() << " book(s):\n";
                drawTable();
                for (BorrowableBook *b : results)
                {
                    b->display();
                }
                endTable();
            }
            else
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                cout << "\nNo matching author found.\n";
            }
            break;
        }

        case 4:
        {
            auto results = searchAvailable(books);
            if (!results.empty())
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(31);
                cout << "\nBooks currently available:\n";
                setColor(7);
                drawTable();
                for (BorrowableBook *b : results)
                {
                    b->display();
                }
                endTable();
            }
            else
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(207);
                cout << "\nNo books currently available.\n";
                setColor(7);
            }
            break;
        }

        case 0:
            cout << "\nReturning to main menu...\n";
            break;

        default:
            cout << "\nInvalid choice. Try again.\n";
        }

    } while (option != 0);
}

void bookIssue(vector<BorrowableBook> &books)
{
    vector<BorrowableBook *> borrowed;

    string user;
    cout << "Enter borrower's student ID: ";
    cin >> user;
    getline(cin, user);

    int d, m, y;
    cout << "Enter borrow date (dd/mm/yy): ";
    cin >> d >> m >> y;
    int amount;
    cout << "Enter how many book you want to borrow: ";
    cin >> amount;
    while (amount--)
    {
        cout << "Enter book's ID: ";
        string input;
        cin >> input;
        BorrowableBook *book = searchByID(books, input);
        if (book == nullptr)
        {
            setColor(12);
            cout << "Book not found.\n";
            setColor(7);
            continue; // Không có thì bỏ qua
        }
        else if (book->getQuantity() <= 0)
        {
            setColor(12);
            cout << "This book is currently not available\n";
            setColor(7);
            continue;
        }

        Date date(d, m, y);
        book->borrow(date, user);
        borrowed.push_back(book);
    }
    if (!borrowed.empty())
    {
        setColor(10);
        cout << "\nUser " << user << " borrowed the following book(s):\n";
        setColor(7);

        drawTable();
        for (BorrowableBook *b : borrowed)
        {
            b->display();
        }
        endTable();
    }
    else
    {
        setColor(14);
        cout << "\nNo books were borrowed.\n";
        setColor(7);
    }
}

// void borrowedList(vector<BorrowableBook> &books) {
//     vector<BorrowableBook> results;
//     for (book: Bo)
// }

// void bookReturn(vector<BorrowableBook> &books)
// {
//     vector<BorrowableBook *> returned;

//     string user;
//     cout << "Enter borrower's ID: ";
//     cin.ignore();
//     getline(cin, user);

// }