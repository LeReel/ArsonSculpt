#include "AS_Application.h"

#include "AS_Shader.h"

//Declare window as global otherwise create an unresolved ext. error
GLFWwindow* window;

// This will identify our vertex array
GLuint VAOs[2];
// This will identify our vertex buffer
GLuint VBOs[2];
// This will identify our element buffer
GLuint EBOs[1];

double lastTime = glfwGetTime();
int nbFrames = 0;

void PrintFPS()
{
    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 1.0)
    {
        // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%f ms/frame\n", 1000.0 / double(nbFrames));
        nbFrames = 0;
        lastTime += 1.0;
    }
}

//Called when window size is changed 
void framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, _width, _height);
}

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

    GLfloat cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
    };

    //? Create a VertexArrayObject that stores:
    //? - Calls to glEnable/disableVertexAttribArray
    //? - Attribute configurations via glVertexAttribPointer
    //? - VBO associated with vertex attributes by calls to glVertexAttribPointer()
    glGenVertexArrays(1, VAOs);
    // Generates buffers, puts the resulting identifier in VBOs
    glGenBuffers(1, VBOs);

    //1st triangle's configuration
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(cubeVertices),
        cubeVertices,
        GL_STATIC_DRAW
    );
    //Position attribute
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);
    //? Each vertex attribute takes its data from memory managed by a VBO.
    //? Which VBO it takes its data from is the one bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer()
    //==============================================================================//

    //==============================================================================//
    //? Element Buffer Object (EBO) is a buffer that stores indices that GL uses to decide what vertices to draw.
    //? This is called Indexed Drawing and avoids repeating vertices that overlap (as we draw mainly in triangles)
    GLuint _indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Give our vertices to OpenGL (stores user-defined data into GPU memory; those data are managed by the currently bound buffer)
    //? GL_STREAM_DRAW:  data is set only once / used at most a few times by the GPU.
    //? GL_STATIC_DRAW:  data is set only once / used many times by the GPU.
    //? GL_DYNAMIC_DRAW: data is changed a lot / used many times by the GPU.
    glGenBuffers(1, &EBOs[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(_indices),
        _indices,
        GL_STATIC_DRAW);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);
    //==============================================================================//

    //!Do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO;
    //!Keep the EBO bound.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Modifying other VAOs requires a call to glBindVertexArray
    //so we generally don't unbind VAOs nor VBOs when it's not directly necessary.
    glBindVertexArray(0);

    // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
    {
        PrintFPS();
        MainLoop();
    }

    DeallocateAllResources();

    glfwTerminate();

    return 0;
}

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
};

void DrawObject(int _index, glm::vec3 _position, AS_Shader _shader)
{
    //! scaling -> rotations -> translations
    glm::mat4 _model = glm::mat4(1.0f);
    _model = glm::scale(_model, glm::vec3((glm::sin((float)glfwGetTime()))));

    float _angle = 20.0f * _index;
    //Multiplying the vertex coords with this model matrix transforms the vertex coords to world coords
    _model = glm::rotate(_model, (float)glfwGetTime() * glm::radians(_angle), glm::vec3(1.0f, 0.3f, 0.5f));

    _model = glm::translate(_model, _position);

    glm::mat4 _view = glm::mat4(1.0f);
    //Moves the entire scene around inversed to where we want the camera to move
    _view = glm::translate(_view, glm::vec3(0.0f, 0.0f, -10.0f));

    glm::mat4 _projection = glm::mat4(1.0f);
    _projection = glm::perspective(glm::radians(110.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    _shader.Use(); //Every shader and rendering call after this will use given program
    //? Finding uniform location doesn't require to use the shader program but updating its value does.
    _shader.SetMat4fv("model", glm::value_ptr(_model));
    _shader.SetMat4fv("view", glm::value_ptr(_view));
    _shader.SetMat4fv("projection", glm::value_ptr(_projection));

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void AS_Application::MainLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    AS_Shader firstShader("../src/SimpleVertexShader.glsl",
                          "../src/SimpleFragmentShader.glsl");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Any subsequent VBO, EBO, glVertexAttribPtr and glEnableVertexAttribArray calls will be stored inside the currently bound VAO
    glBindVertexArray(VAOs[0]);
    for (int i = 0; i < 10; ++i)
    {
        DrawObject(i, cubePositions[i], firstShader);
    }

    glBindVertexArray(NULL);

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
    glClearColor(0.3, 0.3, 0.3, 1);
    glEnable(GL_DEPTH_TEST);

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
            "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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

void AS_Application::DeallocateAllResources()
{
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    //glDeleteProgramsNV(2, shaderPrograms);
}
