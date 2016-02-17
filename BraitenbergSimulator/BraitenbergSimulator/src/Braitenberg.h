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

#include <cstring>
#include <math.h>
#include <iostream>
#include "Vehicle.h"

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
#include "LightRenderer.h"
#include "LightSource.h"
#include "LightSensor.h"
#include "CSL.h"
#include "ConfigLoader.h"

class Braitenberg : public Test
{
	Vehicle* veh;
	bool leftkeydown,rightkeydown;
	ConfigLoader cl;
	LightRenderer lr;
	LightSource test;

	CSL leftCSL;
	CSL rightCSL;


public:
	Braitenberg()
		:cl(m_world), lr(&g_camera), test(m_world,b2Vec2(-10.0f,10.0f)), leftkeydown(false), rightkeydown(false), leftCSL(), rightCSL()
	{
		leftCSL.UpdateCoefficients(0.5f, 1.2f);
		rightCSL.UpdateCoefficients(0.5f, 1.2f);
		lr.AddSource(test);
	}

	~Braitenberg()
	{
		delete veh;
	}

	void Load()
	{
		cl.OpenFile("yaml/testVehicle.yaml");
		veh = new Vehicle(cl.LoadVehicle());
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
			leftkeydown = true;
			break;

		case GLFW_KEY_E:
			rightkeydown = true;
			break;
		}
	}

	void KeyboardUp(int key)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
			leftkeydown = false;
			break;

		case GLFW_KEY_E:
			rightkeydown = false;
			break;
		}
	}

	void Render()
	{
		//lr.Render();
	}

	float Normalise(float input)
	{
		float output = input;
		output = output / 1000; //scale

		if (output > 2.0f)
			output = 2.0f; //clamp

		output = output - 1.0f; //shift
		return output;
	}

	float Clamp(float input)
	{
		if (input < -1.0f)
			return -1.0f;
		else if (input > 1.0f)
			return 1.0f;
		else
			return input;
	}

	void drawVision(b2Vec2 position, relLightPos rlp)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, g_camera.m_width, g_camera.m_height, 0, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		b2Vec2 p1(0, 0);
		b2Vec2 p2(rlp.m_point1 * 20, 0);
		b2Vec2 p3(rlp.m_point2 * 20, 0);
		b2Vec2 p4(20, 0);

		glColor4f(1, 1, 1, 1); //white
		glLineWidth(10);
		glShadeModel(GL_FLAT);
		glBegin(GL_LINE_STRIP);

		b2Vec2 p1t = g_camera.ConvertWorldToScreen(p1 + position);
		b2Vec2 p2t = g_camera.ConvertWorldToScreen(p2 + position);
		b2Vec2 p3t = g_camera.ConvertWorldToScreen(p3 + position);
		b2Vec2 p4t = g_camera.ConvertWorldToScreen(p4 + position);

		glVertex2f(p1t.x, p1t.y);
		glVertex2f(p2t.x, p2t.y);

		glColor4f(1, 0, 0, 1); //red
		glVertex2f(p3t.x, p3t.y);
		glColor4f(1, 1, 1, 1);
		glVertex2f(p4t.x, p4t.y);
		glEnd();
		glLineWidth(1.0f);
		glShadeModel(GL_SMOOTH);
		glPopMatrix();
	}

	void drawSensorCone(LightSensor sensor, float dist)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, g_camera.m_width, g_camera.m_height, 0, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		b2Vec2 p1 = sensor.GetArcEnd(dist,true);
		b2Vec2 p2 = sensor.GetWorldPosition();
		b2Vec2 p3 = sensor.GetArcEnd(dist, false);

		glBegin(GL_LINE_STRIP);

		b2Vec2 p1t = g_camera.ConvertWorldToScreen(p1);
		b2Vec2 p2t = g_camera.ConvertWorldToScreen(p2);
		b2Vec2 p3t = g_camera.ConvertWorldToScreen(p3);
		glColor4f(1, 1, 1, 0); //transparent
		glVertex2f(p1t.x, p1t.y);
		glColor4f(1, 1, 1, 1); //transparent
		glVertex2f(p2t.x, p2t.y);
		glColor4f(1, 1, 1, 0); //white
		glVertex2f(p3t.x, p3t.y);

		glEnd();
		glPopMatrix();
	}

	float calcVal(relLightPos rlp)
	{
		//get light %
		float light = rlp.m_point2 - rlp.m_point1;
		light = (light * 2) - 1;
		return light;
	}

	void Step(Settings* settings)
	{
		//cl.OpenFile("yaml/testVehicle.yaml");
		//cl.LoadVehicle();

		relLightPos rlp;
		relLightPos rlp2;
		rlp.m_point1 = 2;
		rlp.m_point2 = 2;
		rlp2.m_point1 = 2;
		rlp2.m_point2 = 2;

		float rightCSLout, leftCSLout;

		//leftSensor.GetLightBoundary(test.get_position(), sqrt(test.getIntensity() / 255),rlp);
		//drawSensorCone(leftSensor, 10);

		b2Vec2 leftUI(0, 0);
		b2Vec2 leftUI2 = g_camera.ConvertWorldToScreen(leftUI);

		g_debugDraw.DrawString(leftUI2.x, leftUI2.y - 10, "Left sensor");
		drawVision(leftUI, rlp);


		//rightSensor.GetLightBoundary(test.get_position(), sqrt(test.getIntensity() / 255), rlp2);
		//drawSensorCone(rightSensor, 10);

		b2Vec2 rightUI(0, 3);
		b2Vec2 rightUI2 = g_camera.ConvertWorldToScreen(rightUI);

		g_debugDraw.DrawString(rightUI2.x, rightUI2.y -10, "Right sensor");

		drawVision(rightUI, rlp2);

		rightCSLout = rightCSL.Update(calcVal(rlp2));
		leftCSLout = leftCSL.Update(calcVal(rlp));

		float force_gain = 0.5;

		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "pos: %0.2g,%0.2g", rlp.m_point1,rlp.m_point2);
		//display info
		CSLInternalState leftState = leftCSL.GetInternalState();
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "Percentage: %g",calcVal(rlp));
		m_textLine += DRAW_STRING_NEW_LINE;

		g_debugDraw.DrawString(5, m_textLine, "Left CSL");
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "input: %g", leftState.m_input);
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "delay: %g", leftState.m_delay);
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "output: %g", leftState.m_output);

		m_textLine += DRAW_STRING_NEW_LINE;

		//display info
		CSLInternalState rightState = rightCSL.GetInternalState();

		g_debugDraw.DrawString(5, m_textLine, "right CSL");
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "input: %g", rightState.m_input);
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "delay: %g", rightState.m_delay);
		m_textLine += DRAW_STRING_NEW_LINE;
		g_debugDraw.DrawString(5, m_textLine, "output: %g", rightState.m_output);

		veh->RightForce(force_gain * rightCSLout);
		veh->LeftForce(force_gain * leftCSLout);
		
		if (leftkeydown)
			veh->LeftForce(3.0f);
		if (rightkeydown)
			veh->RightForce(3.0f);

		Test::Step(settings);
	}

	static Test* Create()
	{
		return new Braitenberg;
	}
};

#endif
