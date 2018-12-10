#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include "catanwindow.h"
namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void on_OnePlayer_clicked();

    void on_TwoPlayer_clicked();

    void on_ThreePlayer_clicked();

private:
    Ui::MainMenu *ui;
    CatanWindow *board;
};

#endif // MAINMENU_H
