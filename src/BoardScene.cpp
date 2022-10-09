#include "BoardScene.h"
#include "PieceGraphicsItem.h"
#include "SquareGraphicsItem.h"

#include <QGraphicsSceneMouseEvent>

#include <algorithm>

namespace
{
simplechess::Square getSquareUnder(const QPointF coords)
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

PieceGraphicsItem* getPieceAt(QList<QGraphicsItem*> items, const simplechess::Square& target)
{
    const auto rect = getSquare(items, target)->boundingRect();

    for (const auto& item : items)
    {
        auto result = qgraphicsitem_cast<PieceGraphicsItem*>(item);
        if (result && rect.contains(result->scenePos())) {
            return result;
        }
    }
    return nullptr;
}
}

BoardScene::BoardScene(
        QObject* parent,
        const simplechess::Game& game)
    : QGraphicsScene(parent)
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

    const auto dstSquare = getSquareUnder(mouseEvent->scenePos());
    const std::optional<simplechess::Piece> piece = currentGame.currentStage().board().pieceAt(dstSquare);

    if (!selectedSquare)
    {
        if (piece && piece->color() == currentGame.currentStage().activeColor())
        {
            selectedSquare = dstSquare;
        }
    }
    else
    {
        const auto availableMoves = currentGame.availableMovesForPiece(*selectedSquare);
        if (std::any_of(availableMoves.begin(),
                         availableMoves.end(),
                         [&](const simplechess::PieceMove& move) { return move.dst() == dstSquare; } ))
        {
            emit pieceMovedOnBoard(*selectedSquare, dstSquare);
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

    const std::set<simplechess::PieceMove> availableMoves = selectedSquare
            ? currentGame.availableMovesForPiece(*selectedSquare)
            : std::set<simplechess::PieceMove>{};

    for (auto& item : items())
    {
        SquareGraphicsItem* asSquare = qgraphicsitem_cast<SquareGraphicsItem*>(item);
        if (asSquare) {
            if (!selectedSquare)
            {
                asSquare->unmark();
            }
            else if (*selectedSquare == asSquare->square())
            {
                asSquare->markAsSelected();
            }
            else if (std::find_if(availableMoves.begin(), availableMoves.end(), [&](const simplechess::PieceMove& move) {return move.dst() == asSquare->square(); } ) != availableMoves.end())
            {
                // This is a potential landing square
                if (currentGame.currentStage().board().pieceAt(asSquare->square()))
                {
                    asSquare->markAsPossibleOccupiedDestination();
                }
                else
                {
                    asSquare->markAsPossibleDestination();
                }
            }
            else
            {
                asSquare->unmark();
            }
        }
    }
}

void BoardScene::deletePieceAt(const simplechess::Square& square)
{
    PieceGraphicsItem* piece = getPieceAt(items(), square);
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
