/*
* Copyright (c) 2006-2012 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BRAITENBERG_H
#define BRAITENBERG_H

/// This test shows how to use a motor joint. A motor joint
/// can be used to animate a dynamic body. With finite motor forces
/// the body can be blocked by collision with other bodies.

#include <math.h>
#include <iostream>

#include "Vehicle.h"
#include "SimObjectInfo.h"

#if defined(__APPLE_CC__)
#include <OpenGL/gl3.h>
#else
#include <glew/glew.h>
#endif

#include <glfw/glfw3.h>
#include <stdio.h>
#include <stdarg.h>

#include "Test.h"
#include "RenderGL3.h"

#include "Simulator.h"
#include "ResourceManager.h"
#include "UIManager.h"

class Braitenberg : public Test
{
	Simulator sim;
	UIManager uim;

public:
	Braitenberg()
		:sim(WorldPtr(m_world))
	{
		ScenePtr scene = ResourceManager::LoadScene("yaml/testScene.yaml");
		sim.LoadScene(scene);

	}

	void Keyboard(int key)
	{
	}

	void KeyboardUp(int key)
	{
	}

	void BodyClick(const b2Body* body_ptr)
	{
		SimObjectInfo* soi = (SimObjectInfo*)(body_ptr->GetUserData());
		if (soi->m_type == "Vehicle")
		{
			uim.SelectVehicle((Vehicle*)(soi->m_obj));
		}
	}
	
	void NonBodyClick()
	{
		uim.DeselectVehicle();
	}

	void DrawUI()
	{
		uim.DrawUI();
	}

	void Render()
	{
		sim.Render();
	}

	void Step(Settings* settings)
	{
		sim.Step();
		Test::Step(settings);
	}

	static Test* Create()
	{
		return new Braitenberg;
	}
};

#endif
