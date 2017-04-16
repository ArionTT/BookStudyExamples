#pragma once
#include"DXApp.h"


class TestApp :public DXApp
{
public:
	TestApp(HINSTANCE hInst):DXApp(hInst)
	{
		
	}
	~TestApp()
	{

	}

	bool Init()
	{
		if (!DXApp::Init())
			return false;
		return true;
	}
	void OnResize()
	{
		DXApp::OnResize();
	}

	virtual void UpdateScenes(float deltaTime) override
	{
		
	}
	virtual void DrawScenes() override
	{
		RenderBegin();


		RenderEnd();
	}
};