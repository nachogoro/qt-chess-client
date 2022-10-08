#include "./ui_mainwindow.h"
#include "src/mainwindow.h"
#include "src/BoardScene.h"
#include "src/Events.h"
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game(mgr.createNewGame())
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    scene = new BoardScene(this, *this, game);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0, 0, ui->graphicsView->contentsRect().width(), ui->graphicsView->contentsRect().height());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::onMoveReceived(
        MoveSource source,
        const simplechess::Square& src,
        const simplechess::Square& dst) {
    QCoreApplication::postEvent(this, new MoveReceivedEvent(source, src, dst));
}

bool MainWindow::event(QEvent *event)
{
    if (dynamic_cast<MoveReceivedEvent*>(event)) {
        MoveReceivedEvent* mre = dynamic_cast<MoveReceivedEvent*>(event);
        // TODO check move source in case it needs to be discarded (e.g. network move in local play)
        const auto movesForPiece = game.availableMovesForPiece(mre->src());
        const auto move = std::find_if(movesForPiece.begin(), movesForPiece.end(), [&](const simplechess::PieceMove& m) { return m.dst() == mre->dst(); });

        if (move != movesForPiece.end()) {
            game = mgr.makeMove(game, *move);
            scene->onGameUpdated(game);
        }

        return true;
    }

    return QObject::event(event);
}
