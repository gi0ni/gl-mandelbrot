#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>

#include <utils/shader.h>

class State
{
	// Singleton
	private:
	static State* instance;
	State();

	public:
	static State* GetInstance();

	// Library stuff
	GLFWwindow* window = nullptr;
	GLFWwindow* shaderReloadContext = nullptr;
	glm::vec2 windowPx = glm::vec2(1152, 648);
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	std::unordered_map<std::string, Shader*> shaders;
	ImFont* mainFont = nullptr;

	#ifdef _WIN32
	std::string fontPath = "/Windows/Fonts/arial.ttf";
	#endif
	#ifdef __linux__
	std::string fontPath = "/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf";
	#endif

	// Actual data
	float aspect = windowPx.x/windowPx.y;
	glm::vec2 mouseCoord;
	glm::vec2 pivot = glm::vec2(0, 0);

	bool firstMouse = true;
	float zoom = 1.0f;

	bool redrawWindow = true;
};

extern State& g;
