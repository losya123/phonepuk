#include "telephone_book.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include "tablewidgetitem.h"
#include <QSqlError>
#include <QFile>

//Обновление таблицы
void UpdateTable(Ui::Telephone_book* ui, map<QString, Contact>& book) {
    int rowCount = static_cast<int>(book.size());
    ui->tableWidget->setRowCount(rowCount);
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(70);

    // Настройка ширины колонок
    for (int i = 0; i < 7; i++) {
        ui->tableWidget->setColumnWidth(i, 125);
    }

    // Заголовки таблицы
    QStringList headers = { "Фамилия", "Имя", "Отчество", "Адрес", "Дата рождения", "Email", "Номер телефона" };
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    for (const auto& [key, contact] : book) {
        //Объединение номеров в строку
        QString temp_str_for_number = contact.number.join("\n");

        //Заполнение ячеек
        QStringList contact_data = { contact.lastName, contact.firstName, contact.middleName,
                                   contact.address, contact.birthDate, contact.email, temp_str_for_number };
        for (int col = 0; col < contact_data.size(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(contact_data[col]);
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, col, item);
        }
        row++;
    }
}

Telephone_book::Telephone_book(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Telephone_book)
{
    ui->setupUi(this);
    connect(insert_contact, &MainWindow::signal_for_UpdateTable, this, &Telephone_book::slot_for_UpdateTable);
    ui->tableWidget->setColumnCount(7);
    QStringList headers = { "Фамилия", "Имя", "Отчество", "Адрес", "Дата рождения", "Email", "Номер телефона" };
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setSortingEnabled(true);

    UpdateTable(ui, insert_contact->book);
}

Telephone_book::~Telephone_book()
{
    delete insert_contact;
    delete ui;
}

//Открытие окна для добавления контакта
void Telephone_book::on_addContact_clicked()
{
    insert_contact->show();
}


//Удаление контакта
void Telephone_book::on_deleteContact_clicked()
{

    if(insert_contact->book.erase(lastName_for_erase) == 0){
        QMessageBox::warning(this, "Предупреждение", "Для удаления контакта выделите lastName данного контакта!");
    }
    else{
         ui->tableWidget->removeRow(row);

        {
            QSqlDatabase db;
            db = QSqlDatabase::addDatabase("QPSQL", "connection");
            db.setHostName("localhost");
            db.setDatabaseName("Contacts");
            db.setUserName("postgres");
            db.setPassword("2560247");
            db.setPort(5432);

            if(db.open()){
                qDebug("open db for erase");
            }
            else{
                qDebug("no open for erase");
            }
            QSqlQuery query(db);
            int contactId = -1;
            query.prepare("SELECT id FROM contacts WHERE last_name = :lastName;"); 
            query.bindValue(":lastName", lastName_for_erase);

            if (query.exec()) {
                if (query.next()) {
                    contactId = query.value(0).toInt();
                    qDebug() << "Found contact ID: " << contactId << " for lastName: " << lastName_for_erase;
                }
                else {
                    qDebug() << "No contact found with lastName: " << lastName_for_erase;
                }
            }
            else {
                qDebug() << "Error executing query:" << query.lastError().text();
            }
            //1.  Удаление из phone_numbers
            query.prepare("DELETE FROM phone_numbers WHERE contact_id = :contactId;");
            query.bindValue(":contactId", contactId);
            if (!query.exec()) {
                qDebug() << "Ошибка удаления номеров телефонов" << query.lastError().text();
            }

            //2.  Удаление из contacts
            query.prepare("DELETE FROM contacts WHERE id = :contactId;");
            query.bindValue(":contactId", contactId);
            if (!query.exec()) {
                qDebug() << "Ошибка удаления контакта" << query.lastError().text();
            }

            db.close();
        }
        QSqlDatabase::removeDatabase("connection");
    }
}

//Сохранеиние выделенной ячейки
void Telephone_book::on_tableWidget_clicked(const QModelIndex &index)
{
    row = index.row();
    lastName_for_erase = index.data().toString();
}

//Открытие окна поиска
void Telephone_book::on_searchContact_clicked()
{
    search_window = new Search(insert_contact->book);
    search_window->show();
}

void Telephone_book::slot_for_UpdateTable()
{
    UpdateTable(ui, insert_contact->book);
}

void Telephone_book::saveDataToFile() {
    QFile file("contact.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening file for writing:" << file.errorString();
        return;
    }
    QTextStream out(&file);
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableWidget->item(row, col);
            if (item) {
                out << item->text();
            }
            else {
                out << "";
            }
            if (col < ui->tableWidget->columnCount() - 1) {
                out << " ";
            }
        }
        out << "\n";
    }
    file.close();
}
