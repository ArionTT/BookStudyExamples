////***************************************************************************************
//// HillsDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
////
//// Demonstrates drawing hills using a grid and 2D function to set the height of each vertex.
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
//#include<math.h>
//
//struct Vertex
//{
//	XMFLOAT3 Pos;
//	XMFLOAT4 Color;
//};
//
//class HillsApp : public DXApp
//{
//public:
//	HillsApp(HINSTANCE hInstance);
//	~HillsApp();
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
//	float GetHeight(float x, float z)const;
//	float GussianCurve(float x, float z)const;
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
//	// Define transformations from local spaces to world space.
//	XMFLOAT4X4 mGridWorld;
//
//	UINT mGridIndexCount;
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
//HillsApp::HillsApp(HINSTANCE hInstance)
//	: DXApp(hInstance), mVB(0), mIB(0), mFX(0), mTech(0),
//	mfxWorldViewProj(0), mInputLayout(0), mGridIndexCount(0),
//	mTheta(1.5f*MathHelper::Pi), mPhi(0.1f*MathHelper::Pi), mRadius(200.0f)
//{
//	m_mainWndCaption = L"Hills Demo";
//
//	mLastMousePos.x = 0;
//	mLastMousePos.y = 0;
//
//	XMMATRIX I = XMMatrixIdentity();
//	XMStoreFloat4x4(&mGridWorld, I);
//	XMStoreFloat4x4(&mView, I);
//	XMStoreFloat4x4(&mProj, I);
//}
//
//HillsApp::~HillsApp()
//{
//	ReleaseCOM(mVB);
//	ReleaseCOM(mIB);
//	ReleaseCOM(mFX);
//	ReleaseCOM(mInputLayout);
//}
//
//bool HillsApp::Init()
//{
//	if (!DXApp::Init())
//		return false;
//
//	BuildGeometryBuffers();
//	BuildFX();
//	BuildVertexLayout();
//
//	return true;
//}
//
//void HillsApp::OnResize()
//{
//	DXApp::OnResize();
//
//	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
//	XMStoreFloat4x4(&mProj, P);
//}
//
//void HillsApp::UpdateScenes(float dt)
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
//void HillsApp::DrawScenes()
//{
//	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
//	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	m_pImmediateContext->IASetInputLayout(mInputLayout);
//	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
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
//	XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
//	XMMATRIX worldViewProj = world*view*proj;
//
//	D3DX11_TECHNIQUE_DESC techDesc;
//	mTech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		// Draw the grid.
//		mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
//		mTech->GetPassByIndex(p)->Apply(0, m_pImmediateContext);
//		m_pImmediateContext->DrawIndexed(mGridIndexCount, 0, 0);
//	}
//
//	HR(m_pSwapChain->Present(0, 0));
//}
//
//void HillsApp::OnMouseDown(WPARAM btnState, int x, int y)
//{
//	mLastMousePos.x = x;
//	mLastMousePos.y = y;
//
//	SetCapture(m_hwnd);
//}
//
//void HillsApp::OnMouseUp(WPARAM btnState, int x, int y)
//{
//	ReleaseCapture();
//}
//
//void HillsApp::OnMouseMove(WPARAM btnState, int x, int y)
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
//		float dx = 0.2f*static_cast<float>(x - mLastMousePos.x);
//		float dy = 0.2f*static_cast<float>(y - mLastMousePos.y);
//
//		// Update the camera radius based on input.
//		mRadius += dx - dy;
//
//		// Restrict the radius.
//		mRadius = MathHelper::Clamp(mRadius, 50.0f, 500.0f);
//	}
//
//	mLastMousePos.x = x;
//	mLastMousePos.y = y;
//}
//
//float HillsApp::GetHeight(float x, float z)const
//{
//	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
//}
//
////ÕýÌ¬·Ö²¼
//float HillsApp::GussianCurve(float x, float z)const
//{
//	
//	float low = 1/sqrt(2 * MathHelper::Pi);
//	float X = -1 * x*x*(0.0015f) / 2.0f;
//	float Z = -1 * z*z*(0.0015f) / 2.0f;
//	X = pow(MathHelper::Euler, X+Z);
//	return low*X;
//}
//
//void HillsApp::BuildGeometryBuffers()
//{
//	GeometryGenerator::MeshData grid;
//
//	GeometryGenerator geoGen;
//
//	geoGen.CreateGrid(160.0f, 160.0f, 50, 50, grid);
//
//	mGridIndexCount = grid.Indices.size();
//
//	//
//	// Extract the vertex elements we are interested and apply the height function to
//	// each vertex.  In addition, color the vertices based on their height so we have
//	// sandy looking beaches, grassy low hills, and snow mountain peaks.
//	//
//
//	std::vector<Vertex> vertices(grid.Vertices.size());
//	for (size_t i = 0; i < grid.Vertices.size(); ++i)
//	{
//		XMFLOAT3 p = grid.Vertices[i].Position;
//
//		//p.y = GetHeight(p.x, p.z);
//		p.y = GussianCurve(p.x, p.z);
//
//		vertices[i].Pos = p;
//
//		// Color the vertex based on its height.
//		if (p.y < -10.0f)
//		{
//			// Sandy beach color.
//			vertices[i].Color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
//		}
//		else if (p.y < 5.0f)
//		{
//			// Light yellow-green.
//			vertices[i].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
//		}
//		else if (p.y < 12.0f)
//		{
//			// Dark yellow-green.
//			vertices[i].Color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
//		}
//		else if (p.y < 20.0f)
//		{
//			// Dark brown.
//			vertices[i].Color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
//		}
//		else
//		{
//			// White snow.
//			vertices[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//		}
//	}
//
//	D3D11_BUFFER_DESC vbd;
//	vbd.Usage = D3D11_USAGE_IMMUTABLE;
//	vbd.ByteWidth = sizeof(Vertex) * grid.Vertices.size();
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
//	ibd.ByteWidth = sizeof(UINT) * mGridIndexCount;
//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibd.CPUAccessFlags = 0;
//	ibd.MiscFlags = 0;
//	D3D11_SUBRESOURCE_DATA iinitData;
//	iinitData.pSysMem = &grid.Indices[0];
//	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mIB));
//}
//
//void HillsApp::BuildFX()
//{
//	std::ifstream fin("hlsl/hills.fxo", std::ios::binary);
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
//void HillsApp::BuildVertexLayout()
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