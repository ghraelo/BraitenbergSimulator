#include "SceneManager.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>

#include <Windows.h>

#include "MathUtils.h"
#include "LightRayCastCallback.h"
#include "Raycaster.h"
#include "SimObjectInfo.h"

#include <fftw3.h>

SceneManager::SceneManager()
	:m_world(new b2World(b2Vec2(0, 0))),m_dataRecorder(),circ(500)
{
	circ.set_capacity(500);
	for (int i = 0; i < circ.capacity(); i++)
	{
		circ[i] = 0;
	}
	m_world->SetDebugDraw(&g_debugDraw);

}

SceneManager::SceneManager(WorldPtr world)
	:m_dataRecorder(),circ(500)
{
	for (int i = 0; i < circ.capacity(); i++)
	{
		circ.push_back(0);
	}
	m_world = std::move(world);
}

void SceneManager::LoadScene(ScenePtr& ptr_scene)
{
	//get scene ptr
	m_currentScene = std::move(ptr_scene);
	BindPhysics();
	CSVRow header;
	header.m_cellData.push_back("time");
	for (auto& veh : m_currentScene->m_vehicles)
	{
		header.m_cellData.push_back(veh->GetName() + ".x");
		header.m_cellData.push_back(veh->GetName() + ".y");
	}
	m_dataRecorder.BeginFile(header);
	char buffer[100];
	GetCurrentDirectory(100, buffer);
	printf("current directory: %s", buffer);

}

void SceneManager::BindPhysics()
{
	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->BindPhysics(m_world.get());
	}
}

void SceneManager::Step()
{
	if(m_tsCount < 5)
		m_tsCount++;
	else
	{
		m_tsCount = 0;
		Sample();
	}
	//RayCast(b2Vec2(0, 20));

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->Update(m_currentScene->m_lights);
	}
}

void SceneManager::Sample()
{
	CSVRow row;

	std::ostringstream convert;
	convert << sampleCount;
	row.m_cellData.push_back(convert.str());
	convert.str("");
	convert.clear();
	for (auto &obj : m_currentScene->m_vehicles)
	{
		convert << obj->GetPosition().x;
		row.m_cellData.push_back(convert.str());
		convert.str("");
		convert.clear();

		convert << obj->GetPosition().y;
		row.m_cellData.push_back(convert.str());
		convert.str("");
		convert.clear();
	}
	m_dataRecorder.Record(row);
	sampleCount++;
	float dist = b2Distance(m_currentScene->m_vehicles[0]->GetPosition(), m_currentScene->m_lights[0].GetPosition());
	circ.push_back(dist);
}

std::vector<float> fftAutoCorrelation(const boost::circular_buffer<float> &x)
{
	std::vector<double> x_cent;
	x_cent.resize(x.size());

	float mean = std::accumulate(x.begin(), x.end(), 0) / (float)x.size();
	//centre x
	std::transform(x.begin(), x.end(), x_cent.begin(),
		[&](float a) {
		return a - mean;
	}
	);

	int N = x.size();
	double* in = new double[N];

	for (int i = 0; i < N; i++)
	{
		in[i] = x_cent[i];
	}

	fftw_complex *out;
	fftw_plan p,p2;
	int x2 = floor(N /2 + 1);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * floor(N/2 +1));

	p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
	p2 = fftw_plan_dft_c2r_1d(N, out, in, FFTW_ESTIMATE);

	//std::copy(x_cent.begin(), x_cent.end(), in);
	fftw_execute(p); /* repeat as needed */

	//replace cplx nos with norms
	for (int i = 0; i < floor(N / 2 + 1);i++)
	{
		out[i][0] = out[i][0] * out[i][0] + out[i][1] * out[i][1];
		out[i][1] = 0;
	}

	fftw_execute(p2);

	fftw_destroy_plan(p);
	fftw_destroy_plan(p2);
	std::vector<float> acorr(N);


	for (int i = 0; i < N; i++)
	{
		acorr.push_back(in[i] / in[0]);
	}

	delete[] in;
	fftw_free(out);

	return acorr;

}

void SceneManager::Render()
{	
	float first = circ[0];
	std::vector<float> acor;
	//std::vector<float> circ_norm;
	//circ_norm.resize(circ.size());
	//float mean = std::accumulate(circ.begin(), circ.end(), 0)/circ.size();
	//std::transform(circ.begin(),circ.end(),circ_norm.begin(),
	//	[&](float a) {
	//	return a - mean;
	//	}
	//);
	acor = fftAutoCorrelation(circ);
	//compute autocorrelation
	//acor = autoCorrelation(circ_norm);

	//draw axes;
	g_debugDraw.DrawSegment(b2Vec2(20, 0), b2Vec2(20, 10), b2Color(1, 0, 1));
	g_debugDraw.DrawSegment(b2Vec2(20, 5), b2Vec2(30, 5), b2Color(1, 0, 1));
	g_debugDraw.DrawString(b2Vec2(20, 10.1), "Autocorrelation");
	std::vector<float>acor2(acor.size() / 2);
	std::copy(acor.begin() + acor.size() / 2, acor.end(), acor2.begin());
	std::rotate(acor2.begin(), acor2.begin() + acor2.size() / 2, acor2.end());
	float max = *std::max_element(acor2.begin(), acor2.end());
	for (int i = 0; i < acor2.size(); i++)
	{
		g_debugDraw.DrawPoint(b2Vec2(20+(float)(i) / 50, (acor2[i]) * 5 + 5) , 5, b2Color(0, 1, 0));
	}

	//find peaks
	float acc = 0;
	std::vector<float> peak_x;
	for (int i = 1; i < acor2.size() - 1; i++)
	{
		float first = acor2[i+1] - acor2[i]; //first 'derivative'
		float second = acor2[i - 1] - 2*acor2[i] + acor2[i + 1]; //second 'derivative'
		
		if (first > -0.001 && first < 0.001 && second < 0)
		{
			peak_x.push_back(i);
			g_debugDraw.DrawPoint(b2Vec2(20 + (float)(i) / 50, (acor2[i]) * 5 + 5), 10, b2Color(1, 0, 0));
		}
	}

	acc = acc / (acor.size() - 2);
	printf("acc: %f\n", acc);
	//draw axes;
	g_debugDraw.DrawSegment(b2Vec2(0, 0), b2Vec2(0, 10), b2Color(1, 1, 1));
	g_debugDraw.DrawSegment(b2Vec2(0, 0), b2Vec2(10, 0), b2Color(1, 1, 1));
	g_debugDraw.DrawString(b2Vec2(0, 10), "Distance");

	for (int i = 0; i <  circ.size(); i++)
	{
		g_debugDraw.DrawPoint(b2Vec2((float)i/50, circ[i] / 2), 5, b2Color(1, 0, 0));
	}

	//render vehicles
	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->Render(m_sceneRenderer);
}	

	//render lights
	for (auto &obj : m_currentScene->m_lights)
	{
		obj.Render(m_sceneRenderer);
	}
}