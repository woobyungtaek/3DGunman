/*
	�α��� ��
	�Է��� ���� �����Ͱ� ������ ���ٸ� LobbyScene���� �Ѿ��.
*/
#pragma once
class LoginScene : public IScene
{
public:
	LoginScene();
	~LoginScene();

public:
	virtual void Start() override;

private:
	GameObject* mLoginSceneManager;

};

