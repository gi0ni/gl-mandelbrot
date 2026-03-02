#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include "state.h"
#include "input.h"

void FramebufferSize_Callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	g.windowPx.x = width;
	g.windowPx.y = height;

	g.aspect = g.windowPx.x/g.windowPx.y;
}

void Window::Create()
{
	// glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	g.window = glfwCreateWindow(g.windowPx.x, g.windowPx.y, "OpenGL Window", NULL, NULL);
	glfwMakeContextCurrent(g.window);
	glfwSwapInterval(1); // vsync

	glfwWindowHint(GLFW_VISIBLE, false);
	g.shaderReloadContext = glfwCreateWindow(1, 1, "Shader Reload", NULL, g.window);

	// Callbacks
	glfwSetKeyCallback(g.window, Keyboard_Callback);
	// glfwSetInputMode(glb.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(g.window, CursorPos_Callback);
	glfwSetScrollCallback(g.window, Scroll_Callback);
	glfwSetMouseButtonCallback(g.window, Click_Callback);

	// glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, g.windowPx.x, g.windowPx.y);
	glfwSetFramebufferSizeCallback(g.window, FramebufferSize_Callback);

	// stb
	stbi_set_flip_vertically_on_load(true);

	// imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(g.window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");

	ImGuiIO& io = ImGui::GetIO();
	g.mainFont = io.Fonts->AddFontFromFileTTF(g.fontPath.c_str(), 16);
	IM_ASSERT(g.mainFont != nullptr);

	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// io.ConfigFlags |= ImGuiConfigFlags_NoMouse;

	ImGui::StyleColorsDark();
}

void Window::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	glfwDestroyWindow(g.shaderReloadContext);
	glfwDestroyWindow(g.window);
	glfwTerminate();
}
