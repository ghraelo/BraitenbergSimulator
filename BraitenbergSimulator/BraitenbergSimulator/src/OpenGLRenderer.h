#pragma once

#include "DrawCmd.h"
#include "ScissorCmd.h"
#include "Shader.h"

#include <map>
#include <string>

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	void LoadShaders(std::map<std::string, Shader>& shaderMap);
	void AddCommand();
	void Create();
	void Destroy();
	void UseShader(std::string shadername);
	void ExecuteDrawCmd(DrawCmd& cmd);
	void ExecuteScissorCmd(ScissorCmd& cmd);
private:

	std::map<std::string, Shader> m_shaderMap;
};