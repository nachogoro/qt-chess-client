#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtGui>
#include <QtCore>
#include "BoardScene.h"
#include <simplechess/GameManager.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onMoveReceived(
        simplechess::Square src,
        simplechess::Square dst);

private:
    Ui::MainWindow *ui;
    BoardScene *scene;
    simplechess::GameManager mgr;
    simplechess::Game game;

};
#endif // MAINWINDOW_H
