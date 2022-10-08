#ifndef IMOVESLISTENER_H
#define IMOVESLISTENER_H

#include <simplechess/Square.h>

class IMovesListener {
public:
    enum class MoveSource {
        Local,
        Remote
    };

    virtual void onMoveReceived(
            MoveSource source,
            const simplechess::Square& src,
            const simplechess::Square& dst) = 0;
};

#endif // IMOVESLISTENER_H
