#pragma once
class GameObject;

class BTBehaviour : public Object
{
public:
	BTBehaviour() { }
	virtual ~BTBehaviour() {}

public:	
	GameObject* gameObject; // ���� �����ִ� GameObject

	std::string componentName; // Component �̸�

public:
	virtual void Init() {};
	virtual void Start() {};
	virtual void Update(float deltaTime) {};
};

