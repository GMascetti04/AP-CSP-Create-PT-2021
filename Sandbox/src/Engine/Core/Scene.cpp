#include "xpch.h"
#include "Scene.h"
#include "Engine/Renderer/Renderer.h"
#include <Engine/ECS/Components.h>
#include <Engine/Gui/GuiLayer.h>



#if defined(DEBUG) || defined(RELEASE)
extern void engineGuiUpdate();
#endif

bool Scene::pushEvent(Event& event)
{
	m_cameraController->m_onEventFunction(event, *m_cameraController);
	auto script_entities = m_registry.View<ScriptComponent>();

	for (auto ent : script_entities)
		script_entities.get<ScriptComponent>(ent).m_Script->onEvent(event);

	return event.m_handled;
}

void Scene::onUpdate(DeltaTime& ts)
{ 
	auto ScriptEntities = m_registry.View<ScriptComponent>();

	for (auto entity : ScriptEntities)
		ScriptEntities.get<ScriptComponent>(entity).m_Script->onUpdate(ts);
	
	m_cameraController->m_onUpdateFunction(ts, *m_cameraController);

	Renderer::StartFrame();
	Renderer::Clear();
	
	
	auto renderable_entities = m_registry.Group<Sprite2DComponent, TransformComponent>();

	renderable_entities.sort<Sprite2DComponent>([this](const auto& lhs, const auto& rhs) {
		if (this->getRenderLayerPriority((lhs.layer)) != this->getRenderLayerPriority(rhs.layer))
			return this->getRenderLayerPriority(lhs.layer) < this->getRenderLayerPriority(rhs.layer);
		return lhs.order < rhs.order;
		});


	for (auto it = renderable_entities.begin(); it != renderable_entities.end(); it++)
	{
		TransformComponent& transform = renderable_entities.get<TransformComponent>(*it);
		Sprite2DComponent& sprite = renderable_entities.get<Sprite2DComponent>(*it);
		if (sprite.visible)
			Renderer::DrawEntity(sprite, transform);
	}

	Renderer::EndFrame();
	GuiLayer::Begin();
	
	for (auto entity : ScriptEntities)
		ScriptEntities.get<ScriptComponent>(entity).m_Script->onGuiUpdate();

#if defined(DEBUG) || defined(RELEASE)
	engineGuiUpdate();
#endif
	GuiLayer::End();
}


void Scene::useCamera(CameraController* cam)
{
	m_cameraController = cam;
}

void Scene::deleteCamera()
{
	if (m_cameraController)
	{
		delete m_cameraController;
		m_cameraController = nullptr;
	}
}

void Scene::LoadScene(Scene* scene)
{
	auto ScriptEntities = scene->m_registry.View<ScriptComponent>();
	for (auto ent : ScriptEntities)
	{
		ScriptEntities.get<ScriptComponent>(ent).Constructor(ScriptEntities.get<ScriptComponent>(ent), {ent,scene});
		ScriptEntities.get<ScriptComponent>(ent).m_Script->m_entity = Entity(ent, scene);
		ScriptEntities.get<ScriptComponent>(ent).m_Script->onCreate();
	}
}

void Scene::CloseScene(Scene* scene)
{
	auto view = scene->m_registry.View<ScriptComponent>();
	for (auto entity : view)
	{
		
		view.get<ScriptComponent>(entity).m_Script->onDestroy();
		view.get<ScriptComponent>(entity).Destructor(view.get<ScriptComponent>(entity));
	}
	
}


Entity Scene::createEntity()
{
	return this->m_registry.createEntity();
}

void Scene::destroyEntity(Entity ent)
{
	m_registry.destroyEntity(ent);
}



void Scene::createRenderLayer(const std::string& name, int priority)
{
	for (int i = 0; i < m_RenderLayerNames.size(); i++)
	{
		if (m_RenderLayerNames[i] == name)
		{
		
			Log_EngineWarn("RenderLayer in Scene aleady has this name!");
			return;

		}
	}

	m_RenderLayerNames.push_back(name);
	m_RenderLayerNumber.push_back(priority);
}

void Scene::removeRenderLayer(const std::string& name)
{
	for (int i = 0; i < m_RenderLayerNames.size(); i++)
	{
		if (m_RenderLayerNames[i] == name)
		{
			
			m_RenderLayerNames.erase(m_RenderLayerNames.begin() + i);
			m_RenderLayerNumber.erase(m_RenderLayerNumber.begin() + i);
			return;

		}
	}

	Log_EngineWarn("Specified Render Layer Does Not Exist! - ' {0} '",name);
	return;
}

void Scene::changeRenderLayer(const std::string& name, int priority)
{
	for (int i = 0; i < m_RenderLayerNames.size(); i++)
	{
		if (m_RenderLayerNames[i] == name)
		{
			m_RenderLayerNumber[i] = priority;
			return;

		}
	}

	Log_EngineWarn("RenderLayer ' {0} ' does not exist", name);
}

int Scene::getRenderLayerPriority(const std::string& name)
{
	for (int i = 0; i < m_RenderLayerNames.size(); i++)
	{
		if (m_RenderLayerNames[i] == name)
		{
			return m_RenderLayerNumber[i];
			
		}
	}

	Log_EngineWarn("RenderLayer does not exist - It cannot have a priority: -1 returned");
	return -1;
}
