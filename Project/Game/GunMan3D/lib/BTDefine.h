#pragma once

class IScene
{
public:
	IScene() {};

public:
	// �߻� �޼���
	virtual void Start() = 0;

public:
	Camera* mActiveCamera;
};