#pragma once

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

#include <common/shader.hpp>

class AS_Application
{    
public:
    AS_Application();
    ~AS_Application();

    int Run();

private:
    void MainLoop();

    int Init();
    int InitGLModules();

    int InitGLFW();
    void SetWindowHints();
    int OpenWindow();

    int InitGLEW();
    
    void DeallocateAllResources();
};
