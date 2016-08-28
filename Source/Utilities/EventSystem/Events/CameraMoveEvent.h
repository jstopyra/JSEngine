//CameraMoveEvent.h

#ifndef __CAMERAMOVEEVENT_H__
#define __CAMERAMOVEEVENT_H__

#include "../Event.h"
class Event;

class CameraMoveEvent : public Event
{
    int m_xDelta;
	int m_yDelta;
	int m_zDelta;
public:
    ~CameraMoveEvent(){};
    CameraMoveEvent(int xDelta, int yDelta, int zDelta)
        : Event(k_cameraMoveEvent)
        , m_xDelta(xDelta)
		, m_yDelta(yDelta)
		, m_zDelta(zDelta)
    {
        //
    }
    //get the x and y delta
    const int GetDeltaX() const { return m_xDelta; }
	const int GetDeltaY() const { return m_yDelta; }
	const int GetDeltaZ() const { return m_zDelta; }

};

#endif