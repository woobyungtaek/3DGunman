/*
	���� ��
	������ ����Ǵ� ��, ������ ��ư�� ������ Lobby Scene���� �̵��Ѵ�.
*/
#pragma once
class GameManager;

class GameScene : public IScene
{
public:
	GameScene();
	~GameScene();

public:
	virtual void Start() override;

private:
	GameObject* mGameManager;
	GameObject* mMapManager;

};

