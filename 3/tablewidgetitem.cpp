#include "tablewidgetitem.h"

//����� ������������ �������� ������
TableWidgetItem::TableWidgetItem(const QString& text, int type = Type) :
    QTableWidgetItem(text, type)
{
}

//��������� ���
bool TableWidgetItem::compareDates(const QTableWidgetItem& other, const QString& dateFormat) const {
    QDate thisDate = QDate::fromString(this->text(), dateFormat);
    QDate otherDate = QDate::fromString(other.text(), dateFormat);

    return otherDate > thisDate;
}

//��������� �����
bool TableWidgetItem::compareStrings(const QTableWidgetItem& other) const {
    return other.text() > this->text();
}

//��������������� ��������� ��������� ��� ���
bool TableWidgetItem::operator < (const QTableWidgetItem& other) const
{
    if (other.column() == 4) {
        return compareDates(other, "dd.MM.yyyy");
    }

    return compareStrings(other);
}
