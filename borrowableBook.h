#pragma once
#include <iostream>
#include <string>
#include "book.h"
using namespace std;

struct Date {
    int day;
    int month;
    int year;

    Date(int d = 1, int m = 1, int y = 2000) : day(d), month(m), year(y) {}

    string toString() const {
        return to_string(day) + "/" +
               to_string(month) + "/" +
               to_string(year % 100);
    }

    bool isDefault() const {
        return day == 1 && month == 1 && year == 2000;
    }
};

class BorrowableBook : public Book {
protected:
    Date borrowDate;
    string borrowUser;

public:
    BorrowableBook(string id, const string& t, const string& a, int quantity)
        : Book(id, t, a, quantity), borrowDate(1, 1, 2000), borrowUser("") {}

    void borrow(const Date& date, const string& user) {
        if (quantity > 0) {
            quantity--;
            borrowDate = date;
            borrowUser = user;
            cout << "Borrowed by: " << user << " on " << date.toString() << "." << endl;
        } else {
            cout << "Not available\n";
        }
    }

    void returnBook() override {
        quantity++;
        cout << "Book returned by: " << borrowUser << endl;
        borrowDate = Date(); 
        borrowUser = "";
    }

    void displayBorrowedBookInfo() {
        display();
        if (borrowUser.empty() || borrowDate.isDefault()) {
            cout << "| Available for borrow." << endl;
        } else {
            cout << " | Borrowed by " << borrowUser << " on " << borrowDate.toString() << endl;
        }
    }
};
