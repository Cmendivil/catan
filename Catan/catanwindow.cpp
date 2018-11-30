#include <iostream>
#include "catanwindow.h"
#include "ui_catanwindow.h"
#include <QPixmap>
#include <QDebug>

CatanWindow::CatanWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CatanWindow)
{
    srand(time(0));
    boardScene = new QGraphicsScene;
    ui->setupUi(this);
    QGraphicsView* boardView = ui->boardView;
    boardView->setScene(boardScene);
    boardView->setSceneRect(0,0,boardView->frameSize().width(),boardView->frameSize().height());

    buildingScene = new QGraphicsScene;


    ui->buildCampsiteButton->setDisabled(true);
    ui->buildSpaceStationButton->setDisabled(true);
    ui->endTurnButton->setDisabled(true);
    ui->diceButton->setDisabled(true);

    // Drawing the grid
//    for(int i = 0; i < boardView->frameSize().width(); i = i + 150) {
//        boardScene->addLine(i, 0, i, boardView->frameSize().height() - 10);
//    }
//    for(int i = 0; i < boardView->frameSize().height(); i = i + 150) {
//        boardScene->addLine(0, i, boardView->frameSize().width() - 10, i);
//    }

    blackhole_ = new QPixmap(":/img/img/blackhole.png");
    oxygen_ = new QPixmap(":/img/img/oxygen.png");
    metal_ = new QPixmap(":/img/img/metal.png");
    stone_ = new QPixmap(":/img/img/stone.png");

    // Generate materials
    for(int i = 0; i < 3;  i++) {
        for(int j = 0; j < 4; j++) {
            ResourceType type = static_cast<ResourceType>(rand() % 3 + 1);
            resources_[i][j].type = type;
            switch (type) {
                case ResourceType::Oxygen:
                    resources_[i][j].picture = oxygen_;
                    break;
                case ResourceType::Stone:
                    resources_[i][j].picture = stone_;
                    break;
                case ResourceType::Metal:
                    resources_[i][j].picture = metal_;
                    break;
            }
              QPixmap item( *resources_[i][j].picture);
              boardScene->addPixmap(item)->setPos(j*150,i*150);

//            QLabel *image = new QLabel(this);
//            image->setPixmap(*resources_[i][j].picture);
//            image->setGeometry(j*150+10,i*150+10,150,150);
        }
    }

    // Plot the blackhole
    int x = rand() % 4;
    int y = rand() % 3;

    resources_[y][x].type = ResourceType::BlackHole;
    resources_[y][x].picture = blackhole_;
    QPixmap item( *resources_[y][x].picture);
    boardScene->addPixmap(item)->setPos(x*150,y*150);
//    QLabel *image = new QLabel(this);
//    image->setPixmap(*resources_[y][x].picture);
//    image->setGeometry(x*150+10,y*150+10,150,150);

    // Assign numbers to spaces
    for(int i = 75; i < boardView->frameSize().width(); i = i + 150) {
        for(int j = 75; j < boardView->frameSize().height(); j = j + 150) {
            //Find Blackhole and skip the numbering
            if(resources_[(j-75)/150][(i-75)/150].type == ResourceType::BlackHole){
                resources_[(j-75)/150][(i-75)/150].active = false;
                continue;
            }


            int num = rand() % 12 + 1;
            while(num == 7 || num == 1) {
                num = rand() % 12 + 1;
            }
            resources_[(j-75)/150][(i-75)/150].number = num;
            QLabel *label = new QLabel(this);
            label->setText(QString::number(num));
            label->setAlignment(Qt::AlignCenter);
            label->setStyleSheet("font: 30px; background: white;");
            label->setGeometry(i,j,35,35);

        }
    }

    QGraphicsView* buildingView = ui->buildingView;
    buildingView->setScene(buildingScene);
    buildingView->setSceneRect(0,0,buildingView->frameSize().width(),buildingView->frameSize().height());

    // Create Players
    Player* p1 = new Player(1, QColor(255,0,0));
    players_.push_back(p1);
    Player* p2 = new Player(2, QColor(0,255,0));
    players_.push_back(p2);
    Player* p3 = new Player(3, QColor(0,0, 255));
    players_.push_back(p3);


    // Generate building spaces
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 5; j++){
            Campsite * item =  new Campsite(j, i, NULL);
            buildings_[i][j] = item;
            buildingScene->addItem(item);
            connect(item, &Campsite::Click, this, &CatanWindow::ClickSlot);
        }
    }

    currentPlayer = players_[0];
    mode_ = "first";
    ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- click in a white square to build a campsite"));
}

CatanWindow::~CatanWindow()
{
    delete ui;
}

void CatanWindow::firstTurns(){
    if(currentPlayer == players_[players_.size()-1]){
        mode_ = "";
        return on_endTurnButton_clicked();
    }
    currentPlayer = players_[currentPlayer->getId() % players_.size()];
    ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- click in a white square to build a campsite"));

    if(currentPlayer == players_[1]){
        ui->p1Label->setFrameShape(QFrame::NoFrame);
        ui->p2Label->setLineWidth(3);
        ui->p2Label->setFrameShape(QFrame::Box);
    }
    if(currentPlayer == players_[2]){
        ui->p2Label->setFrameShape(QFrame::NoFrame);
        ui->p3Label->setLineWidth(3);
        ui->p3Label->setFrameShape(QFrame::Box);
    }

}

void CatanWindow::ClickSlot(Campsite * c){
    if(mode_ == "camp"){
        if(c->getOwner()==NULL){
            c->setColor(currentPlayer->getColor());
            c->setOwner(currentPlayer);
        }
    }
    else if(mode_ == "first"){
        if(c->getOwner()==NULL){
            c->setColor(currentPlayer->getColor());
            c->setOwner(currentPlayer);
            firstTurns();
        }

    }
    else{
       mode_ = "";
    }
}

void CatanWindow::on_diceButton_clicked()
{
    int diceOne = rand() % 6 + 1;
    int diceTwo = rand() % 6 + 1;
    diceRoll_ = diceOne + diceTwo;

    ui->diceOne->setText(QString::number(diceOne));
    ui->diceTwo->setText(QString::number(diceTwo));

    ui->diceButton->setDisabled(true);
    ui->buildCampsiteButton->setDisabled(true);
    ui->buildSpaceStationButton->setDisabled(true);
    ui->endTurnButton->setDisabled(false);


    distributeResources();
    enableButtons();
    ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- choose one of your available actions"));

}

void CatanWindow::distributeResources()
{
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 4; j++) {
            ResourceType material = resources_[i][j].type;
            if(resources_[i][j].number == diceRoll_ && resources_[i][j].active) {
                if(buildings_[i][j]) {
                    buildings_[i][j]->getOwner()->increaseResource(material, buildings_[i][j]->getReward());
                }
                if(buildings_[i + 1][j]) {
                    buildings_[i + 1][j]->getOwner()->increaseResource(material, buildings_[i + 1][j]->getReward());
                }
                if(buildings_[i][j + 1]) {
                    buildings_[i][j + 1]->getOwner()->increaseResource(material, buildings_[i][j + 1]->getReward());
                }
                if(buildings_[i + 1][j + 1]) {
                    buildings_[i + 1][j + 1]->getOwner()->increaseResource(material, buildings_[i + 1][j + 1]->getReward());
                }
            }
        }
    }

    updateBoard();

}

void CatanWindow::updateBoard() {
    ui->p1_metal->setText(QString("Metal: ") + QString::number(players_[0]->getMetal()));
    ui->p1_stone->setText(QString("Stone: ") + QString::number(players_[0]->getStone()));
    ui->p1_oxygen->setText(QString("Oxygen: ") + QString::number(players_[0]->getOxygen()));


    ui->p2_metal->setText(QString("Metal: ") + QString::number(players_[1]->getMetal()));
    ui->p2_stone->setText(QString("Stone: ") + QString::number(players_[1]->getStone()));
    ui->p2_oxygen->setText(QString("Oxygen: ") + QString::number(players_[1]->getOxygen()));

    ui->p3_metal->setText(QString("Metal: ") + QString::number(players_[2]->getMetal()));
    ui->p3_stone->setText(QString("Stone: ") + QString::number(players_[2]->getStone()));
    ui->p3_oxygen->setText(QString("Oxygen: ") + QString::number(players_[2]->getOxygen()));
}


void CatanWindow::on_endTurnButton_clicked()
{
    currentPlayer = players_[currentPlayer->getId() % players_.size()];
    ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- roll dice"));
    if(currentPlayer == players_[0]){
        ui->p3Label->setFrameShape(QFrame::NoFrame);
        ui->p1Label->setFrameShape(QFrame::Box);
    }
    if(currentPlayer == players_[1]){
        ui->p1Label->setFrameShape(QFrame::NoFrame);
        ui->p2Label->setLineWidth(3);
        ui->p2Label->setFrameShape(QFrame::Box);
    }
    if(currentPlayer == players_[2]){
        ui->p2Label->setFrameShape(QFrame::NoFrame);
        ui->p3Label->setLineWidth(3);
        ui->p3Label->setFrameShape(QFrame::Box);
    }

    ui->diceButton->setDisabled(false);
    ui->buildCampsiteButton->setDisabled(true);
    ui->buildSpaceStationButton->setDisabled(true);
    ui->endTurnButton->setDisabled(true);
}

void CatanWindow::on_buildCampsiteButton_clicked()
{
    mode_ = "camp";
}


void CatanWindow::on_buildSpaceStationButton_clicked()
{
    mode_ = "station";
}

void CatanWindow::enableButtons(){
    int metal = currentPlayer->getMetal();
    int stone = currentPlayer->getStone();
    int oxygen = currentPlayer->getOxygen();
    if(oxygen >= 1 && stone >=2){
        ui->buildCampsiteButton->setDisabled(false);
    }
    if(oxygen >= 2 && stone >=2 && metal >= 2){
        ui->buildSpaceStationButton->setDisabled(false);
    }
}
