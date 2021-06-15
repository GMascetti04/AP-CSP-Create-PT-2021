#pragma once
#include "xpch.h"
#include "Engine/Codes.h"



class Event {
public:
	enum EventCategory {
		none = 0,
		application = 1,
		window = 2,
		input = 4,
		keyboard = 8,
		mouse = 16,
		mouse_button = 32
	};

	enum class EventType {
		none = 0,
		close_window, resize_window, minimize_window,
		mouse_move, mouse_scroll, mouse_press, mouse_release,
		key_press, key_release, key_repeat,
		char_input,
		set_focus, remove_focus, maximize_window, restore_window
	};

public:
	Event() {}

	virtual int get_categories() = 0;
	virtual Event::EventType get_type() = 0;
	
	template<typename EventClass>
	static inline EventClass& EventCast(Event& event)
	{
		return *((EventClass*)(&event));
	}


	template<typename T>
	static inline bool isEvent(Event& event)
	{
		if (T::getEventType() == event.get_type())
			return true;
		return false;
	}


	//will return true if the event is of the specified type
	//and meets the requirenment
	template<typename EventClass, typename ValType>
	static inline bool checkProperty(Event& event,ValType(EventClass::* func)(void),ValType value)
	{
		if (!Event::isEvent<EventClass>(event))
			return false;
		if ((Event::EventCast<EventClass>(event).*func)() == value)
			return true;
		return false;
	}



	bool m_handled = false;

};





class WindowCloseEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::close_window; }

	int get_categories() { return Event::EventCategory::window | Event::EventCategory::application; }
	Event::EventType get_type() { return Event::EventType::close_window; }
};




class MouseMoveEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::mouse_move; }

	MouseMoveEvent(double x, double y) : m_x(x), m_y(y) {}
	
	int get_categories() { return Event::EventCategory::input | Event::EventCategory::mouse;}
	Event::EventType get_type() { return Event::EventType::mouse_move; }
	
	float getX() { return (float)m_x; }
	float getY() { return (float)m_y; }

private:
	double m_x;
	double m_y;
};

class MouseScrollEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::mouse_scroll; }
	
	MouseScrollEvent(double ymag) : y_magnitude(ymag) {}

	int get_categories() { return Event::EventCategory::input | Event::EventCategory::mouse; }
	Event::EventType get_type() { return Event::EventType::mouse_scroll; }
	
	double yOffset() { return y_magnitude; }

private:
	double y_magnitude;
};

class MouseButtonPressEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::mouse_press; }

	MouseButtonPressEvent(codes::MouseCode code) :m_button(code) {}

	int get_categories() { return Event::EventCategory::input | Event::EventCategory::mouse | Event::EventCategory::mouse_button; }
	Event::EventType get_type() { return Event::EventType::mouse_press; }

	codes::MouseCode getButton() { return m_button; }

private:
	codes::MouseCode m_button;
};

class MouseButtonReleaseEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::mouse_release; }

	MouseButtonReleaseEvent(codes::MouseCode code) :m_button(code) {}

	int get_categories() { return Event::EventCategory::input | Event::EventCategory::mouse | Event::EventCategory::mouse_button; }
	Event::EventType get_type() { return Event::EventType::mouse_release; }

	codes::MouseCode getButton() { return m_button; }
	
private:
	codes::MouseCode m_button;
};

class WindowResizeEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::resize_window; }

	WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}

	int get_categories() { return Event::EventCategory::application | Event::EventCategory::window; }
	Event::EventType get_type() { return Event::EventType::resize_window; }

	unsigned int get_width() { return m_width; }
	unsigned int get_height() { return m_height; }

	std::tuple<unsigned int, unsigned int> getDimensions() { return std::tuple<unsigned int, unsigned int>(m_width, m_height); }

private:
	unsigned int m_width;
	unsigned int m_height;
};

class KeyPressEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::key_press; }

	KeyPressEvent(codes::KeyCode code) : m_key(code) {}
	
	int get_categories() { return Event::EventCategory::keyboard | Event::EventCategory::input; }
	Event::EventType get_type() { return Event::EventType::key_press; }
	
	codes::KeyCode get_keyCode() { return m_key; }

private:
	codes::KeyCode m_key;
};

class KeyReleaseEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::key_release; }

	KeyReleaseEvent(codes::KeyCode code) : m_key(code) {}

	int get_categories() { return Event::EventCategory::keyboard | Event::EventCategory::input; }
	Event::EventType get_type() { return Event::EventType::key_release; }

	codes::KeyCode getKey() { return m_key; }

private:
	codes::KeyCode m_key;
};

class KeyRepeatEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::key_repeat; }

	KeyRepeatEvent(codes::KeyCode code) : m_key(code) {}
	
	int get_categories() { return Event::EventCategory::keyboard | Event::EventCategory::input; }
	Event::EventType get_type() { return Event::EventType::key_repeat; }

	codes::KeyCode getKey() { return m_key; }

private:
	codes::KeyCode m_key;
};

class CharInputEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::char_input; }

	CharInputEvent(unsigned int code) : m_code(code) {}

	int get_categories() { return Event::EventCategory::keyboard | Event::EventCategory::input; }
	Event::EventType get_type() { return Event::EventType::char_input; }

	char getChar() { return (char)m_code; }

private:
	unsigned int m_code;
};


class WindowSetFocusEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::set_focus; }

	WindowSetFocusEvent() {}
	
	int get_categories() { return Event::EventCategory::window | Event::EventCategory::application; }
	Event::EventType get_type() { return Event::EventType::set_focus; }

};

class WindowRemoveFocusEvent : public Event {
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::remove_focus; }

	WindowRemoveFocusEvent() {}

	int get_categories() { return Event::EventCategory::window | Event::EventCategory::application; }
	Event::EventType get_type() { return Event::EventType::remove_focus; }
	
};


class WindowMaximizeEvent : public Event
{
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::maximize_window; }

	WindowMaximizeEvent() {};
	
	int get_categories() { return Event::EventCategory::window; }
	Event::EventType get_type() { return Event::EventType::maximize_window; }

};

class WindowRestoreEvent : public Event
{
public:
	static constexpr Event::EventType getEventType() { return Event::EventType::restore_window; }

	WindowRestoreEvent() {};

	int get_categories() { return Event::EventCategory::window; }
	Event::EventType get_type() { return Event::EventType::restore_window; }
	
};