#include <iostream>
#include "inventory.h"
#include "Cashier.h"
#include "admin.h"

using namespace std;



//***********************************Introduction********************************************

/* In this main we are providing the directory to three interfacess 1.Admin 2.Cashier 3.Inventory*/


void displayMainMenu() {
    cout << "\t==========================================" << endl;
    cout << "\t\tWelcome to Juice World" << endl;
    cout << "\t==========================================" << endl;
    cout << "\t\t1. Admin" << endl;
    cout << "\t\t2. Cashier" << endl;
    cout << "\t\t3. Inventory Management" << endl;
    cout << "\t\t4. Exit" << endl;
    cout << "\t==========================================" << endl;
    cout << "\t\tEnter your choice: ";
}


int main() {
    int choice;
    Inventory invent;
    while (true) {
        displayMainMenu();

      cin >> choice;
      cin.clear();
      cin.ignore();

        switch (choice) {
            case 1:
                cout << "\nEntering Admin Menu...\n";
                AdminMenu();
                cout << endl << endl;
                break;
            case 2:
                cout << "\nEntering Cashier Menu...\n";
                cashierMenu();
                cout << endl;
                break;
            case 3:
                cout << "\nEntering Inventory Menu...\n";
                invent.InventoryMenu();
                cout << endl;
                break;
            case 4:
                cout << "\nThank you for using the Juice Store System. Goodbye!\n";

                return 0;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    }
}
