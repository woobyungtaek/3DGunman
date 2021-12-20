#include "pch.h"
#include <windowsx.h>
#include <sstream>
#include "D3DApp.h"

using namespace BTEngine;
using namespace BTInputEvent;

D3DApp* D3DApp::pInstance;

#define ID_TEST_EDITBOX 10001

D3DApp::D3DApp()
{
	mMsg = MSG();
	mhWnd = 0;
	mWC = WNDCLASSEX();

	mScreenSize = POINT();
	mLastMousePos = POINT();

	mbInitSuccess =false;
	mAppPaused = false;
	mResizing = false;

	mTimer = new GameTimer();

	frameCnt = 0;
	timeElapsed = 0;
}

D3DApp::~D3DApp()
{
	delete mTimer;

	UnregisterClass(mClassName, mWC.hInstance);
}

void D3DApp::Init(HINSTANCE hInst)
{
	mScreenSize.x = 1280;
	mScreenSize.y = 720;

	//���� ������ ���
	mWC.cbSize = sizeof(WNDCLASSEX);
	mWC.style = CS_HREDRAW | CS_VREDRAW;
	mWC.lpfnWndProc = (WNDPROC)MainMsgProc;
	mWC.cbClsExtra = 0;
	mWC.cbWndExtra = 0;
	mWC.hInstance = hInst;
	mWC.hIcon = LoadIcon(hInst, (LPCTSTR)IDI_APPLICATION);
	mWC.hCursor = LoadCursor(NULL, IDC_ARROW);
	mWC.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	mWC.lpszMenuName = NULL;
	mWC.lpszClassName = mClassName;
	mWC.hIconSm = LoadIcon(mWC.hInstance, (LPCTSTR)IDI_APPLICATION);
	RegisterClassEx(&mWC);

	mhWnd = CreateWindowExW(
		WS_EX_APPWINDOW,
		mClassName,
		mWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		mScreenSize.x, mScreenSize.y,
		GetDesktopWindow(),
		NULL,
		hInst,
		NULL
	);

	mTimer->Reset();

	ShowWindow(mhWnd, SW_SHOWDEFAULT);
	UpdateWindow(mhWnd);

	// �ʱ�ȭ 
	mbInitSuccess = BTEngine::Init(mScreenSize.x, mScreenSize.y, false, mhWnd, false, 1, 0);
	
	// �ʿ� �� ����
	TestScene* testScene = new TestScene();
	BTScene::CreateScene(testScene);

	LoginScene* loginScene = new LoginScene();
	BTScene::CreateScene(loginScene);

	//������ 0�� ������ �ε� �ȴ�.
	BTScene::LoadScene(0);
}

void D3DApp::Run()
{
	while (true)
	{
		if (PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE))
		{
			if (mMsg.message == WM_QUIT) { break; }
			TranslateMessage(&mMsg);
			DispatchMessage(&mMsg);
		}
		else
		{
			Update();
		}
	}
	return;

}

void D3DApp::Update()
{
	//Update
	mTimer->Tick();
	CalculateFrame();

	//Ű �Է¹ް� ó���ϱ�
	if ( InputKeyDown(VK_ESCAPE) )
	{
		PostQuitMessage(0);
	}

	BTEngine::EngineUpdate( mTimer->DeltaTime() );
}

LRESULT D3DApp::MainMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			pInstance->mAppPaused = true;
			pInstance->mTimer->Stop();
		}
		else
		{
			pInstance->mAppPaused = false;
			pInstance->mTimer->Start();
		}
	}
	break;

	case WM_ENTERSIZEMOVE:
	{
		pInstance->mAppPaused = true;
		pInstance->mResizing = true;
		pInstance->mTimer->Stop();
	}
	break;

	case WM_EXITSIZEMOVE:
	{
		pInstance->mAppPaused = false;
		pInstance->mResizing = false;
		pInstance->mTimer->Start();
		BTEngine::OnResize(pInstance->mScreenSize.x, pInstance->mScreenSize.y);
	}
	break;

	case WM_SIZE:
	{
		pInstance->mScreenSize.x = LOWORD(lParam);
		pInstance->mScreenSize.y = HIWORD(lParam);

		if (pInstance->mResizing) { break; }

		if (pInstance->mbInitSuccess == false) { break; }
		BTEngine::OnResize(pInstance->mScreenSize.x, pInstance->mScreenSize.y);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void D3DApp::CalculateFrame()
{
	++frameCnt;

	if ( (mTimer->TotalTime() - timeElapsed >= 1.0f) )
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		wostringstream outs;
		outs << mClassName << L" / FPS : " << fps << " / " << L"Frame Time : " << mspf << "(ms)";
		SetWindowText(mhWnd, outs.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}