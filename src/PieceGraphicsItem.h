#ifndef PIECEGRAPHICSITEM_H
#define PIECEGRAPHICSITEM_H

#include <simplechess/Piece.h>
#include <QGraphicsPixmapItem>

class PieceGraphicsItem : public QGraphicsPixmapItem
{
    //Q_OBJECT
public:
    PieceGraphicsItem(const simplechess::Piece& piece, QGraphicsItem *parent = nullptr);
    const simplechess::Piece& piece() const;

private:
    const simplechess::Piece mPiece;
};

#endif // PIECEGRAPHICSITEM_H
