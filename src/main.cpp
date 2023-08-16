#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
GLfloat point[] =
{
    0.0f,   0.5f,   0.0f,
    0.5f,   -0.5f,  0.0f,
    -0.5f,  -0.5f,  0.0f
};

GLfloat colors[] =
{
    1.0f,   0.0f,   0.0f,
    0.0f,   1.0f,   0.0f,
    0.0f,   0.0f,   1.0f
};


GLfloat texCoord[] =
{
    0.5f,   1.0f,
    1.0f,   0.0f,
    0.0f,   0.0f
};

const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main(){"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);"
"}";

int g_windowSizeX = 640;
int g_windowSizeY = 480;

// Колбэк изменения размера окна
void glWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY); // область, в которой мы хотим рисовать.  0, 0 - левый нижний угол
}

// Колбэк нажатия клавиш
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE); // закрываем окно

    }
}
int main(int argc, char** argv)
{

    GLFWwindow* pWindow;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    // здесь указывается минимально возможная рабочая версия OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow); // для текущего окна

	if(!gladLoadGL())
	{
		std::cout<<"Can't load GLAD!\n";
		return -1;
	}
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	std::cout<< "OpenGL "<< GLVersion.major << "."<<GLVersion.minor<<std::endl;
    
	
	glClearColor(0, 1, 0, 1);

    


    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }

        
        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        GLuint vao = 0; // vertex array object
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        /* Loop until the user closes the window */
        // Это цикл отрисовки. Он будет продолжаться, пока окно не будет закрыто
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            // Очищает буффер цвета
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            // Меняет местами задний и передний буффер цвета
            // Видеокарта рисует только задний буффер. А монитор отображает только передний буффер.
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents(); // Нажатие клавиш, изменение курсора и т.п.
        }
    }

    glfwTerminate();
    return 0;
}