#pragma once
class LobbySceneManager;

class RoomCell :public GameObject
{
public:
	RoomCell();
	virtual ~RoomCell();

public:
	void InitRoomCell(int index);

	void RefreshRoomCellByLine(int cellIndex, int userCount);

	void OnEnterButtonClickedUp();

public:
	LobbySceneManager* lobbyManager;

	GameObject* mRoomEnterButton;
	TextRenderer* mRoomNumText;
	TextRenderer* mRoomUserCountText;

	int mMaxUserCount = 2;
	int mCurrenCount;

private:
	int mIndex;		// ��ϻ� ��ȣ
	int mCellIndex; // ���� �� ��ȣ

	// �������� �ѱ�� ����
	// 0��° �뿡 5��° �������� �����ؾ��Ѵ�.
};

