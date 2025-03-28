#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace std;

//Подключение к базе данных
QSqlDatabase ConnectToDB(const QString& con) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", con);
    db.setHostName("localhost");
    db.setDatabaseName("Contacts");
    db.setUserName("postgres");
    db.setPassword("2560247");
    db.setPort(5432);

    if (!db.open()) {
        qDebug() << "Ошибка открытия базы данных " << db.lastError().text();
    }
    else {
        qDebug() << "База данных подключена успешно" << db.connectionName();
    }

    return db;
}

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

    {
        QString lastName;
        QString firstName;
        QString middleName;
        QString address;
        QString birthDate;
        QString email;
        QSqlDatabase db = ConnectToDB("connection");
        QSqlQuery query(db);
        QSqlQuery query_2(db);

        query.exec("SELECT id, last_name, first_name, middle_name, address, birth_date, email FROM contacts;"); // Select id
        while (query.next()) {
            int contactId = query.value(0).toInt();
            QString lastName = query.value(1).toString();
            QString firstName = query.value(2).toString();
            QString middleName = query.value(3).toString();
            QString address = query.value(4).toString();
            QString birthDate = query.value(5).toString();
            QString email = query.value(6).toString();

            query_2.prepare("SELECT phone FROM phone_numbers WHERE contact_id = :contactId;");
            query_2.bindValue(":contactId", contactId);
            query_2.exec();

            QVector<QString> numbers;
            while (query_2.next()) {
                numbers.push_back(query_2.value(0).toString());
            }

            Contact Contact(firstName, lastName, middleName, address, birthDate, email, numbers);
            book.emplace(lastName, Contact);
            Contact.Print();
        }

        db.close();
    }
    QSqlDatabase::removeDatabase("connection");
    emit signal_for_UpdateTable();
}

//Добавление контакта
void MainWindow::on_pushButton_clicked()
{
    QRegularExpression regular_for_name("^[A-Za-zА-Яа-яЁё]+(?:[\\s-][A-Za-zА-Яа-яЁё]+)*$");
    QRegularExpression regular_for_number("^(\\+7|8)?\\d{10}$|^(\\+7|8)?\\(\\d{3}\\)\\d{7}$|^(\\+7|8)?\\(\\d{3}\\)\\d{3}-\\d{2}-\\d{2}$");
    QRegularExpression regular_for_email("(^((([0-9A-Za-z]{1}[-0-9A-Za-z\\.]{1,}[0-9A-Za-z]{1}))@([-A-Za-z]{1,}\\.){1,2}[-A-Za-z]{2,})$)");

    QString firstName = ui->firstName->text();
    QString lastName = ui->lastName->text();
    QString middleName = ui->middleName->text();
    QString address = ui->address->text();
    QString birthDate = ui->birthDate->date().toString("yyyy-MM-dd");
    QString email = ui->email->text();
    QString number_1 = ui->phone->text();


    if (!regular_for_name.match(firstName).hasMatch()) {
        QMessageBox::critical(this, "Ошибка", "Некорректный формат имени!");
        return;
    }
    if (!regular_for_name.match(lastName).hasMatch()) {
        QMessageBox::critical(this, "Ошибка", "Некорректный формат фамилии!");
        return;
    }
    if (!regular_for_name.match(middleName).hasMatch()) {
        QMessageBox::critical(this, "Ошибка", "Некорректный формат отчества!");
        return;
    }
    if (ui->birthDate->date() >= QDate::currentDate()) {
        QMessageBox::critical(this, "Ошибка", "Некорректная дата рождения!");
        return;
    }
    if (!regular_for_email.match(email).hasMatch()) {
        QMessageBox::critical(this, "Ошибка", "Некорректный формат почты!");
        return;
    }

    if (!PhoneCheck(line_edit_vector, regular_for_number, number_1, numbers))
    {
        QMessageBox::critical(this, "Ошибка", "Телефон введен некорректно!");
        numbers.clear();
        return;
    }
    Contact contact;
    contact.lastName = lastName;
    contact.firstName = firstName;
    contact.middleName = middleName;
    contact.address = address;
    contact.birthDate = birthDate;
    contact.email = email;

    QString phone = ui->phone->text();
    if (!phone.isEmpty()) {
        contact.number.push_back(phone);
    }
    // Добавляем дополнительные телефоны
    for (QLineEdit* lineEdit : line_edit_vector) {
        if (!PhoneCheck(line_edit_vector, regular_for_number, number_1, numbers))
        {
            QMessageBox::critical(this, "Ошибка", "Телефон введен некорректно!");
            numbers.clear();
            return;
        }
        if (!lineEdit->text().isEmpty())
            contact.number.push_back(lineEdit->text());
    }
   
    book[lastName] = contact;
    // Подключаемся к базе данных PostgreSQL
    QSqlDatabase db = ConnectToDB("connection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        QSqlDatabase::removeDatabase("connection");
        return;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO contacts (first_name, last_name, middle_name, address, birth_date, email) "
        "VALUES (:firstName, :lastName, :middleName, :address, :birthDate, :email) RETURNING id");
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":middleName", middleName);
    query.bindValue(":address", address);
    query.bindValue(":birthDate", birthDate);
    query.bindValue(":email", email);
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to insert to database: " + query.lastError().text());
        db.close();
        QSqlDatabase::removeDatabase("connection");
        return;
    }
    int contactId;
    if (query.next()) {
        contactId = query.value(0).toInt();
        qDebug() << "contact id: " << contactId;
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to find contact ID in database: " + query.lastError().text());
        db.close();
        QSqlDatabase::removeDatabase("connection");
        return;
    }
    for (const QString& number : contact.number) {
        query.prepare("INSERT INTO phone_numbers (contact_id, phone) VALUES (:contactId, :phone)");
        query.bindValue(":contactId", contactId);
        query.bindValue(":phone", number);
        if (!query.exec()) {
            QMessageBox::critical(this, "Error", "Failed to insert number to database: " + query.lastError().text());
            db.close();
            QSqlDatabase::removeDatabase("connection");
            return;
        }
    }
    // Закрываем соединение с базой данных
    emit signal_for_UpdateTable();
    db.close();
    QSqlDatabase::removeDatabase("connection");

    // Очищаем поля ввода
    ui->firstName->clear();
    ui->lastName->clear();
    ui->middleName->clear();
    ui->address->clear();
    ui->birthDate->setDate(QDate::currentDate());
    ui->email->clear();
    ui->phone->clear();
    //удаляем динамические виджеты
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
}


//Добавление дополнительного телефона
void MainWindow::on_insert_number_clicked()
{
    // Создаем новый горизонтальный макет
    QHBoxLayout* new_layout = new QHBoxLayout;
    QLabel* label_1 = new QLabel("Телефон:");
    QLineEdit* new_line_edit = new QLineEdit;
    // Добавляем виджеты в векторы
    layout_vector.push_back(new_layout);
    label_vector.push_back(label_1);
    line_edit_vector.push_back(new_line_edit);
    // Добавляем виджеты в макет
    new_layout->addWidget(label_1);
    new_layout->addWidget(new_line_edit);
    // Добавляем макет в основной вертикальный макет
    ui->verticalLayout->addLayout(new_layout);

}

//Вывод данных найденного пользователя на экран
void MainWindow::on_searchContact_clicked()
{
    lastName_for_update = ui->lineEdit->text();
    // Подключаемся к базе данных PostgreSQL
    QSqlDatabase db = ConnectToDB("connection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Error", "Failed to connect to database.");
        return;
    }

    // Выполняем запрос к базе данных для получения данных по фамилии
    QSqlQuery query(db);
    query.prepare("SELECT contacts.first_name, contacts.last_name, contacts.middle_name, contacts.address, contacts.birth_date, contacts.email, phone_numbers.phone FROM contacts LEFT JOIN phone_numbers ON contacts.id = phone_numbers.contact_id WHERE contacts.last_name = :last_name");
    query.bindValue(":last_name", lastName_for_update);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query: " + query.lastError().text());
        db.close();
        QSqlDatabase::removeDatabase("connection");
        return;
    }

    bool first_row = true;
    while (query.next())
    {
        if (first_row)
        {
            // Заполняем поля интерфейса данными из базы данных
            ui->firstName->setText(query.value("first_name").toString());
            ui->lastName->setText(query.value("last_name").toString());
            ui->middleName->setText(query.value("middle_name").toString());
            ui->address->setText(query.value("address").toString());
            ui->birthDate->setDate(QDate::fromString(query.value("birth_date").toString(), "yyyy-MM-dd"));
            ui->email->setText(query.value("email").toString());
            ui->phone->setText(query.value("phone").toString());
            first_row = false;
        }
      
    }
    if (first_row) {
        QMessageBox::warning(this, "Предупреждение", "Контакта с такой фамилией нет!");
        ui->lineEdit->clear();
    }
    // Закрываем соединение с базой данных
    db.close();
    QSqlDatabase::removeDatabase("connection");
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
    if (!regular_for_name.match(firstName).hasMatch() ||
        !regular_for_name.match(lastName).hasMatch() ||
        !regular_for_name.match(middleName).hasMatch() ||
        ui->birthDate->date() >= QDate::currentDate() ||
        !regular_for_email.match(email).hasMatch() ||
        !PhoneCheck(line_edit_vector, regular_for_number, number_1, numbers)) {
        QMessageBox::warning(this, "Предупреждение", "Проверьте введенные данные!");
        return;
    }
    // Подключаемся к базе данных PostgreSQL
    QSqlDatabase db = ConnectToDB("connection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Error", "Failed to connect to database: " + db.lastError().text());
        QSqlDatabase::removeDatabase("connection");
        return;
    }
    QSqlQuery query(db);
    int contactId = -1;
    query.prepare("SELECT id FROM contacts WHERE last_name = :lastName_for_update");
    query.bindValue(":lastName_for_update", lastName_for_update);
    if (query.exec() && query.next()) {
        contactId = query.value(0).toInt();
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to find ID of contact");
        db.close();
        QSqlDatabase::removeDatabase("connection");
        return;
    }
    // Обновляем данные в базе данных
    query.prepare("UPDATE contacts SET first_name = :firstName, middle_name = :middleName, "
        "address = :address, birth_date = :birthDate, email = :email WHERE id = :contactId;");
    query.bindValue(":firstName", firstName);
    query.bindValue(":middleName", middleName);
    query.bindValue(":address", address);
    query.bindValue(":birthDate", birthDate);
    query.bindValue(":email", email);
    query.bindValue(":contactId", contactId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update " + query.lastError().text());
        db.close();
        QSqlDatabase::removeDatabase("connection");
        return;
    }

    // Обновляем номера телефонов
    query.prepare("DELETE FROM phone_numbers WHERE contact_id = :contactId;");
    query.bindValue(":contactId", contactId);
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to delete old phone numbers: " + query.lastError().text());
        db.close();
        QSqlDatabase::removeDatabase("connection");
        return;
    }
    for (const QString& number : numbers) {
        query.prepare("INSERT INTO phone_numbers (contact_id, phone) VALUES (:contactId, :phone);");
        query.bindValue(":contactId", contactId);
        query.bindValue(":phone", number);
        if (!query.exec()) {
            QMessageBox::critical(this, "Error", "Failed to insert new phone number: " + query.lastError().text());
            db.close();
            QSqlDatabase::removeDatabase("connection");
            return;
        }
    }
    // Закрываем соединение с базой данных
    db.close();
    QSqlDatabase::removeDatabase("connection");
    // Обновляем локальную структуру
    Contact updatedContact(firstName, lastName, middleName, address, birthDate, email, numbers);
    book[lastName] = updatedContact;
    QMessageBox::information(this, "Успех", "Данные успешно обновлены!");
    emit signal_for_UpdateTable();
}

MainWindow::~MainWindow()
{
    QFile file("contact.txt");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Ошибка открытия файла для записи: " << file.errorString();
        return;
    }
    qDebug() << "Файл открыт для записи";

    {
        QTextStream fout(&file);
        for (auto& element : book) {
            element.second.write(fout);
            fout << "\n";
            if (fout.status() != QTextStream::Ok) {
                qDebug() << "Ошибка записи данных в файл " << fout.status();
                file.close();
                return; 
            }
        }
        qDebug() << "Данные записаны";
    }

    file.close();

    delete ui;
}

