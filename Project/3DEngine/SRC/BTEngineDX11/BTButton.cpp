#include "pch.h" 
#include "BTButton.h"

BTButton::BTButton()
{
}

BTButton::~BTButton()
{
}

void BTButton::Init()
{
	mButtonImageRender = gameObject->GetComponent<SpriteRenderer>();
	if (mButtonImageRender == nullptr)
	{
		mButtonImageRender = gameObject->AddComponent<SpriteRenderer>();
	}
}

void BTButton::Start()
{
}

void BTButton::Update(float deltaTime)
{
	if (gameObject->IsActive == false)			{ return; }
	if (mButtonImageRender == nullptr)			{ return; }
	if (gameObject->mRectTransform == nullptr)	{ return; }

	mButtonImageRender->SetSprite(mBasicImg, false);

	RECT rect = gameObject->mRectTransform->GetRectSize();
	POINT screenPos = BTInput::GetMousePosCT();
	if (rect.left <= screenPos.x && screenPos.x <= rect.right &&
		rect.bottom >= screenPos.y && screenPos.y >= rect.top)
	{
		if (BTInput::InputKeyDown(VK_LBUTTON))
		{
			mButtonImageRender->SetSprite(mClickImg, false);

			// 우선 누르면 넘어가자
			if (OnClickEvent != nullptr)
			{
				OnClickEvent();
			}
		}
		else if (BTInput::InputKeyUp(VK_LBUTTON))
		{
			// 눌렀다가 그 위에서 뗏을때
			if (OnClickUpEvent != nullptr)
			{
				OnClickUpEvent();
			}
		}
		else if (BTInput::InputKey(VK_LBUTTON))
		{
			mButtonImageRender->SetSprite(mClickImg, false);
		}
		else if (mMouseOnImg != nullptr)
		{
			mButtonImageRender->SetSprite(mMouseOnImg, false);
		}

	}
}

void BTButton::SetBasicImgByName(std::wstring dataName)
{
	mBasicImg = ResourceManager::GetInstance()->GetTexture(dataName);
}

void BTButton::SetMouseOnImgByName(std::wstring dataName)
{
	mMouseOnImg = ResourceManager::GetInstance()->GetTexture(dataName);
}

void BTButton::SetClickImgByName(std::wstring dataName)
{
	mClickImg = ResourceManager::GetInstance()->GetTexture(dataName);
}

void BTButton::SetClickEvent(std::function<void()> func)
{
	OnClickEvent = func;
}

void BTButton::SetClickUpEvent(std::function<void()> func)
{
	OnClickUpEvent = func;
}
