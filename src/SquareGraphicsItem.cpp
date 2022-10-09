#include "SquareGraphicsItem.h"
#include "qbrush.h"
#include "qpainter.h"
#include "qpen.h"

namespace
{
QPointF topLeftCoords(const simplechess::Square& square)
{
    return {static_cast<qreal>((square.file() - 'a') * SquareGraphicsItem::squareSizePx), static_cast<qreal>(SquareGraphicsItem::squareSizePx*(8 - (square.rank())))};
}

QPointF centerCoords(const simplechess::Square& square)
{
    const auto topLeft = topLeftCoords(square);
    return {topLeft.x() + SquareGraphicsItem::squareSizePx / 2, topLeft.y() + SquareGraphicsItem::squareSizePx / 2};
}

QColor baseColorFor(const simplechess::Square& square)
{
    constexpr QColor lightSquares{240, 217, 181};
    constexpr QColor darkSquares{181, 136, 99};
    return (square.color() == simplechess::Color::Black) ? darkSquares : lightSquares;
}

QColor markColorFor(const simplechess::Square& square)
{
    constexpr QColor lightSquares{130, 151, 105};
    constexpr QColor darkSquares{100, 111, 64};
    return (square.color() == simplechess::Color::Black) ? darkSquares : lightSquares;
}
}

SquareGraphicsItem::SquareGraphicsItem(const simplechess::Square& square, QGraphicsItem *parent)
    : QAbstractGraphicsShapeItem(parent)
    , mSquare(square)
    , mMarkedState(MarkedState::Unmarked)
{
    changeColor(baseColorFor(square));
}

void SquareGraphicsItem::changeColor(QColor color) {
    setBrush(QBrush{color});
    setPen(Qt::NoPen);
}

void SquareGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(boundingRect());

    switch (mMarkedState)
    {
        case MarkedState::Unmarked:
            // Do nothing
            break;
    case MarkedState::Selected:
    {
        QColor color = markColorFor(square());
        painter->setBrush(QBrush{color});
        //painter->setPen(QPen{color});
        painter->drawRect(boundingRect());
        break;
    }
    case MarkedState::PossibleDestination:
    {
        QColor color = markColorFor(square());
        painter->setBrush(QBrush{color});
        painter->drawEllipse(centerCoords(square()), SquareGraphicsItem::squareSizePx / 6, SquareGraphicsItem::squareSizePx / 6);
        break;
    }
    case MarkedState::PossibleOccupiedDestination:
    {
        QColor color = markColorFor(square());
        painter->setBrush(QBrush{color});
        painter->drawEllipse(centerCoords(square()), SquareGraphicsItem::squareSizePx / 2, SquareGraphicsItem::squareSizePx / 2);
        painter->setBrush(QBrush{baseColorFor(square())});
        painter->drawEllipse(centerCoords(square()), SquareGraphicsItem::squareSizePx / 2.5, SquareGraphicsItem::squareSizePx / 2.5);
        break;
    }
    case MarkedState::InCheck:
    {
        QColor color = Qt::red;
        color.setAlphaF(0.5f);
        painter->setBrush({color});
        painter->drawRect(boundingRect());
        break;

    }

    default:
        // Do nothing
        break;
    }
}

QRectF SquareGraphicsItem::boundingRect() const
{
    const qreal pw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF();
    return {{topLeftCoords(mSquare).x() - pw, topLeftCoords(mSquare).y() - pw}, QSizeF(squareSizePx + 2*pw, squareSizePx + 2*pw)};
}

const simplechess::Square& SquareGraphicsItem::square() const
{
    return mSquare;
}

void SquareGraphicsItem::unmark()
{
    mMarkedState = MarkedState::Unmarked;
    update();
}

void SquareGraphicsItem::markAsSelected()
{
    mMarkedState = MarkedState::Selected;
    update();
}

void SquareGraphicsItem::markAsPossibleDestination()
{
    mMarkedState = MarkedState::PossibleDestination;
    update();
}

void SquareGraphicsItem::markAsPossibleOccupiedDestination()
{
    mMarkedState = MarkedState::PossibleOccupiedDestination;
    update();
}

void SquareGraphicsItem::markAsInCheck()
{
    mMarkedState = MarkedState::InCheck;
    update();
}
