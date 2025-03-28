#include "contact.h"

Contact::Contact(QString firstName, QString lastName, QString middleName, QString address, QString birthDate, QString email,
    QVector<QString> number) {
    this->firstName = firstName;
    this->lastName = lastName;
    this->middleName = middleName;
    this->address = address;
    this->birthDate = birthDate;
    this->email = email;
    this->number = number;
}

void Contact::Print() {
    qDebug() << lastName << "\t" << firstName << "\t" << middleName
        << "\t" << address << "\t" << birthDate << "\t" << email << "\t";
    for (int i = 0; i < number.size(); i++) {
        qDebug() << i + 1 << ", " << number[i] << "\t";
    }
    qDebug() << "\n";
}


void Contact::write(QTextStream& fout) {
    fout << firstName << " ";
    fout << lastName << " ";
    fout << middleName << " ";
    fout << address << " ";
    fout << birthDate << " ";
    fout << email << " ";
    size_t l = number.size();
    fout << l << " ";
    for (int i = 0; i < number.size(); i++) {
        fout << number[i] << " ";
    }
}

void Contact::read(QTextStream& fin) {
    fin >> firstName;
    fin >> lastName;
    fin >> middleName;
    fin >> address;
    fin >> birthDate;
    fin >> email;
    size_t l;
    fin >> l;
    for (int i = 0; i < l; i++) {
        QString tmp;
        fin >> tmp;
        number.push_back(tmp);
    }
}


