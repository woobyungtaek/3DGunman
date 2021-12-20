#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
	mTransform = new Transform();
	Object::Scene->mGameObjectVec.push_back(this);
	IsActive = true;
}

GameObject::~GameObject()
{
}

void GameObject::Start()
{
	int count = (int)mStartComponents.size();
	while ((int)mStartComponents.size() > 0)
	{
		mStartComponents[0]->Start();
		mStartComponents.erase(mStartComponents.begin());
	}
}

void GameObject::Update(float deltaTime)
{
	int count = (int)mComponents.size();
	for (int index = 0; index < count; ++index)
	{
		mComponents[index]->Update(deltaTime);
	}
}
