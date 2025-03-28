#include "telephone_book.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Telephone_book w;
    w.show();
    return a.exec();
}
