#include "BoardScene.h"
#include "PieceGraphicsItem.h"
#include "SquareGraphicsItem.h"

#include <QGraphicsSceneMouseEvent>

#include <algorithm>

namespace
{
simplechess::Square toSquare(const QPointF coords)
{
    const uint8_t rank = 8 - static_cast<uint8_t>(coords.y() / 75);
    const char file = 'a' + static_cast<uint8_t>(coords.x() / 75);
    return simplechess::Square::fromRankAndFile(rank, file);
}

QPointF toCoords(const simplechess::Square& square)
{
    return { static_cast<qreal>((square.file() - 'a') * SquareGraphicsItem::squareSizePx),
             static_cast<qreal>(SquareGraphicsItem::squareSizePx*(8 - square.rank()))};
}

QPointF coordsForPieceAt(const simplechess::Square& square)
{
    return { static_cast<qreal>((square.file() - 'a') * SquareGraphicsItem::squareSizePx) + 7.5,
             static_cast<qreal>(SquareGraphicsItem::squareSizePx*(8 - square.rank()) + 7.5)};
}

SquareGraphicsItem* getSquare(QList<QGraphicsItem*> items, const simplechess::Square& target)
{
    for (const auto& item : items)
    {
        auto result = qgraphicsitem_cast<SquareGraphicsItem*>(item);
        if (result && result->square() == target) {
            return result;
        }
    }
    throw std::runtime_error("Failed to find square...");
}
}

BoardScene::BoardScene(
        QObject* parent,
        IMovesListener& movesListener,
        const simplechess::Game& game)
    : QGraphicsScene(parent)
    , movesListener(movesListener)
    , currentGame(game)
{
    for (uint8_t rank = 1; rank <= 8; ++rank)
    {
        for (char file = 'a'; file <= 'h'; ++file)
        {
            const simplechess::Square square = simplechess::Square::fromRankAndFile(rank, file);
            addItem(new SquareGraphicsItem(square));
        }
    }

    updateBoard(game, DrawBehaviour::FullRedraw);
}

void BoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton) {
        return;
    }

    QGraphicsItem* item = itemAt(mouseEvent->scenePos(), QTransform());
    PieceGraphicsItem* pieceItem = qgraphicsitem_cast<PieceGraphicsItem*>(item);

    if (!selectedSquare)
    {
        if (pieceItem && pieceItem->piece().color() == currentGame.currentStage().activeColor())
        {
            selectedSquare = toSquare(mouseEvent->scenePos());
        }
    }
    else
    {
        const auto dstSquare = toSquare(mouseEvent->scenePos());
        const auto availableMoves = currentGame.availableMovesForPiece(*selectedSquare);
        if (std::any_of(availableMoves.begin(),
                         availableMoves.end(),
                         [&](const simplechess::PieceMove& move) { return move.dst() == dstSquare; } ))
        {
            movesListener.onMoveReceived(IMovesListener::MoveSource::Local, *selectedSquare, dstSquare);
        }

        selectedSquare.reset();
    }

    updateBoard(currentGame);
}

void BoardScene::onGameUpdated(const simplechess::Game& game)
{
    updateBoard(game);
    currentGame = game;
}

void BoardScene::updateBoard(const simplechess::Game& game, const DrawBehaviour behaviour)
{
    std::set<simplechess::Square> toBeEmptied;
    std::set<simplechess::Square> toBeUpdated;

    const auto oldMap = currentGame.currentStage().board().occupiedSquares();
    const auto newMap = game.currentStage().board().occupiedSquares();

    for (const auto& squareAndPiece : oldMap)
    {
        const auto square = squareAndPiece.first;
        const auto piece = squareAndPiece.second;
        if (!newMap.count(square) || newMap.at(square) != piece)
        {
            toBeEmptied.insert(square);
        }
    }

    for (const auto& squareAndPiece : newMap) {
        const auto square = squareAndPiece.first;
        const auto piece = squareAndPiece.second;
        if (!oldMap.count(square) || oldMap.at(square) != piece)
        {
            toBeUpdated.insert(square);
        }
    }

    if (behaviour == DrawBehaviour::FullRedraw)
    {
        toBeEmptied = {};
        for (const auto& squareAndPiece : oldMap) {
            toBeEmptied.insert(squareAndPiece.first);
        }

        toBeUpdated = {};
        for (const auto& squareAndPiece : newMap) {
            toBeUpdated.insert(squareAndPiece.first);
        }
    }

    // FIRST EMPTY, THEN CREATE

    for (const auto& square : toBeEmptied)
    {
        deletePieceAt(square);
    }

    for (const auto& square : toBeUpdated)
    {
        createPieceAt(square, newMap.at(square));
    }

    if (selectedSquare)
    {
        SquareGraphicsItem* selectedItem = getSquare(items(), *selectedSquare);
        // TODO encapsulate
        selectedItem->setBrush(QBrush(Qt::green));
        selectedItem->setPen({Qt::green});

        for (const auto& move : currentGame.availableMovesForPiece(*selectedSquare)) {
            SquareGraphicsItem* dstItem = getSquare(items(), move.dst());
            // TODO encapsulate
            dstItem->setBrush(QBrush(Qt::red));
            dstItem->setPen({Qt::red});
        }
    }

    for (auto& item : items()) {
        item->update();
    }
    update();
}

void BoardScene::deletePieceAt(const simplechess::Square& square)
{
    QGraphicsItem* item = itemAt(coordsForPieceAt(square), QTransform());
    PieceGraphicsItem* piece = qgraphicsitem_cast<PieceGraphicsItem*>(item);
    if (piece)
    {
        removeItem(piece);
        delete piece;
    }
}

void BoardScene::createPieceAt(const simplechess::Square& square, const simplechess::Piece& piece)
{
    PieceGraphicsItem* pieceImage = new PieceGraphicsItem(piece);
    addItem(pieceImage);
    pieceImage->setPos(coordsForPieceAt(square));
}