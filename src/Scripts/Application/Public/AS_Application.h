#pragma once
#include <string>

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
    //VBO stores vertices in GPU's memory
    //Sending data to the GPU from the CPU is relatively slow
    //So wherever we can, send as much data as possible at once
    void CreateVBO(int _amnt);
    void CreateEBO();

    void Draw();

    void DeallocateAllResources();
};
