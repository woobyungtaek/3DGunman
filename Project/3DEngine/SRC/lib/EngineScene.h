/*
	EngineScene : �������� Render�� Game�� �̾��ִ� ����, Object, Light, Camera���� ��� �˰� �ִ´�.
	IScene : �ܺο��� ������ �ؾ��ϴ� Scene ����
	Render�� EngineScene�� Render()�� ����
*/
#pragma once

class EngineScene
{
public:
	EngineScene(IScene* scene);
	~EngineScene();

public:
	// ������Ʈ ��
	void SceneStart();
	void SceneUpdate(float deltaTime);
	void SceneRender();

private:
	bool mbChangeScene;

private:
	// Update�� �����ǵ� Scene ��ü
	IScene* mScene; 

	// ������ ī�޶�
	Camera* mEditorCamera;

public:
	// ������Ʈ List
	std::vector<GameObject*> mGameObjectVec;

	// ������ List ( ���� ���� )
	std::vector<Renderer*> mRendererVec;

	// ī�޶�	List
	std::vector<Camera*> mCameraVec;	

	// ����Ʈ	List
	std::vector<DirectionalLight> mDirLightVec;

	// �۾� ��� ���� List

};

