//QuitGameEvent.h

#ifndef __MOUSECLICKEVENT_H__
#define __MOUSECLICKEVENT_H__

#include "../Event.h"
class Event;

class MouseClickEvent : public Event
{
private:
	int m_xPos;
	int m_yPos;
	int m_button;
public:
	~MouseClickEvent() {};
	MouseClickEvent(int xPos, int yPos, int button)
		: Event(k_mouseClickEvent)
		,m_xPos(xPos)
		,m_yPos(yPos)
		,m_button(button)
	{
		//
	}
	//get the x and y delta
	const int GetXPos() const { return m_xPos; }
	const int GetYPos() const { return m_yPos; }
	const int GetButton() const { return m_button; }
};

#endif