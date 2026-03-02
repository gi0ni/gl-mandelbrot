#include "input.h"
#include "state.h"

void Input::Update(GLFWwindow* window)
{
}

void Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void CursorPos_Callback(GLFWwindow* window, double xpos, double ypos)
{
	if(g.firstMouse == true)
	{
		g.firstMouse = false;
		return;
	}

	glm::vec2 mousePos(xpos, g.windowPx.y - ypos);

	g.mouseCoord = mousePos/g.windowPx * 4.0f - 2.0f;
	g.mouseCoord *= glm::vec2(g.aspect, 1);
	g.mouseCoord /= g.zoom;
}

void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void Click_Callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		g.zoom *= 2;
		g.pivot += g.mouseCoord;
		g.redrawWindow = true;
	}

	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		g.zoom /= 2;
		g.redrawWindow = true;
	}
}
