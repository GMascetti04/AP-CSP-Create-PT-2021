#pragma once
#include "xpch.h"

class Shader {
public:

	enum class ShaderType {
		vertex = 0x8B31,
		fragment = 0x8B30
	};

public:

	Shader(Shader::ShaderType type) : m_id(0), m_type(type) {}
	~Shader() {}

	void CompileString(std::string& str);

	unsigned int getID() { return m_id; }
	Shader::ShaderType getType() { return m_type; }

private:
	unsigned int m_id;
	Shader::ShaderType m_type;
};