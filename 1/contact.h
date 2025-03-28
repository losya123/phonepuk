#pragma once
#include <iostream>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <locale>
using namespace std;

class Contact {
private:
    string firstName;
    string lastName;
    string middleName;

    string city;
    string street;
    string houseNumber;
    string apartmentNumber;

    string birthDate;
    string email;
    vector<string> phoneNumbers;

public:
    Contact()
        : firstName(), lastName(), middleName(), city(), street(), houseNumber(), apartmentNumber(), birthDate(), email(), phoneNumbers({}) {}

    Contact(const string& firstName, const string& lastName, const string& middleName, const string& city, const string& street, const string& houseNumber, const string& apartmentNumber, const string& birthDate, const string& email, const vector<string>& phoneNumbers)
        : firstName(firstName), lastName(lastName), middleName(middleName), city(city), street(street), houseNumber(houseNumber), apartmentNumber(apartmentNumber), birthDate(birthDate), email(email), phoneNumbers(phoneNumbers) {
    }

    
    string getFirstName() const {
        return firstName;
    }

    string getLastName() const {
        return lastName;
    }

    string getMiddleName() const {
        return middleName;
    }

    string getCity() const {
        return city;
    }

    string getStreet() const {
        return street;
    }

    string getHouseNumber() const {
        return houseNumber;
    }

    string getApartmentNumber() const {
        return apartmentNumber;
    }

    string getBirthDate() const {
        return birthDate;
    }

    string getEmail() const {
        return email;
    }

    vector<string> getPhoneNumbers() const {
        return phoneNumbers;
    }

    void setFirstName(const string firstName) {
        string trimmedFirstName = regex_replace(firstName, regex("^\\s+|\\s+$"), "");
        bool flag = false;
        while (!flag) {
            try {
                if (trimmedFirstName.empty()) {
                    throw invalid_argument("First name cannot be empty");
                }
                trimmedFirstName = regex_replace(trimmedFirstName, regex("^\\s+|\\s+$"), "");

                if (trimmedFirstName.front() == '-' || trimmedFirstName.back() == '-') {
                    throw invalid_argument("First name cannot start or end with a dash");
                }

                regex pattern("^[A-Za-z€-Ÿ -ï0-9\\s-]+[A-Za-z€-Ÿ -ï]$");
                if (!regex_match(trimmedFirstName, pattern)) {
                    throw invalid_argument("Invalid first name format");
                }

                flag = true;
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << endl;
                cout << "Please try again!\n";
                cout << "Enter first name: ";
                getline(cin, trimmedFirstName);
            }
        }

        this->firstName = trimmedFirstName;

    }

    void setLastName(const string lastName) {
        string trimmedLastName = regex_replace(lastName, regex("^\\s+|\\s+$"), "");
        bool flag = false;
        while (!flag) {
            try {
                if (trimmedLastName.empty()) {
                    throw invalid_argument("Last name cannot be empty");
                }

                if (trimmedLastName.front() == '-' || trimmedLastName.back() == '-') {
                    throw invalid_argument("Last name cannot start or end with a dash");
                }

                regex pattern("^[A-Za-z€-Ÿ -ï0-9\\s-]+[A-Za-z€-Ÿ -ï]$");
                if (!regex_match(trimmedLastName, pattern)) {
                    throw invalid_argument("Invalid last name format");
                }

                flag = true;
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << endl;
                cout << "Please try again!\n";
                cout << "Enter last name: ";
                getline(cin, trimmedLastName);
            }
        }
        this->lastName = trimmedLastName;
    }

    void setMiddleName(const string middleName) {
        string trimmedMiddleName = regex_replace(middleName, regex("^\\s+|\\s+$"), "");
        bool flag = false;
        while (!flag) {
            try {
                if (trimmedMiddleName.empty()) {
                    throw invalid_argument("Middle name cannot be empty");
                }

                if (trimmedMiddleName.front() == '-' || trimmedMiddleName.back() == '-') {
                    throw invalid_argument("Middle name cannot start or end with a dash");
                }

                regex pattern("^[A-Za-z€-Ÿ -ï0-9\\s-]+[A-Za-z€-Ÿ -ï]$");
                if (!regex_match(trimmedMiddleName, pattern)) {
                    throw invalid_argument("Invalid middle name format");
                }

                flag = true;
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << endl;
                cout << "Please try again!\n";
                cout << "Enter middle name: ";
                getline(cin, trimmedMiddleName);
            }
        }

        this->middleName = trimmedMiddleName;
    }

    void setCity(const string newCity) {
        string trimmedCity = regex_replace(newCity, regex("^\\s+|\\s+$"), "");
        bool flag = false;
        while (!flag) {
            try {
                if (trimmedCity.empty()) {
                    throw invalid_argument("City cannot be empty");
                }
                regex pattern("^[ -ï€-Ÿa-zA-Z][ -ï€-Ÿa-zA-Z\\-\\s]*$");
                if (!regex_match(trimmedCity, pattern)) {
                    throw invalid_argument("Invalid city name");
                }

                flag = true;
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << endl;
                cout << "Please try again!\n";
                cout << "Enter city: ";
                getline(cin, trimmedCity);
            }
        }

        this->city = trimmedCity;
    }

    void setStreet(const string newStreet) {
        string trimmedStreet = regex_replace(newStreet, regex("^\\s+|\\s+$"), "");
        bool flag = false;
        while (!flag) {
            try {
                if (trimmedStreet.empty()) {
                    throw invalid_argument("Street cannot be empty");
                }
                regex pattern("^[ -ï€-Ÿa-zA-Z][ -ï€-Ÿa-zA-Z\\-\\s]*$");
                if (!regex_match(trimmedStreet, pattern)) {
                    throw invalid_argument("Invalid street name");
                }

                flag = true;
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << endl;
                cout << "Please try again!\n";
                cout << "Enter street: ";
                getline(cin, trimmedStreet);
            }
        }

        this->street = trimmedStreet;
    }

    void setHouseNumber(const string newHouseNumber) {
        string trimmedHouseNumber = regex_replace(newHouseNumber, regex("\\s+"), "");
        bool flag = false;
        while (!flag) {
            try {
                if (trimmedHouseNumber.empty()) {
                    throw invalid_argument("House number cannot be empty");
                }
                regex pattern("^\\d+[- -ï€-Ÿa-zA-Z]?\\d*$");
                if (!regex_match(trimmedHouseNumber, pattern)) {
                    throw invalid_argument("Invalid house number format");
                }

                flag = true;
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << endl;
                cout << "Please try again!\n";
                cout << "Enter house number: ";
                getline(cin, trimmedHouseNumber);
            }
        }

        this->houseNumber = trimmedHouseNumber;
    }

    void setApartmentNumber(const string newApartmentNumber) {
        string trimmedApartmentNumber = regex_replace(newApartmentNumber, regex("\\s+"), "");
        bool flag = false;
        while (!flag) {
            try {
                if (trimmedApartmentNumber.empty()) {
                    throw invalid_argument("Apartment number cannot be empty");
                }
                regex pattern("^\\d+[- -ï€-Ÿa-zA-Z]?\\d*$");
                if (!regex_match(trimmedApartmentNumber, pattern)) {
                    throw invalid_argument("Invalid apartment number format");
                }

                flag = true;
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << endl;
                cout << "Please try again!\n";
                cout << "Enter apartment number: ";
                getline(cin, trimmedApartmentNumber);
            }
        }

        this->apartmentNumber = trimmedApartmentNumber;
    }

    void setBirthDate(const string birthDate) {
        string trimmedBirthDate = regex_replace(birthDate, regex("\\s+"), "");
        regex pattern("^(\\d{2})-(\\d{2})-(\\d{4})$");
        smatch match;
        bool flag = false;
        while (!flag) {
            try {
                if (trimmedBirthDate.empty()) {
                    throw invalid_argument("Birth date cannot be empty");
                }
                if (!regex_match(trimmedBirthDate, match, pattern)) {
                    throw invalid_argument("Invalid birth date format");
                }
                int day = stoi(match[1].str());
                int month = stoi(match[2].str());
                int year = stoi(match[3].str());
                if (month < 1 || month > 12 || day < 1 || day > 31) {
                    throw invalid_argument("Invalid birth date");
                }
                if (month == 2) {
                    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                        if (day > 29) {
                            throw invalid_argument("Invalid birth date");
                        }
                    }
                    else {
                        if (day > 28) {
                            throw invalid_argument("Invalid birth date");
                        }
                    }
                }
                else if (month == 4 || month == 6 || month == 9 || month == 11) {
                    if (day > 30) {
                        throw invalid_argument("Invalid birth date");
                    }
                }
                time_t t = time(nullptr);
                tm tm_now;
                localtime_s(&tm_now, &t);
                tm birthDateTm = { 0, 0, 0, day, month - 1, year - 1900, 0, 0, 0 };
                time_t birthDateTimestamp = mktime(&birthDateTm);
                if (birthDateTimestamp >= mktime(&tm_now)) {
                    throw invalid_argument("Birth date cannot be in the future");
                }

                flag = true;
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << endl;
                cout << "Please try again!\n";
                cout << "Enter birth date (DD-MM-YYYY): ";
                getline(cin, trimmedBirthDate);
            }
        }

        this->birthDate = trimmedBirthDate;
    }


    void setEmail(const string email) {
        string trimmedEmail = regex_replace(email, regex("^\\s+|\\s+$"), "");
        bool flag = false;
        while (!flag) {
            try {
                if (trimmedEmail.empty()) {
                    throw invalid_argument("Email cannot be empty");
                }
                regex pattern("^[A-Za-z0-9-]+@[A-Za-z0-9]+\\.[A-Za-z]{2,}$");
                if (!regex_match(trimmedEmail, pattern)) {
                    throw invalid_argument("Invalid email format");
                }

                flag = true;
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << endl;
                cout << "Please try again!\n";
                cout << "Enter email: ";
                getline(cin, trimmedEmail);
            }
        }

        this->email = trimmedEmail;
    }

    void setPhoneNumbers(vector<string> phoneNumbers) {
        regex pattern("^(\\+7|8)\\d{10}$|^(\\+7|8)\\(\\d{3}\\)\\d{7}$|^(\\+7|8)\\(\\d{3}\\)\\d{3}-\\d{2}-\\d{2}$");
        vector<string> trimmedPhoneNumber;

        for (size_t i = 0; i < phoneNumbers.size(); i++) {
            bool flag = false;
            string trimmed;
            while (!flag) {
                trimmed = regex_replace(phoneNumbers[i], regex("^\\s+|\\s+$"), "");
                try {
                    if (!regex_match(trimmed, pattern)) {
                        throw invalid_argument("Invalid phone number format");
                    }
                    flag = true;
                }
                catch (const invalid_argument& e) {
                    cout << "Invalid phone number: " << phoneNumbers[i] << endl;
                    cout << "Enter right number: ";
                    while (!flag) {
                        getline(cin, trimmed);
                        try {
                            if (!regex_match(trimmed, pattern)) {
                                throw invalid_argument("Invalid phone number format");
                            }
                            flag = true;
                        }
                        catch (const invalid_argument& e) {
                            cout << "Invalid phone number. Please try again: ";
                        }
                    }
                }
            }
            trimmedPhoneNumber.push_back(trimmed);
        }

        this->phoneNumbers = trimmedPhoneNumber;
    }

    
    string serialize() const {
        stringstream ss;
        ss << firstName << "," << lastName << "," << middleName << "," << city << "," << street << "," << houseNumber << "," << apartmentNumber << "," << birthDate << "," << email;
        for (const auto& phoneNumber : phoneNumbers) {
            ss << "," << phoneNumber;
        }
        return ss.str();
    }

 
    static Contact deserialize(const string& str) {
        stringstream ss(str);
        string firstName, lastName, middleName, city, street, houseNumber, apartmentNumber, birthDate, email, phoneNumber;
        vector<string> phoneNumbers;
        getline(ss, firstName, ',');
        getline(ss, lastName, ',');
        getline(ss, middleName, ',');
        getline(ss, city, ',');
        getline(ss, street, ',');
        getline(ss, houseNumber, ',');
        getline(ss, apartmentNumber, ',');
        getline(ss, birthDate, ',');
        getline(ss, email, ',');
        while (getline(ss, phoneNumber, ',')) {
            if (!phoneNumber.empty()) {
                phoneNumbers.push_back(phoneNumber);
            }
        }
        return Contact(firstName, lastName, middleName, city, street, houseNumber, apartmentNumber, birthDate, email, phoneNumbers);
    }

};