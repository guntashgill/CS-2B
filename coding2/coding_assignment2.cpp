// File name: Coding_assignment2.cpp
// Programmer: Guntash Gill
//Version 1
// Date: 05/03-2024
// Program purpose: Develop an inventory management system for an electronics store

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class InventoryItem {
protected:
    int item_id;
    bool restocking;

public:
    static const int default_item_id = 9999;
    static const bool default_restocking = false;

    InventoryItem(): item_id(default_item_id), restocking(default_restocking) {}

    InventoryItem(int id, bool restocking_flag): item_id(id), restocking(restocking_flag) {}

    ~InventoryItem() {
        cout << "InventoryItem " << setw(4) << setfill('0') << item_id << " with " << (restocking ? "true" : "false") << " restocking destroyed ..." << endl;
    }

    void set_item_id(int id) {
        item_id = id;
    }

    int get_item_id() const {
        return item_id;
    }

    void set_restocking(bool restocking_flag) {
        restocking = restocking_flag;
    }

    bool get_restocking() const {
        return restocking;
    }

    virtual void Display() const {
        cout << "Item ID: " << setw(4) << setfill('0') << item_id << ", Restocking: " << (restocking ? "true" : "false") << endl;
    }
};

class Product : public InventoryItem {
private:
    string name;
    int quantity;
    double price;

public:
    static const string default_name;
    static const int default_quantity;
    static const double default_price;

    Product(): InventoryItem(), name(default_name), quantity(default_quantity), price(default_price) {}

    Product(int id, bool restocking_flag, const string& prod_name, int qty, double pr): InventoryItem(id, restocking_flag), name(prod_name), quantity(qty), price(pr) {}

    ~Product() {
        cout << "Product: " << setw(4) << setfill('0') << item_id << ", Name: " << name  << ", quantity: " << quantity << ", price: " << fixed << setprecision(2) << price  << ", restocking: " << (restocking ? "true" : "false") << " destroyed ..." << endl;
    }

    void set_name(const string& prod_name) {
        name = prod_name;
    }

    string get_name() const {
        return name;
    }

    void set_quantity(int qty) {
        quantity = qty;
    }

    int get_quantity() const {
        return quantity;
    }

    void set_price(double pr) {
        price = pr;
    }

    double get_price() const {
        return price;
    }

    void Display() const override {
        InventoryItem::Display();
        cout << "Name: " << name << ", Quantity: " << quantity << ", Price: " << fixed << setprecision(2) << price;
        if (restocking) {
            cout << " ***** (Restocking)";
        }
        cout << endl;
    }

    double Cost(int qty) const {
        return price * qty;
    }
};

const string Product::default_name = "No Product";
const int Product::default_quantity = 0;
const double Product::default_price = 0.99;

class InventorySystem {
private:
    string store_name;
    InventoryItem* product_list[512];
    int product_count;

    InventoryItem* FindInventoryItem(int id) {
        for (int i = 0; i < product_count; ++i) {
            if (product_list[i]->get_item_id() == id) {
                return product_list[i];
            }
        }
        return nullptr;
    }

public:
    static const int array_size = 512;
    static const string default_store_name;
    static const int default_product_count;
    static const string input_file_name;
    static const string output_file_name;

    InventorySystem() : store_name(default_store_name), product_count(default_product_count) {
        for (int i = 0; i < array_size; ++i) {
            product_list[i] = nullptr;
        }
    }

    InventorySystem(const string& name) : store_name(name), product_count(default_product_count) {
        for (int i = 0; i < array_size; ++i) {
            product_list[i] = nullptr;
        }
    }

    ~InventorySystem() {
        for (int i = 0; i < product_count; ++i) {
            delete product_list[i];
        }
    }

    void BuildInventory() {
        ifstream infile(input_file_name);
        string line;
        while (getline(infile, line)) {
            stringstream ss(line);
            string id_str, name, quantity_str, price_str;

            getline(ss, id_str, ';');
            getline(ss, name, ';');
            getline(ss, quantity_str, ';');
            getline(ss, price_str);

            int id = stoi(id_str);
            int quantity = stoi(quantity_str);
            double price = stod(price_str);
            bool restocking_flag = (quantity == 0);

            product_list[product_count] = new Product(id, restocking_flag, name, quantity, price);
            product_count++;
        }
        infile.close();
    }

    void ShowInventory() const {
        for (int i = 0; i < product_count; ++i) {
            product_list[i]->Display();
        }
    }

    void UpdateInventory() {
        int id, qty;
        cout << "Enter Item ID: ";
        cin >> id;
        cout << "Enter quantity: ";
        cin >> qty;

        InventoryItem* item = FindInventoryItem(id);
        if (item != nullptr) {
            Product* product = static_cast<Product*>(item);
            double cost = product->Cost(qty);
            product->set_quantity(product->get_quantity() - qty);
            if (product->get_quantity() <= 0) {
                product->set_restocking(true);
                product->set_quantity(0);
                cout << "Restocking required for " << product->get_name() << "." << endl;
            }
            cout << "Total cost: $" << fixed << setprecision(2) << cost << endl;
        } else {
            cout << "Product with ID " << id << " not found." << endl;
        }
    }

    void Terminate() {
        ofstream outfile(output_file_name);
        for (int i = 0; i < product_count; ++i) {
            Product* product = static_cast<Product*>(product_list[i]);
            outfile << product->get_item_id() << ";"
                    << product->get_name() << ";"
                    << product->get_quantity() << ";"
                    << fixed << setprecision(2) << product->get_price() << endl;
        }
        outfile.close();
    }
};

const string InventorySystem::default_store_name = "My Store";
const int InventorySystem::default_product_count = 0;
const string InventorySystem::input_file_name = "inventory.txt";
const string InventorySystem::output_file_name = "output.txt";

int main() {
    InventorySystem* store = new InventorySystem("Electronics Store");
    store->BuildInventory();
    store->ShowInventory();
    store->UpdateInventory();
    store->Terminate();
    delete store;
    return 0;
}