#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Name {
public:
    std::string first_name;
    std::string last_name;

    Name(const std::string &first, const std::string &last) : first_name(first), last_name(last) {}

    bool operator==(const Name &other) const {
        return first_name == other.first_name && last_name == other.last_name;
    }

    friend std::ostream &operator<<(std::ostream &os, const Name &name) {
        os << name.first_name << " " << name.last_name;
        return os;
    }
};

class Contact {
protected:
    Name name;
    std::string phone;
    std::string email;
    int location;
    Contact *next;

public:
    Contact(const std::string &first, const std::string &last, const std::string &phone, const std::string &email, int location)
        : name(first, last), phone(phone), email(email), location(location), next(nullptr) {}

    virtual ~Contact() { std::cout << name << " has gone home...\n"; }

    virtual void ShowContact() const = 0;

    const Name& GetName() const {
        return name;
    }

    Contact* GetNext() const {
        return next;
    }

    void SetNext(Contact* next_contact) {
        next = next_contact;
    }
};

class EmployeeContact : public Contact {
    std::string title;
    std::string department;

public:
    EmployeeContact(const std::string &first, const std::string &last, const std::string &phone, const std::string &email, int location, const std::string &title, const std::string &department)
        : Contact(first, last, phone, email, location), title(title), department(department) {}

    void ShowContact() const override {
        std::cout << name << "\t" << title << "\t" << department << "\tRoom " << location << "\t" << phone << "\t" << email << "\n";
    }

    const std::string& GetDepartment() const {
        return department;
    }
};

class ContractorContact : public Contact {
    std::string company;
    int duration;

public:
    ContractorContact(const std::string &first, const std::string &last, const std::string &phone, const std::string &email, int location, const std::string &company, int duration)
        : Contact(first, last, phone, email, location), company(company), duration(duration) {}

    void ShowContact() const override {
        std::cout << name << "\t" << duration << " months (contractor)\t" << company << "\tRoom " << location << "\t" << phone << "\t" << email << "\n";
    }
};

class ContactList {
    Contact *head;

public:
    ContactList() : head(nullptr) {}

    ~ContactList() {
        while (head) {
            Contact *temp = head;
            head = head->GetNext();
            delete temp;
        }
    }

    void Insert(Contact *contact) {
        contact->SetNext(head);
        head = contact;
    }

    Contact *FindContact(const Name &name) const {
        Contact *current = head;
        while (current) {
            if (current->GetName() == name) return current;
            current = current->GetNext();
        }
        return nullptr;
    }

    void ShowAllContacts() const {
        Contact *current = head;
        while (current) {
            current->ShowContact();
            current = current->GetNext();
        }
    }

    void SearchByName(const Name &name) const {
        Contact *contact = FindContact(name);
        if (contact) contact->ShowContact();
        else std::cout << "Sorry no match is found.\n";
    }

    void SearchByDepartment(const std::string &department) const {
        Contact *current = head;
        bool found = false;
        while (current) {
            if (EmployeeContact *emp = dynamic_cast<EmployeeContact*>(current)) {
                if (emp->GetDepartment() == department) {
                    emp->ShowContact();
                    found = true;
                }
            }
            current = current->GetNext();
        }
        if (!found) std::cout << "No contacts found in department " << department << ".\n";
    }

    void Init() {
    Insert(new EmployeeContact("James", "West", "1-408-790-8251", "jamesw@tpcommunications.com", 23, "Software Engineer", "Engineering"));
    Insert(new ContractorContact("Cindy", "Lincoln", "1-408-790-7372", "cindyl@tpcommunications.com", 9, "TK Consultings", 6));
    Insert(new EmployeeContact("David", "Summer", "1-408-790-1682", "davids@tpcommunications.com", 23, "Software Engineer", "Engineering"));
    Insert(new EmployeeContact("Jennifer", "Hans", "1-408-790-2381", "jenniferh@tpcommunications.com", 17, "HR Representative", "Human Resources"));
    Insert(new ContractorContact("Cindy", "Morgan", "1-408-790-3953", "cindym@tpcommunications.com", 43, "TK Consultings", 12));
    Insert(new EmployeeContact("Alice", "Smith", "1-408-790-1111", "alices@tpcommunications.com", 30, "Application Engineer", "Engineering"));
    Insert(new EmployeeContact("Bob", "Johnson", "1-408-790-2222", "bobj@tpcommunications.com", 10, "Mechanical Engineer", "Engineering"));
    Insert(new EmployeeContact("Charlie", "Williams", "1-408-790-3333", "charliew@tpcommunications.com", 12, "Manager", "Sales"));
    Insert(new EmployeeContact("Diana", "Brown", "1-408-790-4444", "dianab@tpcommunications.com", 8, "Product Support Engineer", "Customer Support"));
    Insert(new EmployeeContact("Edward", "Jones", "1-408-790-5555", "edwardj@tpcommunications.com", 15, "Test Technician", "IT"));
    Insert(new EmployeeContact("Fiona", "Garcia", "1-408-790-6666", "fionag@tpcommunications.com", 19, "Business Analyst", "Finance"));
    Insert(new EmployeeContact("George", "Martinez", "1-408-790-7777", "georgem@tpcommunications.com", 11, "Admin Assistant", "Human Resources"));
    Insert(new EmployeeContact("Helen", "Hernandez", "1-408-790-8888", "helenh@tpcommunications.com", 14, "Manufacturing Engineer", "Manufacturer"));
    Insert(new EmployeeContact("Isaac", "Lopez", "1-408-790-9999", "isaacl@tpcommunications.com", 5, "IT Technical Support", "IT"));
    Insert(new EmployeeContact("Julia", "Gonzalez", "1-408-790-0000", "juliag@tpcommunications.com", 18, "Hardware Engineer", "Engineering"));
    Insert(new EmployeeContact("Kevin", "Clark", "1-408-790-1234", "kevinc@tpcommunications.com", 22, "Director", "Marketing"));
    Insert(new ContractorContact("Liam", "Rodriguez", "1-408-790-2345", "liamr@tpcommunications.com", 28, "BuildSmart", 10));
    Insert(new ContractorContact("Mia", "Lewis", "1-408-790-3456", "mial@tpcommunications.com", 32, "Tech Solutions", 8));
    Insert(new ContractorContact("Noah", "Walker", "1-408-790-4567", "noahw@tpcommunications.com", 36, "Creative Minds", 12));
    Insert(new ContractorContact("Olivia", "Young", "1-408-790-5678", "oliviay@tpcommunications.com", 40, "DesignCorp", 6));
}

};


class AddressBook {
    std::string company_name;
    ContactList contact_list;

public:
    AddressBook(const std::string &name) : company_name(name) {}

    void Init() {
        contact_list.Init();
    }

    void Run() {
        int option;
        do {
            Menu();
            option = GetUserOption();
            switch (option) {
                case 1: ShowAllContacts(); break;
                case 2: SearchByName(); break;
                case 3: SearchByDepartment(); break;
                case 4: Quit(); break;
                default: std::cout << "Invalid option. Please try again.\n";
            }
        } while (option != 4);
    }

private:
    void Menu() const {
        std::cout << "TP Communications Address Book Menu\n"
                  << "1. View all contacts\n"
                  << "2. Search by name\n"
                  << "3. Search by department\n"
                  << "4. Quit\n"
                  << "Select an option (1-4): ";
    }

    int GetUserOption() const {
        int option;
        std::cin >> option;
        return option;
    }

    void ShowAllContacts() const {
        contact_list.ShowAllContacts();
    }

    void SearchByName() const {
        std::string first_name, last_name;
        std::cout << "Enter a contact first name: ";
        std::cin >> first_name;
        std::cout << "Enter a contact last name: ";
        std::cin >> last_name;
        contact_list.SearchByName(Name(first_name, last_name));
    }

    void SearchByDepartment() const {
        std::string department;
        std::cout << "Enter a department: ";
        std::cin >> department;
        contact_list.SearchByDepartment(department);
    }

    void Quit() const {
        std::cout << "Thanks for using our AddressBook. See you again!\n";
    }
};


int main() {
    AddressBook *addressBook = new AddressBook("TP Communications");
    addressBook->Init();
    addressBook->Run();
    delete addressBook;
    return 0;
}

