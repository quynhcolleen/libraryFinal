#include "book.h"      // Định nghĩa lớp sách
#include "bookUtils.h" // Các hàm xử lý sách
#include "ui.h"        // Hàm giao diện người dùng
#include <iostream>
#include <vector>
#include <unistd.h>    // Sử dụng hàm sleep()
#include "cardUtils.h" // Xử lý thẻ thư viện
#include "dataIO.h"    // Đọc/ghi dữ liệu từ/to file
#include "log.h"       // Ghi nhật ký hoạt động

using namespace std;


int main()
{
    printLibraryHeader(); // In tiêu đề thư viện

    vector<BorrowableBook> books = loadBooksFromFiles(); // Tải dữ liệu sách từ file

    int choice;
    choices(); // Hiển thị menu lựa chọn chính

    do
    {
        choice = validateInput(1, 6);
        switch (choice)
        {
        case 1: // Quản lý sách
        {
            system("cls");
            printLibraryHeader();
            cout << endl;
            setColor(31); cout << " MANAGE BOOKS "; setColor(7); cout << "\n\n";
            setColor(240); cout << "[1]"; setColor(7); cout << " Add Book\n";
            setColor(240); cout << "[2]"; setColor(7); cout << " Delete/Reduce Book's Quantity\n";
            setColor(240); cout << "[3]"; setColor(7); cout << " Show Library\n";
            setColor(240); std::cout << "[0]"; setColor(7); std::cout << " Return to main menu\n\n";
            setColor(31); cout << " Enter your choice: "; setColor(7); cout << " ";
            int subChoice = validateInput(0, 3);

            switch (subChoice)
            {
            case 1:
                addBook(books); // Thêm sách
                break;
            case 2:
                removeOrReduceBook(books); // Xóa hoặc giảm số lượng sách
                break;
            case 3:
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(31); cout << " LIBRARY BOOKS LIST "; setColor(7);
                cout << "\n\n";
                drawTable(); // In bảng danh sách sách
                // Sắp xếp sách theo ID tăng dần trước khi hiển thị
                sort(books.begin(), books.end(), [](const BorrowableBook &a, const BorrowableBook &b)
                     { return stoi(a.getID()) < stoi(b.getID()); });
                for (BorrowableBook &b : books)
                    b.display(); // Hiển thị từng cuốn sách
                endTable();      // Kết thúc bảng
                break;
            default:
                system("cls");
                printLibraryHeader();
                break;
            }
            choices(); // Hiển thị lại menu chính
            break;
        }
        case 2: // Tìm kiếm sách
            system("cls");
            printLibraryHeader();
            cout << endl;
            searchMenu(books); // Hiển thị menu tìm kiếm
            system("cls"); // Lựa chọn 0. Return to main menu
            printLibraryHeader();
            choices();
            break;

        case 3: // Mượn/trả sách
        {
            system("cls");
            printLibraryHeader();
            cout << endl;
            setColor(31); cout << " BORROW/RETURN LIBRARY BOOK(S) "; setColor(7); cout << "\n\n";
            setColor(240); cout << "[1]"; setColor(7);cout << " Borrow Book\n";
            setColor(240); cout << "[2]"; setColor(7); cout << " Return Book\n";
            setColor(240); cout << "[0]"; setColor(7); cout << " Return to main menu\n\n";
            setColor(31); cout << " Enter your choice: "; setColor(7); cout << " ";

            int subChoice;
            subChoice = validateInput(0, 2);   
            switch (subChoice)
            {
            case 1:
                bookIssue(books); // Mượn sách
                break;
            case 2:
                bookReturn(books); // Trả sách
                break;
            default:
                system("cls");
                printLibraryHeader();
                break;
            }
            choices();
            break;
        }
        case 4: // Quản lý thẻ thư viện
        {
            system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(31); cout << " MANAGE LIBRARY CARD "; setColor(7); cout << "\n\n";
                setColor(240); cout << "[1]"; setColor(7); cout << " Register a library card\n";
                setColor(240); cout << "[2]"; setColor(7); cout << " Delete a library card\n";
                setColor(240); cout << "[3]"; setColor(7); cout << " Show all cards\n";
                setColor(240); cout << "[0]"; setColor(7); cout << " Return to main menu\n\n";
                setColor(31); cout << "Enter your choice: "; setColor(7); cout << " ";
                int subChoice = validateInput(0, 3);
            switch (subChoice)
            {
            case 1:
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(31); cout << " CREATE A LIBRARY CARD "; setColor(7);
                cout << endl;            
                createCard(); // Đăng ký thẻ thư viện mới
                break;

            case 2:
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(31); cout << " DELETE A LIBRARY CARD "; setColor(7);
                cout << "\n\n";
                deleteCard();
                break;
            case 3:
                system("cls");
                printLibraryHeader();
                cout << endl;
                setColor(31); cout << " ALL LIBRARY CARDS LISTED: "; setColor(7);
                cout << "\n\n";
                showAllCards(); // Hiển thị tất cả thẻ thư viện
                break;
            default:
                system("cls");
                printLibraryHeader();
                break;
            }
            choices();
            break;
        }
        case 5: // Xem nhật ký hoạt động
            system("cls");
            printLibraryHeader();
            cout << endl;
            setColor(31);
            cout << " ACTIVITY LOG "; setColor(7); cout << "\n\n";
            {
                ifstream fin("data/logs.txt");
                string line;
                while (getline(fin, line)) {
                    setLogColor(line);
                    cout << line << "\n"; // In từng dòng nhật ký
                    }
                fin.close();
            }
            choices();
            break;

        case 6: // Thoát chương trình
            cout << endl;
            setColor(12);
            cout << "Exiting...";
            sleep(1);
            setColor(14);
            cout << endl
                 << "2024.2 final by Colleen Hoang.";
            sleep(1.5); // Đợi 2 giây trước khi thoát
            exit(0);
            break;

        default:
            system("cls");
            printLibraryHeader();
            break;
        }
    } while (choice != 6); // Lặp lại cho đến khi chọn thoát

    return 0;
}
