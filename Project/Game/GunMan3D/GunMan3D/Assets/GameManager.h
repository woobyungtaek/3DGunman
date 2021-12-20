#pragma once
enum class EGameState
{
	Ready = 0,
	Start,
	Hold,
	OpenFire,
	End,
	ShowResult,
	Wait
};

enum class EWinPlayer
{
	Draw = 0,
	Player1,
	Player2
};

class GameManager : public BTBehaviour
{
public:
	GameManager();
	~GameManager();

public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;

	void CreateObject();
	void InitGame();

private:
	void ReadyFunc();
	void StartFunc();
	void HoldFunc(float deltaTime);
	void OpenFireFunc(float deltaTime);
	void EndFunc();
	void ShowResultFunc(float deltaTime);

	void RefreshReadyImage();
	void ResultImageOnOff(bool bAllOff = false);

	void TestInput(float deltaTime);


	// �¶��� �� �Լ�

	// �÷��̾� ���� 
	void Player_1_On(bool bOn); //������ ����
	void Player_2_On(bool bOn);		 

public:
	void Recieve_OpponentExit();
	void Recieve_RoomUserState(string id1, string id2);
	void Recieve_ReadyState(string id1, int bReady);
	void Recieve_AllPlayerReady(float waitTime); 
	void Recieve_GameResult(string win, string id1, float time1, string id2, float time2);

public:
	Camera* mSceneCamera;
	GameObject* mControlPlayer;

	// �÷��̾��� ��
	GameObject* mPlayer1;
	GameObject* mPlayer2;
	
	wstring mWin_ID;
	wstring mP1_ID;
	wstring mP2_ID;

	bool mPlayerOn_1;
	bool mPlayerOn_2;

	bool mPlayerReady_1;
	SpriteRenderer* mReadyImage_1;
	bool mPlayerReady_2;
	SpriteRenderer* mReadyImage_2;

	// ���� ���࿡ �ʿ��� ��

	GameObject* mHoldImage;
	GameObject* mFireImage;

	GameObject* mWinImage;
	GameObject* mDefeatedImage;
	GameObject* mDrawImage;


	// ���� ���� ����
	EGameState mCurrentState;

	// �¸� �÷��̾�
	EWinPlayer mWinPlayer;

	// ��� �ð�
	float mWaitTime;
	
	// ��� ȭ�� ��� �ð�
	float mResultElapsedTime;

	// ��� �ð�
	float mElapsedTime;

	// �߻� �ð� & �߻� ����
	bool mbFired_P1;
	float mFireTime_P1;

	bool mbFired_P2;
	float mFireTime_P2;
	
	// ���� �ð�
	const float LIMIT_TIME = 1.f;
	const float RESULT_TIME = 5.f;

	// �����
	GameObject* mReadyStateTextUI;
	GameObject* mCurrentRoomNumTextUI;
};

