#ifndef CASHIER_H
#define CASHIER_H
#include <string>
#include <iostream>
using namespace std;

// _______________________________________Table Management___________________________________________


/*
Needed approach is to provide a simple table array that contains 3 tables for the user, this array connected
to a wait list which is a queue that enqueue customer when the table are FULL.

1. After item found -> showTable function appears
2. An option to add a customer Name assign this customer to a table
3. provide automatic transfer if the table are Full --> enqueue to wait list
4. if a table is released the first item in the queue is dequeued and saved to the table array
*/
struct Table {
    int tableId;
    bool isOccupied;
    string customerName;

    Table(int id) : tableId(id), isOccupied(false), customerName("") {}
};


void displayTables();
bool assignTable(const string& customerName);
void releaseTable(int tableId);
void showWaitlist();


class Queue {
private:
    string* qarray;
    int qfront, qrear, qsize, qcapacity;

public:
    Queue(int capacity);
    ~Queue();

    // Queue operations
    bool enqueue(const string& item);
    bool dequeue();
    string peek();
    bool isEmpty();
    bool isFull();
    void displayQueue();
};



//_________________________________Cashier Management_________________________________________


/*
The cashier class should contains all the menu handling functions, providing the cashier all the
suitable approach that leads them to the end until cashier reaches to the table management

1.after loading the menu.txt Order by menu / order by search
2.Provide the cashier an option to choose an item
3. this selected item goes to the order functions then provide the full order functionalities
4. the search method used is a linear search base as the list of the items are not much
5. using an algorithm to help spilling mistakes & additional issues that may happen while searching
6. updating the item.txt after confirming the order
7. editing menu include add - delete - edit functions to edit the menu then update the menu.txt


*/
class Cashier{

    private:



        struct MenuItems{
            int id;
            string name;
            string category;
            string type;
            double price;
            MenuItems* next;
            MenuItems() : id(0), name(""), category(""), type(""), price(0.0), next(nullptr) {}
        };

        MenuItems* head;
        Queue waitlist;

    public:

        Cashier();
        ~Cashier();

        void loadMenu(const string& menuFile);
        void editMenu();
        void addMenu(int nId, string nName, string nCategory, string nType, double nPrice);
        void deleteMenu(int id);
        void showMenu();
        void clear();
        void saveMenuToFile(const string& filename);
        void orderByMenu();
        void orderBySearch(int nId, string nName, string nCategory, string nType, double nPrice);

        void searchMenu();
        int levenshteinDistance(const string& s1, const string& s2);


};

// ____________________________________ADDITIONAL FUNCTIONS_______________________________________


//______________________________________Update the inventory______________________________________

/*
Providing a simple 2D array to import the relation between ordering and the amount that will be deducted
from the item.txt which concerns the inventory


*/


const int ingredientList[][5] = {
    {1, 1, 4, 1, 0},
    {2, 1, 4, 1, 0},
    {3, 1, 4, 1, 0},
    {4, 1, 4, 1, 0},
    {5, 1, 4, 1, 0},
    {6, 1, 0, 1, 0},
    {7, 1, 0, 1, 0},
    {8, 1, 0, 1, 0},
    {9, 1, 0, 1, 0},
    {10, 1, 3, 1, 0},
    {11, 0, 1, 0, 0},
    {12, 1, 0, 0, 0},
    {13, 0, 0, 1, 0},
};
void updateInventory(int id, int orderedQuantity);
void updateItemQuantity(int itemId, int quantity);

// Cashier Menu
void cashierMenu();

#endif
