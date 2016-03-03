#include "OpenGLRenderer.h"

#include <yaml-cpp\yaml.h>

OpenGLRenderer::OpenGLRenderer()
{
}

void OpenGLRenderer::LoadShaders(std::map<std::string, Shader>& shaderMap)
{
	m_shaderMap = shaderMap;
}

void OpenGLRenderer::AddCommand()
{
}

void OpenGLRenderer::UseShader(std::string shadername)
{
	m_shaderMap[shadername].Use();
}

void OpenGLRenderer::ExecuteDrawCmd(DrawCmd& cmd)
{
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, cmd.verticesNum * sizeof(GLfloat), cmd.vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

	m_shaderMap[cmd.shaderID].Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void OpenGLRenderer::ExecuteScissorCmd(ScissorCmd & cmd)
{
	if (cmd.enable)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(cmd.x, cmd.y, cmd.w, cmd.h);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
	}
}
