#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int elementCount) :m_Count(elementCount) {
    
     ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    //VERTEX BUFFER - used to specify triangle coordinates
    unsigned bufferCount = 1;
    GLCall(glGenBuffers(bufferCount, &m_RendererID)); //arg1: Number of buffers to generate. arg2: Location where buffers will be stored
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); //arg1: defines the purpose, or how buffer will be used. The currently bound buffer is considered to be the "selected" buffer
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(unsigned int), data, GL_STATIC_DRAW)); //links a buffer to its data

}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); //arg1: defines the purpose, or how buffer will be used. The currently bound buffer is considered to be the "selected" buffer

}

void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); //arg1: defines the purpose, or how buffer will be used. The currently bound buffer is considered to be the "selected" buffer

}