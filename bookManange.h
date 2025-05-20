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

string normalizeString(const string &s)
{
    stringstream ss(s);
    string res = "", word;
    while (ss >> word)
    {
        res += toupper(word[0]);
        for (int j = 1; j < word.size(); j++)
        {
            res += tolower(word[j]);
        }
        res += " ";
    }
    res.pop_back();
    return s;
}

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
