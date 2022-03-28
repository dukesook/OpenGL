#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR) {
        //keep getting errors
    }
}
bool GLLogCall(const char* function, const char* file, unsigned int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
        std::cout << "\tfunction: " << function << std::endl;
        std::cout << "\tfile:     " << file << std::endl;
        std::cout << "\tline:     " << line << std::endl;
        return false;
    }
    return true;
}