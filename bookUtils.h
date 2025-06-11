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
#include <map>
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
        // Viết thường các chữ cái của từ
        for (size_t j = 0; j < word.size(); j++)
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
    system("cls");
    printLibraryHeader();
    cout << endl;
    setColor(31); cout << " ADD BOOK(S) TO THE LIBRARY "; setColor(7); cout << "\n\n";

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

    setColor(240); cout << " Enter title: "; setColor(7); cout << " ";
    getline(cin, title);
    stringstream ss1(title);
	string normalizedTitle = "", wordTitle;
	while(ss1 >> wordTitle){
		normalizedTitle += toupper(wordTitle[0]);
		for(int j = 1; j < wordTitle.size(); j++){
			normalizedTitle += tolower(wordTitle[j]);
		}
		normalizedTitle += " ";
	}
	normalizedTitle.pop_back();
    title = normalizedTitle;
    cout << "\n";
    // Nếu trống thì nhập lại
    if (title.empty())
    {
        setColor(207); cout << "\n Title cannot be empty. \n"; setColor(7);
        return;
    }

    setColor(240); cout << " Enter author: "; setColor(7); cout << " ";
    getline(cin, author);
    stringstream ss(author);
	string normalizedAuthor = "", word;
	while(ss >> word){
		normalizedAuthor += toupper(word[0]);
		for(int j = 1; j < word.size(); j++){
			normalizedAuthor += tolower(word[j]);
		}
		normalizedAuthor += " ";
	}
	normalizedAuthor.pop_back();
    author = normalizedAuthor;
    cout << "\n";

    // Nếu trống thì nhập lại
    if (author.empty())
    {
        setColor(207); cout << "\n Author cannot be empty. \n"; setColor(7);
        return;
    }

    setColor(240); cout << " Enter quantity: "; setColor(7); cout << " ";
    if (!(cin >> quantity) || quantity <= 0)
    {
        cout << "\nInvalid quantity.\n";
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
            system("cls");
            printLibraryHeader();
            cout << endl;
            b.setQuantity(b.getQuantity() + quantity);
            setColor(47); cout << "\n Book already exists. Quantity updated. \n"; setColor(7); cout << endl;
            cout << "ID: "; setColor(11); cout << b.getID(); setColor(7); cout <<" - ";
            setColor(11); cout << "\"" << title << "\""; setColor(7); cout << " by ";
            setColor(11); cout << author; setColor(7); cout << "." << endl;
            cout << "Quantity added: "; setColor(11); cout << quantity; setColor(7); cout << ". Total: "; setColor(11); cout << b.getQuantity() << "\n\n";
            return;
        } // Nếu sách đã tồn tại => cập nhật số lượng sách (tăng lên)
    }

    // Tạo ID mới dạng 3 chữ số, ví dụ "001"
    stringstream idStream;
    idStream << setw(3) << setfill('0') << (maxID + 1);
    string newID = idStream.str();

    // Thêm sách mới vào danh sách
    books.emplace_back(newID, title, author, quantity);
    cout << endl;
    setColor(47); cout << " New book added: "; setColor(7); cout << "\n\n";
    cout << "ID: "; setColor(11); cout << newID; setColor(7); cout <<" - ";
    setColor(11); cout << "\"" << title << "\""; setColor(7); cout << " by ";
    setColor(11); cout << author; setColor(7); cout << "." << endl;
    cout << "Quantity: "; setColor(11); cout << quantity; setColor(7); cout << ". Total: "; setColor(11); cout << quantity << "\n";

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
        string normalizedAuthor = normalizeString(book.getAuthor()); // Chuẩn hóa title nếu chưa làm
        if (normalizedAuthor.find(inputName) != string::npos)
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
        cout << " SEARCH BOOK ";
        setColor(7);
        cout << "\n\n";
        printSearchMenu();
        cout << endl;
        setColor(31);
        cout << " Enter your choice: ";
        setColor(7);
        cout << " ";
        option = validateInput(0, 4);

        switch (option)
        {
        case 1: // Tìm theo ID
        {
            system("cls");
            printLibraryHeader();
            string id;
            setColor(31);
            cout << "\n SEARCH BOOK \n";
            setColor(7); cout << endl;
            setColor(240); cout << " Enter book ID: "; setColor(7); cout << " ";
            cin >> id;
            BorrowableBook *result = searchByID(books, id);

            if (result != nullptr)
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(47);
                cout << " Book with ID " << id << " found:"; setColor(7); cout << "\n\n";
                drawTable();
                result->display();
                endTable();
                cout << endl;
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
            setColor(31);
            cout << "\n SEARCH BOOK \n";
            setColor(7); cout << endl;

            string keyword;
            setColor(240); cout << " Enter book's title: "; setColor(7); cout << " ";
            getline(cin, keyword);
            auto results = searchByTitle(books, keyword);

            if (!results.empty())
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(47);
                cout << " Found " << results.size() << " book(s): ";
                setColor(7);
                cout << "\n\n";
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
                cout << " No matching title found. ";
                setColor(7);
                cout << "\n\n";
            }
            break;
        }

        case 3: // Tìm theo tác giả
        {
            system("cls");
            printLibraryHeader();
            setColor(31);
            cout << "\n SEARCH BOOK \n";
            setColor(7); cout << endl;

            string keyword;
            setColor(240); cout << " Enter book's author: "; setColor(7); cout << " ";
            getline(cin, keyword);
            auto results = searchByAuthor(books, keyword);

            if (!results.empty())
            {
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(47);
                cout << " Found " << results.size() << " book(s): ";
                setColor(7);
                cout << "\n\n";
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
                cout << " No matching author found. ";
                setColor(7);
                cout << "\n\n";
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
                cout << "\n\n";
                setColor(207);
                cout << " No book is currently available.";
                setColor(7);
                cout << "\n\n";
            }
            break;
        }

        case 0:
            return;
        default:
            setColor(207);
            cout << "\nInvalid choice. Try again.\n\n";
            setColor(7);
        }

    } while (option != 0);
}
// Xóa sách hoặc giảm số lượng sách
void removeOrReduceBook(vector<BorrowableBook> &books)
{
    system("cls");
    printLibraryHeader();
    cout << endl;
    setColor(31); cout << " DELETE/REDUCE BOOK'S QUANTITY "; setColor(7); cout << "\n\n";
    string id;
    setColor(240); cout << " Enter book's ID: "; setColor(7); cout << " ";
    cin >> id;
    while (id.length() < 3)
    {
        id = "0" + id; // Nếu chưa ID là 001 mà nhập 1 thì sẽ tự động thành 001
    }
    BorrowableBook *book = searchByID(books, id);
    if (!book)
    {
        setColor(12);
        cout << "Book not found.\n";
        setColor(7);
        return;
    }
    cout << endl;
    setColor(240);
    cout << " Book found: ";
    setColor(7); cout << "\n\n";
    drawTable();
    book->display();
    endTable();

    int choice;
    cout << endl;
    setColor(240); cout << " What do you want to do? "; setColor(7); cout << "\n\n";
    setColor(240); cout << "[1]"; setColor(7); cout << " Delete book completely\n";
    setColor(240); cout << "[2]"; setColor(7); cout << " Reduce quantity\n\n";
    setColor(31); cout << " Enter your choice: "; setColor(7); cout << " ";
    choice = validateInput(1,2);

    if (choice == 1)
    {
        char confirm;
        cout << endl;
        setColor(224);
        cout << " ! "; setColor(7);
        cout << " Are you sure you want to DELETE this book? (y/n): ";
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y')
        {
            bool found = false;
            // Duyệt qua danh sách sách
            for (auto it = books.begin(); it != books.end(); it++) {
            // Nếu ID sách trùng với ID cần xóa
                if (it->getID() == id) {
                    books.erase(it); // Xóa sách khỏi danh sách
                    found = true;    // Đánh dấu đã tìm thấy
                    break;           // Thoát khỏi vòng lặp
                }
            }

            if (found) {
            saveBooksToFiles(books); // Lưu lại danh sách sách sau khi xóa
            writeLog("DELETE BOOK", "Book ID: " + id); // Ghi lại log xóa sách
            system("cls");
            printLibraryHeader();
            cout << endl;
            setColor(47);
            cout << " Book " << id << " deleted. ";
            setColor(7);
            cout << endl;
            }
        }
        else
        {
            system("cls");
            printLibraryHeader();
            cout << endl;        
            setColor(240); cout << " Cancelled. ";
            setColor(7); cout << endl;
        }
    }

    else if (choice == 2)
    {
        int reduce;
        cout << endl;
        setColor(240);
        cout << " Enter quantity to reduce: ";
        setColor(7); cout << " ";
        cin >> reduce;

        if (reduce < 0)
        {
            cout << endl;
            setColor(207);
            cout << " Invalid number. ";
            setColor(7); cout << "\n";
            return;
        }

        if (reduce >= book->getQuantity())
        {
            cout << endl;
            setColor(207);
            cout << " You are trying to reduce more than available. Use delete instead.\n";
            setColor(7);
            return;
        }

        book->setQuantity(book->getQuantity() - reduce);
        saveBooksToFiles(books);
        writeLog("REDUCE QUANTITY", "Book ID: " + id + ", Reduced by: " + to_string(reduce));
        system("cls");
        printLibraryHeader();
        cout << endl;
        setColor(31); cout << " DELETE/REDUCE BOOK'S QUANTITY "; setColor(7); cout << "\n\n";
        setColor(47);
        cout << " Quantity of book with ID " << id << " has been reduced. New quantity: "; setColor(7); cout << " " << book->getQuantity() << "\n";
    }

    else
    {
        cout << "Invalid choice.\n";
    }
}

// Cho thuê sách
void bookIssue(vector<BorrowableBook> &books)
{
    system("cls");
    printLibraryHeader();
    cout << endl;
    setColor(31); cout << " BORROW LIBRARY BOOK(S) "; setColor(7); cout << "\n\n";
    vector<BorrowableBook *> borrowed;
    string user;
    setColor(240); cout << " Enter borrower's student ID: "; setColor(7); cout << " ";
    cin >> user;
    cout << "\n";
    Date borrowDate = Date::getCurrentDate();
    int amount;
    setColor(240); cout << " Enter how many book you want to borrow: "; setColor(7); cout << " ";
    cin >> amount;
    cout << "\n";
    while (amount--)
    {
        setColor(240); cout << " Enter book's ID: "; setColor(7); cout << " ";
        string input;
        cin >> input;
        BorrowableBook *book = searchByID(books, input);
        if (book == nullptr)
        {
            cout << endl;
            setColor(207);
            cout << " Book not found ";
            setColor(7);
            cout << "\n\n";
            continue; // Không có thì bỏ qua
        }
        else if (book->getQuantity() <= 0)
        {
            cout << endl;
            setColor(207);
            cout << " This book is currently not available ";
            setColor(7);
            cout << "\n\n";
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
        system("cls");
        printLibraryHeader();
        cout << endl;
        setColor(31);
        cout << " User " << user << " borrowed the following book(s): ";
        setColor(7);
        cout << "\n\n";
// Gom nhóm các sách mượn để hiển thị một dòng duy nhất mỗi cuốn
    map<string, pair<BorrowableBook*, int>> borrowedMap;

    for (BorrowableBook *b : borrowed)
    {
        borrowedMap[b->getID()].first = b;
        borrowedMap[b->getID()].second++;
    }

    // In bảng
    drawTable();
    for (const auto &entry : borrowedMap)
    {
    BorrowableBook *book = entry.second.first;
    int count = entry.second.second;
    cout << "| " << setw(4) << left << book->getID()
        << " | " << setw(20) << left << book->getAuthor()
        << " | " << setw(28) << left << book->getTitle()
        << " | " << setw(8) << right << count << " |\n";
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
    system("cls");
    printLibraryHeader();
    cout << endl;
    setColor(31); cout << " RETURN LIBRARY BOOK(S) "; setColor(7); cout << "\n\n";
    string studentID;
    setColor(240); cout << " Enter borrower's student ID: "; setColor(7); cout << " ";
    cin >> studentID;

    // Lọc danh sách đã mượn của sinh viên
    vector<BorrowRec> records = loadBorrowRec();
    vector<BorrowRec> studentRecords;
    vector<BorrowableBook *> borrowedBooks;
    vector<BorrowableBook *> returnedBooks;
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
        cout << endl;
        setColor(207);
        cout << " This student hasn't borrowed any books. ";
        setColor(7);
        cout << endl;
        return;
    }

    // Hiển thị danh sách sách đang mượn
    system("cls");
    printLibraryHeader();
    cout << endl;
    setColor(31); cout << " RETURN LIBRARY BOOK(S) "; setColor(7); cout << "\n\n";
    setColor(240); cout << " Books borrowed by student with ID " << studentID << ": "; setColor(7); cout << "\n\n";
    map<string, pair<BorrowableBook*, int>> borrowedMap;

    for (auto &rec : studentRecords)
    {
        BorrowableBook *book = searchByID(books, rec.bookID);
        if (book != nullptr)
        {
            borrowedMap[book->getID()].first = book;
            borrowedMap[book->getID()].second++;
        }
    }

    // In bảng gom nhóm
    drawTable();
    for (const auto &entry : borrowedMap)
    {
        BorrowableBook *book = entry.second.first;
        int count = entry.second.second;
        cout << "| " << setw(4) << left << book->getID()
             << " | " << setw(20) << left << book->getAuthor()
             << " | " << setw(28) << left << book->getTitle()
             << " | " << setw(8) << right << count << " |\n";
    }   
    endTable();

    int amount;
    cout << endl;
    setColor(240); cout << " Enter the amount of book to be returned: "; setColor(7); cout << " ";
    cin >> amount;
    cout << endl;
    // Xử lý trả từng quyển sách
    for (int i = 0; i < amount; i++)
    {
        string bookID;
        setColor(240);
        cout << " Enter book ID to return (" << i + 1 << "/" << amount << "): "; setColor(7); cout << " ";
        cin >> bookID;
        while (bookID.length() < 3)
        {
            bookID = "0" + bookID; // Nếu chưa ID là 001 mà nhập 1 thì sẽ tự động thành 001
        }
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
                    cout << endl;
                    book->returnBook();
                    setColor(47);
                    cout << " => "; setColor(10); cout << " Book " << bookID << " returned successfully."; setColor(7); cout << "\n\n";
                }
                returnedBooks.push_back(book);
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
            cout << endl;
            setColor(12);
            cout << "Book ID " << bookID << " not found in your borrowed list.\n";
            setColor(7);
            cout << endl;
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
    system("cls");
    printLibraryHeader();
    cout << endl;
    setColor(47);
    cout << " Return process completed! "; setColor(7); cout << "\n\n";
    setColor(240); cout << "Student with ID " << studentID << " returned the following book(s): ";
    setColor(7); cout << "\n\n";

    map<string, pair<BorrowableBook*, int>> returnedMap;
    for (auto *book : returnedBooks)
    {
      if (book != nullptr)
      {
        returnedMap[book->getID()].first = book;
        returnedMap[book->getID()].second++;
      }
    }

    // In ra bảng
    drawTable();
    for (const auto &entry : returnedMap)
    {   
        BorrowableBook *book = entry.second.first;
        int count = entry.second.second;
        cout << "| " << setw(4) << left << book->getID()
            << " | " << setw(20) << left << book->getAuthor()
            << " | " << setw(28) << left << book->getTitle()
            << " | " << setw(8) << right << count << " |\n";
    }
    endTable();
    cout << endl;
}
