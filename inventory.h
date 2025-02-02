#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>

struct Item {
    int id;
    std::string name;
    std::string category;
    std::string quantity;
    Item* next;
};

class Inventory {
public:
    Inventory();
    ~Inventory();

    // Core inventory management Function
    void AddItem(int id, const std::string& name, const std::string& category, const std::string& quantity);
    void RemoveItem(const std::string& name);
    void ViewInventory();
    void LoadFromFile(const std::string& filename);
    void SaveToFile(const std::string& filename);
    void InventoryMenu();
    void DisplayItems(const std::string& category = "") const;

    //Merge sort function
    void SortInventory();




private:
    // Helper functions
    bool ItemExists(const std::string& name, std::string& existingQuantity) const;
    std::string Trim(const std::string& str) const;

    Item* Merge(Item* left, Item* right);
    void Split(Item* source, Item** left, Item** right);
    Item* MergeSort(Item* head);
    Item* head;
};



std::string GetCurrentDate();
void InventoryMenu();

#endif
