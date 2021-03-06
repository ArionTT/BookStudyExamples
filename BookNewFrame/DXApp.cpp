
#include"DXApp.h"
#include<windowsx.h>
#include<sstream>

namespace
{
	DXApp*g_app = 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_app->WndProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hInst) :
	m_appInst(hInst),
	m_hwnd(NULL),
	m_4xMsaaQuality(0),
	m_enable4xMsaa(false),
	m_mainWndCaption(L"D3D11 Application"),
	m_resizing(false),
	m_maximized(false),
	m_minimized(false),
	m_clientWidth(800),
	m_clientHeight(600),
	m_driverType(D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE),
	m_pSwapChain(NULL),
	m_pImmediateContext(NULL),
	m_pDevice(NULL),
	m_pDepthStencilView(NULL),
	m_pRenderTargetView(NULL),
	m_pRenderTexture(NULL),
	m_pDepthTexture(NULL),
	m_BlendOff(NULL),
	m_BlendOn(NULL),
	m_CullBack(NULL),
	m_CullFront(NULL),
	m_CullNone(NULL),
	m_DepthDisable(NULL),
	m_DepthEnable(NULL)
{
	ZeroMemory(&m_screenViewPort, sizeof(D3D11_VIEWPORT));

	// Get a pointer to the application object so we can forward 
	// Windows messages to the object's window procedure through
	// the global window procedure.
	g_app = this;
}
DXApp::~DXApp()
{
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pSwapChain);
	ReleaseCOM(m_pDepthTexture);

	// Restore all default settings.
	if (m_pImmediateContext)
		m_pImmediateContext->ClearState();

	ReleaseCOM(m_pImmediateContext);
	ReleaseCOM(m_pDevice);

}

bool DXApp::Init()
{
	if (!InitMainWindow())
	{
		MessageBox(0, L"Init MainWindow() Fialed", L"Error", MB_OK);
		return false;
	}
	if (!InitD3D11())
	{
		MessageBox(0, L"Init InitD3D11() Fialed", L"Error", MB_OK);
		return false;
	}
	return true;
}
int DXApp::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	m_timer.Reset();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_timer.Tick();
			if (!m_appPaused)
			{
				CaculateFrameState();
				UpdateScenes(m_timer.DeltaTime());
				DrawScenes();
			}
			else
			{
				Sleep(100);
			}
		}
	}
	return (int)msg.wParam;
}

HINSTANCE DXApp::AppInst()const
{
	return m_appInst;
}
HWND DXApp::AppMainWnd() const
{
	return m_hwnd;
}
float DXApp::AspectRatio()const
{
	return static_cast<float>(m_clientWidth) / m_clientHeight;
}

void DXApp::SetBlend(DeviceState state)
{
	switch (state)
	{
	case DXApp::OFF:
		break;
	case DXApp::ON:
		break;
	default:
		break;
	}
}
void DXApp::SetCullState(RasterizeState state)
{
	switch (state)
	{
	case DXApp::Wireframe:
		break;
	case DXApp::Solidframe:
		break;
	case DXApp::CullBack:
		break;
	case DXApp::CullFront:
		break;
	case DXApp::CullNone:
		break;
	default:
		break;
	}
}
void DXApp::SetDepthState(DeviceState state)
{
	switch (state)
	{
	case DXApp::OFF:
		break;
	case DXApp::ON:
		break;
	default:
		break;
	}
}

void DXApp::OnResize()
{
	assert(m_pImmediateContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = NULL;
	}
	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = NULL;
	}
	if (m_pDepthTexture)
	{
		m_pDepthTexture->Release();
		m_pDepthTexture = NULL;
	}

	HRESULT hr = m_pSwapChain->ResizeBuffers(0, m_clientWidth, m_clientHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
	if (FAILED(hr))
	{
		MessageBox(0, L"Resize Buffer Failed", L"error", MB_OK);
		return;
	}
	ID3D11Texture2D*backBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr))
	{
		MessageBox(0, L"Get Buffer Failed", L"error", MB_OK);
		return;
	}
	hr = m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create RTV Failed", L"error", MB_OK);
		return;
	}
	backBuffer->Release();


	D3D11_TEXTURE2D_DESC desc;
	desc.Width = m_clientWidth;
	desc.Height = m_clientHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if (m_enable4xMsaa)
	{
		desc.SampleDesc.Count = 4;
		desc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
	}

	hr = m_pDevice->CreateTexture2D(&desc, 0, &m_pDepthTexture);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create Depth Buffer Failed", L"error", MB_OK);
		return;
	}
	hr = m_pDevice->CreateDepthStencilView(m_pDepthTexture, 0, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create DSV Failed", L"error", MB_OK);
		return;
	}

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	m_screenViewPort.TopLeftX = 0;
	m_screenViewPort.TopLeftY = 0;
	m_screenViewPort.MaxDepth = 1.0f;
	m_screenViewPort.MinDepth = 0.0f;
	m_screenViewPort.Width = static_cast<float>(m_clientWidth);
	m_screenViewPort.Height = static_cast<float>(m_clientHeight);

	m_pImmediateContext->RSSetViewports(1, &m_screenViewPort);

}
void DXApp::OnMouseDown(WPARAM btnState, int x, int y)
{

}
void DXApp::OnMouseUp(WPARAM btnState, int x, int y)
{

}
void DXApp::OnMouseMove(WPARAM btnState, int x, int y)
{

}

LRESULT DXApp::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_appPaused = true;
			m_timer.Stop();
		}
		else
		{
			m_appPaused = false;
			m_timer.Start();
		}
		return 0;
	case WM_SIZE:
		// Save the new client area dimensions.
		m_clientWidth = LOWORD(lParam);
		m_clientHeight = HIWORD(lParam);
		if (m_pDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_appPaused = true;
				m_minimized = true;
				m_maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_appPaused = false;
				m_minimized = false;
				m_maximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (m_minimized)
				{
					m_appPaused = false;
					m_minimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (m_maximized)
				{
					m_appPaused = false;
					m_maximized = false;
					OnResize();
				}
				else if (m_resizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		m_appPaused = true;
		m_resizing = true;
		m_timer.Stop();
		return 0;
	case WM_EXITSIZEMOVE:
		m_appPaused = false;
		m_resizing = false;
		m_timer.Start();
		OnResize();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool DXApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hInstance = m_appInst;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, m_clientWidth, m_clientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	m_hwnd = CreateWindow(L"D3DWndClassName", m_mainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_appInst, 0);
	if (!m_hwnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	return true;
}

bool DXApp::InitD3D11()
{
	// Create the device and device context.

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		m_driverType,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&m_pDevice,
		&featureLevel,
		&m_pImmediateContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	hr = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
	//assert(m4xMsaaQuality > 0);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_clientWidth;
	sd.BufferDesc.Height = m_clientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if (m_enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(hr))
	{
		MessageBox(0, L"Query Interface Faild", L"error", MB_OK);
		return 0;
	}

	IDXGIAdapter* dxgiAdapter = 0;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(hr))
	{
		MessageBox(0, L"Query Adapter Faild", L"error", MB_OK);
		return 0;
	}

	IDXGIFactory* dxgiFactory = 0;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr))
	{
		MessageBox(0, L"Query Factory Faild", L"error", MB_OK);
		return 0;
	}

	hr = dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create Swap Chain Faild", L"error", MB_OK);
		return 0;
	}

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.

	OnResize();

	InitStateDevices();

	return true;
}

void DXApp::CaculateFrameState()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((m_timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << m_mainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";

		SetWindowText(m_hwnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

bool DXApp::InitStateDevices()
{
	//Blend State


	//Cull State


	//Depth State


	return true;
}

void DXApp::RenderBegin()
{
	assert(m_pImmediateContext);
	FLOAT clearColor[4] = { 0.0f,0.6f,0.6f,1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DXApp::RenderEnd()
{
	assert(m_pSwapChain);
	m_pSwapChain->Present(0, 0);
}