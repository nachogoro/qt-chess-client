#ifndef SQUAREGRAPHICSITEM_H
#define SQUAREGRAPHICSITEM_H

#include <simplechess/Square.h>
#include <QAbstractGraphicsShapeItem>

class SquareGraphicsItem : public QGraphicsRectItem
{
public:
    constexpr static int squareSizePx = 75;

    SquareGraphicsItem(const simplechess::Square& square, QGraphicsItem *parent = nullptr);

    const simplechess::Square& square() const;

    /*virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;*/

private:
    const simplechess::Square mSquare;
};

#endif // SQUAREGRAPHICSITEM_H
