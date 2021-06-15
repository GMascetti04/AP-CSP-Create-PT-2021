#pragma once
#include "xpch.h"

enum class  ShaderBool {
	True = 0, False = 1
};
enum class DataType {
	Byte = 0, Unsigned_Byte = 1, Short = 2, Unsigned_Short = 3, Int = 4, Unsigned_Int = 5, Float = 6, Double = 7
};
class ShaderType {
public:
	static void Init();

	static int Size(DataType type);
	static unsigned int Type(DataType type);
	static unsigned char Value(ShaderBool val);

private:
	static int sizes[8];
	static unsigned int types[8];
	static unsigned char bools[2];
};

struct VertexBufferLayoutElement {
	VertexBufferLayoutElement(int count, ShaderBool normalized, DataType type) :
		m_count(count), m_normalized(normalized), m_type(type) {}

	int m_count;
	ShaderBool m_normalized;
	DataType m_type;

};

class VertexBufferLayout {
public:
	VertexBufferLayout() {};
	~VertexBufferLayout() {};

	VertexBufferLayout(std::initializer_list<VertexBufferLayoutElement> elements) : m_elements(elements)  {
		for (int i = 0; i < m_elements.size(); i++) {
			m_stride += (int)m_elements[i].m_count * (int)ShaderType::Size(m_elements[i].m_type);
		}
	}

	std::vector<VertexBufferLayoutElement>& getElements() { return m_elements; }
	int getStride() { return m_stride; }

private:
	std::vector<VertexBufferLayoutElement> m_elements;
	int m_stride = 0; 
};