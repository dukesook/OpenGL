#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID)); //Generate Vertex Array
}
VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void  VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	
	Bind();		//bind vertex array 
	vb.Bind();	//bind vertex buffer

	//SETUP LAYOUT
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;


	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i)); //Enables the selected buffer. arg1: the index that you want to enable.
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*) offset)); //defines an array of generic vertex attribute data //arg1: starting index. arg2: how many numbers are in 1 vertex. arg3: type of data. arg4: true = normalized (0 < x < 1), false = scalar (0 < x < 255). arg5: stride: number of bytes for each vertex. arg6: wtf
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
															//GLCall(glVertexAttribPointer(startingIndex, numbersPerVertex, GL_FLOAT, normalized, stride, 0)); //defines an array of generic vertex attribute data //arg1: starting index. arg2: how many numbers are in 1 vertex. arg3: type of data. arg4: true = normalized (0 < x < 1), false = scalar (0 < x < 255). arg5: stride: number of bytes for each vertex. arg6: wtf
	}

}


void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}