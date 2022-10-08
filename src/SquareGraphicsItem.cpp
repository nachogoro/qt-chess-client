#include "SquareGraphicsItem.h"
#include "qbrush.h"
#include "qpainter.h"
#include "qpen.h"

namespace
{
QPointF toCoords(const simplechess::Square& square)
{
    return {static_cast<qreal>((square.file() - 'a') * SquareGraphicsItem::squareSizePx), static_cast<qreal>(SquareGraphicsItem::squareSizePx*(8 - (square.rank())))};
}
}

SquareGraphicsItem::SquareGraphicsItem(const simplechess::Square& square, QGraphicsItem *parent)
    : QGraphicsRectItem(toCoords(square).x(), toCoords(square).y(), squareSizePx, squareSizePx, parent)
    , mSquare(square)
{
    constexpr QColor lightSquares = QColor{240, 217, 181};
    constexpr QColor darkSquares = QColor{181, 136, 99};
    const QColor color = (square.color() == simplechess::Color::Black) ? darkSquares : lightSquares;
    setBrush(QBrush(color));
    setPen(QPen{color});
}

/*void SquareGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(boundingRect());
}

QRectF SquareGraphicsItem::boundingRect() const
{
    const qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
    return {{toCoords(mSquare).x() - pw, toCoords(mSquare).y() - pw}, QSizeF(squareSizePx + 2*pw, squareSizePx + 2*pw)};
}*/

const simplechess::Square& SquareGraphicsItem::square() const
{
    return mSquare;
}
