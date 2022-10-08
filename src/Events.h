#ifndef EVENTS_H
#define EVENTS_H

#include "IMovesListener.h"
#include <simplechess/Square.h>
#include <QEvent>


class MoveReceivedEvent : public QEvent
{
public:
    MoveReceivedEvent(
            IMovesListener::MoveSource source,
            const simplechess::Square& src,
            const simplechess::Square& dst);

    virtual ~MoveReceivedEvent() = default;

    IMovesListener::MoveSource moveSource() const;
    const simplechess::Square& src() const;
    const simplechess::Square& dst() const;

private:
    const IMovesListener::MoveSource mMoveSource;
    const simplechess::Square mSrc;
    const simplechess::Square mDst;
};

#endif // EVENTS_H
