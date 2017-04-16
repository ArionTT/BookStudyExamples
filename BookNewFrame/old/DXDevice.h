#pragma once

#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib, "d3dcompiler.lib")
#pragma comment ( lib,"D3DX11.lib")
#pragma comment ( lib,"dxgi.lib")

#include<Windows.h>
#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include<D3Dcompiler.h>
#include"SceneObject.h"

struct BaseProjectBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
};

struct BaseOrthoBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX ortho;
};

class BaseVertexBuffer
{
public:
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};

class FullVertexBuffer :public BaseVertexBuffer
{
public:
	XMFLOAT3 normal;
	XMFLOAT4 color;
};





class DXDevice
{
public:
	enum DeviceState
	{
		OFF = 0, ON = 1
	};
	enum CullState
	{
		CullBack = 0, CullFront = 1, CullNone = 2
	};


	//公共函数调用区
public:
	DXDevice() 
	{

	}
	~DXDevice() {}
	HRESULT Initialize(HWND hwnd, int width, int height)
	{
		HRESULT hr = S_OK;

		m_hwnd = hwnd;
		m_Width = width;
		m_Height = width;
		//绘制区大小生成，预留

		hr = InitD3D11Device();
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"Device & DeviceContext创建失败", L"Error", MB_OK);
			return hr;
		}

		hr = InitD3D11SwapChain(true);
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"SwapChain创建失败", L"Error", MB_OK);
			return hr;
		}

		hr = InitD3D11RenderTarget();
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"RenderTargetView创建失败", L"Error", MB_OK);
			return hr;
		}

		hr = InitD3D11DepthStencil(true);
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"DepthStencil View创建失败", L"Error", MB_OK);
			return hr;
		}

		hr = InitD3D11ViewPort();
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"ViewPort创建失败", L"Error", MB_OK);
			return hr;
		}

		hr = InitD3D11Blend();
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"Blend 状态创建失败", L"Error", MB_OK);
			return hr;
		}

		hr = InitD3D11ZBuffer();
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"ZBuffer 状态创建失败", L"Error", MB_OK);
			return hr;
		}

		hr = InitD3D11CullState();
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"Cull 状态创建失败", L"Error", MB_OK);
			return hr;
		}

		hr = InitBaseScene();
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"场景 基础 初始化失败", L"Error", MB_OK);
			return hr;
		}

		hr = InitModelList();
		if (FAILED(hr))
		{
			MessageBox(hwnd, L"场景 物体列表 初始化失败", L"Error", MB_OK);
			return hr;
		}


		return hr;
	}
	void RenderBegin()
	{
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DEFAULT_CLEAR_COLOR);
		m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
	}
	void RenderEnd()
	{
		m_pSwapChain->Present(0, 0);
	}
	void Update()
	{

	}
	HRESULT Release()
	{
		HRESULT hr = S_OK;



		return hr;
	}

	void SetZBuffer(DeviceState state)
	{
		switch (state)
		{
		case DXDevice::OFF:
			break;
		case DXDevice::ON:
			break;
		default:
			break;
		}
	}
	void SetCullState(CullState state)
	{
		switch (state)
		{
		case DXDevice::CullBack:
			break;
		case DXDevice::CullFront:
			break;
		case DXDevice::CullNone:
			break;
		default:
			break;
		}
	}
	void SetBlend(DeviceState state)
	{
		switch (state)
		{
		case DXDevice::OFF:
			break;
		case DXDevice::ON:
			break;
		default:
			break;
		}
	}

	HRESULT ListAppendObject(SceneObject *object)
	{
		HRESULT hr = S_OK;

		return hr;
	}
	HRESULT ListZero()
	{
		HRESULT hr = S_OK;

		return hr;
	}
	HRESULT ListDeleteObject(int id)
	{
		HRESULT hr = S_OK;

		return hr;
	}

	//公共变量区
public:
	//SceneObject 对象添加指针
	vector<SceneObject*> Obj_List;


	//成员函数区
protected:
	HRESULT InitD3D11Device()
	{
		HRESULT hr = S_OK;

		UINT deviceFlag = 0;
#if defined(DEBUG)||defined(_DEBUG)
		deviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevel;
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlag, NULL, 0,
			D3D11_SDK_VERSION, &m_pDevice, &featureLevel, &m_pImmediateContext);

		if (FAILED(hr))
		{
			return hr;
		}

		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(NULL, L"不支持D3D11", L"Error", MB_OK);
			return false;
		}
		hr = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"检查MSAA 4X 质量失败", L"Error", MB_OK);
			return hr;
		}
		return hr;
	}
	HRESULT InitD3D11SwapChain(bool Enable4xMsaa)
	{
		HRESULT hr = S_OK;

		DXGI_SWAP_CHAIN_DESC scd;
		scd.BufferDesc.Width = m_Width;
		scd.BufferDesc.Height = m_Height;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.OutputWindow = m_hwnd;
		scd.Windowed = true;
		scd.BufferCount = 1;
		scd.Flags = 0;
		if (Enable4xMsaa)
		{
			scd.SampleDesc.Count = 4;
			scd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			scd.SampleDesc.Count = 1;
			scd.SampleDesc.Quality = 0;
		}

		IDXGIDevice*dxgiDevice = NULL;
		hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		if (FAILED(hr))
			return hr;
		IDXGIAdapter*dxgiAdapter = NULL;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		if (FAILED(hr))
			return hr;
		IDXGIFactory*dxgiFactory = NULL;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		if (FAILED(hr))
			return hr;

		hr = dxgiFactory->CreateSwapChain(m_pDevice, &scd, &m_pSwapChain);
		if (FAILED(hr))
			return hr;

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();


		return hr;
	}
	HRESULT InitD3D11RenderTarget()
	{
		HRESULT hr = S_OK;

		m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pRenderTexture));
		if (FAILED(hr))
			return hr;

		hr = m_pDevice->CreateRenderTargetView(m_pRenderTexture, NULL, &m_pRenderTargetView);
		if (FAILED(hr))
			return hr;

		return hr;
	}
	HRESULT InitD3D11DepthStencil(bool Enable4xMsaa)
	{
		HRESULT hr = S_OK;

		D3D11_TEXTURE2D_DESC desc;

		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.Height = m_Height;
		desc.Width = m_Width;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		if (Enable4xMsaa)
		{
			desc.SampleDesc.Count = 4;
			desc.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
		}

		hr = m_pDevice->CreateTexture2D(&desc, NULL, &m_pDepthTexture);
		if (FAILED(hr))
			return hr;

		hr = m_pDevice->CreateDepthStencilView(m_pDepthTexture, NULL, &m_pDepthStencilView);
		if (FAILED(hr))
			return hr;

		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		return hr;
	}
	HRESULT InitD3D11ViewPort()
	{
		HRESULT hr = S_OK;

		D3D11_VIEWPORT vp;
		vp.TopLeftX = 10.0f;
		vp.TopLeftY = 10.0f;
		vp.Width = m_Width-200.0f;
		vp.Height = m_Height-20.0f;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_pImmediateContext->RSSetViewports(1, &vp);

		return hr;
	}
	HRESULT InitD3D11ZBuffer()
	{
		HRESULT hr = S_OK;


		return hr;
	}
	HRESULT InitD3D11Blend()
	{
		HRESULT hr = S_OK;


		return hr;
	}
	HRESULT InitD3D11CullState()
	{
		HRESULT hr = S_OK;


		return hr;
	}

	HRESULT InitBaseScene()
	{
		HRESULT hr = S_OK;


		return hr;
	}
	HRESULT InitModelList()
	{
		HRESULT hr = S_OK;


		return hr;
	}

	//成员变量区
protected:
	//D3D
	ID3D11Device*m_pDevice;
	ID3D11DeviceContext*m_pImmediateContext;
	IDXGISwapChain*m_pSwapChain;
	ID3D11RenderTargetView*m_pRenderTargetView;
	ID3D11DepthStencilView*m_pDepthStencilView;
	ID3D11Texture2D*m_pRenderTexture;
	ID3D11Texture2D*m_pDepthTexture;
	ID3D11BlendState*m_BlendOn;
	ID3D11BlendState*m_BlendOff;
	ID3D11RasterizerState*m_CullBack;
	ID3D11RasterizerState*m_CullFront;
	ID3D11RasterizerState*m_CullNone;
	ID3D11DepthStencilState*m_DepthEnable;
	ID3D11DepthStencilState*m_DepthDisable;
	UINT m_4xMsaaQuality;
	bool m_enable4xMsaa;
	//Windows
	HWND m_hwnd;
	int m_Width;
	int m_Height;
	//Proc

	//常量区
protected:
	const float DEFAULT_CLEAR_COLOR[4] = { 0.6f,0.2f,0.0f,1.0f };
	const int DEFAULT_WIDTH = 800;
	const int DEFAULT_HEIGHT = 600;

};