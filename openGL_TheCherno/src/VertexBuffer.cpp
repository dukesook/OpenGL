#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int dataSize) {
    //VERTEX BUFFER - used to specify triangle coordinates
    unsigned bufferCount = 1;
    unsigned int positionsVertexCount = 4;
    unsigned numbersPerVertex = 2;
    unsigned int positionsSize = positionsVertexCount * numbersPerVertex * sizeof(float);
    GLCall(glGenBuffers(bufferCount, &m_RendererID)); //arg1: Number of buffers to generate. arg2: Location where buffers will be stored
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); //arg1: defines the purpose, or how buffer will be used. The currently bound buffer is considered to be the "selected" buffer
    GLCall(glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW)); //links data(positions[]) to the currently bound buffer

}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); //arg1: defines the purpose, or how buffer will be used. The currently bound buffer is considered to be the "selected" buffer

}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); //arg1: defines the purpose, or how buffer will be used. The currently bound buffer is considered to be the "selected" buffer

}