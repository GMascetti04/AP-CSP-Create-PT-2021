#pragma once
#include "Engine/vendor/entt/single_include/entt/entt.hpp"
#include "Engine/Debug/Logger.h"
#include "Entity.h"
class Scene;

//basic wrapper for entt registry
class Registry {
public:
	Registry(Scene* scene) : m_scene(scene) {	}

	
	~Registry() { m_registry.clear(); }

	Entity createEntity() {
		auto id = m_registry.create();
		return Entity(id, m_scene);
	}
	void destroyEntity(Entity ent)
	{
		m_registry.destroy(ent.m_id);
	}

	
	template<typename Component, typename ...Args>
	Component& addComponent(const Entity& entity, Args &&... args)
	{
		if (hasComponent<Component>(entity))
		{
			Log_EngineWarn("Entity already has specified component");
			return m_registry.get<Component>(entity.m_id);
		}
		return m_registry.emplace<Component>(entity.m_id, std::forward<Args>(args)...);
	}

	//returns true if a component was actually removed
	template<typename Component>
	bool removeComponent(const Entity& entity) {
		size_t amount_removed = m_registry.remove_if_exists<Component>(entity.m_id);
		return amount_removed != (size_t)0;
	}

	template<typename Component>
	bool hasComponent(const Entity& entity) { return m_registry.has<Component>(entity.m_id); }

	template<typename Component>
	Component& getComponent(const Entity& entity) {
		return m_registry.get<Component>(entity.m_id);
	}

	void clear()
	{
		m_registry.clear();
	}

	template<typename Component>
	auto View()
	{
		return m_registry.view<Component>();
	}

	template<typename Component1, typename Component2>
	auto Group() {
		return m_registry.group<Component1>(entt::get<Component2>);
	}

	template<typename Component>
	auto Group()
	{
		return m_registry.group<Component>();
	}

private:
	Scene* m_scene; 
	entt::registry m_registry;
};