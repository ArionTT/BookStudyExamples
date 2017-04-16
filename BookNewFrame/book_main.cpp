#include"BoxDemo.h"
#include"HillsDemo.h"
#include"ShapesDemo.h"
#include"SkullDemo.h"
#include"WavesDemo.h"
#include"LightingDemos\LightingDemo.h"
#include"LightingDemos\LitSkullDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//*******************************************
	//Colors Box Demo
	//*******************************************

	//BoxApp theApp(hInstance);

	//if (!theApp.Init())
	//	return 0;

	//return theApp.Run();


	//*******************************************
	//Hills Demo
	//*******************************************
	//HillsApp theApp(hInstance);

	//if (!theApp.Init())
	//	return 0;

	//return theApp.Run();

	//*******************************************
	//Shapes Demo
	//*******************************************
	//ShapesApp theApp(hInstance);

	//if (!theApp.Init())
	//	return 0;

	//return theApp.Run();

	//*******************************************
	//Skull Demo
	//*******************************************
	//SkullApp theApp(hInstance);

	//if (!theApp.Init())
	//	return 0;

	//return theApp.Run();


	//*******************************************
	//Waves Demo
	//*******************************************
	//WavesDemo theApp(hInstance);

	//if (!theApp.Init())
	//	return 0;

	//return theApp.Run();

	//*******************************************
	//Lighting Demo
	//*******************************************
	//LightingApp theApp(hInstance);

	//if (!theApp.Init())
	//	return 0;

	//return theApp.Run();

	//*******************************************
	//Lighting Skull Demo
	//*******************************************
	LitSkullApp theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}