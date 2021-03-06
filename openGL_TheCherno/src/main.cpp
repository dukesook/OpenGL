
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream> //file stream, 
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

//SHADERS
struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};
static struct ShaderProgramSource ParseShader(const std::string& filepath) {

    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}
static unsigned int compileShader(unsigned int type, const std::string source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) { //error
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); //alloca() allocates memory within the current function's stack frame. Memory allocated using alloca() will be removed from the stack when the current function returns. alloca() is limited to small allocations.
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;

}
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program_id = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program_id, vs);
    glAttachShader(program_id, fs);
    glLinkProgram(program_id);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program_id;
}



static void drawTriangle() {
    //Draw a triangle using legacy opengl
    //Place inside game loop
    //typically, you want to use modern opengl
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    //glDrawArrays(GL_TRIANGLES, 0, 6); //use this function when you DON'T have an index buffer. arg1: type. arg2: starting index. arg3: vertex count (2 coordinate = 1 vertex);

}


int main(void)
{
    GLFWwindow* window;

    //INIT GLFW
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //sets version to 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //sets version to x.3
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); //Set to Compatibility - creates Vertex Arraysby default
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Set to core - DOESN'T create Vertex Arrays by default. Now you must bind a vertex array object prior to calling glEnableVertexAttribArray()


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); //synchornizes with monitor refresh rate

    if (glewInit() != GLEW_OK) {
        std::cout << "glewInit() Error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl; //4.6.0 - Build 27.20.100.9621

    //VERTEX  
    float positions[] = {
        -0.5f, -0.5f, //0
         0.5f, -0.5f, //1
         0.5f,  0.5f, //2
        -0.5f,  0.5f  //3
    };

    unsigned int indices[] = {
        0, 1, 2, //triangle #1
        2, 3, 0  //triangle #2
    };

    //VERTEX BUFFER - used to specify triangle coordinates
    unsigned int positionsVertexCount = 4;
    unsigned numbersPerVertex = 2;
    unsigned int positionsSize = positionsVertexCount * numbersPerVertex * sizeof(float);
    VertexBuffer vb(positions, positionsSize);

    //VERTEX ARRAY - defines bytes layout
    unsigned int vao; //Vertex Array Object
    GLCall(glGenVertexArrays(1, &vao)); //Generate Vertex Array
    GLCall(glBindVertexArray(vao));     //Bind Vertex Array
    VertexArray va;
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);



    //TELL OPENGL OUR LAYOUT
    //int startingIndex = 0;
    //bool normalized = false;
    //int stride = sizeof(float) * numbersPerVertex;
    //GLCall(glEnableVertexAttribArray(0)); //Enables the selected buffer. arg1: the index that you want to enable.
    //GLCall(glVertexAttribPointer(startingIndex, numbersPerVertex, GL_FLOAT, normalized, stride, 0)); //defines an array of generic vertex attribute data //arg1: starting index. arg2: how many numbers are in 1 vertex. arg3: type of data. arg4: true = normalized (0 < x < 1), false = scalar (0 < x < 255). arg5: stride: number of bytes for each vertex. arg6: wtf

    //INDEX BUFFER - used to index into our vertex buffer
    unsigned int indicesVertexCount = 6;
    IndexBuffer ib(indices, indicesVertexCount);

    //SHADERS
    ShaderProgramSource source = ParseShader("Basic.shader");
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource); //Vertex Shaders: ???? Fragment Shaders: Tell opengl which color to use
    GLCall(glUseProgram(shader)); //"binding the shader"

    //UNIFORM
    GLCall(int location = glGetUniformLocation(shader, "u_Color")); //getting the location of the "u_Color" variable located in the shader
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f)); //Set the data in the shader. 4f = 4 floats in a vertex

    //UNBIND ALL (just for practice)
    //GLCall(glBindVertexArray(0));
    //GLCall(glUseProgram(0));
    //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float red = 0.0f;
    float increment = 0.05f;

    //GAME LOOP
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        //RE-BIND
        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, red, 0.3f, 0.8f, 1.0f)); //Set the data in the shader. 4f = 4 floats in a vertex

        //Code is replaced by Vertex Array
        GLCall(glBindVertexArray(vao)); //notice, you are not binding your vertex buffer.
        va.Bind();

        //BIND
        ib.Bind();

        //DRAW
        GLCall(glDrawElements(GL_TRIANGLES, indicesVertexCount, GL_UNSIGNED_INT, nullptr));

        if (red > 1.0f) {
            increment = -0.05f;
        }
        else if (red < 0.0f) {
            increment = 0.05f;
        }

        red += increment;

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}


/*

libaraies
    All libraries has 2 essential parts
    1. An 'include' folder the contains all the .h files
    2. A 'lib' folder that contains .lib files.

glew
    openGL Extension Wrangler.
     - We need a way to call the opengl functions.
     - Rembember their implementations are define on your graphics card
     - glew returns function pointers from your GPU
    Download: http://glew.sourceforge.net/index.html
     - I used the binary version as opposed to the source code
     - I stored in here: C:\Users\devon\VisualStudio\solution_game\Dependencies\glew-2.1.0
    Documentation: http://glew.sourceforge.net/basic.html
    VS Properties Page
     - C/C++  -> General -> Additional Include Directores  -> $(SolutionDir)Dependencies\glew-2.1.0\include\GL
     - Linker -> General -> Additional Library Directories -> $(SolutionDir)Dependencies\glew-2.1.0\lib\Release\x64
     - Linker -> Input   -> Additional Dependencies -> glew32s.lib
     - C/C++  -> Preprocessor -> Preprocessor Definitions -> GLEW_STATIC
    Gochas
     - call glewInit() AFTER glfwMakeContextCurrent(window);
     - #include <GL/glew.h> BEFORE #include <GLFW/glfw3.h>

Vertex
    Array of bytes in memory, just of buffer
    stored in the GPU
Shader
    Program that runs on the GPU and draws images
    Vertex Shader - gets called for each vertex, position, called first
    Fragment Shader - aka Pixel Shaders, called after vertex shader. Runs for each pixel which makes operations expensive. Decides which COLOR each pixel should be.
    To render an image, you select 1 Vertex & 1 shader
Uniforms & Attributes - The Cherno #11
    Both send data to the GPU
    Uniforms
        - Set per draw
    Attributes
        - Set per vertex
VERTEX ARRAYS vs VERTEX BUFFERS
Vetex Arrays
    - way to bind vertex buffers with a certain type of specification for that vertex buffer
    - contains a binding between a vertex buffer and a vertex layout.
    - Vertex Arrays are Mandatory
        Core Profile: Does not create Vertex Array automatically



*/


/*
VISUAL STUDIO HOT KEYS
 - ctr + D  Duplicate a line
 - ctr + C  Copy entire line (when nothing is selected)
 - ctr + X  Cut entire line
 - ctr + H  Find and replace
 - alt + up/down            move line of code
 - alt + shift + up/down    multiple cursors
*/

//VIDEO #2 - glfw
/*
 1. Download glfw
        https://www.glfw.org/
	    I downloaded 32 bit precompiled binaries becuase 32 bit works with both 32 & 64.
2. Copy Source Code
        https://www.glfw.org/documentation.html
3. .h files
        Contain function declarations
        C/C++  ->  General  ->  Additional Include Directories:  $(SolutionDir)Dependencies\GLFW\include
4. .lib files
        Contain function definitions
        Linker  ->  General  -> Additional Library Directories:  $(SolutionDir)Dependencies\GLFW\lib-vc2022
5. .lib files
        Linker  ->  Input  -> Additional Dependencies:  glfw3.lib; opengl32.lib
 
 */


//VIDEO #3 - glew
/*
  1. Download glew
        http://glew.sourceforge.net/
  2. .h files
        C/C++  ->  General  ->  Additional Include Directories:  $(SolutionDir)Dependencies\GLEW\include
  3. .lib files
        Linker  ->  General  -> Additional Library Directories:  $(SolutionDir)Dependencies\GLEW\lib\Release\Win32
  4. lib files
     Linker  ->  Input  -> Additional Dependencies:  glew32s.lib
  5. define static
     C/C++  ->  Preprocessor  ->  Preprocessor Definitions:  GLEW_STATIC
 */


//VIDEO #13 - Abstracting OpenGL into Classes
/*
* Vertex Buffer
* Index Buffer
* Renderer - takes in a command, draws on screen
* 
* 
* 

*/