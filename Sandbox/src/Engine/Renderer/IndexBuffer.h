#pragma once


class IndexBuffer {
public:
	IndexBuffer(unsigned int length);
	~IndexBuffer();

	void bind();
	void unbind();

	unsigned int& operator[] (int index) {
		return m_data[index];
	}

	unsigned int length() { return m_length; }
	void bindData();

private:
	static unsigned int s_boundBuffer;

	unsigned int m_length;
	unsigned int* m_data;
	unsigned int m_id;
};