#pragma once

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

class ModelData
{
public:
	ModelData();
	~ModelData();

public:
	// ���� �ڿ�
	ID3D11Buffer* VB;
	ID3D11Buffer* IB;

	vector<Bone> mBoneList;

	// �� ���� �ڿ�
	bool mbNoBuffer;
	bool mbSkin;

	int mMaterialRef;

	wstring mNodeName;
	wstring mParentNodeName;

	UINT VertexCount;
	UINT FaceCount;
	UINT IndexCount;
	UINT Stride;

	// �θ� ���� �ʿ� ��ĵ�
	ModelData* mParentMeshData;
	Matrix mNodeMatrix;		// �����Ǹ� �ȵǴ� TM (���)
	Matrix mLocalMatrix;	// �����Ǹ� �ȵǴ� TM (�籸���� ����)
	Matrix mWorldMatrix;	// 
	Matrix mUseMatrix;		// �������� ���� TM

	vector<XMFLOAT4X4> mFinalBoneTMVec;

	// Material Data
	BTMaterial* mMaterial;

public:
	ModelData* GetCopy();

	void SetLocalMatrix();
	void SetAnimationMatrix(Animation* aniData, int tickFrame);
	void SetBoneMatrix();

	Matrix GetInvertWorldMatrix();
	Matrix GetWorldMatrix();
	Matrix GetUseMatrixTM();

	void SetEffectAndInputLayout(Effect** refEffect, ID3D11InputLayout** refInputLayout);
};