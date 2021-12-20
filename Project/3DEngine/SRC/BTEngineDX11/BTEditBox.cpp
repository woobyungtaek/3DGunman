#include "pch.h"
#include "BTEditBox.h"

BTEditBox* BTEditBox::FocusEditBox;
BTEditBox* BTEditBox::FirstFocus;

BTEditBox::BTEditBox()
{
	mColor = Vector4(0, 0, 0, 1);
	mPadding = Vector2(0, 0);
	mSize = 1;
	mbUseNewLine = false;
	mInputLimit = -1; // 무제한
}

BTEditBox::~BTEditBox()
{
}

void BTEditBox::Init()
{
	mEditBoxImageRender = gameObject->GetComponent<SpriteRenderer>();
	if (mEditBoxImageRender == nullptr)
	{
		mEditBoxImageRender = gameObject->AddComponent<SpriteRenderer>();
		this->SetSortOrder(mEditBoxImageRender->mSortOrder +10);
		this->SetSortOrder(mEditBoxImageRender->mSortOrder);
	}
}

void BTEditBox::Start()
{
}

void BTEditBox::Update(float deltaTime)
{
	if (gameObject->mRectTransform == nullptr) { return; }

	RECT rect = gameObject->mRectTransform->GetRectSize();
	POINT screenPos = BTInput::GetMousePosCT();
	if (BTInput::InputKey(VK_LBUTTON))
	{
		if (rect.left <= screenPos.x && screenPos.x <= rect.right &&
			rect.bottom >= screenPos.y && screenPos.y >= rect.top)
		{
			if (FirstFocus == nullptr)
			{
				FirstFocus = this;
			}
		}
		SetFocusEditBox(FirstFocus);
	}
	InputCheck(deltaTime);
}

void BTEditBox::Render(Camera* pCamera)
{
	RECT rect = gameObject->mRectTransform->GetRectSize();
	Vector2 pos;
	pos.x = rect.left;
	pos.y = rect.top;
	pos += mPadding;

	FirstFocus = nullptr;

	DXTKFont* mFont = BTGameEngine::GetInstance()->GetRender()->GetFont();
	float rot = gameObject->mTransform->Rotation.z / MathHelper::Rad;

	if (mbPassword)
	{
		wstring esStr = L"";
		for (int count = 0; count < (int)mText.size(); ++count)
		{
			esStr += '*';
		}
		mFont->DrawTextColor(pos.x, pos.y, rot, mSize, mColor, (TCHAR*)esStr.c_str());
		return;
	}
	mFont->DrawTextColor(pos.x, pos.y, rot, mSize, mColor, (TCHAR*)mText.c_str());

}

void BTEditBox::SetFocusEditBox(BTEditBox* editBox)
{
	FocusEditBox = editBox;
}

void BTEditBox::SetBackImageByName(std::wstring dataName)
{
	mBackImg = ResourceManager::GetInstance()->GetTexture(dataName);
	mEditBoxImageRender->SetSprite(mBackImg, false);
}

void BTEditBox::InputCheck(float deltaTime)
{
	if (gameObject->IsActive == false) { return; }
	if (FocusEditBox == nullptr) { return; }
	if (FocusEditBox != this) { return; }

#pragma region 기타

	// 입력 문자 지우기
	if (BTInputEvent::InputKeyDown(VK_BACK))
	{
		if (mText.empty() == true) { return; }
		mInputDelay = 0;
		mText.erase(mText.end() - 1);
		return;
	}
	if (BTInputEvent::InputKey(VK_BACK))
	{
		if (mText.empty() == true) { return; }

		mInputDelay += deltaTime;
		if (mInputDelay < 0.25f) { return; }
		mInputDelay = 0;

		mText.erase(mText.end() - 1);
		return;
	}

#pragma endregion

	// 입력 수 제한
	if (mInputLimit >= 0)
	{
		if ((int)mText.size() >= mInputLimit)
		{
			return;
		}
	}


#pragma region 알파벳

	if (BTInputEvent::InputKeyDown('Q'))
	{
		mText += GetInputChar(L'q');
		return;
	}
	if (BTInputEvent::InputKeyDown('W'))
	{
		mText += GetInputChar(L'w');
		return;
	}
	if (BTInputEvent::InputKeyDown('E'))
	{
		mText += GetInputChar(L'e');
		return;
	}
	if (BTInputEvent::InputKeyDown('R'))
	{
		mText += GetInputChar(L'r');
		return;
	}
	if (BTInputEvent::InputKeyDown('T'))
	{
		mText += GetInputChar(L't');
		return;
	}
	if (BTInputEvent::InputKeyDown('Y'))
	{
		mText += GetInputChar(L'y');
		return;
	}
	if (BTInputEvent::InputKeyDown('U'))
	{
		mText += GetInputChar(L'u');
		return;
	}
	if (BTInputEvent::InputKeyDown('I'))
	{
		mText += GetInputChar(L'i');
		return;
	}
	if (BTInputEvent::InputKeyDown('O'))
	{
		mText += GetInputChar(L'o');
		return;
	}
	if (BTInputEvent::InputKeyDown('P'))
	{
		mText += GetInputChar(L'p');
		return;
	}
	if (BTInputEvent::InputKeyDown('A'))
	{
		mText += GetInputChar(L'a');
		return;
	}
	if (BTInputEvent::InputKeyDown('S'))
	{
		mText += GetInputChar(L's');
		return;
	}
	if (BTInputEvent::InputKeyDown('D'))
	{
		mText += GetInputChar(L'd');
		return;
	}
	if (BTInputEvent::InputKeyDown('F'))
	{
		mText += GetInputChar(L'f');
		return;
	}
	if (BTInputEvent::InputKeyDown('G'))
	{
		mText += GetInputChar(L'g');
		return;
	}
	if (BTInputEvent::InputKeyDown('H'))
	{
		mText += GetInputChar(L'h');
		return;
	}
	if (BTInputEvent::InputKeyDown('J'))
	{
		mText += GetInputChar(L'j');
		return;
	}
	if (BTInputEvent::InputKeyDown('K'))
	{
		mText += GetInputChar(L'k');
		return;
	}
	if (BTInputEvent::InputKeyDown('L'))
	{
		mText += GetInputChar(L'l');
		return;
	}
	if (BTInputEvent::InputKeyDown('Z'))
	{
		mText += GetInputChar(L'z');
		return;
	}
	if (BTInputEvent::InputKeyDown('X'))
	{
		mText += GetInputChar(L'x');
		return;
	}
	if (BTInputEvent::InputKeyDown('C'))
	{
		mText += GetInputChar(L'c');
		return;
	}
	if (BTInputEvent::InputKeyDown('V'))
	{
		mText += GetInputChar(L'v');
		return;
	}
	if (BTInputEvent::InputKeyDown('B'))
	{
		mText += GetInputChar(L'b');
		return;
	}
	if (BTInputEvent::InputKeyDown('N'))
	{
		mText += GetInputChar(L'n');
		return;
	}
	if (BTInputEvent::InputKeyDown('M'))
	{
		mText += GetInputChar(L'm');
		return;
	}

#pragma endregion

#pragma region 숫자
	if (BTInputEvent::InputKeyDown('0') || BTInputEvent::InputKeyDown(VK_NUMPAD0))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += ')';
		}
		else
		{
			mText += '0';
		}

		return;
	}
	if (BTInputEvent::InputKeyDown('1') || BTInputEvent::InputKeyDown(VK_NUMPAD1))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '!';
		}
		else
		{
			mText += '1';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown('2') || BTInputEvent::InputKeyDown(VK_NUMPAD2))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '@';
		}
		else
		{
			mText += '2';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown('3') || BTInputEvent::InputKeyDown(VK_NUMPAD3))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '#';
		}
		else
		{
			mText += '3';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown('4') || BTInputEvent::InputKeyDown(VK_NUMPAD4))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '$';
		}
		else
		{
			mText += '4';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown('5') || BTInputEvent::InputKeyDown(VK_NUMPAD5))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '%';
		}
		else
		{
			mText += '5';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown('6') || BTInputEvent::InputKeyDown(VK_NUMPAD6))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '^';
		}
		else
		{
			mText += '6';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown('7') || BTInputEvent::InputKeyDown(VK_NUMPAD7))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '&';
		}
		else
		{
			mText += '7';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown('8') || BTInputEvent::InputKeyDown(VK_NUMPAD8))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '*';
		}
		else
		{
			mText += '8';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown('9') || BTInputEvent::InputKeyDown(VK_NUMPAD9))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '(';
		}
		else
		{
			mText += '9';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown(VK_OEM_MINUS) || BTInputEvent::InputKeyDown(VK_NUMPAD9))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '_';
		}
		else
		{
			//mText += '-';
		}
		return;
	}
	if (BTInputEvent::InputKeyDown(VK_OEM_PLUS) || BTInputEvent::InputKeyDown(VK_NUMPAD9))
	{
		if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
		{
			//mText += '+';
		}
		else
		{
			//mText += '=';
		}
		return;
	}
#pragma endregion

#pragma region 기타 2

	// 뉴라인
	if (mbUseNewLine && BTInputEvent::InputKeyDown(VK_RETURN))
	{
		mText += '\n';
		return;
	}

#pragma endregion

}

void BTEditBox::SetColor(Vector4 color)
{
	mColor = color;
}

void BTEditBox::SetSize(float size)
{
	mSize = size;
}

void BTEditBox::SetPadding(Vector2 padding)
{
	mPadding = padding;
}

void BTEditBox::SetInputLimit(int count)
{
	mInputLimit = count;

	if (mInputLimit < 0) { return; }

	if ((int)mText.size() > count)
	{
		mText.erase(mText.begin() + count, mText.end());
	}
}

void BTEditBox::SetAcceptNewLine(bool bAccept)
{
	mbUseNewLine = bAccept;
}

void BTEditBox::SetPasswordMode(bool bPassword)
{
	mbPassword = bPassword;
}

void BTEditBox::ClearText()
{
	mText = L"";
}

std::wstring BTEditBox::GetText()
{
	return mText;
}

int BTEditBox::GetInputChar(int ch)
{
	if (BTInputEvent::InputKey(VK_LSHIFT) || BTInputEvent::InputKey(VK_RSHIFT))
	{
		ch -= 32;
	}
	return ch;
}