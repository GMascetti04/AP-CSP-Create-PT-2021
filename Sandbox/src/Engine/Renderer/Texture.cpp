#include "xpch.h"
#include "Texture.h"
#include "GL/glew.h"
#include "Engine/vendor/stb_image/stb_image.h"
#include "Engine/Debug/Logger.h"



Texture::~Texture()
{
	delete[] m_data;
	glDeleteTextures(1, &m_id);
}

Texture* Texture::ImageTexture(const char* path)
{
	//opengl requires the image texture flipped
	stbi_set_flip_vertically_on_load(1);

	int width, height;
	unsigned char* data = stbi_load(path, &width, &height, nullptr, 4);

	Texture* texture;
	if (data) {
		unsigned char* textureData = new unsigned char[width * height * 4];
		memcpy(textureData, data, 4 * width * height);
		stbi_image_free(data);
		texture = new Texture(textureData, width, height);
	} else {
		Log_EngineError("Image not found: {0}", path);
		texture = BlankTexture(1,1);
	}

	return texture;
}

Texture* Texture::BlankTexture(int width, int height)
{
	unsigned char* data = new unsigned char[width * height * 4];
	
	for (int i = 0; i < width * height * 4; i += 4) {
		data[i + 0] = (unsigned char)255;
		data[i + 1] = (unsigned char)255;
		data[i + 2] = (unsigned char)255;
		data[i + 3] = (unsigned char)255;
	}

	return new Texture(data, width, height);
}

Texture::Texture(unsigned char* data, int width, int height) : m_id(0), m_data(data), 
m_width(width), m_height(height)
{
	int previous_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture);

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);

	glBindTexture(GL_TEXTURE_2D, previous_texture);
}


void Texture::paintColoredQuad(int x_offset, int y_offset, int height, int width, Vec4<unsigned char> color)
{
	int previous_texture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_texture);
	glBindTexture(GL_TEXTURE_2D, getId());


	unsigned char* temp_buffer = new unsigned char[width * height * 4];
	for (int i = 0; i < width * height * 4; i += 4)
	{
		temp_buffer[i] = color.x;
		temp_buffer[i + 1] = color.y;
		temp_buffer[i + 2] = color.z;
		temp_buffer[i + 3] = color.w;
		
	}

	int i = 0;
	for (int r = y_offset; r < (height + y_offset); r += 1)
	{
		for (int c = x_offset * 4; c < (width + x_offset) * 4; c += 4)
		{
			m_data[(m_width * 4) * r + c + 0] = temp_buffer[i + 0];
			m_data[(m_width * 4) * r + c + 1] = temp_buffer[i + 1];
			m_data[(m_width * 4) * r + c + 2] = temp_buffer[i + 2];
			m_data[(m_width * 4) * r + c + 3] = temp_buffer[i + 3];
			i += 4;
		}
	}
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, temp_buffer);
	glBindTexture(GL_TEXTURE_2D, previous_texture);
	
	delete[] temp_buffer;
}

void Texture::bind(unsigned int Tslot)
{
	glActiveTexture(GL_TEXTURE0 + Tslot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind(unsigned int Tslot) 
{
	glActiveTexture(GL_TEXTURE0 + Tslot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::numberTextureSlots()
{
	int slots;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &slots);
	return slots;
}