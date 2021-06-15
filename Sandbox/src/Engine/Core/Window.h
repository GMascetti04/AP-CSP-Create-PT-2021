#pragma once
#include "xpch.h"
#include "Event.h"

typedef void(*EventCallbackFuntion)(Event&);
struct GLFWwindow;
struct windowProperties {
	std::string name;

	unsigned int height;
	unsigned int width;
};

class Window {
public:
	Window(windowProperties props);
	~Window();

	bool shouldClose();
	void onUpdate();

	unsigned int getWidth();
	unsigned int getHeight();
	std::string getTitle();

	inline void set_event_callback(const EventCallbackFuntion& f) { m_data.func = f; }

	GLFWwindow* getNativeWindow();

	inline void setHeight(unsigned int height) { m_height = height; }
	inline void setWidth(unsigned int width) { m_width = width; }

	inline double getLastPollTime(){ return m_lastPollTime; }

private:

	struct windowData {
		EventCallbackFuntion func;
	};
	unsigned int m_height;
	unsigned int m_width;
	windowData m_data;
	GLFWwindow* m_window;

	double m_lastPollTime = 0.0f;

	std::string m_title;
};