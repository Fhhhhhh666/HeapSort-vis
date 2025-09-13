#include "heapsort.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon appIcon(":/new/prefix1/HeapSort_icon.svg");
    a.setWindowIcon(appIcon);
    heapsort w;
    w.show();
    return a.exec();
}
