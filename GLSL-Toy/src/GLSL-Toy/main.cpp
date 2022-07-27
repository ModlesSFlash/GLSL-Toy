#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

#ifndef MF_DIST
#define MF_LOG(x) x
#include <chrono>
#else 
#define MF_LOG(x)
#endif

#define WIDTH  640
#define HEIGHT 480

struct App
{
    bool quit;
    double shader_time;

    GLuint shader_program;
    struct Shader_Cache
    {
        GLuint vert_shader;
        GLuint frag_shader;
    } shader_cache;
    struct Shader_Path_Cache
    {
        std::string vert_shader_path;
        std::string frag_shader_path;
    } shader_path_cache;
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

GLuint get_compiled_shader_from_file(const std::string& shader_path, const GLuint& shader_type)
{
    // read file from shader_path
    std::string shader_code, line;
    std::ifstream stream(shader_path);
    
    if (stream.is_open())
    {
        while (getline(stream, line))
        {
            shader_code += line + "\n";
        }
        stream.close();
    }
    else
    {
        std::cout << "bruh";
    }
    std::cout << shader_code;
    
    const char* shader_code_c = shader_code.c_str();
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code_c, nullptr); // what the hell is the 4th argument
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

void recompile_shaders()
{
    glDeleteProgram(g_app.shader_program);

    g_app.shader_cache.frag_shader = get_compiled_shader_from_file(g_app.shader_path_cache.frag_shader_path, GL_FRAGMENT_SHADER);

    g_app.shader_program = glCreateProgram();
    glAttachShader(g_app.shader_program, g_app.shader_cache.vert_shader);
    glAttachShader(g_app.shader_program, g_app.shader_cache.frag_shader);
    //glBindFragDataLocation(g_app.shader_program, 0, "outColor");
    glLinkProgram(g_app.shader_program); // what does it mean to link a program?
    glDeleteShader(g_app.shader_cache.frag_shader);

    glfwSetTime(0.0f);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_R || key == GLFW_KEY_F5) && action == GLFW_PRESS)
        recompile_shaders();
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        g_app.quit = true;
}

int main(int argc, char** argv)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);   // to make window run 'always on top'
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // to make window run 'borderless'
                                                // ... except you can't move it at all if you do this bruv

    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
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

    // relative paths won't work in VS unless you change 'working directory' of your IDE to 'target dir'
    // https://github.com/premake/premake-core/issues/392
    // this isn't working is it

    g_app.shader_path_cache.vert_shader_path = "shaders/vert_shader.glsl";
    g_app.shader_path_cache.frag_shader_path = "shaders/frag_shader.glsl";

    g_app.shader_cache.vert_shader = get_compiled_shader_from_file(g_app.shader_path_cache.vert_shader_path, GL_VERTEX_SHADER);
    g_app.shader_cache.frag_shader = get_compiled_shader_from_file(g_app.shader_path_cache.frag_shader_path, GL_FRAGMENT_SHADER);

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
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // DO NOT unbind EBO if you're inside the bound VAO
    }
    glBindVertexArray(0);

    g_app.shader_program = glCreateProgram();
    glAttachShader(g_app.shader_program, g_app.shader_cache.vert_shader);
    glAttachShader(g_app.shader_program, g_app.shader_cache.frag_shader);
    //glBindFragDataLocation(g_app.shader_program, 0, "outColor");
    glLinkProgram(g_app.shader_program); // what does it mean to link a program?
    glDeleteShader(g_app.shader_cache.frag_shader);

    // main loop
    while (!glfwWindowShouldClose(window) && !g_app.quit)
    {
        glfwSetKeyCallback(window, key_callback);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(g_app.shader_program);

        glBindVertexArray(VAO);
        {
            g_app.shader_time = glfwGetTime();
            GLint uTime_location = glGetUniformLocation(g_app.shader_program, "uTime");
            if (uTime_location == -1)
            {
                std::cout << "Error getting uniform location of uTime\n";
            }
            else
            {
                glUniform1f(uTime_location, (float)g_app.shader_time);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(g_app.shader_cache.vert_shader);
    glDeleteProgram(g_app.shader_program);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);

    glfwTerminate();
    glfwDestroyWindow(window);

	return 0;
}