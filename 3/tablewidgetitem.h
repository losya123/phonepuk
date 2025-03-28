#pragma once
#ifndef TABLEWIDGETITEM_H
#define TABLEWIDGETITEM_H
#include <QTableWidgetItem>
#include <QDate>


class TableWidgetItem : public QTableWidgetItem
{
public:
    explicit TableWidgetItem(const QString& text, int type);
    bool compareDates(const QTableWidgetItem& other, const QString& dateFormat) const;
    bool compareStrings(const QTableWidgetItem& other) const;
    bool operator< (const QTableWidgetItem& other) const;

};

#endif
