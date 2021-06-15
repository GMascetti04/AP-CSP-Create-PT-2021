#pragma once
#include "xpch.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ShaderProgram.h"
#include "Engine/Utilities/Vec.h"
#include "Engine/Renderer/Texture.h"
#include "Camera.h"
#include <Engine/ECS/Components.h>
#include "glm/gtc/matrix_transform.hpp"


class Renderer {
public:
	Renderer() = delete;
	~Renderer() = delete;
	Renderer(const Renderer&) = delete;

	static void Init();
	static void Shutdown();

	static void DrawEntity(Sprite2DComponent& sprite, TransformComponent& transform);

	static void StartFrame();
	static void EndFrame();
	static int getDrawCallsPerFrame();
	
	constexpr static unsigned int MAX_INDEX_LENGTH = 10000 * 6;
	constexpr static int MAX_VERTEX_LENGTH = 10000 * 4;

	static glm::vec4 GetClearColor();
	static void SetClearColor(glm::vec4& color);
	static void Clear();
private:
	static void DrawQuad(const Vec2<float>& center, const Vec2<float>& dimensions, Vec4<float> color);
	static void DrawQuad(const Vec2<float>& center, const Vec2<float>& dimensions, Texture* texture, Vec4<float> color);

	static glm::vec4 s_clear_color;
};