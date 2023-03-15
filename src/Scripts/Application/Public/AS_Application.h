#pragma once

class AS_Application
{
public:
    AS_Application();
    ~AS_Application();

    int Run();

private:
    int Init();
    int InitGLFW();
    void SetWindowHints();
    int OpenWindow();
    int InitGLEW();
    
    //Create a VertexArrayObject (contains Vertex Buffer Objects, it stores the information for a complete rendered object
    void CreateVAO();
    /**
     * \brief This needs to be done only once
     * \param _data Pointer to data that will be copied into the data store for init
     */
    void CreateBuffer(const void* _data);

    void BuildAndCompileShaders();

    void DrawTriangle();
};
