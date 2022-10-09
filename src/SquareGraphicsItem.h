#ifndef SQUAREGRAPHICSITEM_H
#define SQUAREGRAPHICSITEM_H

#include <simplechess/Square.h>
#include <QAbstractGraphicsShapeItem>

class SquareGraphicsItem : public QAbstractGraphicsShapeItem
{
public:
    constexpr static int squareSizePx = 76;

    SquareGraphicsItem(const simplechess::Square& square, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    const simplechess::Square& square() const;

    void unmark();
    void markAsSelected();
    void markAsPossibleDestination();
    void markAsPossibleOccupiedDestination();

private:
    enum class MarkedState {
        Unmarked,
        Selected,
        PossibleDestination,
        PossibleOccupiedDestination
    };

    void changeColor(QColor color);

private:
    const simplechess::Square mSquare;
    MarkedState mMarkedState;
};

#endif // SQUAREGRAPHICSITEM_H
