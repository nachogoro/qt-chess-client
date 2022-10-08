#include "PieceGraphicsItem.h"

namespace
{
std::string getResourcePathForPiece(const simplechess::Piece& piece)
{
    // TODO embed png
    const std::string prefix = "/home/ic/QtTestApp/resources/";
    const std::string color = piece.color() == simplechess::Color::White ? "white" : "black";

    switch (piece.type())
    {
        case simplechess::PieceType::Pawn:
            return prefix + color + "_pawn";
        case simplechess::PieceType::Rook:
            return prefix + color + "_rook";
        case simplechess::PieceType::Knight:
            return prefix + color + "_knight";
        case simplechess::PieceType::Bishop:
            return prefix + color + "_bishop";
        case simplechess::PieceType::King:
            return prefix + color + "_king";
        case simplechess::PieceType::Queen:
            return prefix + color + "_queen";
    }

    throw std::runtime_error("Unknown piece type: " + std::to_string(static_cast<int>(piece.type())));
}
}

PieceGraphicsItem::PieceGraphicsItem(const simplechess::Piece& piece, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
    , mPiece(piece)
{
    setPixmap({QString::fromStdString(getResourcePathForPiece(piece))});
}

const simplechess::Piece& PieceGraphicsItem::piece() const
{
    return mPiece;
}
