#pragma once
#ifndef TELEPHONE_BOOK_H
#define TELEPHONE_BOOK_H

#include <QDialog>
#include "mainwindow.h"
#include "search.h"
namespace Ui {
class Telephone_book;
}

class Telephone_book : public QDialog
{
    Q_OBJECT

public:
    explicit Telephone_book(QWidget *parent = nullptr);
    ~Telephone_book();
    void saveDataToFile();

private slots:
    void on_addContact_clicked();

    void on_deleteContact_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_searchContact_clicked();
public slots:
    void slot_for_UpdateTable();

private:
    Ui::Telephone_book *ui;
    MainWindow* insert_contact = new MainWindow;
    Search* search_window;

    int row;
    QString lastName_for_erase;

};
void UpdateTable(Ui::Telephone_book* ui, std::map<QString, Contact>& book);
#endif
