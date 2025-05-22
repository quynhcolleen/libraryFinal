#include "book.h"
#include "bookUtils.h"
#include "ui.h"
#include <iostream>
#include <vector>
#include <unistd.h>

using namespace std;

void choices()
{
    cout << endl;
    setColor(12); cout << "1"; setColor(7); cout << ". Add Book\n";
    setColor(12); cout << "2"; setColor(7); cout << ". Search Book\n";
    setColor(12); cout << "3"; setColor(7); cout << ". Show library\n";
    setColor(12); cout << "4"; setColor(7); cout << ". Borrow Book\n";
    setColor(12); cout << "5"; setColor(7); cout << ". Exit\n";
    cout << "Enter your choice: ";
}

// Vào class Book làm thêm hàm hủy các constructor

int main()
{
    printLibraryHeader();
    vector<BorrowableBook> books;
    BorrowableBook b1("001", "Ky thuat lap trinh", "Colleen", 1);
    BorrowableBook b2("002", "Truong dien tu", "Le Anh Vuong", 1);
    BorrowableBook b3("003", "Chuan hoa ten", "Xuan Vu", 0);
    BorrowableBook b4("004", "Tach mon dai cuong", "Huy Hoang", 3);

    books.push_back(b1);
    books.push_back(b2);
    books.push_back(b3);
    books.push_back(b4);
    int choice;
    choices();
    do
    {
        cin >> choice;
        switch (choice)
        {
        case 1:
            system("cls");
            printLibraryHeader();
            cout << endl;
            addBook(books);
            cout << endl;
            choices();
            break;
        case 2:
            system("cls");
            printLibraryHeader();
            searchMenu(books);
            break;
        case 3:
            system("cls");
            printLibraryHeader();
            drawTable();
            for (BorrowableBook &b : books)
            {
                b.display();
            }
            endTable();
            cout << endl;
            choices();
            break;
        case 4:
            system("cls");
            printLibraryHeader();
            cout << endl;
            bookIssue(books);
            cout << endl;
            break;
        case 5:
            cout << endl;
            setColor(12);
            cout << "Exiting...";
            setColor(14);
            cout << endl
                 << "2024.2 final by Hoang Phi Hung & Le Anh Vuong.";
            sleep(2);
            exit(0);
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
