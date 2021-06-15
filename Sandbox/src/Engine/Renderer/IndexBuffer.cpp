#include "xpch.h"
#include "IndexBuffer.h"
#include "GL/glew.h"

unsigned int IndexBuffer::s_boundBuffer = 0;

IndexBuffer::IndexBuffer( unsigned int length) : m_length(length)
{
	glGenBuffers(1, &m_id);
	m_data = new unsigned int[length];
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_id);
	unbind();
	delete[] m_data;
}

void IndexBuffer::bind()
{
	if (s_boundBuffer != m_id) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_length * sizeof(unsigned int), m_data, GL_DYNAMIC_DRAW);
		s_boundBuffer = m_id;
	}
}

void IndexBuffer::unbind()
{
	s_boundBuffer = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::bindData()
{
	bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_length * sizeof(unsigned int), m_data);
}
