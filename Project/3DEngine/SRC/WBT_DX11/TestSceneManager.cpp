#include "pch.h"
#include "TestSceneManager.h"

TestSceneManager::TestSceneManager()
{
}

TestSceneManager::~TestSceneManager()
{
}

void TestSceneManager::Start()
{
	mTextObj = new GameObject();
	mTextObj->mTransform->Position = Vector3(100, 100, 0);
	mTextObj->AddComponent<TextRenderer>();
	mTextObj->GetComponent<TextRenderer>()->SetText(L"ÀÌ°Å ¶å´Ï±î?");

	mRotTestObj = new GameObject();
	mRotTestObj->AddComponent<MeshRenderer>()->SetMeshDatasByStr(L"Joe_Mesh");
	mRotTestObj->mTransform->Scale = Vector3(0.3f,0.3f, 0.3f);

	mEditBoxObj = new GameObject();
	mEditBoxObj->AddComponent<BTEditBox>();
	mEditBoxObj->mRectTransform->mRect = Vector4(0.1f, 0.7f, 0.3f, 0.3f);
	mEditBoxObj->GetComponent<BTEditBox>()->SetSize(2);
	mEditBoxObj->GetComponent<BTEditBox>()->SetColor(Vector4(0,1,1,1));
	mEditBoxObj->GetComponent<BTEditBox>()->SetInputLimit(15);

	mEditBoxObj2 = new GameObject();
	mEditBoxObj2->AddComponent<BTEditBox>();
	mEditBoxObj2->mRectTransform->mRect = Vector4(0.7f, 0.1f, 0.3f, 0.3f);
	mEditBoxObj2->GetComponent<BTEditBox>()->SetSize(2);
	mEditBoxObj2->GetComponent<BTEditBox>()->SetColor(Vector4(0, 1, 1, 1));
	mEditBoxObj2->GetComponent<BTEditBox>()->SetInputLimit(15);
	mEditBoxObj2->GetComponent<BTEditBox>()->SetPadding(Vector2(0,100));
}

void TestSceneManager::Update(float deltaTime)
{
	mTextObj->mTransform->Position += Vector3(5 * deltaTime, 0, 0);
	if (mTextObj->mTransform->Position.x >= 1920)
	{
		mTextObj->mTransform->Position = Vector3::Zero;
	}

	if (BTInputEvent::InputKeyDown('L'))
	{
		mTextObj->IsActive = !mTextObj->IsActive;
	}
	if (BTInputEvent::InputKey('T'))
	{
		mRotTestObj->mTransform->Rotation.x += 10.f * deltaTime;
	}
	if (BTInputEvent::InputKey('G'))
	{
		mRotTestObj->mTransform->Rotation.x -= 10.f * deltaTime;
	}
	if (BTInputEvent::InputKey('H'))
	{
		mRotTestObj->mTransform->Rotation.z -= 10.f * deltaTime;
	}
	if (BTInputEvent::InputKey('F'))
	{
		mRotTestObj->mTransform->Rotation.z += 10.f * deltaTime;
	}
	if (BTInputEvent::InputKey('R'))
	{
		mEditBoxObj->GetComponent<BTEditBox>()->SetInputLimit(-1);
	}
	if (BTInputEvent::InputKey('Y'))
	{
		mEditBoxObj->GetComponent<BTEditBox>()->SetInputLimit(5);
	}
	if (BTInputEvent::InputKey('I'))
	{
		mEditBoxObj->GetComponent<BTEditBox>()->SetPasswordMode(true);
	}
}
