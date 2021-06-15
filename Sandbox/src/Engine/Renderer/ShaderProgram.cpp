#include "xpch.h"
#include "ShaderProgram.h"
#include "Engine/Debug/Logger.h"
#include "GL/glew.h"

unsigned int ShaderProgram::s_bound = 0;

ShaderProgram::ShaderProgram(Shader vert, Shader frag)
{
	m_id = glCreateProgram();
	glAttachShader(m_id, vert.getID());
	glAttachShader(m_id, frag.getID());
	glLinkProgram(m_id);
	glValidateProgram(m_id);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_id);
}

void ShaderProgram::bind()
{
	if (s_bound != m_id)
	{
		s_bound = m_id;
		glUseProgram(m_id);
	}
}

void ShaderProgram::unbind()
{
	s_bound = 0;
	glUseProgram(0);
}

int ShaderProgram::getUniformLocation(const char* name)
{
	glUseProgram(m_id);
	int location;
	if (m_cache.find(name) != m_cache.end())
		return m_cache[name];

	location = glGetUniformLocation(m_id, name);
	if (location == -1)
		Log_EngineWarn("Uniform has location -1: {0}", name);
	m_cache[name] = location;
	return location;
}

void ShaderProgram::setUniform1i(const char* name, int val)
{
	glUseProgram(m_id);
	int location = getUniformLocation(name);
	if (location == -1)
		Log_EngineWarn("Setting a uniform with location -1: {0}", name);
	glUniform1i(location, val);
}

void ShaderProgram::setUniform1f(const char* name, glm::vec1 vec)
{
	glUseProgram(m_id);
	int location = getUniformLocation(name);
	if (location == -1)
		Log_EngineWarn("Setting a uniform with location -1: {0}", name);
	glUniform1f(location, vec.x);
}

void ShaderProgram::setUniform2f(const char* name, glm::vec2 vec)
{
	glUseProgram(m_id);
	int location = getUniformLocation(name);
	if (location == -1)
		Log_EngineWarn("Setting a uniform with location -1: {0}", name);
	glUniform2f(location, vec.x, vec.y);
}

void ShaderProgram::setUniform3f(const char* name, glm::vec3 vec)
{
	glUseProgram(m_id);
	int location = getUniformLocation(name);
	if (location == -1)
		Log_EngineWarn("Setting a uniform with location -1: {0}", name);
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void ShaderProgram::setUniform4f(const char* name, glm::vec4 vec)
{
	glUseProgram(m_id);
	int location = getUniformLocation(name);
	if (location == -1)
		Log_EngineWarn("Setting a uniform with location -1: {0}", name);
	glUniform4f(location, (float)vec.x, (float)vec.y, (float)vec.z, (float)vec.w);
}

void ShaderProgram::setUniformMat4f(const char* name, glm::mat4 mat)
{
	glUseProgram(m_id);
	int location = getUniformLocation(name);
	if (location == -1)
		Log_EngineWarn("Setting a uniform with location -1: {0}", name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setUniform1iv(const char* name, int count, int* data)
{
	glUseProgram(m_id);
	int location = getUniformLocation(name);
	if (location == -1)
		Log_EngineWarn("Setting a uniform with location -1: {0}", name);
	glUniform1iv(location, count, data);
}
