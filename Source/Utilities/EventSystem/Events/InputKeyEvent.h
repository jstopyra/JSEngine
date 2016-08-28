//InputKeyEvent.h

#ifndef __INPUTKEYEVENT_H__
#define __INPUTKEYEVENT_H__

#include "../Event.h"
#include <SDL.h>

class InputKeyEvent : public Event
{
public:
	

private:
	bool m_wasKeyDown;
	SDL_Keycode m_pressedKey;

public:
	InputKeyEvent(SDL_Keycode pressedKey, bool wasKeyDown)
		:Event(k_inputKeyEvent)
		, m_pressedKey(pressedKey)
		, m_wasKeyDown(wasKeyDown)
	{

	};
	~InputKeyEvent() {};

	//list of keys that we can choose from.

	//-------------------------------------------------------------------------------------------------------------------
	//Getters/Setters
	//-------------------------------------------------------------------------------------------------------------------
	bool WasKeyDown()const { return m_wasKeyDown; }
	SDL_Keycode KeyTypePressed() const { return m_pressedKey; }


private:

};

#endif // !__INPUTKEYEVENT_H__
