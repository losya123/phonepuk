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
#include <QSqlDatabase>
#include <QSqlQuery>


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    map<QString, Contact> book;
    MainWindow(QWidget* parent = nullptr);


    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_insert_number_clicked();

    void on_searchContact_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow* ui;

    QString lastName_for_update;//фамилия для обновления данных

    QVector<QString> numbers;//вспомогательный вектор с номерами

    //Векторы для доп.телефонов
    vector<QHBoxLayout*> layout_vector;
    vector<QLabel*> label_vector;
    vector<QLineEdit*> line_edit_vector;

    const QDate current_date = QDate::currentDate();
signals:
    void signal_for_UpdateTable();
};
#endif // MAINWINDOW_H
