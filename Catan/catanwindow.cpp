#include <iostream>
#include "catanwindow.h"
#include "ui_catanwindow.h"
#include <QPixmap>
#include <QDebug>

/**
    Catan Window Contructor
    @param A vector of players playing, and a QWidget
    @return
*/
CatanWindow::CatanWindow(std::vector<Player*> players, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CatanWindow)
{
    srand(time(0));

    // set up views
    boardScene = new QGraphicsScene;
    lineScene = new QGraphicsScene;
    ui->setupUi(this);
    QGraphicsView* boardView = ui->boardView;
    boardView->setScene(boardScene);
    boardView->setSceneRect(0,0,boardView->frameSize().width(),boardView->frameSize().height());
    QGraphicsView* lineView = ui->lineView;
    lineView->setScene(lineScene);
    lineView->setSceneRect(0,0,lineView->frameSize().width(),lineView->frameSize().height());

    buildingScene = new QGraphicsScene;


    // disable and enable buttons
    ui->buildCampsiteButton->setDisabled(true);
    ui->buildSpaceStationButton->setDisabled(true);
    ui->endTurnButton->setDisabled(true);
    ui->diceButton->setDisabled(true);

    // load images
    blackhole_ = new QPixmap(":/img/img/blackhole.png");
    oxygen_ = new QPixmap(":/img/img/oxygen.png");
    metal_ = new QPixmap(":/img/img/metal.png");
    stone_ = new QPixmap(":/img/img/stone.png");

    // Plot the blackhole
    int x = rand() % 4;
    int y = rand() % 3;

    resources_[y][x].type = ResourceType::BlackHole;
    resources_[y][x].picture = blackhole_;
    resources_[y][x].active = false;
    QPixmap item( *resources_[y][x].picture);
    boardScene->addPixmap(item)->setPos(x*150,y*150);

    // Generate materials
    for(int i = 0; i < 3;  i++) {
        for(int j = 0; j < 4; j++) {
            if (resources_[i][j].type != ResourceType::BlackHole){
                int num = rand() % 100;
                resources_[i][j].active = true;
                if(num < 15) {
                    resources_[i][j].picture = metal_;
                    resources_[i][j].type = ResourceType::Metal;

                } else if(num < 60) {
                    resources_[i][j].picture = stone_;
                    resources_[i][j].type = ResourceType::Stone;

                } else if(num < 100) {
                    resources_[i][j].picture = oxygen_;
                    resources_[i][j].type = ResourceType::Oxygen;

                }
                  QPixmap item( *resources_[i][j].picture);
                  boardScene->addPixmap(item)->setPos(j*150,i*150);
            }
        }
    }


    QGraphicsView* buildingView = ui->buildingView;
    buildingView->setScene(buildingScene);
    buildingView->setSceneRect(0,0,buildingView->frameSize().width(),buildingView->frameSize().height());

    // assign numbers to spaces
    for(int i = 75; i < buildingView->frameSize().width(); i = i + 150) {
        for(int j = 75; j < buildingView->frameSize().height(); j = j + 150) {
            // find blackhole and skip the numbering
            if(resources_[(j-75)/150][(i-75)/150].type == ResourceType::BlackHole){
                continue;
            }


            int num = rand() % 12 + 1;
            while(num == 7 || num == 1) {
                num = rand() % 12 + 1;
            }
            // assign resources spots a MyLabel, number, text, etc.
            resources_[(j-75)/150][(i-75)/150].number = num;
            resources_[(j-75)/150][(i-75)/150].labelNumber = new MyLabel();
            resources_[(j-75)/150][(i-75)/150].labelNumber->setText(QString::number(num));
            resources_[(j-75)/150][(i-75)/150].labelNumber->setAlignment(Qt::AlignCenter);
            resources_[(j-75)/150][(i-75)/150].labelNumber->setStyleSheet("font: 30px; background: white;");
            resources_[(j-75)/150][(i-75)/150].labelNumber->setGeometry(i,j,35,35);
            buildingScene->addWidget(resources_[(j-75)/150][(i-75)/150].labelNumber);
            connect(resources_[(j-75)/150][(i-75)/150].labelNumber, &MyLabel::Robber, this, &CatanWindow::NumberClicked);
        }
    }

    // get players
    players_ = players;


    // generate building spaces
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 5; j++){
            Campsite * item =  new Campsite(j, i, NULL);
            buildings_[i][j] = item;
            buildingScene->addItem(item);
            connect(item, &Campsite::Click, this, &CatanWindow::ClickSlot);
        }
    }
    lastRobbed = NULL;
    materials_distributed_ = 0;
    currentPlayer = players_[0];
    mode_ = "first";
    ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- click in a white square to build a campsite"));
}

/**
    Catan Window Destructor
    @param
    @return
*/
CatanWindow::~CatanWindow()
{
    delete ui;
}

/**
    Go over the first turn for each player. Each player
    will select a campsite to build
    @param
    @return
*/
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
    if(!currentPlayer->isHuman()){
        int i = rand() % 4;
        int j = rand() % 5;
        while(!goodPlace(i,j)){
            i = rand() % 4;
            j = rand() % 5;
            while(buildings_[i][j]->getOwner() != NULL){
                i = rand() % 4;
                j = rand() % 5;
            }
        }
        ClickSlot(buildings_[i][j]);
    }
}

/**
    Slot to modify building when cliked. Checks the mode
    of the game (camp, station)
    @param Campsite pointer
    @return
*/
void CatanWindow::ClickSlot(Campsite * c){

    // load instructions
    ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- choose one of your available actions"));

    // if camp mode, update for campsite
    if(mode_ == "camp"){
        if(c->getOwner()==NULL){
            c->setColor(currentPlayer->getColor());
            c->setOwner(currentPlayer);
            mode_ = "";
            currentPlayer->increaseResource(ResourceType::Stone,-2);
            currentPlayer->increaseResource(ResourceType::Oxygen,-1);
            currentPlayer->increasePoints();
            currentPlayer->increaseUsed(3);
            updateBoard();
            enableButtons();
        }
    }
    // if station mode, update for station
    else if(mode_ == "station"){
        if(c->getOwner()==currentPlayer){
            int x = c->getX()/150;
            int y = c->getY()/150;
            buildingScene->removeItem(c);
            SpaceStation *station = new SpaceStation(x,y,currentPlayer,currentPlayer->getColor());
            buildingScene->addItem(station);
            buildings_[y][x] = station;
            mode_ = "";
            currentPlayer->increaseResource(ResourceType::Metal,-2);
            currentPlayer->increaseResource(ResourceType::Stone,-2);
            currentPlayer->increaseResource(ResourceType::Oxygen,-4);
            currentPlayer->increasePoints();
            currentPlayer->increaseUsed(8);
            updateBoard();
            enableButtons();

        }
    }
    else if(mode_ == "first"){
        if(c->getOwner()==NULL){
            c->setColor(currentPlayer->getColor());
            c->setOwner(currentPlayer);
            currentPlayer->increasePoints();
            GenerateLine();
            firstTurns();
        }

    }
    else{
        mode_ = "";
    }
}

/**
    Slot for dice button
    @param
    @return
*/
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


    // distribute resources based on dice roll
    distributeResources();
    // enable appropriate buttons
    enableButtons();

    // if the dice is a 7, queue the robber
    if(diceRoll_ == 7) {
        mode_ = "robber";
        ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- click a number to deactivate a resource"));
        ui->diceButton->setDisabled(true);
        ui->buildCampsiteButton->setDisabled(true);
        ui->buildSpaceStationButton->setDisabled(true);
        ui->endTurnButton->setDisabled(true);

        if(!currentPlayer->isHuman()){
            qDebug() << "Computer Robber";
            int x = rand() % 4;
            int y = rand() % 3;

            while(resources_[y][x].type == ResourceType::BlackHole){
                x = rand() % 4;
                y = rand() % 3;
            }
            NumberClicked(resources_[y][x].labelNumber);
        }
    }

}

/**
    Distribute resources depending on dice number
    @param
    @return
*/
void CatanWindow::distributeResources()
{
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 4; j++) {
            ResourceType material = resources_[i][j].type;


            if(resources_[i][j].number == diceRoll_ && resources_[i][j].active) {
                if(buildings_[i][j]->getOwner() != NULL) {
                    materials_distributed_++;
                    buildings_[i][j]->getOwner()->increaseResource(material, buildings_[i][j]->getReward());
                }
                if(buildings_[i + 1][j]->getOwner() != NULL) {
                    materials_distributed_++;
                    buildings_[i + 1][j]->getOwner()->increaseResource(material, buildings_[i + 1][j]->getReward());
                }
                if(buildings_[i][j + 1]->getOwner() != NULL) {
                    materials_distributed_++;
                    buildings_[i][j + 1]->getOwner()->increaseResource(material, buildings_[i][j + 1]->getReward());
                }
                if(buildings_[i + 1][j + 1]->getOwner() != NULL) {
                    materials_distributed_++;
                    buildings_[i + 1][j + 1]->getOwner()->increaseResource(material, buildings_[i + 1][j + 1]->getReward());
                }
            }
        }
    }

    updateBoard();

}

/**
    Update the board to current values
    @param
    @return
*/
void CatanWindow::updateBoard() {
    ui->p1_points->setText(QString("Points: ") + QString::number(players_[0]->getPoints()));
    ui->p1_metal->setText(QString("Metal: ") + QString::number(players_[0]->getMetal()));
    ui->p1_stone->setText(QString("Stone: ") + QString::number(players_[0]->getStone()));
    ui->p1_oxygen->setText(QString("Oxygen: ") + QString::number(players_[0]->getOxygen()));
    ui->p1_used->setText(QString("Used: ") + QString::number(players_[0]->getUsed()));

    ui->p2_points->setText(QString("Points: ") + QString::number(players_[1]->getPoints()));
    ui->p2_metal->setText(QString("Metal: ") + QString::number(players_[1]->getMetal()));
    ui->p2_stone->setText(QString("Stone: ") + QString::number(players_[1]->getStone()));
    ui->p2_oxygen->setText(QString("Oxygen: ") + QString::number(players_[1]->getOxygen()));
    ui->p2_used->setText(QString("Used: ") + QString::number(players_[1]->getUsed()));

    ui->p3_points->setText(QString("Points: ") + QString::number(players_[2]->getPoints()));
    ui->p3_metal->setText(QString("Metal: ") + QString::number(players_[2]->getMetal()));
    ui->p3_stone->setText(QString("Stone: ") + QString::number(players_[2]->getStone()));
    ui->p3_oxygen->setText(QString("Oxygen: ") + QString::number(players_[2]->getOxygen()));
    ui->p3_used->setText(QString("Used: ") + QString::number(players_[2]->getUsed()));

    ui->totalDistributed->setText(QString("Total Resources Distributed: ") + QString::number(materials_distributed_));
}


/**
    Slot for the end turn button
    @param
    @return
*/
void CatanWindow::on_endTurnButton_clicked()
{
    currentPlayer->pushToPoints(currentPlayer->getPoints());
    GenerateLine();
    if(isOver()){
        return;
    }
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
    if(!currentPlayer->isHuman()){
        on_diceButton_clicked();
        return takeComputerTurn();
    }

    ui->diceButton->setDisabled(false);
    ui->buildCampsiteButton->setDisabled(true);
    ui->buildSpaceStationButton->setDisabled(true);
    ui->endTurnButton->setDisabled(true);


}

/**
    Slot for the build camp site button
    @param
    @return
*/
void CatanWindow::on_buildCampsiteButton_clicked()
{
    mode_ = "camp";
    ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- click in a white square to build a campsite"));
}


/**
    Slot for the build space station button
    @param
    @return
*/
void CatanWindow::on_buildSpaceStationButton_clicked()
{
    mode_ = "station";
    ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- click in a campsite to upgrade to a space station"));
}

/**
    Enable/disable buttons depending on materials for each player
    @param
    @return
*/
void CatanWindow::enableButtons(){
    ui->instructionsLabel->setText(QString("Player ")+QString::number(currentPlayer->getId())+QString("- choose one of your available actions"));
    int metal = currentPlayer->getMetal();
    int stone = currentPlayer->getStone();
    int oxygen = currentPlayer->getOxygen();
    if((oxygen >= 1 && stone >=2)){
        ui->buildCampsiteButton->setDisabled(false);
    }
    else{
        ui->buildCampsiteButton->setDisabled(true);
    }
    if(oxygen >= 4 && stone >=2 && metal >= 2){
        ui->buildSpaceStationButton->setDisabled(false);
    }
    else{
        ui->buildSpaceStationButton->setDisabled(true);
    }
    ui->endTurnButton->setDisabled(false);
}

/**
    Used for clicking on the number. Used for modifying the robber position.
    @param A MyLabel object
    @return
*/
void CatanWindow::NumberClicked(MyLabel* l){
    if(mode_ == "robber"){
        if(lastRobbed != NULL) {
            lastRobbed->setVisible(true);
            resources_[(lastRobbed->y() - 75)/150][(lastRobbed->x() - 75)/150].active = true;
        }
        lastRobbed = l;
        int x = l->x();
        int y = l->y();
        resources_[(y - 75)/150][(x - 75)/150].active = false;
        l->setVisible(false);
        qDebug() << x << " and " << y;


        mode_ = "";
        enableButtons();
    }
}


/**
    Update the line graph in the lineScene
    @param
    @return
*/
void CatanWindow::GenerateLine(){
    int prevX = 0;
    int prevY = 100;
    std::vector<int> points = currentPlayer->getPointsOverTime();
    for(int j = 0; j < points.size(); j ++){
        lineScene->addLine(prevX, prevY,j*8,100-(points[j]*10),QPen(currentPlayer->getColor()));
        prevX = j*8;
        prevY = 100-points[j]*10;
    }
}

/**
    Check if the game is over
    @param
    @return a boolean showing if game is over
*/
bool CatanWindow::isOver(){
    bool over = false;
    int winner = 0;

    for(int i = 0; i < players_.size(); i++){
        if(players_[i]->getPoints() >= 10){
            over = true;
            winner = players_[i]->getId();
            break;
        }
    }

    if(over){
        ui->instructionsLabel->setText(QString("Player ") + QString::number(winner) + QString(" won!"));
        ui->diceButton->setDisabled(true);
        ui->buildCampsiteButton->setDisabled(true);
        ui->buildSpaceStationButton->setDisabled(true);
        ui->endTurnButton->setDisabled(true);
    }

    return over;


}

/**
    Calculate all the possible actions an AI can take.
    Take a random action based on a random integer
    @param
    @return
*/
void CatanWindow::takeComputerTurn(){
    std::vector<std::string> options;
    options.push_back("turn");
    if((currentPlayer->getOxygen() >= 1 && currentPlayer->getStone() >=2)){
        options.push_back("camp");
    }
    if(currentPlayer->getOxygen() >= 4 && currentPlayer->getStone() >=2 && currentPlayer->getMetal() >= 2){
        options.push_back("station");;
    }

    int size = options.size();

    if(size == 3){
        int number = rand() % 100;
        if(number < 5){
            return on_endTurnButton_clicked();
        }
        else if(number < 30){
            int i = rand() % 4;
            int j = rand() % 5;
            std::vector<Campsite*> camp;

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 5; j++){
                    if(buildings_[i][j]->getOwner()==NULL){
                        camp.push_back(buildings_[i][j]);
                    }
                }
            }
            mode_ = "camp";
            ClickSlot(camp[rand()%camp.size()]);
            takeComputerTurn();
        }
        else{
            int i = rand() % 4;
            int j = rand() % 5;

            while(buildings_[i][j]->getOwner() != currentPlayer){
                i = rand() % 4;
                j = rand() % 5;
            }
            mode_ = "station";
            ClickSlot(buildings_[i][j]);
            takeComputerTurn();
        }
    }
    if(size ==2){
        int number = rand() % 10;
        if(number < 2){
            return on_endTurnButton_clicked();
        }
        else{
            int i = rand() % 4;
            int j = rand() % 5;
            std::vector<Campsite*> camp;

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 5; j++){
                    if(buildings_[i][j]->getOwner()==NULL){
                        camp.push_back(buildings_[i][j]);
                    }
                }
            }
            mode_ = "camp";
            ClickSlot(camp[rand () %camp.size()]);
            takeComputerTurn();
        }
    }
    else{
        on_endTurnButton_clicked();
    }
}

/**
    Check if a spot is a good place for AI to build
    @param row and column
    @return boolean if it's a good spot
*/
bool CatanWindow::goodPlace(int i,int j){
    if(i == 0 and j == 0){
        return false;
    }
    if(i == 0 and j == 4){
        return false;
    }
    if(i == 3 and j == 0){
        return false;
    }
    if(i == 3 and j == 4){
        return false;
    }

    bool stone = false;
    bool oxygen = false;

    if(i == 0){
        if(resources_[i][j].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i][j].type == ResourceType::Oxygen){
            oxygen = true;
        }
        if(resources_[i][j-1].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i][j-1].type == ResourceType::Oxygen){
            oxygen = true;
        }
    }
    else if(i == 3){
        if(resources_[i-1][j].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i-1][j].type == ResourceType::Oxygen){
            oxygen = true;
        }
        if(resources_[i-1][j-1].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i-1][j-1].type == ResourceType::Oxygen){
            oxygen = true;
        }
    }
    if(j == 0){
        if(resources_[i][j].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i][j].type == ResourceType::Oxygen){
            oxygen = true;
        }
        if(resources_[i-1][j].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i-1][j].type == ResourceType::Oxygen){
            oxygen = true;
        }
    }
    else if(j == 4){
        if(resources_[i][j-1].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i][j-1].type == ResourceType::Oxygen){
            oxygen = true;
        }
        if(resources_[i-1][j-1].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i-1][j-1].type == ResourceType::Oxygen){
            oxygen = true;
        }
    }
    else{
        if(resources_[i][j].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i][j].type == ResourceType::Oxygen){
            oxygen = true;
        }
        if(resources_[i-1][j-1].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i-1][j-1].type == ResourceType::Oxygen){
            oxygen = true;
        }
        if(resources_[i][j-1].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i][j-1].type == ResourceType::Oxygen){
            oxygen = true;
        }
        if(resources_[i-1][j].type == ResourceType::Stone){
            stone = true;
        }
        if(resources_[i-1][j].type == ResourceType::Oxygen){
            oxygen = true;
        }
    }

    return oxygen && stone;
}

/**
    Emmit signals when a label is clicked
    @param A Mouse Event
    @return
*/
void MyLabel::mousePressEvent(QMouseEvent *event){
    qDebug() << "Emitting";
    emit Robber(this);
}

