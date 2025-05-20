#include <iostream>
#include <iomanip>

using namespace std;

void drawTable()
{
    cout << "+------+----------------------+------------------------------+----------+\n";
    cout << "| " << setw(4) << left << "ID"
         << " | " << setw(20) << left << "Author"
         << " | " << setw(28) << left << "Title"
         << " | " << setw(8) << right << "Quantity" << " |\n";
    cout << "+------+----------------------+------------------------------+----------+\n";
}

void endTable()
{
    cout << "+------+----------------------+------------------------------+----------+\n";
}