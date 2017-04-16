////***************************************************************************************
//// SkullDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
////
//// Demonstrates loading more complicated geometry from file and rendering it.
////
//// Controls:
////		Hold the left mouse button down and move the mouse to rotate.
////      Hold the right mouse button down to zoom in and out.
////
////***************************************************************************************
//
//#include "DXApp.h"
//#include "d3dx11Effect.h"
//#include "GeometryGenerator.h"
//#include "MathHelper.h"
//
//struct Vertex
//{
//	XMFLOAT3 Pos;
//	XMFLOAT4 Color;
//};
//
//class SkullApp : public DXApp
//{
//public:
//	SkullApp(HINSTANCE hInstance);
//	~SkullApp();
//
//	bool Init();
//	void OnResize();
//	void UpdateScenes(float dt);
//	void DrawScenes();
//
//	void OnMouseDown(WPARAM btnState, int x, int y);
//	void OnMouseUp(WPARAM btnState, int x, int y);
//	void OnMouseMove(WPARAM btnState, int x, int y);
//
//private:
//	void BuildGeometryBuffers();
//	void BuildFX();
//	void BuildVertexLayout();
//
//private:
//	ID3D11Buffer* mVB;
//	ID3D11Buffer* mIB;
//
//	ID3DX11Effect* mFX;
//	ID3DX11EffectTechnique* mTech;
//	ID3DX11EffectMatrixVariable* mfxWorldViewProj;
//
//	ID3D11InputLayout* mInputLayout;
//
//	ID3D11RasterizerState* mWireframeRS;
//
//	// Define transformations from local spaces to world space.
//	XMFLOAT4X4 mSkullWorld;
//
//	UINT mSkullIndexCount;
//
//	XMFLOAT4X4 mView;
//	XMFLOAT4X4 mProj;
//
//	float mTheta;
//	float mPhi;
//	float mRadius;
//
//	POINT mLastMousePos;
//};
//
//
//SkullApp::SkullApp(HINSTANCE hInstance)
//	: DXApp(hInstance), mVB(0), mIB(0), mFX(0), mTech(0),
//	mfxWorldViewProj(0), mInputLayout(0), mWireframeRS(0), mSkullIndexCount(0),
//	mTheta(1.5f*MathHelper::Pi), mPhi(0.1f*MathHelper::Pi), mRadius(20.0f)
//{
//	m_mainWndCaption = L"Skull Demo";
//
//	mLastMousePos.x = 0;
//	mLastMousePos.y = 0;
//
//	XMMATRIX I = XMMatrixIdentity();
//	XMStoreFloat4x4(&mView, I);
//	XMStoreFloat4x4(&mProj, I);
//
//	XMMATRIX T = XMMatrixTranslation(0.0f, -2.0f, 0.0f);
//	XMStoreFloat4x4(&mSkullWorld, T);
//
//	//m_enable4xMsaa = false;
//}
//
//SkullApp::~SkullApp()
//{
//	ReleaseCOM(mVB);
//	ReleaseCOM(mIB);
//	ReleaseCOM(mFX);
//	ReleaseCOM(mInputLayout);
//	ReleaseCOM(mWireframeRS);
//}
//
//bool SkullApp::Init()
//{
//	if (!DXApp::Init())
//		return false;
//
//	BuildGeometryBuffers();
//	BuildFX();
//	BuildVertexLayout();
//
//	D3D11_RASTERIZER_DESC wireframeDesc;
//	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
//	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
//	wireframeDesc.CullMode = D3D11_CULL_BACK;
//	wireframeDesc.FrontCounterClockwise = false;
//	wireframeDesc.DepthClipEnable = true;
//
//	HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));
//
//	return true;
//}
//
//void SkullApp::OnResize()
//{
//	DXApp::OnResize();
//
//	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
//	XMStoreFloat4x4(&mProj, P);
//}
//
//void SkullApp::UpdateScenes(float dt)
//{
//	// Convert Spherical to Cartesian coordinates.
//	float x = mRadius*sinf(mPhi)*cosf(mTheta);
//	float z = mRadius*sinf(mPhi)*sinf(mTheta);
//	float y = mRadius*cosf(mPhi);
//
//	// Build the view matrix.
//	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
//	XMVECTOR target = XMVectorZero();
//	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//
//	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
//	XMStoreFloat4x4(&mView, V);
//}
//
//void SkullApp::DrawScenes()
//{
//	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
//	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	m_pImmediateContext->IASetInputLayout(mInputLayout);
//	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	m_pImmediateContext->RSSetState(mWireframeRS);
//
//	UINT stride = sizeof(Vertex);
//	UINT offset = 0;
//	m_pImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
//	m_pImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
//
//	// Set constants
//
//	XMMATRIX view = XMLoadFloat4x4(&mView);
//	XMMATRIX proj = XMLoadFloat4x4(&mProj);
//	XMMATRIX world = XMLoadFloat4x4(&mSkullWorld);
//	XMMATRIX worldViewProj = world*view*proj;
//
//	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
//
//	D3DX11_TECHNIQUE_DESC techDesc;
//	mTech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		mTech->GetPassByIndex(p)->Apply(0, m_pImmediateContext);
//		m_pImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);
//	}
//
//	HR(m_pSwapChain->Present(0, 0));
//}
//
//void SkullApp::OnMouseDown(WPARAM btnState, int x, int y)
//{
//	mLastMousePos.x = x;
//	mLastMousePos.y = y;
//
//	SetCapture(m_hwnd);
//}
//
//void SkullApp::OnMouseUp(WPARAM btnState, int x, int y)
//{
//	ReleaseCapture();
//}
//
//void SkullApp::OnMouseMove(WPARAM btnState, int x, int y)
//{
//	if ((btnState & MK_LBUTTON) != 0)
//	{
//		// Make each pixel correspond to a quarter of a degree.
//		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
//		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
//
//		// Update angles based on input to orbit camera around box.
//		mTheta += dx;
//		mPhi += dy;
//
//		// Restrict the angle mPhi.
//		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
//	}
//	else if ((btnState & MK_RBUTTON) != 0)
//	{
//		// Make each pixel correspond to 0.2 unit in the scene.
//		float dx = 0.05f*static_cast<float>(x - mLastMousePos.x);
//		float dy = 0.05f*static_cast<float>(y - mLastMousePos.y);
//
//		// Update the camera radius based on input.
//		mRadius += dx - dy;
//
//		// Restrict the radius.
//		mRadius = MathHelper::Clamp(mRadius, 5.0f, 50.0f);
//	}
//
//	mLastMousePos.x = x;
//	mLastMousePos.y = y;
//}
//
//void SkullApp::BuildGeometryBuffers()
//{
//	std::ifstream fin("Models/skull.txt");
//
//	if (!fin)
//	{
//		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
//		return;
//	}
//
//	UINT vcount = 0;
//	UINT tcount = 0;
//	std::string ignore;
//
//	//按行读取，并且以“空格”为分隔符
//	//这一段是去除读取的数据头
//	fin >> ignore >> vcount;
//	fin >> ignore >> tcount;
//	fin >> ignore >> ignore >> ignore >> ignore;
//
//	float nx, ny, nz;
//	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);
//
//	std::vector<Vertex> vertices(vcount);
//	for (UINT i = 0; i < vcount; ++i)
//	{
//		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
//
//		vertices[i].Color = black;
//
//		// Normal not used in this demo.
//		fin >> nx >> ny >> nz;
//	}
//
//	//忽略索引序列前的三行非索引数据行
//	fin >> ignore;
//	fin >> ignore;
//	fin >> ignore;
//
//	mSkullIndexCount = 3 * tcount;
//	std::vector<UINT> indices(mSkullIndexCount);
//	for (UINT i = 0; i < tcount; ++i)
//	{
//		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
//	}
//
//	fin.close();
//
//	D3D11_BUFFER_DESC vbd;
//	vbd.Usage = D3D11_USAGE_IMMUTABLE;
//	vbd.ByteWidth = sizeof(Vertex) * vcount;
//	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vbd.CPUAccessFlags = 0;
//	vbd.MiscFlags = 0;
//	D3D11_SUBRESOURCE_DATA vinitData;
//	vinitData.pSysMem = &vertices[0];
//	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &mVB));
//
//	//
//	// Pack the indices of all the meshes into one index buffer.
//	//
//
//	D3D11_BUFFER_DESC ibd;
//	ibd.Usage = D3D11_USAGE_IMMUTABLE;
//	ibd.ByteWidth = sizeof(UINT) * mSkullIndexCount;
//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibd.CPUAccessFlags = 0;
//	ibd.MiscFlags = 0;
//	D3D11_SUBRESOURCE_DATA iinitData;
//	iinitData.pSysMem = &indices[0];
//	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mIB));
//}
//
//void SkullApp::BuildFX()
//{
//	std::ifstream fin("hlsl/skull.fxo", std::ios::binary);
//
//	fin.seekg(0, std::ios_base::end);
//	int size = (int)fin.tellg();
//	fin.seekg(0, std::ios_base::beg);
//	std::vector<char> compiledShader(size);
//
//	fin.read(&compiledShader[0], size);
//	fin.close();
//
//	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
//		0, m_pDevice, &mFX));
//
//	mTech = mFX->GetTechniqueByName("ColorTech");
//	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
//}
//
//void SkullApp::BuildVertexLayout()
//{
//	// Create the vertex input layout.
//	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
//	};
//
//	// Create the input layout
//	D3DX11_PASS_DESC passDesc;
//	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
//	HR(m_pDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
//		passDesc.IAInputSignatureSize, &mInputLayout));
//}