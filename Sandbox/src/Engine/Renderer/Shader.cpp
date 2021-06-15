#include "xpch.h"
#include "Shader.h"
#include "GL/glew.h"
#include "Engine/Debug/Logger.h"
#include "Engine/Debug/MemoryTracker.h"


void Shader::CompileString(std::string& str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '\r')
			str.erase(i);
	}

	const char* src = str.c_str();
	m_id = glCreateShader((int)(m_type));
	glShaderSource(m_id, 1, &src, nullptr);
	glCompileShader(m_id);

	int compilationResult;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &compilationResult);
	if (compilationResult == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

		char* errors = new char[maxLength];
		glGetShaderInfoLog(m_id, maxLength, nullptr, errors);
		glDeleteShader(m_id);

		Log_EngineError("OpenGL Shader Failed to Compile: {0} \n{1}\n{2}", "Shader defined in .cpp file", &str[0], errors);
		delete[] errors;
	}
}