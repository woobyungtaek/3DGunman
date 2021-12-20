#include "pch.h"
#include "SceneManager.h"

vector<EngineScene*> SceneManager::mBuildSceneVec;

void SceneManager::AddEngineScene(EngineScene* engineScene)
{
	mBuildSceneVec.push_back(engineScene);
}

void SceneManager::LoadScene(int sceneNum, SceneLoadType type)
{
	int sceneCount = (int)mBuildSceneVec.size();
	if (0 > sceneCount || sceneNum >= sceneCount)
	{
		return;
	}

	// �����Ǵ� ������ ��������
	EngineScene* engineScene = mBuildSceneVec[sceneNum];

	BTGameEngine* engine = BTGameEngine::GetInstance();
	//
	if (type == SceneLoadType::Additive)
	{
		engine->AddLoadScene(engineScene);
	}
	else
	{
		engine->ClearLoadSceneVec();
		engine->AddLoadScene(engineScene);
	}
}

void SceneManager::LoadScene(wstring sceneName, SceneLoadType type)
{
}
