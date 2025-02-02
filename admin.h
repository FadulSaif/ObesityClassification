#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include <string>
#include "inventory.h"

using namespace std;
/*

    The admin needed functionalities is to add new cashiers, edit them and remove them, using linked list and hashtable for this approach

*/


// First declaring the linked list NODE UserName as data & next as a pointer aiming to the next cashier..
struct CashierA {
    string UserName;
    CashierA* next;
};


/*
     For the hashTable we declared a hash class in order to using hashing method and explore it
     Using adding a cashier and remove it and view it provide a clear usage of hashtable
*/

class HashTable {
private:
    static const int TABLE_SIZE = 10;
    vector<CashierA> table[TABLE_SIZE];
    CashierA* head;
    CashierA* tail;
    unsigned long long HashFunction(const string& key);
public:
    HashTable() : head(nullptr), tail(nullptr) {}
    ~HashTable();
    void AddCashier(const string& UserName);
    void RemoveCashier(const string& UserName);
    void ViewCashierList();
};

class Admin {
private:
    HashTable CashierTable;
    Inventory inventory;

public:
    Admin();
    ~Admin();

    void DisplayDashboard();
    void AddItem(const string& name, int id, const string& quantity, const string& category);
    void RemoveItem(const string& name);
    void ViewInventory();
    void AddCashier(const string& UserName);
    void RemoveCashier(const string& UserName);
};
void AdminMenu();

#endif
