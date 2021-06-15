#include "xpch.h"
#include "GLFW/glfw3.h"
#include "Core/Application.h"
#include "Input.h"

bool Input::keyPressed(codes::KeyCode code)
{
	GLFWwindow* window = application::getWindow()->getNativeWindow();
	int state = glfwGetKey(window, (int)code);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::mouseButtonPressed(codes::MouseCode code)
{
	GLFWwindow* window = application::getWindow()->getNativeWindow();
	int state = glfwGetMouseButton(window, (int)code);
	return state == GLFW_PRESS;
}

std::tuple<float, float> Input::mousePosition()
{
	return std::tuple<float, float>(getMouseX(), getMouseY());
}

float Input::percentMouseX()
{
	return (float)getMouseX()/ (float)application::getWindow()->getWidth();
}

float Input::percentMouseY()
{
	return ((float)getMouseY() / (float)application::getWindow()->getHeight());
}

float Input::getMouseX()
{
	double x_cord;
	GLFWwindow* window = application::getWindow()->getNativeWindow();
	glfwGetCursorPos(window, &x_cord, nullptr);
	return static_cast<float>(x_cord);
}

float Input::getMouseY()
{
	double y_cord;
	GLFWwindow* window = application::getWindow()->getNativeWindow();
	glfwGetCursorPos(window, nullptr, &y_cord);

	//make bottom of screen equal 0
	return static_cast<float>((application::getWindow()->getHeight()-y_cord));
}