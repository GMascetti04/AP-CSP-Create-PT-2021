#pragma once
#include "xpch.h"
#include "Engine/Renderer/CameraController.h"
#include "Engine/ECS/Registry.h"


class Scene {
public:
	Scene(const char* name) :m_name(name), m_nextPriority(0) {}
	~Scene() {}

	bool pushEvent(Event& event);
	void onUpdate(DeltaTime& ts); 
	
	void useCamera(CameraController* cam);
	void deleteCamera();

	static void LoadScene(Scene* scene);
	static void CloseScene(Scene* scene);

	const char* getName() { return m_name; }

	CameraController* getCameraController() { return m_cameraController; }

	Entity createEntity();
	void destroyEntity(Entity ent);

	void createRenderLayer(const std::string& name, int priority);

	void removeRenderLayer(const std::string& name);

	void changeRenderLayer(const std::string& name, int priority);

	int getRenderLayerPriority(const std::string& name);

private:
	friend class Entity;
	const char* m_name;

	CameraController* m_cameraController;
	
	int m_nextPriority;
	
	//the const char* is the name, and the int is the priority of the layer
	std::unordered_map<const char*, int> m_EntityLayers; 

	std::vector<std::string> m_RenderLayerNames;
	std::vector<int> m_RenderLayerNumber;

	Registry m_registry = Registry(this);
};