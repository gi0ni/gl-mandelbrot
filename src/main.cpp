#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <utils/colors.h>
#include <utils/logger.h>
#include <utils/watcher.h>

#include <string>
#include <mutex>

#include "state.h"
#include "window.h"
#include "input.h"
#include "game.h"
#include "render.h"

std::mutex renderMutex;

void ShaderReload(const std::string& filepath)
{
	static bool init = false;
	if(init == false)
	{
		glfwMakeContextCurrent(g.shaderReloadContext);
		Log(OK, "Init OpenGL shared context for shader reloading.");
		init = true;
	}

	Log(ZERO, "\n{}[RELOAD] {}", Color::MAGENTA, filepath);

	for(auto& s : g.shaders)
	{
		Shader* shader = s.second;

		if(shader->dependencies.find(filepath) != shader->dependencies.end())
		{
			renderMutex.lock();

			std::vector<std::string> components;
			for(auto& c : shader->dependencies)
			{
				const std::string& path = c.first;
				unsigned& type = c.second;

				if(type != -1)
					components.push_back(path);
			}

			s.second = new Shader(components);
			delete shader;
			
			renderMutex.unlock();
		}
	}
}

int main()
{
	EnableUTF8();
	Window::Create();

	Game::Init();
	Render::Init();

	Watcher watcher("shd", ShaderReload);

	while(glfwWindowShouldClose(g.window) == false)
	{
		glfwPollEvents();
		Input::Update(g.window);

		// ### imgui begin ###
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(g.mainFont);
		ImGui::Begin("Debug");
		// ###

		Game::Update();
		renderMutex.lock();
		Render::Update();
		renderMutex.unlock();

		// ### imgui end ###
		ImGui::PopFont();
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// ###
		
		glfwSwapBuffers(g.window);

		float currentFrame = glfwGetTime();
		g.deltaTime = currentFrame - g.lastFrame;
		g.lastFrame = currentFrame;
	}

	watcher.Join();

	Render::Close();
	Game::Close();

	Window::Destroy();
	return 0;
}
