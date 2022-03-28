#pragma once

class VertexBuffer {
private:
	unsigned int m_RendererID; //openGL needs an ID for each object
public:
	VertexBuffer(const void* data, unsigned int size_bytes);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};