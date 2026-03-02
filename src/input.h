#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Input
{
	void Update(GLFWwindow* window);
}

void Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void CursorPos_Callback(GLFWwindow* window, double xpos, double ypos);
void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
void Click_Callback(GLFWwindow* window, int button, int action, int mods);
