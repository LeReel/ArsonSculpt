#include "src/Scripts/Application/Public/AS_Application.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW.
// Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM (3D maths)
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>

//Declare window as global otherwise create an unresolved ext. error
GLFWwindow* window;

// This will identify our vertex array
GLuint vertexArrayID;
// This will identify our vertex buffer
GLuint vertexBufferID;
// And this will identify our shader program
GLuint shaderProgram;

AS_Application::AS_Application()
{
}

AS_Application::~AS_Application()
{
}

int AS_Application::Run()
{
    if (Init() == -1)
    {
        return -1;
    }
    
    CreateVAO();
    CreateVBO();

    shaderProgram = LoadShaders("../src/SimpleVertexShader.vertexshader",
                                "../src/SimpleFragmentShader.fragmentshader");

    GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };
    
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
    {
        MainLoop();
    }

    DeallocateAllResources();

    glfwTerminate();

    return 0;
}

void AS_Application::MainLoop()
{
    //Watch out, can cause flickering
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    DrawTriangle();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int AS_Application::Init()
{
    if (InitGLModules() == -1)
    {
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0, 0, 1, 1);

    return 0;
}

int AS_Application::InitGLModules()
{
    if (InitGLFW() == -1)
    {
        return -1;
    }
    SetWindowHints();

    if (OpenWindow() == -1)
    {
        return -1;
    }
    //Make the window's context current for the calling thread
    glfwMakeContextCurrent(window);

    if (InitGLEW() == -1)
    {
        return -1;
    }

    return 0;
}

int AS_Application::InitGLFW()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    return 0;
}

void AS_Application::SetWindowHints()
{
    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // We don't want the old OpenGL 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int AS_Application::OpenWindow()
{
    window = glfwCreateWindow(1024, 768, "Arson Sculpt", NULL, NULL);
    if (window == NULL)
    {
        fprintf(
            stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    return 0;
}

int AS_Application::InitGLEW()
{
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    return 0;
}

void AS_Application::CreateVAO()
{
    //Create a VertexArrayObject (contains Vertex Buffer Objects, it stores the information for a complete rendered object
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
}

void AS_Application::CreateVBO()
{
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexBufferID);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
}

void AS_Application::DrawTriangle()
{
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(
        0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
}

void AS_Application::DeallocateAllResources()
{
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteProgram(shaderProgram);
}
