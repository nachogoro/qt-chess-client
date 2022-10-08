#include "Board.h"

#include <QPainter>

Board::Board() {
}

void Board::paintEvent(QPaintEvent* pe)
{
    // Initialization
    const unsigned int numCellX = 8;
    const unsigned int numCellY = 8;
    QRect wRect = rect();
    unsigned int cellSizeX = wRect.width() / numCellX;
    unsigned int cellSizeY = wRect.height() / numCellY;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw the background. The whole widget is drawed.
    painter.setBrush(QColor(0,0,0)); //black
    painter.drawRect(wRect);

    // Draw the cells which are of the other color
    // Note: the grid may not fit in the whole rect, because the size of the widget
    // should be a multiple of the size of the cells. If not, a black line at the right
    // and at the bottom may appear.
    painter.setBrush(QColor(255,255,255)); //white
    for(unsigned int j = 0; j < numCellY; j++)
        for(unsigned int i = j % 2; i < numCellX; i+=2)
            painter.drawRect(i * cellSizeX, j * cellSizeY, cellSizeX, cellSizeY);
}

int Board::heightForWidth(int width) const
{
    return width;
}

bool Board::hasHeightForWidth() const
{
    return true;
}
