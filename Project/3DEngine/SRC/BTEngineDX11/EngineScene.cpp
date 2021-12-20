
#include "pch.h"
#include "EngineScene.h"

EngineScene::EngineScene(IScene* scene)
{
	mScene = scene;
	Object::Scene = this;

	// ������ ī�޶�
	mEditorCamera = new Camera();
	mEditorCamera->LookAt(Vector3(0.0f, 8.0f, -8.0f), Vector3(0, 0, 0), Vector3(0, 1.0f, 0));
	mEditorCamera->IsEditorCamera = true;
	mCameraVec.push_back(mEditorCamera);
	mScene->mActiveCamera = mEditorCamera;

	// DirectionalLight (�ִ� 3��)
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

	// ī�޶�� Update �� UpdateViewMatrix �ϵ��� �ؾ��� 
	for (int index = 0; index < mCameraVec.size(); ++index)
	{
		mCameraVec[index]->UpdateViewMatrix();
	}
	
	// Start �Լ��� �ִٸ� ������.
	for (int index = 0; index < mGameObjectVec.size(); ++index)
	{
		if (mGameObjectVec[index]->IsActive == false) { continue; }
		mGameObjectVec[index]->Start();
	}

	// ���� ������Ʈ ��� Update / ī�޶� Update �ϵ��� �ؾ��ϳ�?
	for (int index = 0; index < mGameObjectVec.size(); ++index)
	{
		if (mGameObjectVec[index]->IsActive == false) { continue; }
		mGameObjectVec[index]->Update(deltaTime);
	}

	for (int index = 0; index < mRendererVec.size(); ++index)
	{
		// EditorCamera�� �ƴ϶� ���� Ȱ��ȭ ī�޶��
		// ������ ��带 ���Ҷ� Ȱ�� ī�޶� Editorī�޶�� �ٲ۴�.
		if (mRendererVec[index]->gameObject->IsActive == false) { continue; }
		mRendererVec[index]->UpdateRenderer(mEditorCamera);
	}
}

void EngineScene::SceneRender()
{
	// ī�޶� �׷����ϴ� ( ��ī�̹ڽ� )
	for (int index = 0; index < mCameraVec.size(); ++index)
	{
		mCameraVec[index]->Render();
	}

	// ������ ������Ʈ ��� �׸���	
	for (int index = 0; index < mRendererVec.size(); ++index)
	{
		if (mRendererVec[index]->gameObject->IsActive == false) { continue; }
		mRendererVec[index]->Render(mEditorCamera);
	}

	// �̰� ���� �ٸ� �׸����� Alpha���� �Ⱥ���... Begin�� End �Լ��� �ʿ��ѵ�
	BTGameEngine::GetInstance()->GetRender()->GetFont()->DrawTextColor(
		10, 10, 0, 1, Vector4(1.f, 1.f, 1.f, 1.f),(TCHAR*)L" ");

}

