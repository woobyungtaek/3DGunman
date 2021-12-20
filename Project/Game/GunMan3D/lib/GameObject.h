#pragma once

using namespace std;

class BTBehaviour;

class GameObject : public Object
{
public:
	GameObject();
	~GameObject();

public:
	bool IsActive;
	Transform* mTransform;
	RectTransform* mRectTransform;
	vector<BTBehaviour*> mStartComponents;
	vector<BTBehaviour*> mComponents;

public:
	void Start();
	void Update(float deltaTime);

public:
	template<typename T>
	T* GetComponent();

	template<typename T>
	T* AddComponent();

	template<typename T>
	bool RemoveComponent();
};

// ������Ʈ�� ȹ���մϴ�.(����� BTHaviour)
template<typename T>
inline T* GameObject::GetComponent()
{
	string key = string(typeid(T).name());

	int count = (int)mComponents.size();

	for (int index = 0; index < count; ++index)
	{
		T* temp = dynamic_cast<T*>(mComponents[index]);
		if (temp != nullptr)
		{
			return temp;
		}
	}

	return nullptr;
}

// ������Ʈ�� �߰�, ȹ���մϴ�.(����� BTHaviour)
template<typename T>
inline T* GameObject::AddComponent()
{
	string key = string(typeid(T).name());

	int count = (int)mComponents.size();
	for (int index = 0; index < count; ++index)
	{
		if (mComponents[index]->componentName == key)
		{
			// �ߺ� ������Ʈ�� �߰� �Ұ�
			return nullptr;
		}
	}

	T* instT = new T();

	BTBehaviour* tempBehaviour = static_cast<BTBehaviour*>(instT);
	tempBehaviour->componentName = key;
	tempBehaviour->gameObject = this;
	tempBehaviour->Init();

	mComponents.push_back(instT);
	mStartComponents.push_back(instT);

	return instT;
}

// ������Ʈ�� �����մϴ�.(����� BTHaviour)
template<typename T>
inline bool GameObject::RemoveComponent()
{
	string key = string(typeid(T).name());

	vector< BTBehaviour*>::iterator iter;

	for (iter = mComponents.begin(); iter != mComponents.end(); ++iter)
	{
		if ((*iter)->componentName == key)
		{
			mComponents.erase(iter);
			return true;
		}
	}
	return false;
}
