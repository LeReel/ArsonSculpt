#include "AS_Application.h"

#include "src/Scripts/filesystem.h"

//Declare window as global otherwise create an unresolved ext. error
GLFWwindow* window;

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

const char* VERTEX_SHADER_PATH = "../src/SimpleVertexShader.glsl";
const char* FRAGMENT_SHADER_PATH = "../src/SimpleFragmentShader.glsl";

float lastFrame = 0.0f;
float deltaTime = 0.0f;

AS_Camera camera(glm::vec3(0.0f, 0.0f, 25.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

AS_Application::AS_Application()
{
}

AS_Application::~AS_Application()
{
}

//Called when window size is changed 
void framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, _width, _height);
}

//Listens to mouse-movement events
void mouse_callback(GLFWwindow* _window, double _xPos, double _yPos)
{
    float xpos = static_cast<float>(_xPos);
    float ypos = static_cast<float>(_yPos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

//Listens to mouse-scroll events
void scroll_callback(GLFWwindow* _window, double _xOffset, double _yOffset)
{
    camera.ProcessMouseScroll(static_cast<float>(_yOffset));
}

void AS_Application::ProcessInput(GLFWwindow* _window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

int AS_Application::Run()
{
    if (Init() == -1)
    {
        return -1;
    }

    AS_Shader myShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    AS_Model myModel("../resources/objects/cars/Plane_Car.obj");

    // Check if the ESC key was pressed or the window was closed
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window);

        MainLoop(myShader, myModel);
    }

    //TODO: Implement in objects/meshes ?
    DeallocateAllResources();

    glfwTerminate();

    return 0;
}

void AS_Application::MainLoop(AS_Shader _shader, AS_Model _model)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader.Use();

    // view/projection transformations
    glm::mat4 _projection = glm::perspective(glm::radians(camera.Zoom),
                                             (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                             0.1f,
                                             500.0f);
    glm::mat4 _view = camera.GetViewMatrix();
    _shader.SetMat4("projection", _projection);
    _shader.SetMat4("view", _view);

    // render the loaded model
    glm::mat4 _modelMat = glm::mat4(1.0f);
    _modelMat = glm::translate(_modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
    // translate it down so it's at the center of the scene
    _modelMat = glm::scale(_modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
    // it's a bit too big for our scene, so scale it down
    _shader.SetMat4("model", _modelMat);

    _model.Draw(_shader);

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
    //Hides the cursor and captures it
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Arson Sculpt", NULL, NULL);
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

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
    //glDeleteVertexArrays(2, VAOs);
    //glDeleteBuffers(2, VBOs);
    //glDeleteProgramsNV(2, shaderPrograms);
}
