#pragma once

using namespace SimpleMath;

class ModelData;
class ModelDataSet;

class Renderer : public BTBehaviour
{
public:
	Renderer();
	virtual ~Renderer();

public:
	// ���� ������ ����
	virtual void Render(Camera* pCamera) abstract;

	// �������� ����
	virtual void Update(float deltaTime) override;

	void UpdateRenderer(Camera* pCamera);
	void SettingBuffers(ModelData* meshData);
	void SetMeshDatasByStr(wstring dataName);

	void CommonSetting();

	void SetSortOrder(int order);

public:
	int mSortOrder;

	ModelDataSet* mModelDataSet;	// ���� MeshDatas

protected:
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RasterizerState* mpRenderState;	// ���� ����

	Effect* mEffect;

	UINT mOffset;
	ID3D11InputLayout* mInputLayout;	// ��ǲ ���̾ƿ�

	D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopo;

	// ���
	Matrix mWorld;		// ���� ��ȯ ���
	Matrix mView;		// �þ� ��ȯ ���
	Matrix mProj;		// ���� ��ȯ ���

	Vector3 mEyePos;
};