#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtGui>
#include <QtCore>
#include "BoardScene.h"
#include "IMovesListener.h"
#include <simplechess/GameManager.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public IMovesListener
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void onMoveReceived(
            MoveSource source,
            const simplechess::Square& src,
            const simplechess::Square& dst) override;

    virtual bool event(QEvent *event) override;

private:
    void handleReceivedMove(
            MoveSource source,
            const simplechess::Square& src,
            const simplechess::Square& dst);

    Ui::MainWindow *ui;
    BoardScene *scene;
    simplechess::GameManager mgr;
    simplechess::Game game;

};
#endif // MAINWINDOW_H
