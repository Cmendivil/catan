#ifndef CATANWINDOW_H
#define CATANWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "campsite.h"
#include "player.h"
#include <QLabel>
#include <QMouseEvent>



class MyLabel : public QLabel
{
    Q_OBJECT
    using QLabel::QLabel;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void Robber(MyLabel*);
};
struct materials{
    int number;
    bool active;
    ResourceType type;
    QPixmap* picture;
    MyLabel* labelNumber;
};
namespace Ui {
class CatanWindow;
}

class CatanWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CatanWindow(std::vector<Player*>,QWidget *parent = nullptr);
    ~CatanWindow();
    void startGame();
    void distributeResources();
    void firstTurns();
    void updateBoard();
    void enableButtons();
    void GenerateLine();
    bool isOver();
    void takeComputerTurn();
    bool goodPlace(int,int);

private slots:
    void on_diceButton_clicked();
    void on_endTurnButton_clicked();
    void on_buildCampsiteButton_clicked();
    void on_buildSpaceStationButton_clicked();

public slots:
    void NumberClicked(MyLabel *l);
    void ClickSlot(Campsite * c);

private:
    Ui::CatanWindow *ui;
    QGraphicsScene *boardScene;
    QGraphicsScene *buildingScene;
    QGraphicsScene *lineScene;
    QPixmap *metal_;
    QPixmap *oxygen_;
    QPixmap *stone_;
    QPixmap *blackhole_;
    int diceRoll_;
    std::string mode_;
    materials resources_[3][4];
    Campsite* buildings_[4][5];
    std::vector<Player*> players_;
    Player *currentPlayer;
    int materials_distributed_;
    QLabel *lastRobbed;

};


#endif // CATANWINDOW_H
