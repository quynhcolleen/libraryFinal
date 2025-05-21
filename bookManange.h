#include <iostream>
#include <string>
#include "borrowableBook.h"
#include "book.h"
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctype.h>
#pragma once

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

void bookIssue(vector<BorrowableBook> &books)
{
    vector<BorrowableBook *> borrowed;

    string user;
    cout << "Enter borrower's student ID: ";
    cin >> user;
    cin.ignore();
    getline(cin, user);

    int d, m, y;
    cout << "Enter borrow date (dd/mm/yy):\n";
    cin >> d >> m >> y;
    int amount;
    cout << "\nEnter how many book you want to borrow: ";
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
