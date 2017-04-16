#pragma once

#include<iostream>
#include <cassert>
#include<Windows.h>
#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include<D3Dcompiler.h>
#include"GameTimer.h"
#include"d3dUtil.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::wstring;


class DXApp
{
public:
	DXApp(HINSTANCE hInst);
	~DXApp();

	HINSTANCE AppInst()const;
	HWND AppMainWnd() const;
	float AspectRatio()const;
	int Run();

	virtual bool Init();
	virtual void OnResize();
	virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void UpdateScenes(float deltaTime) = 0;
	virtual void DrawScenes() = 0;
	virtual void RenderBegin();
	virtual void RenderEnd();

	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
	//virtual void OnKeyDown();
	//virtual void OnKeyUp();

protected:
	bool InitMainWindow();
	bool InitD3D11();
	void CaculateFrameState();

	bool InitStateDevices();

protected:
	enum DeviceState
	{
		OFF = 0, ON = 1
	};
	enum RasterizeState
	{
		Wireframe,Solidframe, CullBack , CullFront , CullNone
	};
	void SetBlend(DeviceState state);
	void SetCullState(RasterizeState state);
	void SetDepthState(DeviceState state);

protected:
	//Windows
	HWND m_hwnd;
	HINSTANCE m_appInst;
	wstring m_mainWndCaption;
	bool m_appPaused;
	int m_winHeight;
	int m_winWidth;
	bool m_maximized;
	bool m_minimized;
	bool m_resizing;
	int m_clientWidth;
	int m_clientHeight;
	//D3D
	IDXGISwapChain* m_pSwapChain;
	ID3D11Device*m_pDevice;
	ID3D11DeviceContext*m_pImmediateContext;
	ID3D11RenderTargetView*m_pRenderTargetView;
	ID3D11DepthStencilView*m_pDepthStencilView;
	D3D11_VIEWPORT m_screenViewPort;
	ID3D11Texture2D*m_pRenderTexture;
	ID3D11Texture2D*m_pDepthTexture;
	ID3D11BlendState*m_BlendOn;
	ID3D11BlendState*m_BlendOff;
	ID3D11RasterizerState*m_CullBack;
	ID3D11RasterizerState*m_CullFront;
	ID3D11RasterizerState*m_CullNone;
	ID3D11DepthStencilState*m_DepthEnable;
	ID3D11DepthStencilState*m_DepthDisable;
	D3D_DRIVER_TYPE m_driverType;
	UINT m_4xMsaaQuality;
	bool m_enable4xMsaa;
	//Class
	GameTimer m_timer;

};
