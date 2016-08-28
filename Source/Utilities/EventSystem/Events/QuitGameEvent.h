//QuitGameEvent.h

#ifndef __QUITGAMEEVENT_H__
#define __QUITGAMEEVENT_H__

#include "../Event.h"
class Event;

class QuitGameEvent : public Event
{
public:
    ~QuitGameEvent(){};
    QuitGameEvent()
        : Event(k_quitGameEvent)
    {
        //
    }

};

#endif