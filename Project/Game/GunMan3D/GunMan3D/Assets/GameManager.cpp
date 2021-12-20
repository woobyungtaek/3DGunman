#include "pch.h"
#include "GameManager.h"


GameManager::GameManager()
{
	srand(std::time(nullptr));
}

GameManager::~GameManager()
{
}

void GameManager::Start()
{
	CreateObject();

	mSceneCamera->mPosition = Vector3(12.59f, -4.918f, 14.5744f);
	mSceneCamera->LookAt(mSceneCamera->mPosition, mSceneCamera->mPosition + Vector3(-0.27f, 0.026f, 0.9597f), Vector3(0, 1.0f, 0));
	mSceneCamera->IsEditorCamera = false;
	mControlPlayer = mPlayer1;

	InitGame();
}

void GameManager::Update(float deltaTime)
{
	// >> �� ��Ʈ��
	//TestInput(deltaTime);
	if (BTInputEvent::InputKeyDown(VK_BACK))
	{
		if (GameDataManager::IsOnlineMode == true)
		{
			StringPacket* strPacket = new StringPacket();
			sprintf_s(strPacket->buffer, sizeof(strPacket->buffer), "1,1,%d", GameDataManager::RoomNumber);
			GameDataManager::mMGNetwork->SendPakcet(strPacket, GameDataManager::mMGNetwork->m_ServerList.at("Game"));

			BTScene::LoadScene((int)EGunManScene::LobbyScene);
		}
		else
		{
			BTScene::LoadScene((int)EGunManScene::LoginScene);
		}
	}

	// >> ���� ����
	switch (mCurrentState)
	{
	case EGameState::Ready:
	{
		ReadyFunc();
	}
	break;
	case EGameState::Start:
	{
		StartFunc();
	}
	break;
	case EGameState::Hold:
	{
		HoldFunc(deltaTime);
	}
	break;
	case EGameState::OpenFire:
	{
		OpenFireFunc(deltaTime);
	}
	break;
	case EGameState::End:
	{
		EndFunc();
	}
	break;
	case EGameState::ShowResult:
	{
		ShowResultFunc(deltaTime);
	}
	break;

	default:
		break;
	}

}

void GameManager::InitGame()
{
	if (mCurrentRoomNumTextUI != nullptr)
	{
		if (GameDataManager::IsOnlineMode == true)
		{
			// �ش� ���ȣ�� �� ������ ��û�Ѵ�.
			// ���� ���� ���� �Ŀ� �Űܾ���
			// ������ ��� ���� �����ϰ�
			// ������ ��� �ƹ��͵� ���� �ʴ´�.
			StringPacket* strPacket = new StringPacket();
			sprintf_s(strPacket->buffer, sizeof(strPacket->buffer), "0,2,%d", GameDataManager::RoomNumber);
			bool bSuccess = GameDataManager::mMGNetwork->SendPakcet(strPacket, GameDataManager::mMGNetwork->m_ServerList.at("Game"));

			// ���ȣ ���
			mCurrentRoomNumTextUI->GetComponent<TextRenderer>()->SetText(L"Room : %d", GameDataManager::RoomNumber + 1);

			// �¶��� ��� ���� ������ ����
			Player_1_On(true);
			Player_2_On(false);
		}
		else
		{
			// ���� ��� �Ѵ� ������ ����
			Player_1_On(true);
			Player_2_On(true);
		}

	}
}

void GameManager::CreateObject()
{
	LobbyClient* lobbyClient = static_cast<LobbyClient*>(GameDataManager::mMGNetwork->m_pPacketSystem);
	lobbyClient->gameManager = this;

	mPlayer1 = new GameObject();
	mPlayer2 = new GameObject();

	mPlayer1->AddComponent<MeshRenderer>()->SetMeshDatasByStr(L"03IK-Joe");
	mPlayer2->AddComponent<MeshRenderer>()->SetMeshDatasByStr(L"03IK-Joe");

	mPlayer1->mTransform->Position = Vector3(11.f, -7.133f, 16.6f);
	mPlayer1->mTransform->Rotation = Vector3(0, -177.492538f, 0);
	mPlayer1->mTransform->Scale = Vector3(0.0252845f, 0.0252845f, 0.0252845f);

	mPlayer2->mTransform->Position = Vector3(12.239f, -6.351f, 22.5529f);
	mPlayer2->mTransform->Rotation = Vector3(0, 5.84110498f, 0);
	mPlayer2->mTransform->Scale = Vector3(0.0178356f, 0.0178356f, 0.0178356f);

	GameObject* temp1 = new GameObject();
	mReadyImage_1 = temp1->AddComponent<SpriteRenderer>();
	mReadyImage_1->SetSpriteByName(L"Ready_Off");
	temp1->mRectTransform->mRect = Vector4(0.5f, 0.2f, 0.6f, 0.2f);
	temp1->IsActive = false;

	GameObject* temp2 = new GameObject();
	mReadyImage_2 = temp2->AddComponent<SpriteRenderer>();
	mReadyImage_2->SetSpriteByName(L"Ready_Off");
	temp2->mRectTransform->mRect = Vector4(0.73f, 0.15f, 0.25f, 0.65f);
	temp2->IsActive = false;

	mHoldImage = new GameObject();
	mHoldImage->AddComponent<SpriteRenderer>()->SetSpriteByName(L"Hold");
	mHoldImage->mRectTransform->mRect = Vector4(0.292f, 0.292f, 0.411f, 0.411f);
	mHoldImage->IsActive = false;

	mFireImage = new GameObject();
	mFireImage->AddComponent<SpriteRenderer>()->SetSpriteByName(L"Fire");
	mFireImage->mRectTransform->mRect = Vector4(0.292f, 0.292f, 0.411f, 0.411f);
	mFireImage->IsActive = false;

	mDefeatedImage = new GameObject();
	mDefeatedImage->AddComponent<SpriteRenderer>()->SetSpriteByName(L"Defeated");
	mDefeatedImage->mRectTransform->mRect = Vector4(0.2678f, 0.2678f, 0.441f, 0.441f);

	mDefeatedImage->IsActive = false;

	mWinImage = new GameObject();
	mWinImage->AddComponent<SpriteRenderer>()->SetSpriteByName(L"Win");
	mWinImage->mRectTransform->mRect = Vector4(0.284f, 0.284f, 0.441f, 0.441f);
	mWinImage->IsActive = false;

	mDrawImage = new GameObject();
	mDrawImage->AddComponent<SpriteRenderer>()->SetSpriteByName(L"Draw");
	mDrawImage->mRectTransform->mRect = Vector4(0.2678f, 0.2678f, 0.441f, 0.441f);
	mDrawImage->IsActive = false;

	mReadyStateTextUI = new GameObject();
	mReadyStateTextUI->AddComponent<TextRenderer>()->SetSize(3.f);
	mReadyStateTextUI->mTransform->Position = Vector3(10.f, 50.f, 0);

	mCurrentRoomNumTextUI = new GameObject();
	mCurrentRoomNumTextUI->AddComponent<TextRenderer>()->SetSize(2.f);
	mCurrentRoomNumTextUI->mTransform->Position = Vector3(10.f, 5.f, 0);
}

// ��� ���� �� ���� �� �Լ�
void GameManager::ReadyFunc()
{
	// �÷��̾� ĳ���� ���� ����
	mPlayer1->IsActive = mPlayerOn_1;
	mPlayer2->IsActive = mPlayerOn_2;

	if (GameDataManager::IsOnlineMode == true)
	{
		mReadyStateTextUI->GetComponent<TextRenderer>()->SetText(L"���� Ű : SpaceBar\n������ : BackSpace");
		if (BTInputEvent::InputKeyDown(VK_SPACE) && mPlayerOn_1)
		{
			mPlayerReady_1 = !mPlayerReady_1;

			//<Send> �÷��̾� 1 Ready����  ����
			StringPacket* strPacket = new StringPacket();
			sprintf_s(strPacket->buffer, sizeof(strPacket->buffer), "1,3");
			GameDataManager::mMGNetwork->SendPakcet(strPacket, GameDataManager::mMGNetwork->m_ServerList.at("Game"));
		}
		RefreshReadyImage();
	}
	else
	{
		mReadyStateTextUI->GetComponent<TextRenderer>()->SetText(L"1P Ű : SpaceBar\n2P Ű : Enter\n������ : BackSpace");
		if (BTInputEvent::InputKeyDown(VK_SPACE) && mPlayerOn_1)
		{
			mPlayerReady_1 = !mPlayerReady_1;

			//<Send> �÷��̾� 1 Ready����  ����
		}

		// ���� ��� 2P Ű ���� �ʿ�
		if (BTInputEvent::InputKeyDown(VK_RETURN) && mPlayerOn_2)
		{
			// �÷��̾� 2 Ready����  ����
			mPlayerReady_2 = !mPlayerReady_2;
		}

		RefreshReadyImage();

		//<Receive> ���� �Ǿ�� ��, ���� ���� �ǰ� �Ѵ� ����� �ð��� �޴´�. > �Լ� AllPlayerReady
		// ���� ���   �̴�� ����
		if (mPlayerReady_1 && mPlayerReady_2)
		{
			mPlayerReady_1 = false;
			mPlayerReady_2 = false;
			mbFired_P1 = false;
			mbFired_P2 = false;
			mReadyImage_1->gameObject->IsActive = false;
			mReadyImage_2->gameObject->IsActive = false;

			//�Ѵ� ����� ���� ����
			mCurrentState = EGameState::Start;
		}
	}
}

void GameManager::StartFunc()
{
	if (GameDataManager::IsOnlineMode == true)
	{
		mElapsedTime = 0.0f;
		mFireTime_P1 = 0.0f;
		mFireTime_P2 = 0.0f;

		mHoldImage->IsActive = true;
		mCurrentState = EGameState::Hold;
	}
	else
	{
		// ���� ��� ���� ���� �� ����
		int rndNum = rand() % 50;
		mWaitTime = (float)rndNum / 10.f;
		mWaitTime += 1.0f; // ��ٷ����ϴ� �ð� 1.f ~ 5.0f ����

		mElapsedTime = 0.0f;
		mFireTime_P1 = 0.0f;
		mFireTime_P2 = 0.0f;

		mHoldImage->IsActive = true;
		mCurrentState = EGameState::Hold;
	}
}

// �߻� ��� �� ���� �� �Լ�
void GameManager::HoldFunc(float deltaTime)
{
	mElapsedTime += deltaTime;


	mHoldImage->mTransform->Scale += Vector3::One * 0.1f * deltaTime;

	if (mElapsedTime >= mWaitTime)
	{
		mElapsedTime = 0.f;
		mbFired_P1 = false;
		mbFired_P2 = false;
		mHoldImage->mTransform->Scale = Vector3::One;
		mHoldImage->IsActive = false;
		mFireImage->mTransform->Scale = Vector3::One * 4.f;
		mFireImage->IsActive = true;
		mCurrentState = EGameState::OpenFire;
	}

	if (GameDataManager::IsOnlineMode == true)
	{
		mReadyStateTextUI->GetComponent<TextRenderer>()->SetText(L"���� Ű : SpaceBar\n������ : BackSpace");

		if (BTInputEvent::InputKeyDown(VK_SPACE))
		{
			// <Send> ���� �÷��̾ �̸� ������
			mResultElapsedTime = 0;
			mHoldImage->IsActive = false;
			mHoldImage->mTransform->Scale = Vector3::One;


			// �ʹ� ���� ���� -1
			StringPacket* strPacket = new StringPacket();
			sprintf_s(strPacket->buffer, sizeof(strPacket->buffer), "1,5, %f", -1.f);
			GameDataManager::mMGNetwork->SendPakcet(strPacket, GameDataManager::mMGNetwork->m_ServerList.at("Game"));
			mCurrentState = EGameState::Wait;
		}
	}
	else
	{
		// ���� ���
		mReadyStateTextUI->GetComponent<TextRenderer>()->SetText(L"1P Ű : SpaceBar\n2P Ű : Enter\n������ : BackSpace" );
		if (BTInputEvent::InputKeyDown(VK_SPACE))
		{
			mHoldImage->IsActive = false;
			mHoldImage->mTransform->Scale = Vector3::One;

			mWinPlayer = EWinPlayer::Player2;
			mCurrentState = EGameState::ShowResult;

			mResultElapsedTime = 0;
			return;
		}
		// ���� ���
		if (BTInputEvent::InputKeyDown(VK_RETURN))
		{
			mHoldImage->IsActive = false;
			mHoldImage->mTransform->Scale = Vector3::One;

			mWinPlayer = EWinPlayer::Player1;
			mCurrentState = EGameState::ShowResult;

			mResultElapsedTime = 0;
			return;
		}
	}

}

// �߻� ���� ���� �� ���� �� �Լ�
void GameManager::OpenFireFunc(float deltaTime)
{
	mElapsedTime += deltaTime;

	// Test��
	//mReadyStateTextUI->GetComponent<TextRenderer>()->SetText(L"Player 1 : %f\nPlayer 2 : %f", mFireTime_P1, mFireTime_P2);

	if (mFireImage->mTransform->Scale.x > 1.f)
	{
		mFireImage->mTransform->Scale -= Vector3::One * 25.f * deltaTime;
	}

	if (mElapsedTime >= LIMIT_TIME)
	{
		mFireImage->IsActive = false;
		mCurrentState = EGameState::End;
		return;
	}

	if (GameDataManager::IsOnlineMode == true)
	{
		if (BTInputEvent::InputKeyDown(VK_SPACE) && !mbFired_P1)
		{
			mbFired_P1 = true;
			mFireTime_P1 = mElapsedTime;
		}
	}
	else
	{
		// ���� ����
		if (BTInputEvent::InputKeyDown(VK_SPACE) && !mbFired_P1)
		{
			mbFired_P1 = true;
			mFireTime_P1 = mElapsedTime;
		}
		if (BTInputEvent::InputKeyDown(VK_RETURN) && !mbFired_P2)
		{
			mbFired_P2 = true;
			mFireTime_P2 = mElapsedTime;
		}
	}
}

// ��� ó�� �� ���� �� �Լ�
void GameManager::EndFunc()
{
	if (GameDataManager::IsOnlineMode == true)
	{
		// �¶��� ���
		if (mbFired_P1 == false)
		{
			// <Send> �߻� ���� ��� -1
			StringPacket* strPacket = new StringPacket();
			sprintf_s(strPacket->buffer, sizeof(strPacket->buffer), "1,5,%f", -1.f);
			GameDataManager::mMGNetwork->SendPakcet(strPacket, GameDataManager::mMGNetwork->m_ServerList.at("Game"));
			mCurrentState = EGameState::Wait;
		}
		else
		{
			// <Send> �߻� �ߴٰ� �ð� ��Ƽ� �������� (�ð� ��Ƽ� ������)
			StringPacket* strPacket = new StringPacket();
			sprintf_s(strPacket->buffer, sizeof(strPacket->buffer), "1,5,%f", mFireTime_P1);
			GameDataManager::mMGNetwork->SendPakcet(strPacket, GameDataManager::mMGNetwork->m_ServerList.at("Game"));
			mCurrentState = EGameState::Wait;
		}
	}
	else
	{
		// <Server> �������� ó���� �˷�����Ѵ�.
		// �Ѵ� �߻����� �ʾҰų�, ���� �ð��� �Ϻ��ϰ� ���� ��
		if (mbFired_P1 == false && mbFired_P2 == false
			|| mFireTime_P1 == mFireTime_P2)
		{
			// �Ѵ� �Է��� ���� ��� Draw
			mWinPlayer = EWinPlayer::Draw;
			mCurrentState = EGameState::ShowResult;
			mResultElapsedTime = 0;
			ResultImageOnOff();
			return;
		}
		// �� ���ϳ��� �Է� �߰� �ð��� ���� ���� �̱��.
		if (mFireTime_P1 < mFireTime_P2)
		{
			if (mbFired_P1 == false)
			{
				mWinPlayer = EWinPlayer::Player2;
			}
			else
			{
				mWinPlayer = EWinPlayer::Player1;
			}
		}
		else
		{
			if (mbFired_P2 == false)
			{
				mWinPlayer = EWinPlayer::Player1;
			}
			else
			{
				mWinPlayer = EWinPlayer::Player2;
			}
		}
		ResultImageOnOff();
		mCurrentState = EGameState::ShowResult;
		mResultElapsedTime = 0;
	}
}

void GameManager::ShowResultFunc(float deltaTime)
{
	// �ִϸ��̼� ����ϴ� ����

	mResultElapsedTime += deltaTime;

	// 1P �������� ������ ���.
	if (mWinPlayer == EWinPlayer::Player1)
	{
		//Win
		if (mWinImage->mTransform->Scale.x > 1.f)
		{
			mWinImage->mTransform->Scale -= Vector3::One * deltaTime * 10.f;
		}
	}
	else if (mWinPlayer == EWinPlayer::Player2)
	{
		// Defeated
		mDefeatedImage->mTransform->Position.y -= 20.f * deltaTime;
	}
	else
	{
		//draw
		mDrawImage->mTransform->Scale = Vector3::One * 1.3f;
	}

	if (mResultElapsedTime >= RESULT_TIME)
	{
		// ���� ���·� ����
		mReadyImage_1->gameObject->IsActive = mPlayerOn_1;
		mReadyImage_2->gameObject->IsActive = mPlayerOn_2;

		ResultImageOnOff(true);

		mCurrentState = EGameState::Ready;
	}

	if (GameDataManager::IsOnlineMode == true)
	{
		mReadyStateTextUI->GetComponent<TextRenderer>()->SetText(L"%s : %f\n%s : %f\nWinPlayer :%s",
			mP1_ID.c_str(), mFireTime_P1,
			mP2_ID.c_str(), mFireTime_P2,
			mWin_ID.c_str());
	}
	else
	{
		wstring winP;
		if (mWinPlayer == EWinPlayer::Player1)
		{
			winP = L"1P";
		}
		else if (mWinPlayer == EWinPlayer::Player2)
		{
			winP = L"2P";
		}
		else
		{
			winP = L"Draw";
		}
		mReadyStateTextUI->GetComponent<TextRenderer>()->SetText(L"1P : %f\n2P : %f\nWinPlayer : %s",
			mP1_ID.c_str(), mFireTime_P1,
			mP2_ID.c_str(), mFireTime_P2,
			winP.c_str());
	}
}

// ���� ���� �̹��� ��ü
void GameManager::RefreshReadyImage()
{
	if (mPlayerReady_1)
	{
		mReadyImage_1->SetSpriteByName(L"Ready_On");
	}
	else
	{
		mReadyImage_1->SetSpriteByName(L"Ready_Off");
	}
	if (mPlayerReady_2)
	{
		mReadyImage_2->SetSpriteByName(L"Ready_On");
	}
	else
	{
		mReadyImage_2->SetSpriteByName(L"Ready_Off");
	}
}

void GameManager::ResultImageOnOff(bool bAllOff)
{
	if (bAllOff)
	{
		mDefeatedImage->IsActive = false;
		mWinImage->IsActive = false;
		mDrawImage->IsActive = false;
		return;
	}

	if (mWinPlayer == EWinPlayer::Player1)
	{
		mDefeatedImage->IsActive = false;
		mWinImage->IsActive = true;
		mDrawImage->IsActive = false;
		mWinImage->mTransform->Scale = Vector3::One * 5.f;
	}
	else if (mWinPlayer == EWinPlayer::Player2)
	{
		mDefeatedImage->IsActive = true;
		mWinImage->IsActive = false;
		mDrawImage->IsActive = false;
		mDefeatedImage->mTransform->Position.y = 50.f;
	}
	else
	{
		mDefeatedImage->IsActive = false;
		mWinImage->IsActive = false;
		mDrawImage->IsActive = true;
	}
}

//Test ��

void GameManager::TestInput(float deltaTime)
{
	if (BTInputEvent::InputKeyDown(VK_F9))
	{
		mSceneCamera->mPosition = Vector3(12.59f, -4.918f, 14.5744f);
		mSceneCamera->LookAt(mSceneCamera->mPosition, mSceneCamera->mPosition + Vector3(-0.27f, 0.026f, 0.9597f), Vector3(0, 1.0f, 0));
		mSceneCamera->IsEditorCamera = !mSceneCamera->IsEditorCamera;
	}

	if (BTInputEvent::InputKeyDown(VK_F5))
	{
		static bool bP1 = true;
		bP1 = !bP1;
		if (bP1)
		{
			mControlPlayer = mPlayer1;
		}
		else
		{
			mControlPlayer = mPlayer2;
		}
	}
	if (BTInputEvent::InputKey(VK_I))
	{
		mControlPlayer->mTransform->Position +=
			Vector3::Forward * 5.f * deltaTime;
	}
	if (BTInputEvent::InputKey(VK_K))
	{
		mControlPlayer->mTransform->Position +=
			Vector3::Backward * 5.f * deltaTime;
	}
	if (BTInputEvent::InputKey(VK_J))
	{
		mControlPlayer->mTransform->Position +=
			Vector3::Left * 5.f * deltaTime;
	}
	if (BTInputEvent::InputKey(VK_L))
	{
		mControlPlayer->mTransform->Position +=
			Vector3::Right * 5.f * deltaTime;
	}
	if (BTInputEvent::InputKey(VK_U))
	{
		mControlPlayer->mTransform->Position +=
			Vector3::Down * 5.f * deltaTime;
	}
	if (BTInputEvent::InputKey(VK_O))
	{
		mControlPlayer->mTransform->Position +=
			Vector3::Up * 5.f * deltaTime;
	}
	if (BTInputEvent::InputKey(VK_N))
	{
		mControlPlayer->mTransform->Rotation.y -= 20.f * deltaTime;

	}
	if (BTInputEvent::InputKey(VK_M))
	{
		mControlPlayer->mTransform->Rotation.y += 20.f * deltaTime;
	}
	if (BTInputEvent::InputKey(VK_V))
	{
		mControlPlayer->mTransform->Scale += Vector3::One * 0.1f * deltaTime;

	}
	if (BTInputEvent::InputKey(VK_B))
	{
		mControlPlayer->mTransform->Scale -= Vector3::One * 0.1f * deltaTime;
	}
}


// �¶��� ����

void GameManager::Player_1_On(bool bOn)
{
	mPlayerOn_1 = bOn;
	mReadyImage_1->gameObject->IsActive = bOn;
	mPlayer1->IsActive = bOn;
}

void GameManager::Player_2_On(bool bOn)
{
	mPlayerOn_2 = bOn;
	mReadyImage_2->gameObject->IsActive = bOn;
	mPlayer2->IsActive = bOn;
}


// ���ú� �Լ�

void GameManager::Recieve_RoomUserState(string id1, string id2)
{
	mP1_ID.clear();
	mP2_ID.clear();
	Player_1_On(false);
	Player_2_On(false);
	if (id1.empty() == false)
	{
		mP1_ID.append(id1.begin(), id1.end());
		Player_1_On(true);
	}
	if (id2.empty() == false)
	{
		mP2_ID.append(id2.begin(), id2.end());
		Player_2_On(true);
	}
}

void GameManager::Recieve_OpponentExit()
{
	Player_2_On(false);
}

void GameManager::Recieve_ReadyState(string id1, int bReady)
{
	if (id1.empty() == true)
	{
		return;
	}

	wstring wId1;
	wId1.append(id1.begin(), id1.end());

	if (wId1 == GameDataManager::ID)
	{
		if (bReady == 0) { mPlayerReady_1 = false; }
		else { mPlayerReady_1 = true; }

	}
	else
	{
		if (bReady == 0) { mPlayerReady_2 = false; }
		else { mPlayerReady_2 = true; }
	}
}

void GameManager::Recieve_AllPlayerReady(float waitTime)
{
	mPlayerReady_1 = false;
	mPlayerReady_2 = false;
	mbFired_P1 = false;
	mbFired_P2 = false;
	mReadyImage_1->gameObject->IsActive = false;
	mReadyImage_2->gameObject->IsActive = false;

	// �̽����� mWaitTime�� �޾ƿ� �� �ֳ�?
	mWaitTime = waitTime;

	mCurrentState = EGameState::Start;
}

void GameManager::Recieve_GameResult(string win, string id1, float time1, string id2, float time2)
{
	wstring winner;
	winner.append(win.begin(), win.end());
	mWin_ID = winner;

	wstring wId1;
	wId1.append(id1.begin(), id1.end());

	wstring wId2;
	wId2.append(id2.begin(), id2.end());

	if (winner == L"x")
	{
		mWinPlayer = EWinPlayer::Draw;
	}
	else if (winner == GameDataManager::ID)
	{
		mWinPlayer = EWinPlayer::Player1;
	}
	else
	{
		mWinPlayer = EWinPlayer::Player2;
	}

	mFireTime_P1 = time1;
	mFireTime_P2 = time2;

	// ���� �簳�� (Ŭ�󿡼� �������� �뵵, DB�� ���� �������� �Ѵ�)
	switch (mWinPlayer)
	{
	case EWinPlayer::Draw:
		GameDataManager::DRAW += 1;
		break;
	case EWinPlayer::Player1:
		GameDataManager::WIN += 1;
		break;
	case EWinPlayer::Player2:
		GameDataManager::LOSE += 1;
		break;
	}

	mResultElapsedTime = 0;
	ResultImageOnOff();
	mCurrentState = EGameState::ShowResult;
}
