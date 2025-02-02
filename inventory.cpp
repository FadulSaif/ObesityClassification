#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "inventory.h"

using namespace std;


/*
    In this part we are providing the needed logic for inventory, Implementing two important data structure concepts,
    Linked list for inventory --> including add, remove and view

    Merge sort to order the items menu based on the quantity (Lowest to highest).

*/

//******************************************************Inventory functions******************************************
Inventory::Inventory() : head(nullptr) {
    LoadFromFile("items.txt");
}


Inventory::~Inventory() {
    Item* current = head;
    while (current != nullptr) {
        Item* next = current->next;
        delete current;
        current = next;
    }
}


std::string Inventory::Trim(const std::string& str) const {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}


void Inventory::AddItem(int id, const std::string& name, const std::string& category, const std::string& quantity) {

    Item* newItem = new Item{id, name, category, quantity, nullptr};

    if (head == nullptr) {
        head = newItem;
    } else {
        Item* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newItem;
    }
}


void Inventory::RemoveItem(const std::string& name) {
    Item* current = head;
    Item* previous = nullptr;
    while (current != nullptr) {
        if (current->name == name) {
            if (previous == nullptr) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            cout << "Item \"" << name << "\" removed successfully.\n";
            return;
        }
        previous = current;
        current = current->next;
    }
    cout << "Item \"" << name << "\" not found.\n";
}

void Inventory::DisplayItems(const std::string& category) const {
    Item* current = head;
    bool categoryHasItems = false;

    if (!category.empty()) {
        cout << "\nCategory: " << category << endl;
        cout << "Name  |  Quantity\n--------------------------" << endl;
    }

    while (current != nullptr) {
        if (category.empty() || current->category == category) {
            categoryHasItems = true;
            cout << current->name << "  |  " << current->quantity << endl;
        }
        current = current->next;
    }

    if (!categoryHasItems && !category.empty()) {
        cout << "No items in this category." << endl;
    }
}


void Inventory::ViewInventory() {
    SortInventory();

    Item* current = head;
    cout << "Sorted Inventory (by Quantity):\n";
    cout << "ID\tName\t\tCategory\tQuantity\n";
    cout << "-------------------------------------------\n";

    while (current != nullptr) {
        cout << "ID: " << current->id << ", Name: " << current->name
             << ", Category: " << current->category << ", Quantity: " << current->quantity << endl;
        current = current->next;
    }
}

bool Inventory::ItemExists(const std::string& name, std::string& existingQuantity) const {
    Item* current = head;
    while (current != nullptr) {
        if (current->name == name) {
            existingQuantity = current->quantity;
            return true;
        }
        current = current->next;
    }
    return false;
}


void Inventory::LoadFromFile(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file " << filename << endl;
        return;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, name, category, quantity;

        getline(ss, idStr, '|');
        getline(ss, name, '|');
        getline(ss, category, '|');
        getline(ss, quantity, '|');

        idStr = Trim(idStr);
        name = Trim(name);
        category = Trim(category);
        quantity = Trim(quantity);

        if (idStr.empty() || !isdigit(idStr[0])) {
            cerr << "Error: Invalid ID in line: " << line << endl;
            continue;
        }

        int id = stoi(idStr);
        AddItem(id, name, category, quantity);
    }
    file.close();
}

void Inventory::SaveToFile(const std::string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file for saving." << endl;
        return;
    }

    file << "ID|Name|Category|Quantity\n";
    Item* current = head;
    while (current != nullptr) {
        file << current->id << "|"
             << current->name << "|"
             << current->category << "|"
             << current->quantity << "\n";
        current = current->next;
    }
    file.close();
}

//***********************************************************Sort Algorithm - Merge sort************************************
// Helper function to merge two sorted linked lists
Item* Inventory::Merge(Item* left, Item* right) {
    if (!left) return right;
    if (!right) return left;

    Item* result = nullptr;


    if (stoi(left->quantity) <= stoi(right->quantity)) {
        result = left;
        result->next = Merge(left->next, right);
    } else {
        result = right;
        result->next = Merge(left, right->next);
    }

    return result;
}

// Helper function to split the linked list into two halves
void Inventory::Split(Item* source, Item** left, Item** right) {
    Item* slow = source;
    Item* fast = source->next;


    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }


    *left = source;
    *right = slow->next;
    slow->next = nullptr;
}

// Recursive merge sort for linked list
Item* Inventory::MergeSort(Item* head) {
    if (!head || !head->next) return head;

    Item* left = nullptr;
    Item* right = nullptr;


    Split(head, &left, &right);


    left = MergeSort(left);
    right = MergeSort(right);


    return Merge(left, right);
}


void Inventory::SortInventory() {
    head = MergeSort(head);
}
//*********************************************End of merge functions***********************************************




//*********************************************Main Menu & utility functions****************************************
// Menu for managing the inventory
void Inventory::InventoryMenu() {

    string command;
    cout << "Welcome to Inventory Management System" << endl;

    while (true) {
        cout << "\nEnter a command ('view' to view all items, 'add' to add a new item, 'remove' to remove an item, 'exit' to quit): ";
        getline(cin, command);

        if (command == "exit") {
            break;
        } else if (command == "view") {
            ViewInventory();
        } else if (command == "add") {
            string name, category, quantity;
            cout << "Enter item name: ";
            getline(cin, name);
            cout << "Enter item category: ";
            getline(cin, category);
            cout << "Enter item quantity: ";
            getline(cin, quantity);

            if (quantity.empty() || !isdigit(quantity[0])) {
                cout << "Invalid quantity. Setting to 0.\n";
                quantity = "0";
            }

            string existingQuantity;
            if (ItemExists(name, existingQuantity)) {
                int oldQuantity = stoi(existingQuantity);
                int newQuantity = stoi(quantity);
                int totalQuantity = oldQuantity + newQuantity;

                Item* current = head;
                while (current != nullptr) {
                    if (current->name == name) {
                        current->quantity = to_string(totalQuantity);
                        break;
                    }
                    current = current->next;
                }
                cout << "Item already exists. Quantity updated!" << endl;
            } else {
                int id = 0;
                if (head != nullptr) {
                    Item* current = head;
                    while (current->next != nullptr) {
                        current = current->next;
                    }
                    id = current->id + 1;
                }
                AddItem(id, name, category, quantity);
                cout << "New item added successfully!" << endl;
            }

            SaveToFile("items.txt");
        } else if (command == "remove") {
            string name;
            cout << "Enter the name of the item to remove: ";
            getline(cin, name);
            RemoveItem(name);
            SaveToFile("items.txt");
        } else {
            cout << "Invalid command. Please try again." << endl;
        }
    }
}


std::string GetCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;

    stringstream date;
    date << year << "/";
    if (month < 10) date << "0";
    date << month << "/";
    if (day < 10) date << "0";
    date << day;

    return date.str();
}
//**********************************************************End************************************************
