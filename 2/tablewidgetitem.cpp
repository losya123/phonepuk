#include "tablewidgetitem.h"

//Вызов конструктора базоваго класса
TableWidgetItem::TableWidgetItem(const QString& text, int type = Type) :
    QTableWidgetItem(text, type)
{
}

//Сравнение дат
bool TableWidgetItem::compareDates(const QTableWidgetItem& other, const QString& dateFormat) const {
    QDate thisDate = QDate::fromString(this->text(), dateFormat);
    QDate otherDate = QDate::fromString(other.text(), dateFormat);

    return otherDate > thisDate;
}

//Сравнение строк
bool TableWidgetItem::compareStrings(const QTableWidgetItem& other) const {
    return other.text() > this->text();
}

//Переопределение оператора сравнения для дат
bool TableWidgetItem::operator < (const QTableWidgetItem& other) const
{
    if (other.column() == 4) {
        return compareDates(other, "dd.MM.yyyy");
    }

    return compareStrings(other);
}
