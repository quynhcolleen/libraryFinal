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
    printLibraryHeader();

    // Đọc danh sách sách và lưu vào vector
    vector<BorrowableBook> books = loadBooksFromFiles();

    int choice;
    choices(); // Menu chính

    do
    {
        cin >> choice; // Nhập lựa chọn
        switch (choice)
        {
        case 1:                   // Quản lý sách
            system("cls");        // Clear màn hình
            printLibraryHeader(); // In tiêu đề
            cout << "\n1. Add Book\n2. Delete Book\n3. Show Library\nOption: ";
            int subChoice; // Lưu lựa chọn con
            cin >> subChoice;
            switch (subChoice)
            {
            case 1:
                addBook(books); // Thêm sách
                break;
            case 2:
                removeOrReduceBook(books); // Giảm hoặc xóa sách
                break;
            case 3:
                system("cls");
                printLibraryHeader();
                cout << endl;
                drawTable(); // Vẽ bảng sách

                // Sắp xếp sách theo ID tăng dần
                sort(books.begin(), books.end(), [](const BorrowableBook &a, const BorrowableBook &b)
                     { return stoi(a.getID()) < stoi(b.getID()); });
                // Hiển thị sách
                for (BorrowableBook &b : books)
                    b.display();

                endTable();
                break;
            default:
                cout << "Invalid option.\n"; // Trường hợp nhập sai
                break;
            }
            choices(); // Hiển thị lại menu chính
            break;

        case 2: // Tìm kiếm sách
            system("cls");
            printLibraryHeader();
            cout << endl;
            searchMenu(books); // Gọi menu tìm kiếm
            system("cls");     // Lựa chọn 0. Return to main menu
            printLibraryHeader();
            choices();
            break;

        case 3: // Mượn & trả sách
            system("cls");
            printLibraryHeader();
            setColor(240); cout << "1."; setColor(7); cout << " Borrow Book\n";
            setColor(240); cout << "2."; setColor(7); cout << " Return Book\n";
            cout << "\n Option: ";
            cin >> subChoice; // Lưu lựa chọn con
            switch (subChoice)
            {
            case 1:
                bookIssue(books); // Thuê sách
                break;
            case 2:
                bookReturn(books); // Trả sách
                break;
            default:
                cout << "Invalid option.\n";
                break;
            }
            choices();
            break;

        case 4: // Tạo thẻ sinh viên
            system("cls");
            printLibraryHeader();
            setColor(240); cout << "1."; setColor(7); cout << " Register a library card\n";
            setColor(240); cout << "2."; setColor(7); cout << " Show all cards\n";
            cout << "\n Option: ";
            cin >> subChoice;
            switch (subChoice) // Lưu lựa chọn con
            {
            case 1:
                createCard(); // Tạo thẻ
                break;
            case 2:
                system("cls");
                printLibraryHeader();
                showAllCards(); // Hiện danh sách thẻ
                break;
            default:
                cout << "Invalid option.\n";
                break;
            }
            choices();
            break;
        case 5: // Hiện lịch sử truy cập
            system("cls");
            printLibraryHeader();
            cout << "\n--- Activity Log ---\n";
            {
                ifstream fin("data/logs.txt"); // Mở file log để đọc
                string line;
                while (getline(fin, line))
                    cout << line << "\n"; // In log ra màn hình
                fin.close();
            }
            choices();
            break;

        case 6:
            cout << endl;
            setColor(12);
            cout << "Exiting...";
            setColor(14);
            cout << endl
                 << "2024.2 final by Hoang Phi Hung & Le Anh Vuong.";
            sleep(2); // Outro 2 giây để hiện credit
            exit(0);  // Thoát chương trình và trả về 0
            break;
        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (choice != 6);

    return 0;
}
