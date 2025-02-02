#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <limits>
using namespace std;
#include "admin.h"
#include "inventory.h"



//*******************************************************Hashing *******************************************************

// HashFunction: Generates a hash value for a given key
unsigned long long HashTable::HashFunction(const string& key) {
    unsigned long long hash = 0;
    const int multiplier = 31;
    for (char ch : key) {
        hash = (hash * multiplier + tolower(ch)) % TABLE_SIZE;
    }
    return hash;
}


HashTable::~HashTable() {
    CashierA* current = head;
    while (current != nullptr) {
        CashierA* next = current->next;
        delete current;
        current = next;
    }
}


void HashTable::AddCashier(const string& UserName) {
    int index = HashFunction(UserName);


    CashierA* newCashier = new CashierA{UserName, nullptr};


    table[index].push_back(*newCashier);

    if (head == nullptr) {
        head = newCashier;
        tail = newCashier;
    } else {
        tail->next = newCashier;
        tail = newCashier;
    }

    cout << "Cashier \"" << UserName << "\" added successfully.\n";
}


void HashTable::RemoveCashier(const string& UserName) {
    int index = HashFunction(UserName);


    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
        if (it->UserName == UserName) {
            table[index].erase(it);
            break;
        }
    }


    CashierA* current = head;
    CashierA* previous = nullptr;

    while (current != nullptr) {
        if (current->UserName == UserName) {
            if (previous == nullptr) {
                head = current->next;
            } else {
                previous->next = current->next;
            }

            if (current == tail) {
                tail = previous;
            }

            delete current;
            cout << "Cashier \"" << UserName << "\" removed successfully.\n";
            return;
        }
        previous = current;
        current = current->next;
    }

    cout << "Cashier \"" << UserName << "\" not found.\n";
}


void HashTable::ViewCashierList() {
    CashierA* current = head;
    if (current == nullptr) {
        cout << "No cashiers found.\n";
        return;
    }

    cout << "=== Cashiers ===\n";
    int counter = 1;
    while (current != nullptr) {
        cout << "Cashier " << counter << ": " << current->UserName << endl;
        current = current->next;
        counter++;
    }
}
//*********************************************************End of Hashing**************************************


//*********************************************************Admin Logic*****************************************

Admin::Admin() {}
Admin::~Admin() {}


void Admin::DisplayDashboard() {
    cout << "\n=== Admin Dashboard ===\n\n";
    cout << "=== Inventory Items ===\n";
    inventory.ViewInventory();
    cout << "\n";
    CashierTable.ViewCashierList();
}
void Admin::AddItem(const string& name, int id, const string& quantity, const string& category) {
    inventory.AddItem(id, name, category, quantity);
    cout << "Item \"" << name << "\" added to inventory.\n";
}

void Admin::RemoveItem(const string& name) {
    inventory.RemoveItem(name);
    cout << "Item \"" << name << "\" removed successfully.\n";
}


void Admin::ViewInventory() {
    inventory.ViewInventory();
}


void Admin::AddCashier(const string& UserName) {
    CashierTable.AddCashier(UserName);
}


void Admin::RemoveCashier(const string& UserName) {
    CashierTable.RemoveCashier(UserName);
}


//*****************************************************************End of Admin logic******************************************

void AdminMenu() {
    Admin NewAdmin;
    int Choice;
    string UserName, name;
    int id;
    string quantity, category;

    cout << endl;
    cout << "\tWelcome Admin!\n";

    while (true) {
        cout << "\n========= Admin Menu ====================\n";
        cout << "1. Display Dashboard\n";
        cout << "2. Add Cashier\n";
        cout << "3. Remove Cashier\n";
        cout << "4. Add item\n";
        cout << "5. Remove item\n";
        cout << "6. Exit\n";
        cout << "=========================================\n";

        while (!(cin >> Choice) || Choice < 1 || Choice > 6) {
            cout << "Invalid input! Please enter a number between 1 and 6: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover input

        switch (Choice) {
        case 1:
            NewAdmin.DisplayDashboard();
            break;
        case 2:
            cout << "Enter the cashier's username: ";
            getline(cin, UserName);
            NewAdmin.AddCashier(UserName);
            break;
        case 3:
            cout << "Enter the cashier's username to remove: ";
            getline(cin, UserName);
            NewAdmin.RemoveCashier(UserName);
            break;
        case 4:
            cout << "Enter item ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter item name: ";
            getline(cin, name);
            cout << "Enter item category: ";
            getline(cin, category);
            cout << "Enter item quantity: ";
            getline(cin, quantity);
            NewAdmin.AddItem(name, id, quantity, category);
            break;
        case 5:
            cout << "Enter item name to remove: ";
            getline(cin, name);
            NewAdmin.RemoveItem(name);
            break;
        case 6:
            cout << "Exiting the admin menu. Goodbye!\n";
            return;
        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }
    }
}


