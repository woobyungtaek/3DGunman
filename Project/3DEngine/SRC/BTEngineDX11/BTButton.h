#pragma once
class BTButton : public BTBehaviour
{
public:
	BTButton();
	~BTButton();

public:
	// ������ �Լ�
	virtual void Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;

	// Set �Լ�
	void SetBasicImgByName	(std::wstring dataName);
	void SetMouseOnImgByName(std::wstring dataName);
	void SetClickImgByName	(std::wstring dataName);

	void SetClickEvent		(std::function<void()> func);
	void SetClickUpEvent	(std::function<void()> func);

public:
	std::function<void()> OnClickEvent;
	std::function<void()> OnClickUpEvent;

private:
	SpriteRenderer* mButtonImageRender;
	
	ID3D11ShaderResourceView* mBasicImg;
	ID3D11ShaderResourceView* mMouseOnImg;
	ID3D11ShaderResourceView* mClickImg;
};