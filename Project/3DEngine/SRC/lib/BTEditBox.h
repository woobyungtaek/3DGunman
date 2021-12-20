#pragma once
class BTEditBox : public Renderer
{
public:
	BTEditBox();
	~BTEditBox();
public:
	// ������ �Լ�
	virtual void Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Camera* pCamera) override;

	static void SetFocusEditBox(BTEditBox* editBox);

public:
	static BTEditBox* FocusEditBox; // ��� �����Ǿ���ϴ� EditBox
	static BTEditBox* FirstFocus;

	SpriteRenderer* mEditBoxImageRender;
private:

	ID3D11ShaderResourceView* mBackImg;

	// �ؽ�Ʈ
	std::wstring	mText;

	// �ؽ�Ʈ �Ӽ�
	float			mSize;
	Vector4			mColor;
	Vector2			mPadding;

	// ����� �Է� ������ ��
	float			mInputDelay;

	// �Է� �Ӽ� ��
	int				mInputLimit;	// �ִ� �Է� �� -1�� ������
	bool			mbUseNewLine;	// Enter�� �̿��� New Line ���
	bool			mbPassword;		// *�� ǥ���մϴ�.

public:
	void SetBackImageByName(std::wstring dataName);

	void SetColor(Vector4 color);
	void SetSize(float size);
	void SetPadding(Vector2 padding);

	void SetInputLimit(int count);
	void SetAcceptNewLine(bool bAccept);
	void SetPasswordMode(bool bPassword);

	void ClearText();

	std::wstring GetText();

private:
	void InputCheck(float deltaTime);
	int  GetInputChar(int ch);
};

