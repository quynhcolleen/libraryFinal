#include "book.h"
#include "tableLayout.h"
#include "borrowableBook.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include "bookManange.h"

using namespace std;

void choices()
{
    cout << "1. Add Book\n";
    cout << "2. Search Book\n";
    cout << "3. Show library\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

int main()
{
    printLibraryHeader();
    vector<BorrowableBook> books;
    BorrowableBook b1("001", "Ky thuat lap trinh", "Colleen", 1);
    BorrowableBook b2("002", "Truong dien tu", "Le Anh Vuong", 1);

    books.push_back(b1);
    books.push_back(b2);
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
            // searchBook(books);
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
            cout << endl;
            cout << "The program is terminated.";
            cout << endl
                 << "2024.2 final by Hoang Phi Hung & Le Anh Vuong.";
            exit(0);
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
