#include "AS_Application.h"

//Declare window as global otherwise create an unresolved ext. error
GLFWwindow* window;

// This will identify our vertex array
GLuint VAOs[2];
// This will identify our vertex buffer
GLuint VBOs[2];
// This will identify our element buffer
GLuint EBOs[1];
// And this will identify our shader program
GLuint shaderPrograms[2];

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

    //==============================================================================//
    GLfloat triangleVertices1[] = {
        //Triangle1
        -1.0f, -1.0f, 0.0f,
        -0.5f, -0.25f, 0.0f,
        -1.0f, -0.25f, 0.0f,
    };
    GLfloat triangleVertices2[] = {
        //Triangle2
        0.25f, 0.25f, 0.0f,
        0.5f, 1.0f, 0.0f,
        0.75f, 0.25f, 0.0f,
    };

    //? Create a VertexArrayObject that stores:
    //? - Calls to glEnable/disableVertexAttribArray
    //? - Attribute configurations via glVertexAttribPointer
    //? - VBO associated with vertex attributes by calls to glVertexAttribPointer()
    glGenVertexArrays(2, VAOs);
    // Generates buffers, puts the resulting identifier in VBOs
    glGenBuffers(2, VBOs);

    //1st triangle's configuration
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(triangleVertices1),
        triangleVertices1,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0);
    //? Each vertex attribute takes its data from memory managed by a VBO.
    //? Which VBO it takes its data from is the one bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer()
    glEnableVertexAttribArray(0);

    //2nd triangle's configuration
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(triangleVertices2),
        triangleVertices2,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);
    //==============================================================================//

    //==============================================================================//
    // GLfloat rectangleVertices1[] = {
    //     0.5f, 0.5f, 0.0f, // top right
    //     0.5f, -0.5f, 0.0f, // bottom right
    //     -0.5f, -0.5f, 0.0f, // bottom left
    //     -0.5f, 0.5f, 0.0f // top left 
    // };
    //
    // // Give our vertices to OpenGL (stores user-defined data into GPU memory; those data are managed by the currently bound buffer)
    // //? GL_STREAM_DRAW:  data is set only once / used at most a few times by the GPU.
    // //? GL_STATIC_DRAW:  data is set only once / used many times by the GPU.
    // //? GL_DYNAMIC_DRAW: data is changed a lot / used many times by the GPU.
    // glBufferData(
    //     GL_ARRAY_BUFFER, //Type of the buffer we want to copy data into
    //     sizeof(rectangleVertices1), //Size (in bytes) of the data we want to pass to the buffer
    //     rectangleVertices1, //Actual data we want to pass
    //     GL_STATIC_DRAW //How we want the GPU to manage the given data
    // );
    // //? Element Buffer Object (EBO) is a buffer that stores indices that GL uses to decide what vertices to draw.
    // //? This is called Indexed Drawing and avoids repeating vertices that overlap (as we draw mainly in triangles)
    // GLuint _indices[] = {
    //     0, 1, 3,
    //     1, 2, 3
    // };
    // glBufferData(
    //     GL_ELEMENT_ARRAY_BUFFER,
    //     sizeof(_indices),
    //     _indices,
    //     GL_STATIC_DRAW);
    //==============================================================================//


    // Shaders will process this data
    shaderPrograms[0]= LoadShaders("../src/SimpleVertexShader.glsl",
                                "../src/SimpleFragmentShader.glsl");
    shaderPrograms[1]= LoadShaders("../src/SimpleVertexShader.glsl",
                                "../src/SecondFragmentShader.glsl");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
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

    //Every shader and rendering call after this will use given program
    glUseProgram(shaderPrograms[0]);
    //Any subsequent VBO, EBO, glVertexAttribPointer() and glEnableVertexAttribArray calls
    //will be stored inside the currently bound VAO
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glUseProgram(shaderPrograms[1]);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

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
    glClearColor(1, .6f, 0, 1);

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
    glDeleteProgramsNV(2, shaderPrograms);
}

