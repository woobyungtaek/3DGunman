#pragma once
class LoginSceneManager : public BTBehaviour
{
public:
	LoginSceneManager();
	virtual ~LoginSceneManager();

public:
	virtual void Init();
	virtual void Start();
	virtual void Update(float deltaTime);

	void PopupSignUp();

	void OnLoginButtonClicked();
	void OnLocalButtonClicked();

	void OnSignUpButtonClicked();
	void OnSignUpOkButtonClicked();
	void OnSignUpCancelButtonClicked();


public:
	TextRenderer* GetLoginLog();
	TextRenderer* GetSignupLog();

private:
	GameObject* mBackground;

	// �α��� �˾�
	GameObject* mLoginPopupBack;
	GameObject* mLoginBtn;
	GameObject* mLocalBtn;
	GameObject* mSignInBtn;

	GameObject* mIdTextImg;
	GameObject* mPasswordTextImg;

	GameObject* mIdEditBox;
	GameObject* mPasswordEditBox;

	GameObject* mLoginPopupLog;
	GameObject* mSignupPopupLog;

	// ȸ�� ���� �˾�
	GameObject* mDimmed;
	GameObject* mSignUpPopupBack;

	GameObject* mIdEditBox_signup;		// 0 ���̵�
	GameObject* mPasswordEdit_signup;	// 1 ��й�ȣ
	GameObject* mConfirmEdit_signup;	// 2 ��й�ȣ Ȯ��

	GameObject* mIdImg_signup;
	GameObject* mPasswordImg_signup;
	GameObject* mConfirmImg_signup;

	GameObject* mSignupPopup_OK_Btn;
	GameObject* mSignupPopup_Cancel_Btn;

	vector<BTEditBox*> mSingupEditBoxVec;

	vector<SpriteRenderer*> mLoginSceneUIVec;

	string		mMGServerIP;
};

