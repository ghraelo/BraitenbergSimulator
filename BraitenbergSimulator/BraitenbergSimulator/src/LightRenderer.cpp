#include "LightRenderer.h"

//#include <algorithm>

LightRenderer::LightRenderer(Camera* m_camera)
	:g_light_texture(), g_texture_data(), camera(m_camera)
{
	CreateTexture();
}
LightRenderer::~LightRenderer()
{

}

void LightRenderer::AddSource(LightSource& theSource)
{
	light_sources.push_back(theSource);
	//regenerate texture
	CreateTexture();
}

void LightRenderer::SourceMoved()
{

}

int LightRenderer::GetLightIntensity(b2Vec2 point)
{
	int height = camera->m_height;
	int width = camera->m_width;
	const int depth = 4;

	b2Vec2 screen_coord = camera->ConvertWorldToScreen(point);

	return g_texture_data[(int)(screen_coord.x) * width * depth + (int)(screen_coord.y) * depth + 3];
}

void LightRenderer::CreateTexture()
{
	//create image array (y,x,pixel)
	int height = camera->m_height;
	int width = camera->m_width;
	const int depth = 4;

	g_texture_data = new GLubyte[height*width * depth];

	std::vector<LightSource>::size_type size = light_sources.size();

	//iterate through light sources

	for (int ix = 0; ix < height; ++ix)
	{
		for (int iy = 0; iy < width; ++iy)
		{
			for (int i = 0; i < size; i++)
			{
				b2Vec2 pos = camera->ConvertWorldToScreen(light_sources[i].get_position());

				int x = (int)(pos.x);
				int y = (int)(pos.y);

				int dx = ix - x;
				int dy = iy - y;
				//printf("dx: %d, dy: %d\n",dx,dy);
				int color = 255;
				int radius = abs(dx) * abs(dx) + abs(dy) * abs(dy);

				if (radius != 0)
				{
					color = std::min((int)light_sources[i].getIntensity() / radius, 255);
				}

				int maxRadius = light_sources[i].getIntensity();

				if ((ix < x + maxRadius) && (ix > x - maxRadius) && (iy < y + maxRadius) && (iy > y - maxRadius))
				{
					//printf("radius: %d\n",radius);

					g_texture_data[ix * width * depth + iy * depth + 0] = 255;   //red
					g_texture_data[ix * width * depth + iy * depth + 1] = 255;   //green
					g_texture_data[ix * width * depth + iy * depth + 2] = 255;   //blue
					g_texture_data[ix * width * depth + iy * depth + 3] = color; //alpha
				}
			}
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &g_light_texture);

	glBindTexture(GL_TEXTURE_2D, g_light_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, camera->m_width,
		camera->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		g_texture_data);

	delete[] g_texture_data;
}

void LightRenderer::Draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, camera->m_width*camera->m_zoom, camera->m_height*camera->m_zoom, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, g_light_texture);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0); // Upper left

	glTexCoord2f(1, 0);
	glVertex2f(camera->m_width, 0); // Upper right

	glTexCoord2f(1, 1);
	glVertex2f(camera->m_width, camera->m_height); // Lower right

	glTexCoord2f(0, 1);
	glVertex2f(0, camera->m_height); // Lower left
	glEnd();
	glPopMatrix();
}

void LightRenderer::Render()
{
	//create texture
	//CreateTexture(camera);
	//draw it
	Draw();

}

