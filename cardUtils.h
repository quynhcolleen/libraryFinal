#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <string>
#include "date.h"
#include "fstream"
#include "ui.h"
#pragma once
using namespace std;

// Kiểu dữ liệu thẻ thư viện
struct Card
{
    string studentID;
    string fullName;
    Date dob;
    Date issueDate;
};

// Hàm để lấy các ID có sẵn trong 
set<string> getExistingIDs() { // Dùng set với O(1)
    ifstream fin("data/cards.txt");
    set<string> ids;
    if (!fin) return ids;

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string id;
        getline(ss, id, ',');
        ids.insert(id);
    }
    return ids;
}

string generateStudentID(int yearOfBirth, const set<string>& existingIDs) {
    int startYear = yearOfBirth + 18;
    string prefix = to_string(startYear);
    string id;
    srand(time(0));

    do {
        string suffix = to_string(rand() % 10000);
        while (suffix.length() < 4) suffix = "0" + suffix;
        id = prefix + suffix;
    } while (existingIDs.count(id));

    return id;
}
void saveCard(const Card &card)
{
    ofstream fout("data/cards.txt", ios::app);
    if (!fout)
    {
        cout << "Couldn't open \"cards.txt\".\n\n";
        return;
    }
    fout << card.studentID << "," << card.fullName << ","
         << card.dob.toString() << "," << card.issueDate.toString() << "\n";
    fout.close();
}
// Hàm tạo thẻ thư viện cho sinh viên
void createCard()
{
    Card card;
    cout << endl;
    // Nhập họ tên đầy đủ
    setColor(240);
    cout << " Enter full name: "; setColor(7); cout << " ";
    getline(cin, card.fullName);
	stringstream ss(card.fullName);
	string normalizedName = "", word;
	while(ss >> word){
		normalizedName += toupper(word[0]);
		for(int j = 1; j < word.size(); j++){
			normalizedName += tolower(word[j]);
		}
		normalizedName += " ";
	}
	normalizedName.pop_back();
    card.fullName = normalizedName;
    if (card.fullName.empty()) // Kiểm tra rỗng
    {
        system("cls");
        printLibraryHeader();
        cout << endl;
        setColor(207); cout << " Full name cannot be empty. ";
        setColor(7);
        cout << "\n\n";
        return;
    }

    int d, m, y;
    cout << endl;
    setColor(240);
    cout << " Enter date of birth: "; setColor(7); cout << "\n\n";
    setColor(47); cout << " => "; setColor(10); cout << " Day: "; setColor(7);
    if (!(cin >> d)) // Kiểm tra nhập có hợp lệ không (có phải số không)
    {
        cin.clear();             // Xóa cờ lỗi
        cin.ignore(10000, '\n'); // Bỏ qua các ký tự còn lại trong dòng
        setColor(12); cout << "\n Invalid input for day.\n"; setColor(12); 
        return;
    }
    setColor(47); cout << " => "; setColor(10); cout << " Month: "; setColor(7);
    if (!(cin >> m))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        setColor(12); cout << "\n Invalid input for month.\n"; setColor(12); 
        return;
    }
    setColor(47); cout << " => "; setColor(10); cout << " Year: "; setColor(7);
    if (!(cin >> y))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        setColor(12); cout << "\n Invalid input for year.\n"; setColor(12); 
        return;
    }

    // Kiểm tra xem ngày sinh có hợp lệ không
    if (!Date::isValidDate(d, m, y))
    {
        cout << endl;
        setColor(206); cout << " Invalid date. "; setColor(7); cout << endl;
        return;
    }

    card.dob = Date(d, m, y);

    // Tự sinh MSSV
    set<string> existingIDs = getExistingIDs();
    card.studentID = generateStudentID(y, existingIDs);

    // Gán ngày cấp thẻ là ngày hiện tại
    card.issueDate = Date::getCurrentDate();

    // Lưu thẻ thư viện vào hệ thống
    saveCard(card);
    system("cls");
    printLibraryHeader();
    cout << endl;
    setColor(31); cout << " CREATE A LIBRARY CARD "; setColor(7);
    cout << "\n\n";        
    setColor(47); cout << " Card created successfully: "; setColor(7);
    cout << endl;
    cout << "\n+------------+----------------------------+--------------+--------------+\n";
    cout << "| "; setColor(11); cout << "Student ID"; setColor(7); cout << " | ";
    setColor(11); cout << "Full Name"; setColor(7); cout << "                  | "; setColor(11); cout << "D.o.B";
    setColor(7); cout << "        | "; setColor(11); cout << "Issue Date"; setColor(7); cout << "   |\n";
    cout << "+------------+----------------------------+--------------+--------------+\n";
    cout << "| " << setw(10) << left << card.studentID
         << " | " << setw(26) << left << card.fullName
         << " | " << setw(12) << left << card.dob.toString()
         << " | " << setw(12) << left << card.issueDate.toString() << " |\n";
    cout << "+------------+----------------------------+--------------+--------------+\n";

    setColor(7);
}

void deleteCard()
{
    setColor(240);
    cout << " Enter student ID to delete: "; setColor(7); cout << " ";
    string targetID;
    cin >> targetID;

    ifstream fin("data/cards.txt");
    if (!fin)
    {
        cout << "Couldn't open \"cards.txt\".\n";
        return;
    }

    vector<Card> cards;
    vector<Card> updatedCards;
    Card *targetCard = nullptr;

    string line;
    while (getline(fin, line))
    {
        stringstream ss(line);
        string id, name, dobStr, issueStr;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, dobStr, ',');
        getline(ss, issueStr);

        int d, m, y;
        sscanf(dobStr.c_str(), "%d/%d/%d", &d, &m, &y);
        Date dob(d, m, y);
        sscanf(issueStr.c_str(), "%d/%d/%d", &d, &m, &y);
        Date issue(d, m, y);

        Card card = {id, name, dob, issue};

        if (id == targetID)
        {
            targetCard = new Card(card); // Lưu con trỏ đến card muốn xóa
        }
        else
        {
            updatedCards.push_back(card);
        }
    }
    fin.close();

    if (!targetCard)
    {
        cout << endl;
        setColor(207);
        cout << " Card not found. ";
        setColor(7);
        cout << "\n";
        return;
    }

    // Hiển thị thông tin thẻ trước khi xóa
    cout << "\n";
    setColor(240);
    cout << " Card found: ";
    setColor(7);
    cout << "\n\n+------------+----------------------------+--------------+--------------+\n";
    cout << "| "; setColor(11); cout << "Student ID"; setColor(7); cout << " | ";
    setColor(11); cout << "Full Name"; setColor(7); cout << "                  | "; setColor(11); cout << "D.o.B";
    setColor(7); cout << "        | "; setColor(11); cout << "Issue Date"; setColor(7); cout << "   |\n";
    cout << "+------------+----------------------------+--------------+--------------+\n";
    cout << "| " << setw(10) << left << targetCard->studentID
         << " | " << setw(26) << left << targetCard->fullName
         << " | " << setw(12) << left << targetCard->dob.toString()
         << " | " << setw(12) << left << targetCard->issueDate.toString() << " |\n";
    cout << "+------------+----------------------------+--------------+--------------+\n";

    // Xác nhận xóa
    cout << endl;
    setColor(224);
    cout << " ! "; setColor(7);
    cout << " Are you sure you want to DELETE this book? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y')
    {
        system("cls");
        printLibraryHeader();
        cout << endl;        
        setColor(240); cout << " Cancelled. ";
        setColor(7); cout << endl;
        delete targetCard;
        return;
    }

    // Ghi lại danh sách đã cập nhật (đã xóa)
    ofstream fout("data/cards.txt");
    if (!fout)
    {
        cout << "Couldn't open \"cards.txt\".\n";
        delete targetCard;
        return;
    }

    for (const Card &card : updatedCards)
    {
        fout << card.studentID << "," << card.fullName << ","
             << card.dob.toString() << "," << card.issueDate.toString() << "\n";
    }

    fout.close();
    writeLog("DELETE CARD", "Student ID: " + targetID);
    system("cls");
    printLibraryHeader();
    cout << endl;    
    setColor(47); cout << " Card deleted successfully. "; setColor(7);
    cout << endl;
    setColor(7);

    delete targetCard;
}

// In toàn bộ thẻ thư viện ra màn hình
void showAllCards()
{
    ifstream fin("data/cards.txt");
    if (!fin)
    {
        cout << "Couldn't open \"cards.txt\".\n\n";
        return;
    }

    string line;
    vector<vector<string>> rows;

    while (getline(fin, line))
    {
        stringstream ss(line);
        string id, name, dob, issue;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, dob, ',');
        getline(ss, issue);
        rows.push_back({id, name, dob, issue});
    }

    if (rows.empty())
    {
        cout << "\nNo cards found.\n";
        return;
    }
    cout << "+------------+----------------------------+--------------+--------------+\n";
    cout << "| "; setColor(14); cout << "Student ID"; setColor(7); cout << " | ";
    setColor(14); cout << "Full Name"; setColor(7); cout << "                  | ";
    setColor(14); cout << "D.o.B"; setColor(7); cout << "        | ";
    setColor(14); cout << "Issue Date"; setColor(7); cout << "   |\n";
    cout << "+------------+----------------------------+--------------+--------------+\n";

    for (const auto &row : rows)
    {
        cout << "| " << setw(10) << left << row[0]
             << " | " << setw(26) << left << row[1]
             << " | " << setw(12) << left << row[2]
             << " | " << setw(12) << left << row[3] << " |\n";
    }

    cout << "+------------+----------------------------+--------------+--------------+\n";
}
