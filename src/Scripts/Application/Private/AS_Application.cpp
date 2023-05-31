#include "AS_Application.h"

#include "src/Scripts/filesystem.h"

#include "src/Scripts/ImGUI/imgui.h"
#include "src/Scripts/ImGUI/imgui_impl_glfw.h"
#include "src/Scripts/ImGUI/imgui_impl_opengl3.h"

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
bool leftMousePressed = false;

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

void mouse_button_callback(GLFWwindow* _window, int _button, int _action, int _mods)
{
	firstMouse = leftMousePressed = _button == GLFW_MOUSE_BUTTON_LEFT && _action == GLFW_PRESS;
}

//Listens to mouse-movement events
void mouse_callback(GLFWwindow* _window, double _xPos, double _yPos)
{
	if (!leftMousePressed)
	{
		return;
	}

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

	std::vector<AS_Model> _models;
	//AS_Model plane_car("../resources/objects/cars/Plane_Car/Plane_Car.obj");
	//AS_Model vazz("../resources/objects/cars/Vazz/Vazz.obj");

	//_models.push_back(plane_car);
	//_models.push_back(vazz);

	InitIMGUI();

	// Check if the ESC key was pressed or the window was closed
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);
		MainLoop(myShader, _models);
	}

	//TODO: Implement in objects/meshes ?
	DeallocateAllResources();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void AS_Application::MainLoop(AS_Shader _shader, std::vector<AS_Model> _models)
{
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ComputeCoordinateSystems(_shader);

	for (AS_Model _model : _models)
	{
		_model.Draw(_shader);
	}

	RenderUI();

	glfwSwapBuffers(window);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

	glfwSetMouseButtonCallback(window, mouse_button_callback);
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

void AS_Application::InitIMGUI()
{
	ImGui::CreateContext();

	ImGuiIO& _io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void AS_Application::RenderUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//UI code goes here
	bool _showText = false;
	if (ImGui::Button("Show demo window", ImVec2(50, 50)))
	{
		ImGui::Text("Hello le Hueco Mundo");
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

float angle = 0;
void AS_Application::ComputeCoordinateSystems(AS_Shader _shader)
{
	_shader.Use();

	// view/projection transformations
	glm::mat4 _projectionMat = glm::perspective(glm::radians(camera.Zoom),
		(float)SCR_WIDTH / (float)SCR_HEIGHT,
		0.1f,
		500.0f);
	glm::mat4 _viewMat = camera.GetViewMatrix();
	_shader.SetMat4("projection", _projectionMat);
	_shader.SetMat4("view", _viewMat);

	glm::mat4 _modelMat = glm::mat4(1.0f);
	_modelMat = glm::translate(_modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
	_modelMat = glm::scale(_modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
	_modelMat = glm::rotate(_modelMat, angle, glm::vec3(0, 1, 0));
	angle += deltaTime;
	_shader.SetMat4("model", _modelMat);
}

void AS_Application::DeallocateAllResources()
{
	//glDeleteVertexArrays(2, VAOs);
	//glDeleteBuffers(2, VBOs);
	//glDeleteProgramsNV(2, shaderPrograms);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
