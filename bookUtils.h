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
#include "dataIO.h"
#include "borrowRec.h"
#include "log.h"
using namespace std;

// Chuẩn hóa đầu vào
string normalizeString(const string &s)
{
    stringstream ss(s); // Tạo một stringstream từ chuỗi đầu vào để tách từ
    string res, word;

    // Duyệt từng từ trong chuỗi
    while (ss >> word)
    {
        // Viết hoa chữ cái đầu tiên của từ
        res += toupper(word[0]);

        // Viết thường các chữ cái còn lại của từ
        for (size_t j = 1; j < word.size(); j++)
        {
            res += tolower(word[j]);
        }

        // Thêm một dấu cách sau mỗi từ
        res += " ";
    }

    // Xóa dấu cách dư cuối cùng (nếu có từ được thêm vào)
    if (!res.empty())
        res.pop_back();

    // Trả về chuỗi đã chuẩn hóa
    return res;
}

// Thêm sách vào thư viện
void addBook(vector<BorrowableBook> &books)
{
    // Tìm ID lớn nhất hiện tại để gán ID mới tiếp theo
    int maxID = 0;
    for (const auto &book : books)
    {
        int currentID = stoi(book.getID()); // ID đang kiểu string nên ép kiểu về int
        if (currentID > maxID)
        {
            maxID = currentID;
        }
    }

    // Nhập thông tin sách
    string title, author;
    int quantity;

    cin.ignore();
    cout << "Enter title: ";
    getline(cin, title);

    cout << "Enter author: ";
    getline(cin, author);

    cout << "Enter quantity: ";
    if (!(cin >> quantity) || quantity <= 0) // Kiểm tra hợp lệ input
    {
        cout << "Invalid quantity.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }
    // Chuẩn hóa chuỗi để so sánh
    string normTitle = normalizeString(title);
    string normAuthor = normalizeString(author);

    for (BorrowableBook &b : books) // Kiểm tra sách đã tồn tại chưa
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
        } // Nếu sách đã tồn tại => cập nhật số lượng sách (tăng lên)
    }

    // Tạo ID mới dạng 3 chữ số, ví dụ "001"
    stringstream idStream;
    idStream << setw(3) << setfill('0') << (maxID + 1);
    string newID = idStream.str();

    // Thêm sách mới vào danh sách
    books.emplace_back(newID, title, author, quantity);

    cout << "\nNew book added:\n";
    cout << "ID: " << newID << endl;
    cout << title << " by " << author << endl;
    cout << "Quantity: " << quantity << ". Total: " << quantity << "\n";

    // Ghi log thao tác thêm sách
    writeLog("ADD BOOK", "Title: " + title + ", Author: " + author + ", Quantity: " + to_string(quantity));
    // Lưu dữ liệu vào file
    saveBooksToFiles(books);
}

// Tìm sách theo ID, không dùng vector như các hàm khác vì 1 ID sách chỉ kèm 1 sách duy nhất
BorrowableBook *searchByID(vector<BorrowableBook> &books, const string &searchID)
{

    // Chuẩn hóa input vì ID đang để dạng 3 chữ số
    string inputID = searchID;
    while (inputID.length() < 3)
    {
        inputID = "0" + inputID; // Nếu chưa ID là 001 mà nhập 1 thì sẽ tự động thành 001
    }

    for (BorrowableBook &book : books)
    {
        // Trả về con trỏ đến sách nếu tìm thấy
        if (book.getID() == inputID)
        {
            return &book; // Trả về sách tương ứng
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
    return results; // Trả về vector gồm các sách tương ứng
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
    return results; // Trả về vector gồm các sách tương ứng
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
    return results; // Trả về vector gồm các sách tương ứng
}

// Menu cho phần tìm kiếm
void searchMenu(vector<BorrowableBook> &books)
{
    int option;
    do
    {
        setColor(31);
        cout << " SEARCH BOOK \n\n";
        setColor(7);
        printSearchMenu();
        cout << endl;
        setColor(31);
        cout << "Enter your choice:";
        setColor(7);
        cout << " ";
        cin >> option;

        switch (option)
        {
        case 1: // Tìm theo ID
        {
            system("cls");
            printLibraryHeader();
            string id;
            setColor(31);
            cout << " SEARCH BOOK \n\n";
            setColor(7);
            cout << "\nEnter book ID: ";
            cin >> id;
            BorrowableBook *result = searchByID(books, id);

            if (result != nullptr)
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(31);
                cout << " Book with ID " << id << " found: \n";
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
                setColor(207);
                cout << " No book found with that ID. \n\n";
                setColor(7);
            }
            break;
        }

        case 2: // Tìm theo tiêu đề
        {
            system("cls");
            printLibraryHeader();
            cin.ignore();
            string keyword;
            cout << "\nEnter title: ";
            getline(cin, keyword);
            auto results = searchByTitle(books, keyword);

            if (!results.empty())
            {
                system("cls");
                printLibraryHeader();
                setColor(47);
                cout << "\n Found " << results.size() << " book(s): \n";
                setColor(7);
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
                setColor(207);
                cout << "\n No matching title found.\n";
                setColor(7);
            }
            break;
        }

        case 3: // Tìm theo tác giả
        {
            cin.ignore();
            string keyword;
            setColor(31);
            cout << " SEARCH BOOK \n\n";
            setColor(7);
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
                setColor(207);
                cout << "\nNo matching author found.\n\n";
                setColor(7);
            }
            break;
        }

        case 4: // Tìm theo có sẵn
        {
            auto results = searchAvailable(books);
            if (!results.empty())
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(31);
                cout << " Books currently available: \n\n";
                setColor(7);
                drawTable();
                for (BorrowableBook *b : results)
                {
                    b->display();
                }
                endTable();
                cout << endl;
            }
            else
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(207);
                cout << "\nNo books currently available.\n\n";
                setColor(7);
            }
            break;
        }

        case 0:
            return;
        default:
            setColor(207); cout << "\nInvalid choice. Try again.\n\n"; setColor(7);
        }

    } while (option != 0);
}
// Xóa sách hoặc giảm số lượng sách
void removeOrReduceBook(vector<BorrowableBook> &books)
{
    string id;
    cout << "Enter book ID: ";
    cin >> id;

    BorrowableBook *book = searchByID(books, id);
    if (!book)
    {
        setColor(12);
        cout << "Book not found.\n";
        setColor(7);
        return;
    }

    cout << "\nBook found:\n";
    drawTable();
    book->display();
    endTable();

    int choice;
    cout << "\nWhat do you want to do?\n";
    cout << "1. Delete book completely\n";
    cout << "2. Reduce quantity\n\n";
    cout << "Your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        char confirm;
        cout << "Are you sure you want to DELETE this book? (y/n): ";
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y')
        {
            // tìm vị trí của sách
            for (size_t i = 0; i < books.size(); ++i)
            {
                if (books[i].getID() == id)
                {
                    swap(books[i], books.back());
                    books.pop_back(); // Xóa phần tử trong vector theo cách swap-pop
                    break;
                }
            }
            saveBooksToFiles(books);
            writeLog("DELETE BOOK", "Book ID: " + id);
            setColor(10);
            cout << "Book " << id << " deleted.\n";
            setColor(7);
        }
        else
        {
            cout << "Cancelled.\n";
        }
    }

    else if (choice == 2)
    {
        int reduce;
        cout << "Enter quantity to reduce: ";
        cin >> reduce;

        if (reduce <= 0)
        {
            cout << "Invalid number.\n";
            return;
        }

        if (reduce >= book->getQuantity())
        {
            cout << "You are trying to reduce more than available. Use delete instead.\n";
            return;
        }

        book->setQuantity(book->getQuantity() - reduce);
        saveBooksToFiles(books);
        writeLog("REDUCE QUANTITY", "Book ID: " + id + ", Reduced by: " + to_string(reduce));
        setColor(10);
        cout << "Quantity reduced. New quantity: " << book->getQuantity() << "\n";
        setColor(7);
    }

    else
    {
        cout << "Invalid choice.\n";
    }
}

// Cho thuê sách
void bookIssue(vector<BorrowableBook> &books)
{
    vector<BorrowableBook *> borrowed;

    string user;
    cout << "Enter borrower's student ID: ";
    cin >> user;

    Date borrowDate = Date::getCurrentDate();
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

        book->borrow(borrowDate, user);
        borrowed.push_back(book);
        BorrowRec rec{user, book->getID(), borrowDate};
        saveBR(rec);
        writeLog("BORROW", "Student: " + user + " borrowed book ID: " + book->getID());
    }
    if (!borrowed.empty())
    {
        setColor(10);
        cout << "\nUser " << user << " borrowed the following book(s):\n";
        setColor(7);

        drawTable();
        for (BorrowableBook *b : borrowed)
        {
            int qty = countBorrowed(user, b->getID());
            cout << "| " << setw(4) << left << b->getID()
                 << " | " << setw(20) << left << b->getAuthor()
                 << " | " << setw(28) << left << b->getTitle()
                 << " | " << setw(8) << right << qty << " |\n";
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

// Trả sách
void bookReturn(vector<BorrowableBook> &books)
{ // Thêm & để tránh copy
    string studentID;
    cout << "Enter student's ID: ";
    cin >> studentID;

    // Lọc danh sách đã mượn của sinh viên
    vector<BorrowRec> records = loadBorrowRec();
    vector<BorrowRec> studentRecords;
    vector<BorrowableBook *> borrowedBooks;
    for (auto &rec : records)
    {
        if (rec.studentID == studentID)
        {
            studentRecords.push_back(rec);
            BorrowableBook *book = searchByID(books, rec.bookID);
            if (book != nullptr)
            {
                borrowedBooks.push_back(book);
            }
        }
    }

    // Kiểm tra nếu sinh viên không mượn sách nào
    if (studentRecords.empty())
    {
        setColor(12);
        cout << "This student hasn't borrowed any books.\n";
        setColor(7);
        return;
    }

    // Hiển thị danh sách sách đang mượn
    system("cls");
    printLibraryHeader();
    cout << "\nBooks borrowed by student " << studentID << ":\n";
    drawTable();
    for (auto &rec : studentRecords)
    {
        BorrowableBook *book = searchByID(books, rec.bookID);
        if (book != nullptr)
        {
            int qty = countBorrowed(studentID, book->getID());
            cout << "| " << setw(4) << left << book->getID()
                 << " | " << setw(20) << left << book->getAuthor()
                 << " | " << setw(28) << left << book->getTitle()
                 << " | " << setw(8) << right << qty << " |\n";
        }
    }
    endTable();

    int amount;
    cout << "Enter the amount of book to be returned: ";
    cin >> amount;

    // Xử lý trả từng quyển sách
    for (int i = 0; i < amount; i++)
    {
        string bookID;
        cout << "Enter book ID to return (" << i + 1 << "/" << amount << "): ";
        cin >> bookID;

        // Tìm sách trong danh sách mượn của sinh viên
        bool found = false;
        for (auto it = studentRecords.begin(); it != studentRecords.end(); ++it)
        {
            if (it->bookID == bookID)
            {
                found = true;

                // Trả sách
                BorrowableBook *book = searchByID(books, bookID);
                if (book != nullptr)
                {
                    book->returnBook();
                    cout << "Book " << bookID << " returned successfully." << endl;
                }

                // Xóa khỏi danh sách tạm
                studentRecords.erase(it);

                // Cập nhật record chính
                for (auto it2 = records.begin(); it2 != records.end(); ++it2)
                {
                    if (it2->studentID == studentID && it2->bookID == bookID)
                    {
                        records.erase(it2);
                        break;
                    }
                }
                writeLog("RETURN", "Student: " + studentID + " returned book ID: " + bookID);
                break; // Thoát sau khi xử lý
            }
        }

        if (!found)
        {
            setColor(12);
            cout << "Book ID " << bookID << " not found in your borrowed list.\n";
            setColor(7);
            i--; // Giảm counter để nhập lại
        }
    }

    // Cập nhật file borrowed.txt
    ofstream fout("data/borrowed.txt");
    if (!fout)
    {
        cout << "Couldn't open \"borrowed.txt\"" << endl;
        return;
    }

    for (const auto &rec : records)
    {
        fout << rec.toString() << endl;
    }
    fout.close();

    // Lưu lại trạng thái sách
    saveBooksToFiles(books);

    setColor(10);
    cout << "\nReturn process completed!\n";
    setColor(7);
}
