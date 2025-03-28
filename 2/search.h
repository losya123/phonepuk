#pragma once
#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include "Contact.h"

namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(std::map<QString, Contact> book, QWidget *parent = nullptr);
    ~Search();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Search *ui;

    std::map<QString, Contact> book;

    QString lastName_for_search;
};

#endif // SEARCH_H
