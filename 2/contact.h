#ifndef CONTACT_H
#define CONTACT_H
#include <QVector>
#include <QTextStream>
#include <QDebug>
using namespace std;

struct Contact {
    QString firstName;
    QString lastName;
    QString middleName;
    QString address;
    QString birthDate;
    QString email;
    QVector<QString> number;

    Contact(QString firstName = "", QString lastName = "", QString middleName = "", QString address = "", QString birthDate = "",
        QString email = "", QVector<QString> number = {});
    void Print();
    void write(QTextStream& fout);
    void read(QTextStream& fin);
};


#endif // CONTACT_H