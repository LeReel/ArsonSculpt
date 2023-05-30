#pragma once
#include "AS_Model.h"
//! Shader -> Camera -> Mesh -> Model -> Application

class AS_Application
{
    //AS_Model m_model;
    //AS_Shader m_shader;
    
public:
    AS_Application();
    ~AS_Application();

    void ProcessInput(GLFWwindow* _window);

    int Run();

private:
    void MainLoop(AS_Shader _shader, AS_Model _model);

    int Init();
    int InitGLModules();

    int InitGLFW();
    void SetWindowHints();
    int OpenWindow();

    int InitGLEW();
    
    void DeallocateAllResources();
};
