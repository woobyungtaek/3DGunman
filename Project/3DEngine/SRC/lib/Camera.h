#pragma once

class ModelData;
class ModelDataSet;

class Camera
{
public:
	Camera();
	~Camera();

	void SetLens(float fovY, float aspect, float zn, float zf);
	void SetLens(float fovY, float width, float height, float nearZ, float farZ);

	void LookAt(Vector3 pos, Vector3 target, Vector3 worldUP);

	void Walk	(float deltaTime);
	void Strafe	(float deltaTime);
	void UpDown	(float deltaTime);

	void Pitch(float angle);
	void RotateY(float angle);

	void CameraMove(float deltaTime);
	void CameraRotate(float x, float y, float lastX, float lastY);

	void UpdateViewMatrix();
	void Render();

//Get, Set �Լ���
public: 
	Matrix GetView();
	Matrix GetProj();

	Vector3 GetPos();
	Vector3 GetLook();

	float GetPitchAngle();
	float GetRotYAngle();

	float GetNearHeight();
	float GetFarHeight();
	
	void SetProj(Matrix proj);
	void SetCubeMap(wstring name);


public:
	bool IsEditorCamera;

	Vector3 mPosition; //��ġ

	// ���� angle
	float mAddXAngle;
	float mAddYAngle;

private:
	POINT mLastMousePos;

	Vector2 mScreenSize;

	Vector3 mRight;		//������
	Vector3 mUp;		//��
	Vector3 mLook;		//�ü�

	float mPitchAngle;
	float mRotYAngle;


	// �������� ���
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;

	// �� �����̽�(???)
	float mNearWindowHeight; 
	float mFarWindowHeight;

	Matrix mView;
	Matrix mProj;

	// SkyBox ������
	UINT mOffset;
	ModelDataSet* mSkySphere;

	ID3D11ShaderResourceView* mCubeSRV;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RasterizerState* mpRenderState;
	ID3D11InputLayout* mInputLayout;
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopo;
};

