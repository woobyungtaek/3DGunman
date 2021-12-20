#pragma once

namespace BTEngine
{
	bool Init(int sWidth, int sHeight, bool vsync, HWND hWnd, bool fullscreen, float sDepth, float sNear);
	
	void OnResize(int sWidth, int sHeight);

	void OnResize();

	void EngineUpdate(float deltaTime);

}

namespace BTInputEvent
{
	bool InputKey(int vk);
	bool InputKeyDown(int vk);
	bool InputKeyUp(int vk);

	POINT GetMousePosCT();
	POINT GetMousePosSC();
}

namespace BTLoader
{
	void LoadModelDataSet(std::wstring name, std::wstring pFilePath);
	void LoadAnimation(std::wstring name, std::wstring pFilePath);
	void LoadTextureFromDDS(std::wstring name, std::wstring pFilePath);
	void LoadTexturePng(std::wstring name, std::wstring pFilePath);
}


class BTMaterial;
namespace BTResource
{
	// Material ���� ȹ��
	BTMaterial* GetMaterial(std::wstring name, int refNum);

	// Material�� MapResource ����
	void SetDiffuseMapOnMat(BTMaterial* targetMat, std::wstring diffuseName);
	void SetNormalMapOnMat(BTMaterial* targetMat, std::wstring normalName);
	// Effect������ �ʿ�

	// MeshData�� Material ����
	void SetMatOnModelData(std::wstring name, int meshNum, BTMaterial* mat);
	void SetMatOnModelDataSet(std::wstring name, BTMaterial* mat);

}

namespace BTScene
{
	void CreateScene(IScene* pScene);
	void LoadScene  (int sceneNum, int type = 0);
} 