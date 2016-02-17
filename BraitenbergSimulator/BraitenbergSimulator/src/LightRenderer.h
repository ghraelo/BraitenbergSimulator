#pragma once

#include "LightSource.h"
#include <vector>
#include <glew/glew.h>
#include <GL/gl.h>
#include "Shader.h"

#include "DebugDraw.h"

class LightRenderer
{
public:
	LightRenderer(Camera* camera);
	~LightRenderer();
	void Render();
	void CreateTexture();
	void Draw();
	void AddSource(LightSource& ls);
	void SourceMoved();
	int GetLightIntensity(b2Vec2 point);
	std::vector<LightSource> light_sources;
private:

	GLuint g_light_texture;
	GLubyte* g_texture_data;
	Camera* camera;
};