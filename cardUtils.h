#include <iostream>
#include <string>
#include "date.h"
#include "fstream"
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
void saveCard(const Card &card)
{
    ofstream fout("data/cards.txt", ios::app);
    if (!fout)
    {
        cout << "Couldn't open \"cards.txt\".\n";
        return;
    }
    fout << card.studentID << "," << card.fullName << ","
         << card.dob.toString() << "," << card.issueDate.toString() << "\n";
    fout.close();
}

void createCard()
{
    Card card;
    cin.ignore();

    cout << "Enter student ID: ";
    getline(cin, card.studentID);

    cout << "Enter full name: ";
    getline(cin, card.fullName);

    int d, m, y;
    cout << "Enter date of birth:\n";
    cout << "  Day: ";
    cin >> d;
    cout << "  Month: ";
    cin >> m;
    cout << "  Year: ";
    cin >> y;

    card.dob = Date(d, m, y);

    // Tự động lấy ngày hiện tại làm ngày cấp
    card.issueDate = Date::getCurrentDate();

    saveCard(card);
    system("cls");
    cout << "\nLibrary card created successfully!\n";
}

// In toàn bộ thẻ thư viện ra màn hình
void showAllCards()
{
    ifstream fin("data/cards.txt");
    if (!fin)
    {
        cout << "Couldn't open \"cards.txt\".\n";
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
    cout << "| Student ID | Full Name                  | D.o.B        | Issue Date   |\n";
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
