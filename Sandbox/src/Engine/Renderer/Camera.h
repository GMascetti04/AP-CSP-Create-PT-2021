#pragma once
#include "xpch.h"
#include "Engine/Utilities/Vec.h"
#include "glm/glm.hpp"
#include "glm//gtc/constants.hpp"

/*
* Note:
* Math from: https://jsantell.com/model-view-projection/
*/
class Camera {
public:
	Camera(float left_bound, float right_bound, float down_bound, float up_bound);
	~Camera() {}

	void setProjectionBounds(float left_bound, float right_bound, float down_bound, float up_bound);

	void setX(float val) { m_position.x = val; }
	void setY(float val) { m_position.y = val; }

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() { return m_position; }


	glm::mat4 getViewProjMat() { return m_ViewProjectionMatrix; }


private:
	void ComputeViewMatrix();
	void ComputeViewProjectionMatrix();

	glm::mat4 m_viewMatrix; //inverse of model matrix
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_ViewProjectionMatrix; 

	glm::vec2 m_position {0.0f,0.0f};
};