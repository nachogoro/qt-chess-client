#ifndef BOARDSCENE_H
#define BOARDSCENE_H

#include "IMovesListener.h"
#include "SquareGraphicsItem.h"
#include <QGraphicsScene>
#include <simplechess/Game.h>

class BoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    BoardScene(
            QObject* parent,
            IMovesListener& movesListener,
            const simplechess::Game& game);
    void onGameUpdated(const simplechess::Game& game);

protected:
    enum class DrawBehaviour
    {
        /**
         * Only modify the changes from the latest state.
         */
        Differential,

        /**
         * Redraw the whole board, ignoring previous state.
         */
        FullRedraw
    };

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void updateBoard(const simplechess::Game& game, DrawBehaviour behaviour = DrawBehaviour::Differential);

    void deletePieceAt(const simplechess::Square& square);
    void createPieceAt(const simplechess::Square& square, const simplechess::Piece& piece);

private:
    IMovesListener& movesListener;
    std::optional<simplechess::Square> selectedSquare;
    simplechess::Game currentGame;
};

#endif // BOARDSCENE_H
