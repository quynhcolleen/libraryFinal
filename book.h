#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include "date.h"
#include "ui.h"

using namespace std;

class Book
{
protected:
    string id;
    string title;
    string author;
    int quantity;

public:
    Book() : id("0"), title(""), author(""), quantity(0) {}
    Book(string id, const string &t, const string &a, int q)
        : id(id), title(t), author(a), quantity(q) {}
    string getID() const
    {
        return id;
    }
    string getTitle() const
    {
        return title;
    }
    string getAuthor() const
    {
        return author;
    }
    int getQuantity() const
    {
        return quantity;
    }

    void setTitle(const string &t)
    {
        title = t;
    }
    void setAuthor(const string &a)
    {
        author = a;
    }
    void setQuantity(int q)
    {
        quantity = q;
    }

    bool isAvailable() const
    {
        return quantity > 0;
    }
    void issue()
    {
        if (isAvailable())
        {
            quantity--;
        }
    }
    virtual void returnBook()
    {
        quantity++;
    }

    void display()
    {
        cout << "| " << setw(4) << left << id;
        cout << " | " << setw(20) << left << author;
        cout << " | " << setw(28) << left << title;
        cout << " | " << setw(8) << right << quantity << " |\n";
    };
};


class BorrowableBook : public Book
{
protected:
    Date borrowDate;
    string borrowUser;

public:
    BorrowableBook(string id, const string &t, const string &a, int quantity)
        : Book(id, t, a, quantity), borrowDate(1, 1, 2000), borrowUser("") {}

    void borrow(const Date &date, const string &user)
    {
        if (quantity > 0)
        {
            quantity--;
            borrowDate = date;
            borrowUser = user;
            cout << endl;
            setColor(47); cout << " => "; setColor(11);
            cout << " Borrowed by: " << user << " on " << date.toString() << "." << "\n\n"; setColor(7);
        }
        else
        {
            cout << "Not available\n";
        }
    }

    void returnBook() override
    {
        quantity++;
        borrowDate = Date();
        borrowUser = "";
    }

    void displayBorrowedBookInfo()
    {
        display();
        if (borrowUser.empty() || borrowDate.isDefault())
        {
            cout << "| Available for borrow." << endl;
        }
        else
        {
            cout << " | Borrowed by " << borrowUser << " on " << borrowDate.toString() << endl;
        }
    }
};
