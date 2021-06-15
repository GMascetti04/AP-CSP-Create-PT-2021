#include "xpch.h"
#include "VertexBufferLayout.h"
#include "GL/glew.h"

static_assert(sizeof(unsigned char) == sizeof(GLboolean), "size of GLboolean must equal size of unsigned char");
static_assert(sizeof(unsigned int) == sizeof(GLenum), "size of GLenum must equal size of unsigned int");
static_assert(sizeof(int) == sizeof(GLsizei), "size of GLsizei must equal size of int");


int ShaderType::sizes[8];
unsigned int ShaderType::types[8];
unsigned char ShaderType::bools[2];

void ShaderType::Init()
{

	//data from https://www.khronos.org/opengl/wiki/OpenGL_Type
	bools[(int)ShaderBool::True]  = GL_TRUE;
	bools[(int)ShaderBool::False] = GL_FALSE;

	types[(int)DataType::Byte]           = GL_BYTE;
	types[(int)DataType::Unsigned_Byte]  = GL_UNSIGNED_BYTE;
	types[(int)DataType::Short]          = GL_SHORT;
	types[(int)DataType::Unsigned_Short] = GL_UNSIGNED_SHORT;
	types[(int)DataType::Int]            = GL_INT;
	types[(int)DataType::Unsigned_Int]   = GL_UNSIGNED_INT;
	types[(int)DataType::Float]          = GL_FLOAT;
	types[(int)DataType::Double]         = GL_DOUBLE;
	

	sizes[(int)DataType::Byte]           = 1;
	sizes[(int)DataType::Unsigned_Byte]  = 1;
	sizes[(int)DataType::Short]          = 2;
	sizes[(int)DataType::Unsigned_Short] = 2;
	sizes[(int)DataType::Int]            = 4;
	sizes[(int)DataType::Unsigned_Int]   = 4;
	sizes[(int)DataType::Float]          = 4;
	sizes[(int)DataType::Double]         = 8;

}

int ShaderType::Size(DataType type)
{
	return sizes[(int)type];
}

unsigned int ShaderType::Type(DataType type)
{
	return types[(int)type];
}

unsigned char ShaderType::Value(ShaderBool val)
{
	return bools[(int)val];
}
