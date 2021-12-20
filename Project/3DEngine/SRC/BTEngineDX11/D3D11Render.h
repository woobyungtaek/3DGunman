/*
	Core�� Render�� �ǹ�.

	Core�� ������ �Ǿ���ϰ�.
	Render�� ���� ���� �Ǿ
	Core�� Render�� �̿��ϴ� ���°� �Ǿ����
	Core�� �׸��� �׸����� Scene�� �޾ƿͼ� �׷����Ѵ�.
*/

#pragma once

class D3D11Render
{
public:
	D3D11Render();
	~D3D11Render();

	bool Init(int sWidth, int sHeight, bool vsync, HWND hWnd, bool fullscreen, float sDepth, float sNear);
	void Release();

	void BeginRender(float r, float g, float b, float a);
	void BeginSceneRender();
	void EndSceneRender();
	void EndRender();

	void OnResize(int sWidth, int sHeight);

public:
	HWND GetHWnd();

	ID3D11Device*				GetDevice();
	ID3D11DeviceContext*		GetDeviceContext();

	ID3D11RasterizerState*		GetRasterState();
	ID3D11RasterizerState*		GetWireframeRS();
	ID3D11RasterizerState*		GetSolidRS();

	ID3D11DepthStencilState*	GetNormalDSS();

	ID3D11DepthStencilState*	GetDS();

	ID3D11DepthStencilState*	GetDisableDS();

	DXTKFont*					GetFont();

	Vector2 GetScreenSize();
	
private:
	void CreateRenderState();


public:
	bool mbVSync;
	int mVideoCardMemory = 0;
	char mVideoCardDesc[128] = { 0, };

	HWND mhWnd;

	ID3D11Device*		 mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;

	IDXGISwapChain* mSwapChain = nullptr;
	bool			mEnable4xMsaa;
	UINT			m4xMsaaQuality;

	ID3D11Texture2D*		mDepthStencilBuffer = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;

	D3D11_VIEWPORT			mScreenViewport;

	ID3D11InputLayout*		mInputLayout;

	ID3D11RasterizerState*	mRasterState = nullptr;

	ID3D11RasterizerState*	mWireframeRS;
	ID3D11RasterizerState*	mSolidRS;

	ID3D11DepthStencilState* NormalDSS;
	ID3D11DepthStencilState* mDepthStencilState = nullptr;
	ID3D11DepthStencilState* mDepthDisabledStencilState = nullptr;

	bool bTest;

	Matrix mWorld;
	Matrix mView;
	Matrix mProj;

	// ��Ʈ
	DXTKFont* mpFont;

private:
	int mScreenWidth;
	int mScreenHeight;

};