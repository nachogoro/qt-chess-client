#ifndef BOARD_H
#define BOARD_H

#include <QWidget>

class Board : public QWidget
{
    Q_OBJECT
public:
    Board();

    void paintEvent(QPaintEvent* pe) override;
    int heightForWidth(int w) const override;
    bool hasHeightForWidth() const override;
};

#endif // BOARD_H
