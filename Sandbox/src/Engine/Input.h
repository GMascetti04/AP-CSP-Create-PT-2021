#pragma once
#include "xpch.h"
#include "Codes.h"

namespace Input {
	//returns true if the specified key is currently pressed
	bool keyPressed(codes::KeyCode code);

	//returns true if the specified mouse button is currently pressed
	bool mouseButtonPressed(codes::MouseCode code);

	//returns the current mouse position
	//note: bottom left is (0,0) 
	std::tuple<float, float> mousePosition();

	//returns a number from 0.0 - 1.0
	//returns coordinate / dimension length
	float percentMouseX();
	float percentMouseY();

	float getMouseX();
	float getMouseY();
}