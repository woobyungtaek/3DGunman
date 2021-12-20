#pragma once
class TestSceneManager : public BTBehaviour
{
public:
	TestSceneManager();
	virtual ~TestSceneManager();

public:
	virtual void Start();
	virtual void Update(float deltaTime);

private:
	GameObject* mTextObj;
	GameObject* mRotTestObj;

	GameObject* mEditBoxObj;
	GameObject* mEditBoxObj2;
};

