#pragma once

#include "Engine/Renderer/Texture.h"
#include "Engine/Utilities/Vec.h"
#include "Engine/DeltaTime.h"
#include "Entity.h"
#include "Engine/Input.h"
#include "Engine/Renderer/CameraController.h"




struct TagComponent {
	TagComponent(const char* str) : m_tag(str) { }
	const char* m_tag;
};


struct TransformComponent {
	TransformComponent() {}
	TransformComponent(float x_, float y_, float rot_, float x_scal, float y_scal) : x(x_), y(y_), rot(rot_), x_scale(x_scal), y_scale(y_scal) {}
	void SetPosition(float x_, float y_) { x = x_; y = y_; }
	float x, y; 
	float rot; 
	float x_scale, y_scale; 
};

struct Sprite2DComponent {
	Sprite2DComponent() {}
	Sprite2DComponent(bool vis, const char* lay, int ord, Texture* tex, Vec4<float> col) : visible(vis), layer(lay), order(ord), texture(tex), color(col) {}
	bool visible;
	const char* layer;
	int order;
	Texture* texture; 
	Vec4<float> color;

};


class Script;
struct ScriptComponent {
	ScriptComponent() {}

	Script* m_Script = nullptr;

	std::function<void(ScriptComponent&, Entity)> Constructor; 
	std::function<void(ScriptComponent&)> Destructor; 

	template<typename ScriptClass, typename... Args>
	void Attach(Args&&... args) {
		Constructor = [](ScriptComponent& sc, Entity entity) { sc.m_Script = (Script*)new ScriptClass(); sc.m_Script->m_entity = entity; };
		Destructor = [](ScriptComponent& sc) {delete sc.m_Script; sc.m_Script = nullptr; };
	}

};


class Script { 
	
public:
	Script() {}
	virtual ~Script() {}

	virtual void onCreate() {}
	virtual void onDestroy() {}

	virtual void onUpdate(DeltaTime& ts) {}
	virtual void onEvent(Event& event) {}
	virtual void onGuiUpdate() {}

protected:
	
	template<typename Component>
	Component& getComponent()
	{
		return m_entity.getComponent<Component>();
	}

	template<typename Component>
	bool hasComponent()
	{
		return m_entity.hasComponent<Component>();
	}


	Entity& getEntity() { return m_entity; }

	//will get where in the world space the mouse currently is
	std::tuple<float, float> getMousePosition();

	//convert from world cordinates to actual screen cordinates
	std::tuple<int, int> worldToScreenSpace(float x, float y);

private:
	friend class Scene;
	friend struct ScriptComponent;
	Entity m_entity;
};
