
#include "pch.h"
#include "EngineScene.h"

EngineScene::EngineScene(IScene* scene)
{
	mScene = scene;
	Object::Scene = this;

	// 에디터 카메라
	mEditorCamera = new Camera();
	mEditorCamera->LookAt(Vector3(0.0f, 8.0f, -8.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
	mEditorCamera->IsEditorCamera = true;
	mCameraVec.push_back(mEditorCamera);
	mScene->mActiveCamera = mEditorCamera;

	// DirectionalLight (최대 3개)
	DirectionalLight* dirLight = new DirectionalLight();
	dirLight->Ambient = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight->Diffuse = Vector4(0.15f, 0.15f, 0.15f, 1.0f);
	dirLight->Specular = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	Vector3 dir = Vector3(126.695f, -273.327f, 524.328f);
	dir.Normalize();
	dirLight->Direction = dir;
	mDirLightVec.push_back(*dirLight);

	//DirectionalLight* dirLight2 = new DirectionalLight();
	//dirLight2->Ambient	 = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
	//dirLight2->Diffuse	 = Vector4(0.15f, 0.15f, 0.15f, 1.0f);
	//dirLight2->Specular  = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	//dirLight2->Direction = Vector3(0, -1.f, 0);
	//mDirLightVec.push_back(*dirLight2);

	delete dirLight;
	//delete dirLight2;

	if (false)
	{
		// AxisObject
		GameObject* axisObject = new GameObject();
		HelperRenderer* axisHelper = axisObject->AddComponent<HelperRenderer>();
		axisHelper->SetMeshDatasByStr(L"Axis");

		// GridObject
		GameObject* gridObject = new GameObject();
		HelperRenderer* gridHelper = gridObject->AddComponent<HelperRenderer>();
		gridHelper->SetMeshDatasByStr(L"Grid");
	}
}

EngineScene::~EngineScene()
{
}

void EngineScene::SceneStart()
{
	Object::Scene = this;
	mScene->Start();
}

void EngineScene::SceneUpdate(float deltaTime)
{
	Object::Scene = this;

	mEditorCamera->CameraMove(deltaTime);

	// 카메라들 Update 후 UpdateViewMatrix 하도록 해야함 
	for (int index = 0; index < mCameraVec.size(); ++index)
	{
		mCameraVec[index]->UpdateViewMatrix();
	}
	
	// Start 함수가 있다면 돌린다.
	for (int index = 0; index < mGameObjectVec.size(); ++index)
	{
		if (mGameObjectVec[index]->IsActive == false) { continue; }
		mGameObjectVec[index]->Start();
	}

	// 게임 오브젝트 모두 Update / 카메라도 Update 하도록 해야하낭?
	for (int index = 0; index < mGameObjectVec.size(); ++index)
	{
		if (mGameObjectVec[index]->IsActive == false) { continue; }
		mGameObjectVec[index]->Update(deltaTime);
	}

	for (int index = 0; index < mRendererVec.size(); ++index)
	{
		// EditorCamera가 아니라 현재 활성화 카메라로
		// 에디터 모드를 원할때 활성 카메라를 Editor카메라로 바꾼다.
		if (mRendererVec[index]->gameObject->IsActive == false) { continue; }
		mRendererVec[index]->UpdateRenderer(mEditorCamera);
	}
}

void EngineScene::SceneRender()
{
	// 카메라가 그려야하는 ( 스카이박스 )
	for (int index = 0; index < mCameraVec.size(); ++index)
	{
		mCameraVec[index]->Render();
	}

	// 렌더러 오브젝트 모두 그리기	
	for (int index = 0; index < mRendererVec.size(); ++index)
	{
		if (mRendererVec[index]->gameObject->IsActive == false) { continue; }
		mRendererVec[index]->Render(mEditorCamera);
	}

	// 이걸 빼면 다른 그림들의 Alpha값이 안빠짐... Begin과 End 함수가 필요한듯
	BTGameEngine::GetInstance()->GetRender()->GetFont()->DrawTextColor(
		10, 10, 0, 1, Vector4(1.f, 1.f, 1.f, 1.f),(TCHAR*)L" ");

}

