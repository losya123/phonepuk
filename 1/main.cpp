#include "user.h"
#include "manager.h"

int main() {
    setlocale(LC_ALL, "");
    ContactManager contactManager("contacts.txt");
    int choice;
    do {
        UserInterface ui;
        ui.displayMenu();
        choice = ui.getIntInput("Enter your choice: ");
        switch (choice) {
        case 1: {
            Contact contact = ui.getContactInput();
            contactManager.addContact(contact);
            break;
        }
        case 2: {
            string firstName = ui.getStringInput("Enter first name: ");
            string lastName = ui.getStringInput("Enter last name: ");
            contactManager.deleteContact(firstName, lastName);
            break;
        }
        case 3: {
            string firstName = ui.getStringInput("Enter first name: ");
            string lastName = ui.getStringInput("Enter last name: ");
            Contact contact = ui.getContactInput();
            contactManager.editContact(firstName, lastName, contact);
            break;
        }
        case 4: {
            vector<Contact> contacts = contactManager.getContacts();
            ui.displayContacts(contacts);
            break;
        }
        case 5: {
            ui.displayContacts(contactManager.searchContactsInteractive());
            break;
        }
        case 6: {
            contactManager.sortContactsInteractive();
            break;
        }
        case 7: {
            break;
        }
        default: {
            cout << "Invalid choice\n";
            break;
        }
        }
    } while (choice != 7);

    return 0;
}