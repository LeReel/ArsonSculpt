#pragma once
#include "AS_Model.h"
//! Shader -> Camera -> Mesh -> Model -> Application

class AS_Application
{
    std::string m_FilePath;
    std::string m_SelectedFile;
    
    std::vector<AS_Model> m_models;

public:
    AS_Application();
    ~AS_Application();

    void ProcessInput(GLFWwindow* _window);

    int Run();

private:
    void MainLoop(AS_Shader _shader, std::vector<AS_Model> _models);

    int Init();
    int InitGLModules();

    int InitGLFW();
    void SetWindowHints();
    int OpenWindow();

    int InitGLEW();

    void InitIMGUI();

    void OpenFileExplorer();

    void RenderUI();
    void ComputeCoordinateSystems(AS_Shader _shader);

    void DeallocateAllResources();
};
