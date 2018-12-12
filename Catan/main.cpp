/**
Phi Trang
Cristian Mendivil
Catan Project
Catan Game where players can play against other
players or the computer
*/

#include "mainmenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainMenu w;
    w.show();

    return a.exec();
}
