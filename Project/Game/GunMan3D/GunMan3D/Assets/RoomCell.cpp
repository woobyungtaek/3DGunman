#include "pch.h"
#include "RoomCell.h"

RoomCell::RoomCell()
{
	mCurrenCount = 0;
}

RoomCell::~RoomCell()
{
	delete mRoomEnterButton;
	delete mRoomNumText;
}

// ��� �� ��ȣ�� �޾ƿ� ��ġ�� �����Ѵ�.
void RoomCell::InitRoomCell(int index)
{
	mIndex = index;

	this->AddComponent<SpriteRenderer>();
	this->GetComponent<SpriteRenderer>()->SetSpriteByName(L"SmallRoomBackground");

	// ��ȣ�� ��ǥ�� �ٲ���Ѵ�.
	POINT coordi;
	coordi.x = mIndex % 5;
	coordi.y = mIndex / 5;

	float posX = 0.025f;
	float posY = 0.025f;
	float minX = coordi.x * 0.1354f + posX;
	float maxX = 1 - (coordi.x + 1) * 0.1354f - posX;

	float minY = 1 - (coordi.y + 1) * 0.2037f - posY; 
	float maxY = coordi.y * 0.2037f + posY;

	mRectTransform->mRect = Vector4(minX, maxX, maxY, minY);

	GameObject* temp1 = new GameObject();
	mRoomNumText= temp1->AddComponent<TextRenderer>();

	mRoomEnterButton = new GameObject();
	BTButton* button = mRoomEnterButton->AddComponent<BTButton>();
	button->SetBasicImgByName(L"RoomEnterButton");
	button->SetClickImgByName(L"RoomEnterButton_PUSH");
	button->SetClickUpEvent( [&](){ OnEnterButtonClickedUp(); } );

	mRoomEnterButton->mRectTransform->mRect = mRectTransform->mRect;
	mRoomEnterButton->mRectTransform->mRect += Vector4(0.03f, 0.03f, 0.13f, 0.03f);
	mRoomEnterButton->GetComponent<SpriteRenderer>()->SetSortOrder(5);

	GameObject* temp2 = new GameObject();
	mRoomUserCountText = temp2->AddComponent<TextRenderer>();

}

void RoomCell::RefreshRoomCellByLine(int cellIndex, int userCount)
{
	// ���� ǥ�õ� Cell���� ǥ��
	mCellIndex = cellIndex;

	float rPosX = (mRectTransform->mRect.x + 0.0125f) * mRectTransform->mScreenSize.x;
	float rPosY = (mRectTransform->mRect.z + 0.025f) * mRectTransform->mScreenSize.y;

	mRoomNumText->gameObject->mTransform->Position = Vector3(rPosX, rPosY, 0);
	mRoomNumText->SetText(L"%d", mCellIndex+1);
	mRoomNumText->SetSize(1.5f);
	mRoomNumText->SetColor(Vector4(0.f, 0.f,0.f, 1.f));

	mCurrenCount = userCount;
	mRoomUserCountText->gameObject->mTransform->Position = Vector3(rPosX + 30, rPosY + 30, 0);
	mRoomUserCountText->SetText(L"( %d / %d )", mCurrenCount, mMaxUserCount);
	mRoomUserCountText->SetSize(1.75f);
	mRoomUserCountText->SetColor(Vector4(0.f, 0.f, 0.f, 1.f));
}

void RoomCell::OnEnterButtonClickedUp()
{
	int roomUserCount = lobbyManager->mRoomHeadCountVec[mCellIndex];
	if (roomUserCount >= 2)
	{
		//�ο����� ������ ���� �Ұ�
		return;
	}

	GameDataManager::RoomNumber = mCellIndex;

	// GameScene�� ��� ���� �ǰ� ������ �޼����� �޾ƾ��Ѵ�.
	// �� ��� �ε� �ȴ����� �޼����� �������Ѵ�.
	BTScene::LoadScene((int)EGunManScene::GameScene);
}

