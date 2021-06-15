#include "xpch.h"
#include "GL/glew.h"
#include "Window.h"
#include "Event.h"
#include "Engine/Codes.h"

#include "GLFW/glfw3.h"
#include "Engine/Debug/MemoryTracker.h"
#include "Engine/Debug/Logger.h"

#define WINDOW_DATA(x) (*(windowData*)glfwGetWindowUserPointer( x ))

Window::Window(windowProperties props)
{
	this->m_title = props.name;

	//initialize glfw
	int success = glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (success == GLFW_FALSE)
	{
		log_EngineCritical("Error initializing GLFW");
		std::cin.get();
		exit(-1);
	}
	
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	
	this->m_window = glfwCreateWindow(props.width, props.height, props.name.c_str(), NULL, NULL);
	if (!(this->m_window))
	{
		log_EngineCritical("Error creating GLFW window");
		std::cin.get();
		exit(-1);
	}

	glfwMakeContextCurrent(m_window);

	if (glewInit() != GLEW_OK) {  
		log_EngineCritical("Error initializing GLEW");
		std::cin.get();
		exit(-1);
	}

	Log_EngineDebug(glGetString(GL_VENDOR));
	Log_EngineDebug(glGetString(GL_VERSION));


	glfwSetWindowUserPointer(m_window, &m_data);

	//set callbacks
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
		WindowResizeEvent event(w, h);
		WINDOW_DATA(window).func(event);
		});

	glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused) {
		if (focused == GLFW_TRUE) { //window was given focus
			WindowSetFocusEvent event;
			WINDOW_DATA(window).func(event);
		}
		else {
			WindowRemoveFocusEvent event;
			WINDOW_DATA(window).func(event);
		}
		});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
		WindowCloseEvent d;
		WINDOW_DATA(window).func(d);
		});

	glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* window, int maximized) {
		if (maximized == GLFW_TRUE)
		{
			WindowMaximizeEvent event;
			WINDOW_DATA(window).func(event);
		} else {
			WindowRestoreEvent event;
			WINDOW_DATA(window).func(event);
		}

		});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS)
		{
			KeyPressEvent event((codes::KeyCode)key);
			WINDOW_DATA(window).func(event);
		}
		else if (action == GLFW_RELEASE)
		{
			KeyReleaseEvent event((codes::KeyCode)key);
			WINDOW_DATA(window).func(event);
		}
		else
		{
			KeyRepeatEvent event((codes::KeyCode)key);
			WINDOW_DATA(window).func(event);
		}
		});

	glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
		CharInputEvent event(codepoint);
		WINDOW_DATA(window).func(event);
		});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
		if (action == GLFW_PRESS) {
			MouseButtonPressEvent event((codes::MouseCode)button);
			WINDOW_DATA(window).func(event);
		}
		else { 
			MouseButtonReleaseEvent event((codes::MouseCode)button);
			WINDOW_DATA(window).func(event);
		}
		});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x, double y) {
		if (x == 0) { 
			MouseScrollEvent event(y);
			WINDOW_DATA(window).func(event);
		}
		});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
		MouseMoveEvent event(xpos, ypos);
		WINDOW_DATA(window).func(event);
		});
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	this->m_window = nullptr;
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_window);
}

void Window::onUpdate()
{
	glfwSwapBuffers(m_window);

	double timeBeforePoll = glfwGetTime();
	glfwPollEvents();
	double timeAfterPoll = glfwGetTime();

	m_lastPollTime = glfwGetTime() - timeBeforePoll;
}

unsigned int Window::getHeight() {
	int m_height;
	glfwGetWindowSize(m_window, nullptr, &m_height);
	return (unsigned int)m_height;
}

unsigned int Window::getWidth() {
	int m_width;
	glfwGetWindowSize(m_window,&m_width, nullptr);
	return (unsigned int)m_width;
}

std::string Window::getTitle() {
	return m_title;
}

GLFWwindow* Window::getNativeWindow() {
	return this->m_window;
}