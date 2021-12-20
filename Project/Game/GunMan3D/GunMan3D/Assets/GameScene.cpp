#include "pch.h"
#include "GameManager.h"
#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Start()
{
	BTEngine::OnResize();
	mActiveCamera->SetCubeMap(L"GunmanSkyBox2");

	if (mMapManager == nullptr)
	{
		mMapManager = new GameObject();
		mMapManager->AddComponent<MapManager>()->CreateMap();
	}

	// ����  �Ŵ��� ����
	if (mGameManager == nullptr)
	{
		mGameManager = new GameObject();
		mGameManager->AddComponent<GameManager>();
		mGameManager->GetComponent<GameManager>()->mSceneCamera = mActiveCamera;
	}
	mGameManager->GetComponent<GameManager>()->InitGame();
}
