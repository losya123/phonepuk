#include "search.h"
#include "ui_search.h"
#include <QMessageBox>
#include <QDebug>

Search::Search(std::map<QString, Contact> book, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Search)
{
    this->book = book;
    ui->setupUi(this);

    // Заполнение ComboBox значениями
    ui->comboBox->addItem("Фамилия");
    ui->comboBox->addItem("Имя");
    ui->comboBox->addItem("Отчество");
    ui->comboBox->addItem("Адрес");
    ui->comboBox->addItem("Дата рождения");
    ui->comboBox->addItem("Email");
    ui->comboBox->addItem("Номер телефона");
}

Search::~Search()
{
    delete ui;
}

//Вывод найденого контакта
void Search::on_pushButton_clicked()
{
    QString searchText = ui->lineEdit->text();
    QString searchParameter = ui->comboBox->currentText();
    QString ans;
    bool found = false;
    for (const auto& [key, contact] : book) {
        if (searchParameter == "Фамилия" && contact.lastName == searchText) {
            found = true;
            ans = contact.lastName + "\t" + contact.firstName + "\t" +
                contact.middleName + "\n" + contact.address + "\t" +
                contact.birthDate + "\t" + contact.email + "\n";
            for (const QString& number : contact.number) {
                ans += number;
                ans += "\n";
            }
            break;
        }
        else if (searchParameter == "Имя" && contact.firstName == searchText) {
            found = true;
            ans = contact.lastName + "\t" + contact.firstName + "\t" +
                contact.middleName + "\n" + contact.address + "\t" +
                contact.birthDate + "\t" + contact.email + "\n";
            for (const QString& number : contact.number) {
                ans += number;
                ans += "\n";
            }
            break;
        }
        else if (searchParameter == "Отчество" && contact.middleName == searchText) {
            found = true;
            ans = contact.lastName + "\t" + contact.firstName + "\t" +
                contact.middleName + "\n" + contact.address + "\t" +
                contact.birthDate + "\t" + contact.email + "\n";
            for (const QString& number : contact.number) {
                ans += number;
                ans += "\n";
            }
            break;
        }
        else if (searchParameter == "Адрес" && contact.address == searchText) {
            found = true;
            ans = contact.lastName + "\t" + contact.firstName + "\t" +
                contact.middleName + "\n" + contact.address + "\t" +
                contact.birthDate + "\t" + contact.email + "\n";
            for (const QString& number : contact.number) {
                ans += number;
                ans += "\n";
            }
            break;
        }
        else if (searchParameter == "Дата рождения" && contact.birthDate == searchText) {
            found = true;
            ans = contact.lastName + "\t" + contact.firstName + "\t" +
                contact.middleName + "\n" + contact.address + "\t" +
                contact.birthDate + "\t" + contact.email + "\n";
            for (const QString& number : contact.number) {
                ans += number;
                ans += "\n";
            }
            break;
        }
        else if (searchParameter == "Email" && contact.email == searchText) {
            found = true;
            ans = contact.lastName + "\t" + contact.firstName + "\t" +
                contact.middleName + "\n" + contact.address + "\t" +
                contact.birthDate + "\t" + contact.email + "\n";
            for (const QString& number : contact.number) {
                ans += number;
                ans += "\n";
            }
            break;
        }
        else if (searchParameter == "Номер телефона") {
            for (const QString& number : contact.number) {
                if (number == searchText) {
                    found = true;
                    ans = contact.lastName + "\t" + contact.firstName + "\t" +
                        contact.middleName + "\n" + contact.address + "\t" +
                        contact.birthDate + "\t" + contact.email + "\n";
                    for (const QString& number_1 : contact.number) {
                        ans += number_1;
                        ans += "\n";
                    }
                    break;
                }

            }
            if (found)
                break;
        }
    }

    if (found) {
        ui->answer->setText(ans);
        ui->lineEdit->clear();
    }
    else {
        QMessageBox::warning(0, "Предупреждение", "Контакта с таким параметром не найдено!");
    }
}
