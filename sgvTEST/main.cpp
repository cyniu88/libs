#include "eye.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eye w;
    w.show();

    return a.exec();
}
