#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "catanwindow.h"
#include "player.h"
#include <QDebug>

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

}

MainMenu::~MainMenu()
{
    delete ui;
}

/**
    1 player option
    @param
    @return
*/
void MainMenu::on_OnePlayer_clicked()
{
    Player* p1 = new Player(1,QColor(255,0,0),true);
    Player* p2 = new Player(2,QColor(0,255,0),false);
    Player* p3 = new Player(3,QColor(0,0,255),false);
    std::vector<Player*> players;
    players.push_back(p1);
    players.push_back(p2);
    players.push_back(p3);
    board = new CatanWindow(players);
    ui->stackedWidget->insertWidget(1,board);
    ui->stackedWidget->setCurrentIndex(1);
}

/**
    2 player option
    @param
    @return
*/
void MainMenu::on_TwoPlayer_clicked()
{
    Player* p1 = new Player(1,QColor(255,0,0),true);
    Player* p2 = new Player(2,QColor(0,255,0),true);
    Player* p3 = new Player(3,QColor(0,0,255),false);
    std::vector<Player*> players;
    players.push_back(p1);
    players.push_back(p2);
    players.push_back(p3);
    board = new CatanWindow(players);
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->insertWidget(1,board);
    ui->stackedWidget->setCurrentIndex(1);
}

/**
    3 player option
    @param
    @return
*/
void MainMenu::on_ThreePlayer_clicked()
{
    Player* p1 = new Player(1,QColor(255,0,0),true);
    Player* p2 = new Player(2,QColor(0,255,0),true);
    Player* p3 = new Player(3,QColor(0,0,255),true);
    std::vector<Player*> players;
    players.push_back(p1);
    players.push_back(p2);
    players.push_back(p3);
    board = new CatanWindow(players);
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->insertWidget(1,board);
    ui->stackedWidget->setCurrentIndex(1);
}
