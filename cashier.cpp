#include "cashier.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

//_______________________Cashier Logic_____________________________________________

/*
Implementing the needed logic for the declared functions in the cashier class

*/
Cashier::Cashier() : head(nullptr), waitlist(3) {

}

Cashier::~Cashier() {

    clear();
}


void Cashier::clear() {
    MenuItems* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}


void Cashier::loadMenu(const string& menuFile) {
    ifstream file(menuFile);
    if (!file) {
        cout << "Error: Unable to open file " << menuFile << endl;
        return;
    }

    clear();

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;


        int id;
        string name, category, type;
        double price;

        try {
            getline(ss, token, '|');
            id = stoi(token);

            getline(ss, name, '|');
            getline(ss, category, '|');
            getline(ss, type, '|');

            getline(ss, token, '|');
            price = stod(token);


            addMenu(id, name, category, type, price);

        } catch (exception& e) {
            cout << "Error parsing line: " << line << endl;
            continue;
        }
    }

    file.close();

}


void Cashier::showMenu() {
    cout << endl << endl;
    if (!head) {
        cout << "The Menu is empty! Please load the menu first." << endl;
        return;
    }


    MenuItems* current = head;
    cout << "======================" << endl;
    cout << "        Menu" << endl;
    cout << "======================" << endl<<endl;

    while (current) {
        cout << "ID: " << current->id
             << " | Name: " << current->name
             << " | Category: " << current->category
             << " | Type: " << current->type
             << " | Price: $" << current->price << endl;
        current = current->next;
    }
    cout << "--------------------" << endl;


}

void Cashier::orderByMenu(){
    showMenu();

    int orderId;
    cout << "Please Enter the item ID you want to order (0 to cancel): ";
    cin >> orderId;

    if (orderId == 0) {
        cout << "Order canceled. Returning to menu.\n";
        return;
    }

    /*Here in this step should iterate the linkedList to find The item with
    the needed id first:
    */
    MenuItems* current = head;
    MenuItems* selectedItem = nullptr;

    while(current != nullptr){
        if(current->id == orderId){
            selectedItem = current;
            break;
        }
        current = current->next;
    }

    if(!selectedItem){
        cout << "Invalid ID number, going to main menu.."<<endl;

    }


   string customerName;
    int qty;

    cout << "\nOrder Process:\n";
    cout << "Enter customer name: ";
    cin >> customerName;
    cout << "Enter quantity: ";
    cin >> qty;


    double totalPrice = selectedItem->price * qty;

    // Display the cashout summary
    cout << "\n\nCashout:\n";
    cout << "Customer Name: " << customerName << endl;
    cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
    cout << "Order Details:\n";
    cout << "ID: " << selectedItem->id
         << " | Name: " << selectedItem->name
         << " | Category: " << selectedItem->category
         << " | Type: " << selectedItem->type
         << " | Price per unit: $" << selectedItem->price << endl;

    cout << "\nOrder placed successfully!" << endl;

    // Table management
    cout << "\n=========================================" << endl;
    cout << "          TABLE MANAGEMENT MENU" << endl;
    cout << "=========================================" << endl;
    cout << "Current Table Status:" << endl;
    cout << "-----------------------------------------" << endl;
    displayTables();
    cout << "\n_____________________________________\n";
    int ch;
    cout << "\nWhat would you like to do for the customer?" << endl;
    cout << "=========================================" << endl;
    cout << "1. Assign a Table" << endl;
    cout << "2. Cancel Order" << endl;
    cout << "=========================================" << endl;
    cout << "Please choose an action (1-3): ";
    cin >> ch;
    cout << endl;
    switch(ch){
        case 1:

            if (!assignTable(customerName)) {
                // If no table is available, add the customer to the waitlist
                if (waitlist.enqueue(customerName)) {
                    cout << customerName << " has been added to the waitlist.\n";
                } else {
                    cout << "Waitlist is full. " << customerName << " cannot be added to the waitlist.\n";
                }
            }
            break;
        case 2:
            cout << "Order cancelled!\n";
            break;
        default:
            cout << "Enter a valid choice: "<<endl;
            break;
    }

    updateInventory(orderId, qty);


}


void Cashier::searchMenu() {

    cout << "Enter the name of the item to search: ";
    string query;
    cin >> query;

    if (!head) {
        cout << "Menu is empty! Cannot search." << endl;
        return;
    }

    MenuItems* current = head;
    bool found = false;

    cout << "\nSearch results for: " << query << endl;


    while (current != nullptr) {

        if (current->name == query) {
            found = true;
            cout << "ID: " << current->id << " | " << current->name << " | "
                 << current->category << " | " << current->type << " | $"
                 << fixed << setprecision(2) << current->price << endl;


            cout << "Would you like to order this item? (Y/N): ";
            char choice;
            cin >> choice;

            if (choice == 'Y' || choice == 'y') {

                orderBySearch(current->id, current->name, current->category, current->type, current->price);
                return;
            } else {
                cout << "Returning to the menu.\n";
                return;
            }
        }

        else if (levenshteinDistance(current->name, query) <= 2) {
            found = true;
            cout << "(Approximate Match) ID: " << current->id << " | " << current->name
                 << " | " << current->category << " | " << current->type
                 << " | $" << fixed << setprecision(2) << current->price << endl;
            cout << "Levenshtein Distance: " << levenshteinDistance(current->name, query) << endl;

            cout << "Would you like to order this item? (Y/N): ";
            char choice;
            cin >> choice;

            if (choice == 'Y' || choice == 'y') {

                orderBySearch(current->id, current->name, current->category, current->type, current->price);
                return;
            } else {
                cout << "Returning to the menu.\n";
                return;
            }
        }

        current = current->next;
    }

    if (!found) {
        cout << "No matching items found." << endl;
    }
}

int Cashier::levenshteinDistance(const string& s1, const string& s2) {

    string str1 = s1;
    string str2 = s2;


    for (int i = 0; i < str1.size(); i++) {
        if (str1[i] >= 'A' && str1[i] <= 'Z') {
            str1[i] = str1[i] + ('a' - 'A');
        }
    }


    for (int i = 0; i < str2.size(); i++) {
        if (str2[i] >= 'A' && str2[i] <= 'Z') {
            str2[i] = str2[i] + ('a' - 'A');
        }
    }

    int len1 = str1.size();
    int len2 = str2.size();


    int** dp = new int*[len1 + 1];
    for (int i = 0; i <= len1; ++i) {
        dp[i] = new int[len2 + 1];
    }


    for (int i = 0; i <= len1; ++i) dp[i][0] = i;
    for (int j = 0; j <= len2; ++j) dp[0][j] = j;

    // Step 3: Fill the DP table
    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            dp[i][j] = min(dp[i - 1][j] + 1,
                           min(dp[i][j - 1] + 1,
                               dp[i - 1][j - 1] + cost));
        }
    }


    int result = dp[len1][len2];


    for (int i = 0; i <= len1; ++i) {
        delete[] dp[i];
    }
    delete[] dp;

    return result;
}



//___________________________________________________________________________

void Cashier::orderBySearch(int nId, string nName, string nCategory, string nType, double nPrice){
    showMenu();


   cout << "\nItem Found!\n";
    cout << "ID: " << nId
         << " | Name: " << nName
         << " | Category: " << nCategory
         << " | Type: " << nType
         << " | Price per unit: $" << fixed << setprecision(2) << nPrice << endl;

    cout << "Would you like to place an order for this item? (Y/N): ";
    char choice;
    cin >> choice;

    if (choice == 'N' || choice == 'n') {
        cout << "Order canceled. Returning to menu.\n";
        return;
    } else if (choice != 'Y' && choice != 'y') {
        cout << "Invalid input. Returning to menu.\n";
        return;
    }

    string customerName;
    int qty;

    cout << "\nOrder Process:\n";
    cout << "Enter customer name: ";
    cin >> customerName;

    cout << "\nEnter quantity: ";
    cin >> qty;

    double totalPrice = nPrice * qty;


    cout << "\n\nCashout:\n";
    cout << "Customer Name: " << customerName << endl;
    cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
    cout << "Order Details:\n";
    cout << "ID: " << nId
         << " | Name: " << nName
         << " | Category: " << nCategory
         << " | Type: " << nType
         << " | Price per unit: $" << fixed << setprecision(2) << nPrice << endl;

    cout << "\nOrder placed successfully!" << endl;


    cout << "This is the tables status so far: \n";
    cout << "_____________________________________";
    displayTables();
    cout << "\n_____________________________________\n";
    int ch;
    cout << "Choose what do you want to do for the customer: \n";
    cout << "1.Assign a table"<< endl;
    cout << "2.Cancel Order " << endl;
    cout << "choice: ";
    cin >> ch;
    cout << endl;
    switch(ch){
        case 1:

            if (!assignTable(customerName)) {

                if (waitlist.enqueue(customerName)) {
                    cout << customerName << " has been added to the waitlist.\n";
                } else {
                    cout << "Waitlist is full. " << customerName << " cannot be added to the waitlist.\n";
                }
            }
            break;
        case 2:
            cout << "Order cancelled!\n";
            break;
        default:
            cout << "Enter a valid choice: "<<endl;
            break;
    }

    updateInventory(nId, qty);

}




// Providing the 4.Edit function logic


void Cashier::addMenu(int nId, string nName, string nCategory, string nType, double nPrice) {
    MenuItems* newItem = new MenuItems();
    newItem->id = nId;
    newItem->name = nName;
    newItem->category = nCategory;
    newItem->type = nType;
    newItem->price = nPrice;
    newItem->next = nullptr;

    if (!head) {
        head = newItem;
    } else {
        MenuItems* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newItem;
    }
    cout << "Item added successfully!" << endl;
}




void Cashier::editMenu() {

    cout << "Current Menu:" << endl;
    showMenu();


    int id;
    cout << "\nEnter the ID of the item you want to edit: ";
    cin >> id;


    if (!head) {
        cout << "Menu is empty. Please load the menu first!" << endl;
        return;
    }


    MenuItems* current = head;
    bool found = false;

    while (current != nullptr) {
        if (current->id == id) {
            found = true;

            cout << "Item Found! Current Details:" << endl;
            cout << "ID: " << current->id
                 << " | Name: " << current->name
                 << " | Category: " << current->category
                 << " | Type: " << current->type
                 << " | Price: $" << current->price << endl;


            int choice;
            do {
                cout << "\nChoose what you want to edit: " << endl;
                cout << "1. Name\n2. Category\n3. Type\n4. Price\n5. Exit" << endl;
                cout << "Choice: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                        cout << "Enter new name: ";
                        cin.ignore();
                        getline(cin, current->name);
                        break;

                    case 2:
                        cout << "Enter new category: ";
                        cin.ignore();
                        getline(cin, current->category);
                        break;

                    case 3:
                        cout << "Enter new type: ";
                        cin.ignore();
                        getline(cin, current->type);
                        break;

                    case 4:
                        cout << "Enter new price: ";
                        cin >> current->price;
                        break;

                    case 5:
                        cout << "Exiting edit menu..." << endl;
                        break;

                    default:
                        cout << "Invalid choice! Try again." << endl;
                }


                cout << "Updated Details:" << endl;
                cout << "ID: " << current->id
                     << " | Name: " << current->name
                     << " | Category: " << current->category
                     << " | Type: " << current->type
                     << " | Price: $" << current->price << endl;

            } while (choice != 5);


            saveMenuToFile("menu.txt");
            cout << "Changes saved successfully!" << endl;
            break;
        }


        current = current->next;
    }


    if (!found) {
        cout << "Item with ID " << id << " not found in the menu!" << endl;
    }
}

void Cashier::deleteMenu(int id) {

    if (!head) {
        cout << "Menu is empty! Cannot delete." << endl;
        return;
    }


    if (head->id == id) {
        MenuItems* temp = head;
        head = head->next;
        delete temp;
        cout << "Item with ID " << id << " deleted successfully!" << endl;


        saveMenuToFile("menu.txt");
        return;
    }


    MenuItems* current = head;
    MenuItems* prev = nullptr;

    while (current != nullptr && current->id != id) {
        prev = current;
        current = current->next;
    }


    if (!current) {
        cout << "Item with ID " << id << " not found!" << endl;
        return;
    }


    prev->next = current->next;
    delete current;

    cout << "Item with ID " << id << " deleted successfully!" << endl;


    saveMenuToFile("menu.txt");
}

void Cashier::saveMenuToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    MenuItems* current = head;
    while (current != nullptr) {
        file << current->id << "|"
             << current->name << "|"
             << current->category << "|"
             << current->type << "|"
             << current->price << endl;
        current = current->next;
    }

    file.close();
    cout << "Menu successfully saved to " << filename << endl;
}





//___________________________________End of Menu logic_____________________________________________


//________________________________________Logic to update inventory______________________________

void updateItemQuantity(int itemId, int quantity) {

    fstream file("items.txt", ios::in | ios::out);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file!" << endl;
        return;
    }

    string line;
    bool itemFound = false;


    stringstream modifiedContents;


    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, name, category, qtyStr;
        int id, qty;


        getline(ss, idStr, '|');
        getline(ss, name, '|');
        getline(ss, category, '|');
        getline(ss, qtyStr, '|');


        id = stoi(idStr);
        qty = stoi(qtyStr);

        if (id == itemId) {
            qty -= quantity;
            itemFound = true;
        }


        modifiedContents << id << "|" << name << "|" << category << "|" << qty << endl;
    }

    if (itemFound) {
        file.close();
        file.open("items.txt", ios::out | ios::trunc);
        file << modifiedContents.str();
        cout << "Inventory updated successfully!" << endl;
    } else {
        cout << "Item ID not found in the inventory!" << endl;
    }

    file.close();
}

void updateInventory(int id, int orderedQuantity) {

    for (int i = 0; i < 13; i++) {
        if (ingredientList[i][0] == id) {
            if (ingredientList[i][1] > 0) {
                updateItemQuantity(12, orderedQuantity);
            }
            if (ingredientList[i][2] > 0) {
                updateItemQuantity(11, orderedQuantity);
            }
            if (ingredientList[i][3] > 0) {
                updateItemQuantity(13, orderedQuantity);
            }
        }
    }


    updateItemQuantity(id, orderedQuantity);
}
//_________________Inventory end_____________________________







//_______________________Queue Implementation____________________
Queue::Queue(int capacity) {
    qarray = new string[capacity];
    qfront = 0;
    qrear = -1;
    qsize = 0;
    qcapacity = capacity;
}


Queue::~Queue() {
    delete[] qarray;
}

bool Queue::enqueue(const string& item) {
    if (isFull()) {
        cout << "Queue is full. Cannot enqueue!" << endl;
        return false;
    }
    qrear = (qrear + 1) % qcapacity;
    qarray[qrear] = item;
    qsize++;
    return true;
}

bool Queue::dequeue() {
    if (isEmpty()) {
        cout << "Queue is empty. Cannot dequeue!" << endl;
        return false;
    }
    qfront = (qfront + 1) % qcapacity;
    qsize--;
    return true;
}

string Queue::peek() {
    if (isEmpty()) {
        cout << "Queue is empty. Cannot peek!" << endl;
        return "";
    }
    return qarray[qfront];
}


bool Queue::isEmpty() {
    return qsize == 0;
}

bool Queue::isFull() {
    return qsize == qcapacity;
}

void Queue::displayQueue() {
    if (isEmpty()) {
        cout << "No customers on the waitlist.\n";
        return;
    }

    int index = qfront;
    cout << "Waitlist: ";
    for (int i = 0; i < qsize; ++i) {
        cout << qarray[index] << " -> ";
        index = (index + 1) % qcapacity;
    }
    cout << "NULL" << endl;
}

//________________________________________End Of Queue logic________________________________________________


//_________________________________________Table Management_________________________________________________
/*
Implementing the needed logic for table Management + providing the logic of the wait list queue
*/
Queue waitlist(3);
Table tables[3] = { Table(1), Table(2), Table(3) };

void displayTables() {
    cout << "\n\nTable Status:\n";
    for (int i = 0; i < 3; ++i) {
        cout << "Table " << tables[i].tableId << ": ";
        if (tables[i].isOccupied) {
            cout << "Occupied by " << tables[i].customerName << endl;
        } else {
            cout << "Free" << endl;
        }
    }
    cout << endl;
}

// Function to assign a table to a customer or add them to the waitlist
bool assignTable(const string& customerName) {
    for (int i = 0; i < 3; ++i) {
        if (!tables[i].isOccupied) {
            tables[i].isOccupied = true;
            tables[i].customerName = customerName;
            cout << "Assigned Table " << tables[i].tableId << " to " << customerName << ".\n";
            return true;
        }
    }

    if (waitlist.enqueue(customerName)) {
        cout << "No free tables available. " << customerName << " has been added to the waitlist.\n";
    }
    return false;
}


void releaseTable() {
    int tableId;
    cout << "Enter the Table ID you want to release: ";
    cin >> tableId;

    bool found = false;
    for (int i = 0; i < 3; ++i) {
        if (tables[i].tableId == tableId) {
            if (tables[i].isOccupied) {
                tables[i].isOccupied = false;
                tables[i].customerName = "";
                cout << "Table " << tableId << " has been released.\n";

                if (!waitlist.isEmpty()) {
                    string customerName = waitlist.peek();
                    waitlist.dequeue();
                    tables[i].isOccupied = true;
                    tables[i].customerName = customerName;
                    cout << "Assigned Table " << tableId << " to " << customerName << " from the waitlist.\n";
                }
            } else {
                cout << "Table " << tableId << " is already free.\n";
            }
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Invalid table ID.\n";
    }
}

// Main menu for table management
void tableManagementMenu() {
    int choice;
    cout << "\n\n";
    displayTables();
    cout << "\n\n_______________________\n";
    cout << "1. Release table\n";
    cout << "2. Show Waitlist\n";
    cout << "3. Exit\n";
    cout << "Choice: ";
    cin >> choice;

    switch(choice) {
        case 1:
            releaseTable();
            break;
        case 2:
            showWaitlist();
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice.\n";
    }
}

void showWaitlist() {
    cout << "\nWaitlist:\n";
    if (waitlist.isEmpty()) {
        cout << "No customers on the waitlist.\n";
    } else {
        waitlist.displayQueue();
    }
}

//_____________________________________End of Table Mangement_______________________________________________________



//__________________________________________________________________________________________________________________________




void cashierMenu(){

    Cashier newCashier;


    string menuFile = "menu.txt";

    newCashier.loadMenu(menuFile);

    int choice;
    cout << endl;
    cout << "\n\nWelcome, Cashier!\n";

    while (true) {
        cout << "\n=========================================" << endl;
        cout << "           CASHIER         " << endl;
        cout << "=========================================" << endl;
        cout << "1. Order by Menu" << endl;
        cout << "2. Order by Search" << endl;
        cout << "3. Table Management & Waitlist" << endl;
        cout << "4. Edit Menu" << endl;
        cout << "5. Exit" << endl;
        cout << "=========================================" << endl;
        cout << "Please select an option (1-5): ";
        cin >> choice;

        switch(choice) {
            case 1: {
                cout << "Ordering by menu...\n";
                newCashier.orderByMenu();
                cout <<endl << endl;
                break;
            }
            case 2: {
                cout << "Ordering by search...\n";
                newCashier.searchMenu();
                cout << endl<< endl;
                break;
            }


            case 3: {
                cout << "Viewing table status...\n";
                tableManagementMenu();
                cout << endl<< endl;
                break;
            }
            case 4: {
                cout << "Editing menu...\n";
                newCashier.editMenu();
                cout << endl<< endl;
                break;
            }
            case 5: {
                cout << "Exiting...\n";
                cout << endl<< endl;
                return;
            }
            default: {
                cout << endl<< endl;
                cout << "Invalid choice! Please try again.\n";
            }
        }
    }
}
