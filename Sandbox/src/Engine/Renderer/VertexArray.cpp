#include "xpch.h"
#include "VertexArray.h"
#include "GL/glew.h"

unsigned int VertexArray::s_bound = 0;

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
	unbind();
}

void VertexArray::bind()
{
	if (s_bound != m_id)
	{
		glBindVertexArray(m_id);
		s_bound = m_id;
	}
}

void VertexArray::unbind()
{
	s_bound = 0;
	glBindVertexArray(0);
}



