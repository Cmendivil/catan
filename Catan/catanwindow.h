#ifndef CATANWINDOW_H
#define CATANWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "campsite.h"
#include "player.h"

struct materials{
    int number;
    bool active;
    ResourceType type;
    QPixmap* picture;
};

namespace Ui {
class CatanWindow;
}

class CatanWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CatanWindow(QWidget *parent = nullptr);
    ~CatanWindow();

    void distributeResources();
    void firstTurns();
    void updateBoard();
    void enableButtons();



private slots:
    void on_diceButton_clicked();

    void on_endTurnButton_clicked();

    void on_buildCampsiteButton_clicked();

    void on_buildSpaceStationButton_clicked();

public slots:
    void ClickSlot(Campsite * c);

private:
    Ui::CatanWindow *ui;
    QGraphicsScene *boardScene;
    QGraphicsScene *buildingScene;
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
};

#endif // CATANWINDOW_H
