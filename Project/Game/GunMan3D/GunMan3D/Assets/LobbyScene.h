/*
	���� ��
	���� ������ ��ܰ� ���� ������ ���� �����ش�.
	���� �����ϸ� GameScene���� �Ѿ��.
*/
#pragma once
class LobbyScene : public IScene
{
public:
	LobbyScene();
	~LobbyScene();

public:
	virtual void Start() override;

private:
	GameObject* mLobbySceneManager;
};

