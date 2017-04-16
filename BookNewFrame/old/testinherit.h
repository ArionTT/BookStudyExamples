#pragma once
#include"D3DApp.h"
#include"SceneObject.h"


class MyApp :public D3DApp
{
public:
	void AppRender() override;
	BOOL AppStart() override;

public:
	SceneObject*cube=NULL;
};


BOOL MyApp::AppStart()
{
	BOOL result = 1;

	cube = new SceneObject();
	Renderer render;
	cube->AppendComponent<Renderer>(&render);
	SCENE_LIST_APPEND(cube);

	return result;
}

void MyApp::AppRender()
{
	RENDER_BEGINE;




	RENDER_END;
}

