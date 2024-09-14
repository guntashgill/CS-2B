// File name: coding_assignment4.cpp
// Date: 06-18-2024
// Program purpose: This program is to manage customer service for a wireless phone carrier, tracking customer accounts and their media messages.

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <exception>
using namespace std;

class AccountNotFoundException : public std::exception {
private:
    string phone_number;
public:
    AccountNotFoundException(const string& phone) : phone_number(phone) {}
    const char* what() const noexcept override {
        static string message;
        message = "Account " + phone_number + " does not exist!";
        return message.c_str();
    }
};

// Media class
class Media {
private:
    double size;
public:
    static const double DEFAULT_SIZE;
    static const double CHARGE_PER_MB;
    static const double MIN_CHARGE;
    static const double SIZE_THRESHOLD;

    Media(double s = DEFAULT_SIZE) : size(s) {}
    double GetCharge() const {
        return (size > MIN_CHARGE) ? size * CHARGE_PER_MB : MIN_CHARGE;
    }
    double GetSize() const { return size; }
    void SetSize(double s) { size = s; }
};

const double Media::DEFAULT_SIZE = 0.0;
const double Media::CHARGE_PER_MB = 0.05;
const double Media::MIN_CHARGE = 1.00;
const double Media::SIZE_THRESHOLD = 100.0;

// Message class template
template <typename T>
class Message {
private:
    string to;
    T m_data;
public:
    Message(string receiver = "000-000-0000", T data = T()) : to(receiver), m_data(data) {}
    string get_to() const { return to; }
    T get_data() const { return m_data; }
    void set_to(const string& receiver) { to = receiver; }
    void set_data(const T& data) { m_data = data; }
};

// SmartCarrier class
class SmartCarrier {
private:
    string carrier_name;
    bool init;
    map<string, vector<Message<Media>*>> account_list;

public:
    static const string DefaultCarrierName;

    SmartCarrier(string name = DefaultCarrierName) : carrier_name(name), init(false) {}
    ~SmartCarrier() {
        for (auto& account : account_list) {
            for (auto msg : account.second) {
                delete msg;
            }
        }
        cout << "Carrier " << carrier_name << " is shutting down." << endl;
    }

    void Init() {
        account_list["669-444-4444"];
        account_list["831-777-7777"].push_back(new Message<Media>("408-222-2222", Media(120.0)));
        account_list["408-111-1111"].push_back(new Message<Media>("415-678-9011", Media(2.08)));
        account_list["408-111-1111"].push_back(new Message<Media>("408-555-8888", Media(87.35)));
        account_list["408-111-1111"].push_back(new Message<Media>("669-000-1234", Media(35.59)));

        account_list["510-555-5555"].push_back(new Message<Media>("408-999-9999", Media(55.2)));
        account_list["650-888-8888"].push_back(new Message<Media>("408-888-8888", Media(78.4)));
        account_list["415-444-1234"].push_back(new Message<Media>("408-777-7777", Media(23.7)));
        account_list["925-333-5678"].push_back(new Message<Media>("408-666-6666", Media(15.1)));
        account_list["707-222-3456"].push_back(new Message<Media>("408-555-1234", Media(92.3)));
        account_list["209-123-7890"].push_back(new Message<Media>("408-444-5678", Media(48.5)));

        init = true;
    }

    void StartService() {
        if (!init) {
            cerr << "System not initialized. Exiting..." << endl;
            return;
        }

        int choice;
        do {
            Menu();
            choice = GetChoice();
            try {
                switch (choice) {
                    case 1: ListAccounts(); break;
                    case 2: InsertMessage(); break;
                    case 3: PurgeLargeMessages(); break;
                    case 4: DisconnectAccount(); break;
                    case 5: break;
                    default: cerr << "Invalid choice. Please try again." << endl;
                }
            } catch (const AccountNotFoundException& e) {
                cerr << e.what() << endl;
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        } while (choice != 5);

        cout << "Exiting program." << endl;
    }

private:
    void Menu() {
        cout << "\nFOOTHILL WIRELESS" << endl;
        cout << "ACCOUNT ADMIN" << endl;
        cout << "1. List all accounts and their messages' usages" << endl;
        cout << "2. Add a message to an account" << endl;
        cout << "3. Purge large messages from an account" << endl;
        cout << "4. Disconnect an account" << endl;
        cout << "5. Quit" << endl;
        cout << "Enter your choice: ";
    }

    int GetChoice() {
        int choice;
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cerr << "Invalid input. Please enter a number between 1 and 5: ";
            cin >> choice;
        }
        return choice;
    }

    void ListAccounts() {
        cout << "Account\t\tTotal messages\tTotal messages' sizes (MB)\tCharge (dollar)" << endl;
        for (const auto& account : account_list) {
            double total_size = 0.0;
            double total_charge = 0.0;
            for (const auto& msg : account.second) {
                total_size += msg->get_data().GetSize();
                total_charge += msg->get_data().GetCharge();
            }
            cout << account.first << "\t" << account.second.size() << "\t\t" << total_size << "\t\t" << total_charge << endl;
        }
    }

    void InsertMessage() {
        string phone;
        double size;
        cout << "Enter phone number: ";
        cin >> phone;
        cout << "Enter media size (MB): ";
        cin >> size;

        Media media(size);
        if (account_list.find(phone) == account_list.end()) {
            throw AccountNotFoundException(phone);
        }

        account_list[phone].push_back(new Message<Media>(phone, media));
    }

    void PurgeLargeMessages() {
        string phone;
        cout << "Enter phone number: ";
        cin >> phone;

        if (account_list.find(phone) == account_list.end()) {
            throw AccountNotFoundException(phone);
        }

        auto& messages = account_list[phone];
        for (auto it = messages.begin(); it != messages.end();) {
            if ((*it)->get_data().GetSize() > Media::SIZE_THRESHOLD) {
                delete *it;
                it = messages.erase(it);
            } else {
                ++it;
            }
        }
    }

    void DisconnectAccount() {
        string phone;
        cout << "Enter phone number: ";
        cin >> phone;

        if (account_list.find(phone) == account_list.end()) {
            throw AccountNotFoundException(phone);
        }

        auto& messages = account_list[phone];
        for (auto msg : messages) {
            delete msg;
        }
        account_list.erase(phone);
    }
};

const string SmartCarrier::DefaultCarrierName = "XYZ Wireless";

int main() {
    SmartCarrier carrier("Foothill Wireless");
    carrier.Init();
    carrier.StartService();
    return 0;
}


