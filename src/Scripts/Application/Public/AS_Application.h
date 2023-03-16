#pragma once

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

    void CreateVAO();
    void CreateVBO();

    void DrawTriangle();

    void DeallocateAllResources();
};
