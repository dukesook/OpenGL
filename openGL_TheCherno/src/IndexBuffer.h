#pragma once

class IndexBuffer {
private:
	unsigned int m_RendererID; //openGL needs an ID for each object
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int elementCount);
	~IndexBuffer();

	void Bind() const; 
	void Unbind() const;

	inline unsigned int GetCount() const {
		return m_Count;
	}
};