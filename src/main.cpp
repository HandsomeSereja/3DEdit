#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>


GLuint points_vbo;
GLuint color_vbo;

int size = 12;
float *point = new float[size];
float *colors = new float[size];

glm::ivec2 g_windowSize(640, 480);

int g_windowSizeX = 640;
int g_windowSizeY = 480;
double xpos, ypos;
int vertex_count = 4;

void show_array(float*& arr, int& size)
{
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " | ";
    }
    std::cout << std::endl;
}

void push_back(float *&arr, int &size, double value)
{
    float* newarray = new float[size+1];
    for (int i = 0; i < size; i++)
    {
        newarray[i] = arr[i];
    }
    newarray[size] = value;
    delete[] arr;
    arr = newarray;
}
void delArray(float*& arr, int& size)
{
    float* newarray = new float[size-3];
    for (int i = 0; i < size-3; i++)
    {
        newarray[i] = arr[i];
    }
    delete[] arr;
    arr = newarray;
}

void fillarray() 
{
    point[0] = -1.0f; point[1] = 1.0f; point[2] = 0.0f;
    point[3] = -1.0f; point[4] = 0.8f; point[5] = 0.0f;
    point[6] = -0.85f; point[7] = 1.0f; point[8] = 0.0f;
    point[9] = -0.85f; point[10] = 0.8f; point[11] = 0.0f;
    colors[0] = 1; colors[1] = 1; colors[2] = 0;
    colors[3] = 1; colors[4] = 1; colors[5] = 0;
    colors[6] = 1; colors[7] = 1; colors[8] = 0;
    colors[9] = 1; colors[10] = 1; colors[11] = 0;
    show_array(point,size);
    show_array(colors, size);
}

void glfwWindowSizeCallback(GLFWwindow* Window, int width, int height) 
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, width, height);

}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
 
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &xpos, &ypos);
        float x = float(xpos) / g_windowSize.x * (1.0 - -1.0) + -1.0;
        float y = float(ypos) / g_windowSize.y * (-1.0 - 1.0) + 1.0;
        std::cout << x << "|" << y << std::endl;
        if (x > -1.0f && x < -0.85f && y < 1.0f && y > 0.8f) 
        {
            if (vertex_count >= 5)
            {
                delArray(point, size);
                delArray(colors, size);
                size = size - 3;
                vertex_count = vertex_count - 1;
                delArray(point, size);
                delArray(colors, size);
                size = size - 3;
                vertex_count = vertex_count - 1;
                std::cout << size;
            }
        }
        else {
            push_back(point, size, x);
            push_back(colors, size, 1.0);
            size = size + 1;
            push_back(point, size, y);
            push_back(colors, size, 0.0);
            size = size + 1;
            push_back(point, size, 0);
            push_back(colors, size, 0.0);
            size = size + 1;
            vertex_count = vertex_count + 1;
            show_array(point, size);
        }
    }
}

int main(int argc, char** argv)
{
    GLFWwindow* window;
    fillarray();
    /* Initialize the library */
    if (!glfwInit())
	{
		std::cout << "glfwInit ERROR" << std::endl;
		return -1;
	}
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "3DEdit", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Load window ERROR" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	
	if (!gladLoadGL())
	{
		std::cout << "ERROR GL LOAD" << std::endl;
		return -1;
	}
	
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	
	glClearColor(0, 1, 0, 1);

    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can`t create shader program" << std::endl;
            return -1;
        }

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            

            GLuint vao = 0;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &points_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
            glBufferData(GL_ARRAY_BUFFER, size * 4, point, GL_STATIC_DRAW);

            glGenBuffers(1, &color_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
            glBufferData(GL_ARRAY_BUFFER, size * 4, colors, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            glPointSize(5);
            glLineWidth(5);
            glDrawArrays(GL_LINES, 4, vertex_count-4);
            glDrawArrays(GL_POINTS, 4, vertex_count-4);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    delete[] point;
    delete[] colors;
    return 0;
}