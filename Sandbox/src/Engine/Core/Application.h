#pragma once
#include "xpch.h"
#include "Window.h"
#include "Scene.h"


namespace application {
	Window* getWindow();
	void SetActiveScene(const char* sceneName);

	Scene& getActiveScene();

	Scene& getScene(const char* sceneName);
	void createScene(const char* sceneName);
	void deleteScene(const char* sceneName);
}

struct ApplicationProperties {
	const char* title;
	unsigned int width;
	unsigned int height;
};