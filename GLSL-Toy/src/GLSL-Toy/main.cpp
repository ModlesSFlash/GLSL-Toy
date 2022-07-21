#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <string>

#ifndef MF_DIST
#define MF_LOG(x) x
#include <chrono>
#else 
#define MF_LOG(x)
#endif

#define WIDTH  1280
#define HEIGHT 720

struct App
{
    bool quit;
} g_app;

void update()
{
	MF_LOG(auto buf_time = std::chrono::high_resolution_clock::now());

	//stuff

	MF_LOG(std::cout << "update() took " << (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - buf_time).count()) / 1000000.f << "\tms\n");
}

void render()
{
	MF_LOG(auto buf_time = std::chrono::high_resolution_clock::now());

    

	MF_LOG(std::cout << "render() took " << (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - buf_time).count()) / 1000000.f << "\tms\n");
}

GLuint get_compiled_shader(const char* shader_code, const GLuint& shader_type)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code, nullptr); // what the hell is the 4th argument
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        GLchar* info_log = new GLchar[length + 1];
        glGetShaderInfoLog(shader, length, &length, info_log);

        std::cout << info_log << std::endl;
        delete[] info_log;
    }
    return shader;
}

void recompile_shader()
{

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        recompile_shader();
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        g_app.quit = true;
}

int main(int argc, char** argv)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL); // how do i make it borderless and run on top?
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    char const* frag_shader_text = R"GLSL(
	#version 450
	out vec4 outColor;
	void main()
	{
        vec2 fragCoord = gl_FragCoord.xy / vec2(1280.f, 720.f);
        //fragCoord -= fragCoord;
		outColor = vec4(fragCoord.xy, 0.0, 1.0);
	}
	)GLSL";

    const char* vert_shader_text = R"GLSL(
    #version 450
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
    )GLSL";   

    GLuint vert_shader = get_compiled_shader(vert_shader_text, GL_VERTEX_SHADER);
    GLuint frag_shader = get_compiled_shader(frag_shader_text, GL_FRAGMENT_SHADER);

    GLuint VAO, VBO, EBO;

    // not sure what is this for exactly since the thing works without it anyway, but i guess it's needed for storing configurations for different vertex arrays or smth
    glGenVertexArrays(1, &VAO);

    // need that for storing vertices in the GPU memory, google the purpose of VBO (Vertex Buffer Object)
    glGenBuffers(1, &VBO);

    // need that for drawing the triangle with only 4 vertices instead of 6, google the purpose of EBO or IBO (Element Buffer Object or Index Buffer Object)
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // use VAO
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        {
            GLfloat const vertices[] = { -1.0f,  -1.0f,  // 0
                                         -1.0f,   1.0f,  // 1
                                          1.0f,   1.0f,  // 2
                                          1.0f,  -1.0f };// 3
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // pass the data from array to GPU mem. 'STATIC' since we're never gonna change these vertices
            GLint pos_attrib = 0; // glGetAttribLocation(program, "position") ;
            glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(pos_attrib);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        {
            GLint const elements[] = { 0, 1, 2, 0, 2, 3 }; // order of passing vertices into the vertex shader, corresponding to the IDs written near the vertices
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW); // pass the data from array to GPU mem. 'STATIC' for the same reason stated above
        }
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); / DO NOT unbind EBO if you're inside the bound VAO
    }
    glBindVertexArray(0);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    //glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program); // what does it mean to link a program?
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    while (!glfwWindowShouldClose(window) && !g_app.quit)
    {
        glfwSetKeyCallback(window, key_callback);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(program);
        glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    glDeleteProgram(program);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);

    glfwTerminate();
    glfwDestroyWindow(window);

	return 0;
}