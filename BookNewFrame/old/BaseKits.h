#pragma once
#include<vector>
#include<Windows.h>
using std::vector;


struct Position
{
	float x, y, z;
};

struct Rotation
{
	float x, y, z;
};

struct Scale
{
	float x, y, z;
};

class Mesh
{
public:

};

class Texture2D
{
public:

};

class Material
{
public:
	Texture2D tex;
};

class Object
{
public:

};


class Transform:public Object
{
public:
	Position position;
	Rotation rotate;
	Scale scale;

};



class Renderer:public Object
{
public:
	enum BaseMesh
	{
		Cube = 0, Sphere = 1, Looper = 2, Plane = 3
	};
public:
	Mesh mesh;
	Material mat;
public:
	bool InitMesh(char*path)
	{
		bool result = 1;


		return result;
	}
	bool InitMesh(BaseMesh type)
	{
		bool result = 1;
		switch (type)
		{
		case Renderer::Cube:
			break;
		case Renderer::Sphere:
			break;
		case Renderer::Looper:
			break;
		case Renderer::Plane:
			break;
		default:
			break;
		}

		return result;
	}
	bool InitMaterial(Material *mat)
	{
		bool result = 1;
		return result;
	}

public:

};



class Timer
{
public:
	Timer() : m_SecondsPerCount(0.0),
					m_DeltaTime(-1.0),
					m_BaseTime(0),
					m_PausedTime(0),
					m_PrevTime(0),
					m_CurrTime(0),
					m_Stopped(false)
	{
		__int64 countsPerSec;
		QueryPerformanceCounter((LARGE_INTEGER*)&countsPerSec);
		m_SecondsPerCount = 1.0 / (double)countsPerSec;
	}

	float TotalTime() const
	{
		if (m_Stopped)
		{
			return (float)(((m_StopTime - m_PausedTime) - m_BaseTime)*m_SecondsPerCount);
		}
		else
		{
			return (float)(((m_CurrTime - m_PausedTime) - m_BaseTime)*m_SecondsPerCount);
		}
	}
	float DeltaTime() const
	{
		return (float)m_DeltaTime;
	}

	void Reset()
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_BaseTime = currTime;
		m_PrevTime = currTime;
		m_StopTime = 0;
		m_Stopped = false;
	}
	void Start()
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

		if (m_Stopped)
		{
			m_PausedTime += (startTime - m_StopTime);
			m_PrevTime = startTime;
			m_StopTime = 0;
			m_Stopped = false;
		}
	}
	void Stop()
	{
		if (!m_Stopped)
		{
			__int64 currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
			m_StopTime = currTime;
			m_Stopped = true;
		}
	}
	void Tick()
	{
		if (m_Stopped)
		{
			m_DeltaTime = 0.0;
			return;
		}

		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_CurrTime = currTime;
		m_DeltaTime = (m_CurrTime - m_PrevTime)*m_SecondsPerCount;

		m_PrevTime = m_CurrTime;

		if (m_DeltaTime < 0.0)
		{
			m_DeltaTime = 0.0;
		}
	}

private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	bool m_Stopped;

};


