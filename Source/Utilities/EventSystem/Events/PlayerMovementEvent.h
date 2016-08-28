//PlayerMovementEvent.h

#ifndef __PLAYERMOVEMENT_H__
#define __PLAYERMOVEMENT_H__

#include "../Event.h"
class Event;

class PlayerMovementEvent : public Event
{
    int m_xDelta;
    int m_yDelta;
public:
    ~PlayerMovementEvent(){};
    PlayerMovementEvent(int xDelta, int yDelta)
        : Event(k_playerMovementEvent)
        , m_xDelta(xDelta)
        , m_yDelta(yDelta)
    {
        //
    }
    //get the x and y delta
    const int GetDeltaX() const { return m_xDelta; }
    const int GetDeltaY() const { return m_yDelta; }

};

#endif