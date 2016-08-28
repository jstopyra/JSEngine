#ifndef __UPDATEEVENT_H__
#define __UPDATEEVENT_H__

#include "../Event.h"

class UpdateEvent :public Event
{


	float m_deltaTime;

public:
	UpdateEvent(float deltaTime)
		:Event(k_updateEvent)
		,m_deltaTime(deltaTime)
	{

	}

	const float GetDeltaTime() const { return m_deltaTime; }
};
#endif // !__UPDATEEVENT_H__
