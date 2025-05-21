#include <iostream>
#include <vector>
#include "book.h"
#include "borrowableBook.h"
#include "bookManange.h"
#include "tableLayout.h"
#include "utils.h"
#pragma once
using namespace std;

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
                setColor(39);
                cout << "\nBook with ID " << id << " found:\n";
                setColor(7);
                drawTable();
                result->display();
                endTable();
            }
            else
            {
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
                cout << "\nFound " << results.size() << " book(s):\n";
                drawTable();
                for (BorrowableBook *b : results)
                {
                    system("cls");
                    printLibraryHeader();
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
