#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

using namespace std;

//Проверка на соответствие регулярному выражению
bool isValidNumber(const QString& number, const QRegularExpression& regex) {
    return regex.match(number).hasMatch();
}

//Добавление номера
void addNumberIfValid(const QString& number, QVector<QString>& numbers) {
    if (!number.isEmpty()) {
        numbers.push_back(number);
    }
}

//Проверка номера на коррректность
bool PhoneCheck(vector<QLineEdit*> line_edit_vector, QRegularExpression regular_for_number, QString number_1, QVector<QString>& numbers) {
    if (regular_for_number.match(number_1).hasMatch()) {
        if (number_1 != "") {
            numbers.push_back(number_1);
        }
        for (int i = 0; i < line_edit_vector.size(); i++) {
            if (regular_for_number.match(line_edit_vector[i]->text()).hasMatch()) {
                if (line_edit_vector[i]->text() != "") {
                    numbers.push_back(line_edit_vector[i]->text());
                }
            }
            else {
                return false;
            }
        }
        return true;
    }
    else {
        return false;
    }
}
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Чтение данных из файла
    QFile file("contact.txt");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Ошибка открытия файла для чтения";
        file.open(QFile::WriteOnly | QFile::Text);
    }
    else {
        qDebug() << "Файл открыт для чтения";

        QTextStream fin(&file);
        if (!fin.atEnd()) {
            size_t size;
            fin >> size;
            for (int i = 0; i < size; i++) {
                Contact Contact;
                Contact.read(fin);
                book.emplace(Contact.lastName, Contact);
            }
        }
        qDebug() << "Данные считаны";
    }
    file.flush();
    file.close();
}

MainWindow::~MainWindow()
{
    QFile file("contact.txt");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Ошибка открытия файла для записи ";
    }
    else {
        qDebug() << "Файл открыт для записи";

        //Запись данных в файл
        QTextStream fout(&file);
        size_t l = book.size();
        fout << l << " ";
        for (auto element : book) {
            element.second.write(fout);
        }
        qDebug() << "Данные записаны";
    }
    file.flush();
    file.close();

    delete ui;
}
//Добавление контакта
void MainWindow::on_pushButton_clicked()
{
    QRegularExpression regular_for_name("(^[А-Яа-яA-Za-z]{1}[-0-9А-Яа-яA-Za-z\\.]{0,}[0-9А-Яа-яA-Za-z]{1}$)");
    QRegularExpression regular_for_number("(^((8|\\+7)[\\- ]?)?(\\(?\\d{3}\\)?[\\- ]?)?[\\d\\- ]{7,10}$)");
    QRegularExpression regular_for_email("(^((([0-9A-Za-z]{1}[-0-9A-Za-z\\.]{1,}[0-9A-Za-z]{1}))@([-A-Za-z]{1,}\\.){1,2}[-A-Za-z]{2,})$)");

    QString firstName = ui->firstName->text();
    QString lastName = ui->lastName->text();
    QString middleName = ui->middleName->text();
    QString address = ui->address->text();
    QString birthDate = ui->birthDate->date().toString("dd.MM.yyyy");
    QString email = ui->email->text();
    QString number_1 = ui->phone->text();

    QVector<QString> numbers;
    if (!regular_for_name.match(firstName).hasMatch()) {
        QMessageBox::warning(this, "Предупреждение", "Некорректный формат имени!");
        return;
    }
    if (!regular_for_name.match(lastName).hasMatch()) {
        QMessageBox::warning(this, "Предупреждение", "Некорректный формат фамилии!");
        return;
    }
    if (!regular_for_name.match(middleName).hasMatch()) {
        QMessageBox::warning(this, "Предупреждение", "Некорректный формат отчества!");
        return;
    }
    if (ui->birthDate->date() >= QDate::currentDate()) {
        QMessageBox::warning(this, "Предупреждение", "Дата рождения не может быть больше сегодняшнего дня!");
        return;
    }
    if (!regular_for_email.match(email).hasMatch()) {
        QMessageBox::warning(this, "Предупреждение", "Некорректный формат почты!");
        return;
    }
    if (!PhoneCheck(line_edit_vector, regular_for_number, number_1, numbers)) {
        QMessageBox::warning(this, "Предупреждение", "Телефон введен некорректно!");
        return;
    }
    Contact tmp(firstName, lastName, middleName, address, birthDate, email, numbers);
    book.emplace(lastName, tmp);

    ui->statusbar->showMessage("Контакт зарегистрирован!");

    //отчистка полей ввода
    ui->firstName->clear();
    ui->lastName->clear();
    ui->middleName->clear();
    ui->birthDate->setDate(QDate::fromString("dd.MM.yyyy"));
    ui->address->clear();
    ui->email->clear();
    ui->phone->clear();
    ui->lineEdit->clear();

    for (int i = 0; i < line_edit_vector.size(); i++) {
        line_edit_vector[i]->clear();
        delete line_edit_vector[i];
        delete layout_vector[i];
        delete label_vector[i];
    }

    line_edit_vector.clear();
    layout_vector.clear();
    label_vector.clear();
    numbers.clear();

    emit signal_for_update_table();
}



//Добавление дополнительного телефона
void MainWindow::on_insert_number_clicked()
{
    QHBoxLayout* new_layout = new QHBoxLayout;
    QLabel* label_1 = new QLabel("Телефон:");
    QLineEdit* new_line_edit = new QLineEdit;

    layout_vector.push_back(new_layout);
    label_vector.push_back(label_1);
    line_edit_vector.push_back(new_line_edit);

    new_layout->addWidget(label_1);
    new_layout->addWidget(new_line_edit);

    ui->verticalLayout->addLayout(new_layout);
}

//Вывод данных найденного пользователя на экран
void MainWindow::on_searchContact_clicked()
{
    lastName_for_update = ui->lineEdit->text();

    if (book.find(lastName_for_update) != book.end()) {
        ui->firstName->setText(book[lastName_for_update].firstName);
        ui->lastName->setText(book[lastName_for_update].lastName);
        ui->middleName->setText(book[lastName_for_update].middleName);
        ui->address->setText(book[lastName_for_update].address);
        ui->birthDate->setDate(QDate::fromString(book[lastName_for_update].birthDate, "dd.MM.yyyy"));
        ui->email->setText(book[lastName_for_update].email);
        ui->phone->setText(book[lastName_for_update].number[0]);

        for (int i = 1; i < book[lastName_for_update].number.size(); i++) {

            QHBoxLayout* new_layout = new QHBoxLayout;
            QLabel* label_1 = new QLabel("Телефон:");
            QLineEdit* new_line_edit = new QLineEdit;

            layout_vector.push_back(new_layout);
            label_vector.push_back(label_1);
            line_edit_vector.push_back(new_line_edit);

            new_layout->addWidget(label_1);
            new_layout->addWidget(new_line_edit);
            new_line_edit->setText(book[lastName_for_update].number[i]);

            ui->verticalLayout->addLayout(new_layout);
        }

    }
    else {
        QMessageBox::warning(0, "Предупреждение", "Контакта с такой фамилией нет!");
        ui->lineEdit->clear();
    }
}

//Обновление данных пользователя
void MainWindow::on_pushButton_3_clicked()
{
    QRegularExpression regular_for_name("(^[А-Яа-яA-Za-z]{1}[-0-9А-Яа-яA-Za-z\\.]{0,}[0-9А-Яа-яA-Za-z]{1}$)");
    QRegularExpression regular_for_number("(^((8|\\+7)[\\- ]?)?(\\(?\\d{3}\\)?[\\- ]?)?[\\d\\- ]{7,10}$)");
    QRegularExpression regular_for_email("(^((([0-9A-Za-z]{1}[-0-9A-Za-z\\.]{1,}[0-9A-Za-z]{1}))@([-A-Za-z]{1,}\\.){1,2}[-A-Za-z]{2,})$)");

    QString firstName = ui->firstName->text();
    QString lastName = ui->lastName->text();
    QString middleName = ui->middleName->text();
    QString address = ui->address->text();
    QString birthDate = ui->birthDate->date().toString("yyyy-MM-dd");
    QString email = ui->email->text();
    QString number_1 = ui->phone->text();
    QVector<QString> numbers;
    if (!regular_for_name.match(firstName).hasMatch()) {
        QMessageBox::warning(this, "Предупреждение", "Некорректный формат имени!");
        return;
    }
    if (!regular_for_name.match(lastName).hasMatch()) {
        QMessageBox::warning(this, "Предупреждение", "Некорректный формат фамилии!");
        return;
    }
    if (!regular_for_name.match(middleName).hasMatch()) {
        QMessageBox::warning(this, "Предупреждение", "Некорректный формат отчества!");
        return;
    }
    if (ui->birthDate->date() >= QDate::currentDate()) {
        QMessageBox::warning(this, "Предупреждение", "Дата рождения не может быть больше сегодняшнего дня!");
        return;
    }
    if (!regular_for_email.match(email).hasMatch()) {
        QMessageBox::warning(this, "Предупреждение", "Некорректный формат почты!");
        return;
    }
    if (!PhoneCheck(line_edit_vector, regular_for_number, number_1, numbers)) {
        QMessageBox::warning(this, "Предупреждение", "Телефон введен некорректно!");
        return;
    }
    book[lastName_for_update].firstName = firstName;
    book[lastName_for_update].lastName = lastName;
    book[lastName_for_update].middleName = middleName;
    book[lastName_for_update].address = address;
    book[lastName_for_update].birthDate = birthDate;
    book[lastName_for_update].email = email;
    book[lastName_for_update].number = numbers;
    if (lastName_for_update != ui->lastName->text()) {
        auto it = book.find(lastName_for_update);
        book[ui->lastName->text()] = (*it).second;
        book.erase(it);
    }
    QMessageBox::information(this, "Успех", "Информация о контакте обновлена!");

    //отчистка полей ввода
    ui->firstName->clear();
    ui->lastName->clear();
    ui->middleName->clear();
    ui->birthDate->setDate(QDate::fromString("01.01.2000", "dd.MM.yyyy"));
    ui->address->clear();
    ui->email->clear();
    ui->phone->clear();
    ui->lineEdit->clear();
    //очищаем динамические виджеты
    for (auto layout : layout_vector) {
        delete layout;
    }
    for (auto label : label_vector) {
        delete label;
    }
    for (auto line_edit : line_edit_vector) {
        delete line_edit;
    }

    layout_vector.clear();
    label_vector.clear();
    line_edit_vector.clear();
    emit signal_for_update_table();
}
