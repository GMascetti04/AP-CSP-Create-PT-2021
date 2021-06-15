#include "xpch.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

/*
* Note:
* Math from: https://jsantell.com/model-view-projection/
*/
Camera::Camera(float left_bound, float right_bound, float down_bound, float up_bound)
	: m_projectionMatrix(glm::ortho(left_bound, right_bound, down_bound, up_bound, -1.0f, 1.0f)), m_viewMatrix(1.0f)
{
	ComputeViewMatrix();
	ComputeViewProjectionMatrix();
}

void Camera::setProjectionBounds(float left_bound, float right_bound, float down_bound, float up_bound)
{
	m_projectionMatrix = glm::ortho(left_bound, right_bound, down_bound, up_bound, -1.0f, 1.0f);
	ComputeViewProjectionMatrix();
}

void Camera::ComputeViewMatrix()
{
	m_viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3( m_position.x , m_position.y,0.0f )));
}

void Camera::ComputeViewProjectionMatrix()
{
	m_ViewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}
void Camera::setPosition(const glm::vec2& pos) {
	m_position = pos; 
	ComputeViewMatrix();
	ComputeViewProjectionMatrix();
}

