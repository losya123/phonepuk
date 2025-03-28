#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Contact.h"
#include <map>
#include <QRegularExpression>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDate>


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::map<QString, Contact> book;
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_insert_number_clicked();

    void on_pushButton_3_clicked();

    void on_searchContact_clicked();

private:
    Ui::MainWindow* ui;

    QString lastName_for_update;//фамилия, которую вводит пользователь для обновления данных о контакте

    QVector<QString> numbers;// вспомогательный вектор с номерами

    //вспомогательные вектора для добавления дополнительных телефонов контактов
    vector<QHBoxLayout*> layout_vector;
    vector<QLabel*> label_vector;
    vector<QLineEdit*> line_edit_vector;

    const QDate current_date = QDate::currentDate();
signals:
    void signal_for_update_table();
};
#endif // MAINWINDOW_H
