#pragma once
#include "manager.h"
#include <iostream>
#include <string>
#include <vector>

class UserInterface {
public:
    void displayMenu() {
        cout << "1. Add contact\n";
        cout << "2. Delete contact\n";
        cout << "3. Edit contact\n";
        cout << "4. Display contacts\n";
        cout << "5. Find contact\n";
        cout << "6. Sort contacts\n";
        cout << "7. Exit\n";
    }

    string getStringInput(const string& prompt) {
        string input;
        cout << prompt;
        getline(cin, input);
        return input;
    }

    int getIntInput(const string& prompt) {
        int input;
        cout << prompt;
        cin >> input;
        cin.ignore();
        return input;
    }

    Contact getContactInput() {
            Contact contact;
            contact.setFirstName(getStringInput("Enter first name: "));
            contact.setLastName(getStringInput("Enter last name: "));
            contact.setMiddleName(getStringInput("Enter middle name: "));
            contact.setCity(getStringInput("Enter city: "));
            contact.setStreet(getStringInput("Enter street: "));
            contact.setHouseNumber(getStringInput("Enter house number: "));
            contact.setApartmentNumber(getStringInput("Enter apartment number: "));
            contact.setBirthDate(getStringInput("Enter birth date (DD-MM-YYYY): "));
            contact.setEmail(getStringInput("Enter email: "));
            vector<string> phoneNumbers;
            string phoneNumber;
            while (true) {
                phoneNumber = getStringInput("Enter phone number (or 'q' to quit): ");
                if (phoneNumber == "q") {
                    if (phoneNumbers.empty()) {
                        cout << "At least one phone number is required. Please try again.\n";
                    }
                    else {
                        break;
                    }
                }
                else {
                    phoneNumbers.push_back(phoneNumber);
                }
            }
            contact.setPhoneNumbers(phoneNumbers);
            return contact;
        
    }

    void displayContacts(const vector<Contact>& contacts) {
        for (const auto& contact : contacts) {
            cout << contact.getFirstName() << " " << contact.getLastName() << " " << contact.getMiddleName() << endl;
            cout << "Address: " << contact.getCity() << ", " << contact.getStreet() << ", " << contact.getHouseNumber() << ", " << contact.getApartmentNumber() << endl;
            cout << contact.getBirthDate() << endl;
            cout << contact.getEmail() << endl;
            for (const auto& phoneNumber : contact.getPhoneNumbers()) {
                cout << phoneNumber << endl;
            }
            cout << endl;
        }
    }



};