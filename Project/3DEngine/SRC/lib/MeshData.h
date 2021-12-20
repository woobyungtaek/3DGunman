#pragma once
/*
	VertexTex�� ���� > ���� ū ����
	VB, IB�� ���鶧 �ʿ��� �κи� ��
*/
using namespace BTStruct;

template<typename T>
class MeshData
{
public:
	vector<T>	 mVertices;
	vector<UINT> mIndices;

	bool mbSkin;

	int mVertexCount;
	int mFaceCount;
	int mIndexCount;

	int mMaterialRef;

	UINT mStride;

public:
	int GetByteWidth_VB()
	{
		return mStride * mVertexCount;
	}

	int GetByteWidth_IB()
	{
		return sizeof(int) * mIndexCount;
	}
};

