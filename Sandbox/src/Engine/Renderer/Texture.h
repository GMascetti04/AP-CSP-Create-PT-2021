#pragma once
#include "Engine/Utilities/Vec.h"


class Texture {
public:
	~Texture();

	static Texture* ImageTexture(const char* path);
	
	/*
	* draw a colored rectangle inside of the texture
	* note: bottom left is (0,0)
	*/
	void paintColoredQuad(int x_offset, int y_offset, int height, int width, Vec4<unsigned char>color);

	//create a white blank texture of a specified size
	static Texture* BlankTexture(int width, int height);

	

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }

	static int numberTextureSlots();

	inline unsigned int getId() { return m_id; }
private:
	Texture(unsigned char* data, int height, int width);

	friend class Renderer;

	void bind(unsigned int Tslot);
	void unbind(unsigned int Tslot);

	unsigned int m_id;
	unsigned char* m_data;
	int m_width;
	int m_height;
};