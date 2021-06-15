#pragma once
#include "xpch.h"
#include "Engine/vendor/entt/single_include/entt/entt.hpp"

class Scene;
class Entity {
	
public:
	Entity() {}
	Entity(entt::entity id, Scene* scene) : m_id(id), m_scene(scene) { }
	
	template<typename Component, typename... Args>
	Component& addComponent(Args &&... args) {
		return m_scene->m_registry.addComponent<Component>(*this, std::forward<Args>(args)...);
		
	}

	template<typename Component>
	bool hasComponent() {
		return m_scene->m_registry.hasComponent<Component>(*this);
	}

	template<typename Component>
	bool removeComponent() {
		return m_scene->m_registry.removeComponent<Component>(*this);
	}

	template<typename Component>
	Component& getComponent() {
		return m_scene->m_registry.getComponent<Component>(*this);
	}

	entt::entity getID() {
		return m_id;
	}

	Scene* getScene() { return m_scene; }

private:
	friend class Registry; 
	entt::entity m_id;
	Scene* m_scene;
};