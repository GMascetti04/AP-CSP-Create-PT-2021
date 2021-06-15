/*
* AP Computer Science Principles Create Performance Task
* libraries used:
*	[1] GLFW - cross-platform window library
*	link: https://github.com/glfw/glfw
*	[2] GLEW - cross-platform extension loading library
*	link: https://github.com/nigels-com/glew
*	[3] OpenGL - cross-platform rendering API
*	link: https://www.opengl.org//
*	[4] imgui - graphical user interface library
*	link: https://github.com/ocornut/imgui
*	[5] entt - game programming library (used for the ECS)
*	link: https://github.com/skypjack/entt
*	[6] spdlog - logging library
*	link: https://github.com/gabime/spdlog
*	[7] glm - math library for OpenGL
*	link: https://github.com/g-truc/glm
*	[8] stb_image - image loader
*	link: https://github.com/nothings/stb
*/
#include "xpch.h"
#include "Engine.h" 
#include "Engine/Entry.h"
#include "Chessboard.h"
#include "ChessPiece.h"


Camera* myCam;
Texture* texture;

void onStartup() { 

	

	application::createScene("Chess Game");
	

	float m_aspectRatio = (float)application::getWindow()->getWidth() / (float)application::getWindow()->getHeight();
	
	float m_zoom = (float)application::getWindow()->getHeight();
	myCam = new Camera(-m_aspectRatio * m_zoom / 2.0f, m_aspectRatio * m_zoom / 2.0f, -m_zoom / 2.0f, m_zoom / 2.0f);


	Scene& myScene = application::getScene("Chess Game");


	myScene.useCamera(new CameraController(myCam));

	application::SetActiveScene("Chess Game");


	myScene.createRenderLayer("Board", 1);
	myScene.createRenderLayer("Pieces", 2);

	
	Entity id = myScene.createEntity();
	id.addComponent<TagComponent>("Board");
	texture = Texture::BlankTexture(800, 800);
	id.addComponent<Sprite2DComponent>(true, "Board", 0, texture, Vec4<float>{1.0f, 1.0f, 1.0f, 1.0f});
	id.addComponent<TransformComponent>(0.0f, 0.0f, 0.0f, 800.0f, 800.0f);
	id.addComponent<ScriptComponent>().Attach<BoardScript>();


	Scene::LoadScene(&myScene);
}

void onShutdown() {
	Scene& myScene = application::getScene("Chess Game");
	myScene.deleteCamera();

	myScene.removeRenderLayer("Board");
	myScene.removeRenderLayer("Pieces");
	
	Scene::CloseScene(&myScene);
	application::deleteScene("Chess Game");

	delete myCam;
	delete texture;
}


ApplicationProperties createApplication() {
	return { "Chess Game",1000,1000 };
}