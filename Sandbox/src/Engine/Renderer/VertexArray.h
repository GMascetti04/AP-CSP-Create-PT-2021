#pragma once
#include "xpch.h"
#include "Engine/Renderer/VertexBufferLayout.h"
#include "Engine/Renderer/VertexBuffer.h"

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void bind();
	void unbind();

	template<typename VertexClass>
	void setLayout(VertexBuffer<VertexClass>& vb, VertexBufferLayout& layout)
	{
		vb.bind();
		bind();
		std::vector<VertexBufferLayoutElement>& elements = layout.getElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); ++i) {
			VertexBufferLayoutElement& element = elements.at(i);
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.m_count, ShaderType::Type(element.m_type),
				ShaderType::Value(element.m_normalized), layout.getStride(), (const void*)(offset));

			offset += element.m_count * ShaderType::Size(element.m_type);
		}
	}

private:
	static unsigned int s_bound;
	unsigned int m_id;
};