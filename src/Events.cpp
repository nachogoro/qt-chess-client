#include "Events.h"

MoveReceivedEvent::MoveReceivedEvent(
        IMovesListener::MoveSource moveSource,
        const simplechess::Square& src,
        const simplechess::Square& dst)
    : QEvent(QEvent::Type::User)
    , mMoveSource(moveSource)
    , mSrc(src)
    , mDst(dst)
{}

IMovesListener::MoveSource MoveReceivedEvent::moveSource() const
{
    return mMoveSource;
}

const simplechess::Square& MoveReceivedEvent::src() const
{
    return mSrc;
}

const simplechess::Square& MoveReceivedEvent::dst() const
{
    return mDst;
}
