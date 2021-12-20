#pragma once


class D3DApp
{
private:
	static D3DApp* pInstance;
	D3DApp();

public:
	static D3DApp* GetInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new D3DApp();
		}

		return pInstance;
	}
	~D3DApp();	

public:
	void Init(HINSTANCE hInst);
	void Run();

	void Update();

	void AllSceneLoad();

public:
	static LRESULT CALLBACK MainMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void CalculateFrame();
	

private:
	MSG mMsg;
	HWND mhWnd;
	WNDCLASSEX mWC;

	const TCHAR* mClassName = _T("GunMan3D");
	const TCHAR* mWindowName = _T("GunMan3D");

	bool mbInitSuccess;
	bool mAppPaused;
	bool mResizing;

	POINT mScreenSize;
	POINT mLastMousePos;

	GameTimer* mTimer;

	int frameCnt;
	float timeElapsed;
};

