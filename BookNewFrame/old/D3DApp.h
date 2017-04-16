#include<Windows.h>
#include"DXDevice.h"
#include<iostream>
using std::cout;
using std::endl;
using std::cin;

#define RENDER_BEGINE m_device->RenderBegin()
#define RENDER_END m_device->RenderEnd()
#define ZBUFFER(state)  m_device->SetZBuffer(state)
#define CULL_STATE(state) m_device->SetCullState(state)
#define BLEND(state) m_device->SetBlend(state)
#define SCENE_LIST_APPEND(obj) m_device->ListAppendObject(obj)


LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


class D3DApp
{
	//公共调用区
public:
	D3DApp()
	{
		m_device = new DXDevice();
		m_Timer = new Timer();
	}
	~D3DApp(){}
	BOOL Initialize()
	{
		BOOL result=1;

		//Windows Initialize Area
		HINSTANCE hInstance;
		hInstance = GetModuleHandle(NULL);
		WNDCLASS wndcls;
		wndcls.cbClsExtra = 0;
		wndcls.cbWndExtra = 0;
		wndcls.lpszMenuName = NULL;
		wndcls.lpfnWndProc = WinProc;
		wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndcls.hCursor = LoadCursor(hInstance, IDC_ARROW);
		wndcls.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wndcls.lpszClassName = defaultClassName;
		wndcls.style = CS_HREDRAW | CS_VREDRAW;
		wndcls.hInstance = hInstance;

		RegisterClass(&wndcls);


		m_hwnd = CreateWindow(
			defaultClassName, L"默认创建 windows 窗口",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			20, 20, defaultWidth, defaultHeight,
			NULL, NULL, hInstance, NULL);

		

		m_device->Initialize(m_hwnd, defaultWidth, defaultHeight);

		//Custom Initialize Area
		AppStart();

		return result;
	}
	BOOL Initialize(wchar_t*title, int width, int height)
	{
		BOOL result = 1;

		//Windows Initialize Area
		HINSTANCE hInstance;
		hInstance = GetModuleHandle(NULL);
		WNDCLASS wndcls;
		wndcls.cbClsExtra = 0;
		wndcls.cbWndExtra = 0;
		wndcls.lpszMenuName = NULL;
		wndcls.lpfnWndProc = WinProc;
		wndcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndcls.hCursor = LoadCursor(hInstance, IDC_ARROW);
		wndcls.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wndcls.lpszClassName = title;
		wndcls.style = CS_HREDRAW | CS_VREDRAW;
		wndcls.hInstance = hInstance;

		RegisterClass(&wndcls);



		m_hwnd= CreateWindow(
			title, title,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			20, 20, width, height,
			NULL, NULL, hInstance, NULL);


		m_device->Initialize(m_hwnd, width, height);

		//Custom Initialize Area
		AppStart();

		return result;
	}
	BOOL DisplayWindow()
	{
		BOOL result = 1;

		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);

		MSG msg;
		bool done = false;
		m_Timer->Reset();
		ZeroMemory(&msg, sizeof(MSG));

		while (!done)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (WM_QUIT == msg.message)
				done = true;
			else
			{
				m_Timer->Tick();
				if (!m_AppPaused)
				{
					CaculateFrameStates();
					m_device->Update();
					AppRender();
				}
				else
				{
					Sleep(100);
				}
			}
		}

		return (int)msg.wParam;
	}
	virtual void AppRender() 
	{

	}
	virtual BOOL AppStart()
	{
		BOOL result = 1;

		return result;
	}
	BOOL Release()
	{
		BOOL result = 1;

		m_device->Release();
		CloseWindow(m_hwnd);

		return result;
	}

	//内部函数区
protected:
	void CaculateFrameStates()
	{

	}


	//成员区
protected:
	DXDevice*m_device;
	Timer*m_Timer;
	HWND m_hwnd;
	bool m_AppPaused;
	int m_width, m_height;

	//常量区
protected:
	const float DEFAULT_CLEAR_COLOR[4] = { 0.6f,0.2f,0.0f,1.0f };
	const char*defaultTitle = "D3D Window";
	const wchar_t*defaultClassName = L"Window";
	const int defaultWidth = 800;
	const int defaultHeight = 600;

};


LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	case WM_DESTROY:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		CloseWindow(hwnd);
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}