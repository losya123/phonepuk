#pragma once
#include "contact.h"
#include "user.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>

class ContactManager{
private:
    vector<Contact> contacts;
    string filename;

public:
    ContactManager(const string& filename) : filename(filename) {
        loadContacts();
    }

    void addContact(const Contact& contact) {
        contacts.push_back(contact);
        saveContacts();
    }

    void deleteContact(const string& firstName, const string& lastName) {
        auto it = find_if(contacts.begin(), contacts.end(),
            [&](const Contact& c) {
                return c.getFirstName() == firstName && c.getLastName() == lastName;
            });
        try {
            if (it == contacts.end()) {
                throw runtime_error("Contact not found");
            }
            contacts.erase(it);
            saveContacts();
        }
        catch (const runtime_error& e) {
            cerr << e.what() << '\n';
        }
    }

    void editContact(const string& firstName, const string& lastName, const Contact& newContact) {
        auto it = find_if(contacts.begin(), contacts.end(),
            [&](const Contact& c) {
                return c.getFirstName() == firstName && c.getLastName() == lastName;
            });
        try {
            if (it == contacts.end()) {
                throw runtime_error("Contact not found");
            }
            *it = newContact;
            saveContacts();
        }
        catch (const runtime_error& e) {
            cerr << e.what() << '\n';
        }
    }

    const vector<Contact>& getContacts() const {
        return contacts;
    }

    void sortContacts(const string& field) {
        try {
            if (field == "first name") {
                sort(contacts.begin(), contacts.end(),
                    [](const Contact& a, const Contact& b) {
                        return a.getFirstName() < b.getFirstName();
                    });
            }
            else if (field == "last name") {
                sort(contacts.begin(), contacts.end(),
                    [](const Contact& a, const Contact& b) {
                        return a.getLastName() < b.getLastName();
                    });
            }
            else if (field == "middle name") {
                sort(contacts.begin(), contacts.end(),
                    [](const Contact& a, const Contact& b) {
                        return a.getMiddleName() < b.getMiddleName();
                    });
            }
            else if (field == "city") {
                sort(contacts.begin(), contacts.end(),
                    [](const Contact& a, const Contact& b) {
                        return a.getCity() < b.getCity();
                    });
            }
            else if (field == "street") {
                sort(contacts.begin(), contacts.end(),
                    [](const Contact& a, const Contact& b) {
                        return a.getStreet() < b.getStreet();
                    });
            }
            else if (field == "house number") {
                sort(contacts.begin(), contacts.end(),
                    [](const Contact& a, const Contact& b) {
                        return a.getHouseNumber() < b.getHouseNumber();
                    });
            }
            else if (field == "apartment number") {
                sort(contacts.begin(), contacts.end(),
                    [](const Contact& a, const Contact& b) {
                        return a.getApartmentNumber() < b.getApartmentNumber();
                    });
            }
            else if (field == "birth date") {
                sort(contacts.begin(), contacts.end(),
                    [](const Contact& a, const Contact& b) {
                        return a.getBirthDate() < b.getBirthDate();
                    });
            }
            else if (field == "emai") {
                sort(contacts.begin(), contacts.end(),
                    [](const Contact& a, const Contact& b) {
                        return a.getEmail() < b.getEmail();
                    });
            }
            else {
                throw invalid_argument("Invalid field");
            }
        }
        catch (const runtime_error& e) {
            cerr << e.what() << '\n';
        }
        saveContacts();
    }

    void sortContactsInteractive() {
        string input;
        string field;
        do {
            cout << "Select parameter to sort by:\n"
                << "1. First name\n"
                << "2. Last name\n"
                << "3. Middle name\n"
                << "4. City\n"
                << "5. Street\n"
                << "6. House number\n"
                << "7. Apartment number\n"
                << "8. Birth date\n"
                << "9. Email\n"
                << "0. Cancel\n"
                << "Enter your choice: ";
            getline(cin, input);
            if (input == "1") {
                field = "first name";
            }
            else if (input == "2") {
                field = "last name";
            }
            else if (input == "3") {
                field = "middle name";
            }
            else if (input == "4") {
                field = "city";
            }
            else if (input == "5") {
                field = "street";
            }
            else if (input == "6") {
                field = "house number";
            }
            else if (input == "7") {
                field = "apartment number";
            }
            else if (input == "8") {
                field = "birth date";
            }
            else if (input == "9") {
                field = "email";
            }
            else if (input != "0") {
                cout << "Invalid choice. Please try again.\n";
            }
        } while (input != "0" && input != "1" && input != "2" && input != "3" && input != "4" && input != "5" && input != "6" && input != "7" && input != "8" && input != "9");

        if (input != "0") {
            sortContacts(field);
        }
    }

    vector<Contact> searchContacts(const map<string, string>& params) {
        vector<Contact> result;
        for (const auto& contact : contacts) {
            bool match = true;
            for (const auto& param : params) {
                if (param.first == "first name" && contact.getFirstName() != param.second) {
                    match = false;
                    break;
                }
                else if (param.first == "last name" && contact.getLastName() != param.second) {
                    match = false;
                    break;
                }
                else if (param.first == "middle name" && contact.getMiddleName() != param.second) {
                    match = false;
                    break;
                }
                else if (param.first == "city" && contact.getCity() != param.second) {
                    match = false;
                    break;
                }
                else if (param.first == "street" && contact.getStreet() != param.second) {
                    match = false;
                    break;
                }
                else if (param.first == "house number" && contact.getHouseNumber() != param.second) {
                    match = false;
                    break;
                }
                else if (param.first == "apartment number" && contact.getApartmentNumber() != param.second) {
                    match = false;
                    break;
                }
                else if (param.first == "birth date" && contact.getBirthDate() != param.second) {
                    match = false;
                    break;
                }
                else if (param.first == "email" && contact.getEmail() != param.second) {
                    match = false;
                    break;
                }
                else if (param.first == "phone number" && find(contact.getPhoneNumbers().begin(), contact.getPhoneNumbers().end(), param.second) == contact.getPhoneNumbers().end()) {
                    match = false;
                    break;
                }
            }
            if (match) {
                result.push_back(contact);
            }
        }
        return result;

    }

    vector<Contact> searchContactsInteractive() {
        map<string, string> params;
        string input;
        do {
            cout << "Select parameter to search by:\n"
                << "1. First name\n"
                << "2. Last name\n"
                << "3. Middle name\n"
                << "4. City\n"
                << "5. Street\n"
                << "6. House number\n"
                << "7. Apartment number\n"
                << "8. Birth date\n"
                << "9. Email\n"
                << "0. Search\n"
                << "Enter your choice: ";
            getline(cin, input);
            if (input == "1") {
                cout << "Enter first name: ";
                getline(cin, input);
                params["first name"] = input;
            }
            else if (input == "2") {
                cout << "Enter last name: ";
                getline(cin, input);
                params["last name"] = input;
            }
            else if (input == "3") {
                cout << "Enter middle name: ";
                getline(cin, input);
                params["middle name"] = input;
            }
            else if (input == "4") {
                cout << "Enter city: ";
                getline(cin, input);
                params["city"] = input;
            }
            else if (input == "5") {
                cout << "Enter street: ";
                getline(cin, input);
                params["street"] = input;
            }
            else if (input == "6") {
                cout << "Enter house number: ";
                getline(cin, input);
                params["house number"] = input;
            }
            else if (input == "7") {
                cout << "Enter apartment number: ";
                getline(cin, input);
                params["apartment number"] = input;
            }
            else if (input == "8") {
                cout << "Enter birth date: ";
                getline(cin, input);
                params["birth date"] = input;
            }
            else if (input == "9") {
                cout << "Enter email: ";
                getline(cin, input);
                params["email"] = input;
            }
            else if (input != "0") {
                cout << "Invalid choice. Please try again.\n";
            }
        } while (input != "0");

        return searchContacts(params);
    }

private:
    void loadContacts() {
        try {
            ifstream file(filename);
            if (!file.is_open()) {
                throw runtime_error("Error opening file");
            }
            //для работы с файлом
            contacts.clear();
            string line;
            while (getline(file, line)) {
                contacts.push_back(Contact::deserialize(line));
            }
        }
        catch (const runtime_error& e) {
            cerr << e.what() << '\n';
        }
    }

    void saveContacts() {
        ofstream file(filename);
        try {
            if (!file.is_open()) {
                throw runtime_error("Error opening file");
            }
            for (const auto& contact : contacts) {
                file << contact.serialize() << endl;
            }
        }
        catch (const runtime_error& e) {
            cerr << e.what() << '\n';
        }
    }

};