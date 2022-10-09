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

QPointF getCoordsForPieceAt(const simplechess::Square& square)
{
    const QPointF squareTopLeft{
        static_cast<qreal>((square.file() - 'a') * SquareGraphicsItem::squareSizePx),
        static_cast<qreal>(SquareGraphicsItem::squareSizePx*(8 - square.rank()))};


    return {squareTopLeft.x(), squareTopLeft.y()};
}

SquareGraphicsItem* getGraphicsSquare(QList<QGraphicsItem*> items, const simplechess::Square& target)
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

PieceGraphicsItem* getGraphicsPieceAt(QList<QGraphicsItem*> items, const simplechess::Square& target)
{
    const auto rect = getGraphicsSquare(items, target)->sceneBoundingRect();

    for (const auto& item : items)
    {
        const auto itemCenter = item->sceneBoundingRect().center();
        auto result = qgraphicsitem_cast<PieceGraphicsItem*>(item);

        if (result && rect.contains(itemCenter)) {
            return result;
        }
    }
    return nullptr;
}

std::set<simplechess::Square> getChangedSquares(
        std::map<simplechess::Square, simplechess::Piece> oldBoard,
        std::map<simplechess::Square, simplechess::Piece> newBoard)
{
    std::set<simplechess::Square> result;

    for (const auto& squareAndPiece : oldBoard)
    {
        const auto square = squareAndPiece.first;
        const auto piece = squareAndPiece.second;
        if (!newBoard.count(square) || newBoard.at(square) != piece)
        {
            result.insert(square);
        }
    }

    for (const auto& squareAndPiece : newBoard) {
        const auto square = squareAndPiece.first;
        const auto piece = squareAndPiece.second;
        if (!oldBoard.count(square) || oldBoard.at(square) != piece)
        {
            result.insert(square);
        }
    }

    return result;
}

std::set<simplechess::Square> getAllSquares()
{
    std::set<simplechess::Square> allSquares;

    for (uint8_t rank = 1; rank <= 8; ++rank)
    {
        for (char file = 'a'; file <= 'h'; ++file)
        {
            const simplechess::Square square = simplechess::Square::fromRankAndFile(rank, file);
            allSquares.insert(square);
        }
    }
    return allSquares;
}
}

BoardScene::BoardScene(
        QObject* parent,
        const simplechess::Game& game)
    : QGraphicsScene(parent)
    , currentGame(game)
{
    for (auto& square : getAllSquares())
    {
        addItem(new SquareGraphicsItem(square));
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

    if (piece && piece->color() == currentGame.currentStage().activeColor())
    {
        selectedSquare = dstSquare;
    }
    else if (selectedSquare)
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
    // Squares whose contents have changed
    const auto oldBoard = currentGame.currentStage().board().occupiedSquares();
    const auto newBoard = game.currentStage().board().occupiedSquares();

    const std::set<simplechess::Square> toBeUpdated = behaviour == DrawBehaviour::Differential
            ? getChangedSquares(oldBoard, newBoard)
            : getAllSquares();

    for (const auto& square : toBeUpdated)
    {
        deletePieceAt(square);
        if (newBoard.count(square))
        {
            createPieceAt(square, newBoard.at(square));
        }
    }

    const std::set<simplechess::PieceMove> availableMoves = selectedSquare
            ? currentGame.availableMovesForPiece(*selectedSquare)
            : std::set<simplechess::PieceMove>{};

    for (auto& item : items())
    {
        SquareGraphicsItem* asSquare = qgraphicsitem_cast<SquareGraphicsItem*>(item);
        if (asSquare) {
            const auto& chessSquare = asSquare->square();

            if (!selectedSquare)
            {
                asSquare->unmark();
            }
            else if (*selectedSquare == chessSquare)
            {
                asSquare->markAsSelected();
            }
            else if (std::find_if(
                         availableMoves.begin(),
                         availableMoves.end(),
                         [&](const simplechess::PieceMove& move) {return move.dst() == chessSquare; } )
                     != availableMoves.end())
            {
                // This is a potential landing square
                if (currentGame.currentStage().board().pieceAt(chessSquare))
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
    PieceGraphicsItem* piece = getGraphicsPieceAt(items(), square);
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
    pieceImage->setPos(getCoordsForPieceAt(square));
}
