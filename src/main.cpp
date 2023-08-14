#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
int main(void)
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

    /* Loop until the user closes the window */
    // Это цикл отрисовки. Он будет продолжаться, пока окно не будет закрыто
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        // Очищает буффер цвета
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        // Меняет местами задний и передний буффер цвета
        // Видеокарта рисует только задний буффер. А монитор отображает только передний буффер.
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents(); // Нажатие клавиш, изменение курсора и т.п.
    }

    glfwTerminate();
    return 0;
}