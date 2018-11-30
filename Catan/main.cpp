#include "catanwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CatanWindow w;
    w.show();

    return a.exec();
}
