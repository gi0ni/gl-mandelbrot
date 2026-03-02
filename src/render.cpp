#include "render.h"

#include <glad/glad.h>
#include <imgui.h>
#include <utils/shader.h>

#include "state.h"

unsigned texture;
unsigned TEXTURE_W = 1920;
unsigned TEXTURE_H = 1080;

void Render::Init()
{
	// ### Fullscreen quad data ###
	float vertices[] = {
		-1.0f, -1.0f,   0.0f, 0.0f,
		-1.0f, +1.0f,   0.0f, 1.0f,
		+1.0f, -1.0f,   1.0f, 0.0f,
		+1.0f, +1.0f,   1.0f, 1.0f,
	};

	unsigned indices[] = {
		2, 1, 0,
		3, 1, 2
	};

	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// ###

	// ### Image buffer ###
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_W, TEXTURE_H, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, texture, 0, false, 0, GL_READ_WRITE, GL_RGBA32F); // TIL: First param has to be 0, not GL_TEXTURE0
	// ###

	glClearColor(0, 0, 0, 1);
	g.shaders["present"] = new Shader({ "shd/present.vert", "shd/present.frag" });
	g.shaders["mandelbrot"] = new Shader({ "shd/mandelbrot.comp" });
}

void Render::Close()
{
}

void Render::Update()
{
	ImGui::Text("%.2f ms, %.2f fps", g.deltaTime * 1000, 1/g.deltaTime);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui::Text("Mouse: %f %f", g.mouseCoord.x, g.mouseCoord.y);
	ImGui::Text("Pivot: %f %f", g.pivot.x, g.pivot.y);

	Shader* shader = g.shaders["mandelbrot"];

	if(g.redrawWindow == true)
	{
		shader->Use();
		shader->SetFloat("aspect", g.aspect);
		shader->SetVec2("pivot", g.pivot);
		shader->SetFloat("zoom", g.zoom);
		glDispatchCompute(TEXTURE_W, TEXTURE_H, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		g.redrawWindow = false;
	}

	shader = g.shaders["present"];
	shader->Use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}
