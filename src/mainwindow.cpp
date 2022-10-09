#include "./ui_mainwindow.h"
#include "src/mainwindow.h"
#include "src/BoardScene.h"
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game(mgr.createNewGame())
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    scene = new BoardScene(this, game);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0, 0, ui->graphicsView->contentsRect().width(), ui->graphicsView->contentsRect().height());
    QObject::connect(scene, &BoardScene::pieceMovedOnBoard, this, &MainWindow::onMoveReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::onMoveReceived(
        simplechess::Square src,
        simplechess::Square dst) {

    // TODO check move source in case it needs to be discarded (e.g. network move in local play)
    const auto movesForPiece = game.availableMovesForPiece(src);
    const auto move = std::find_if(
                movesForPiece.begin(),
                movesForPiece.end(),
                [&](const simplechess::PieceMove& m) { return m.dst() == dst; });

    if (move != movesForPiece.end()) {
        game = mgr.makeMove(game, *move);
        scene->onGameUpdated(game);
    }
}
