#include "pch.h"
#include "ModelData.h"

ModelData::ModelData() :
	mbNoBuffer(false),
	VB(nullptr), IB(nullptr),
	IndexCount(0), Stride(0),
	mParentMeshData(nullptr)
{
}

ModelData::~ModelData()
{
}

ModelData* ModelData::GetCopy()
{
	ModelData* copyData  = new ModelData();

	copyData->mbNoBuffer = mbNoBuffer;
	copyData->mbSkin = mbSkin;

	copyData->mNodeName		  = mNodeName;
	copyData->mParentNodeName = mParentNodeName;

	copyData->VB = VB;
	copyData->IB = IB;

	copyData->VertexCount	= VertexCount;
	copyData->FaceCount		= FaceCount;
	copyData->IndexCount	= IndexCount;
	copyData->Stride		= Stride;

	copyData->mLocalMatrix = mLocalMatrix;
	copyData->mNodeMatrix = mNodeMatrix;

	copyData->mBoneList = mBoneList;

	copyData->mMaterial = mMaterial;

	return copyData;
}

// TM ����
void ModelData::SetLocalMatrix()
{
	if (mParentMeshData != nullptr)
	{
		mLocalMatrix = mWorldMatrix * mParentMeshData->GetInvertWorldMatrix();
	}
	else
	{
		mLocalMatrix = mWorldMatrix;
	}
}

void ModelData::SetAnimationMatrix(Animation* aniData, int tickFrame)
{
	mUseMatrix = mLocalMatrix;

	// �ʿ� ���� ���� ���� 
	int currentIdx;
	int nextIdx;

	Vector3		s;
	Quaternion	r;
	Vector3		t;

	Vector3		cPos;
	Quaternion	cRot;

	// ��� ����
	mUseMatrix.Decompose(s, r, t);

#pragma region ������ ���ϱ�
	if (aniData->m_position.size() > 0)
	{
		// �ε����� 0���� ���������� �⺻������ 1�� ���ְ� ����� ���̱� ������
		currentIdx = -1;
		nextIdx = -1;

		for (int index = 0; index < aniData->m_position.size(); ++index)
		{
			// ���� ƽ�������� �ִϸ��̼� Ű������ ���� ������
			if (tickFrame < aniData->m_position[index]->m_time)
			{
				currentIdx = index - 1;
				nextIdx = index;
				break;
			}
		}

		// �ʱⰪ�� -1�̶� ������ ���� ��� ( ������ ���� = ���� ������ �����Ӻ��� ũ�� )
		if (nextIdx + currentIdx < 0)
		{
			currentIdx = (int)aniData->m_position.size() - 1;
			cPos = aniData->m_position[currentIdx]->m_pos;
		}
		else
		{
			//TRACE("Cur : %d / Next : %d \n", currentIdx, nextIdx);
			// ������ 0 ���� 1���̰� ���ؾ���
			float cTime = (float)aniData->m_position[currentIdx]->m_time;
			float nTime = (float)aniData->m_position[nextIdx]->m_time;

			// (�и�)���� - �α� / (����) �α� - ���� 	
			float lerpVal = ((float)tickFrame - cTime) / (nTime - cTime);

			// lerp�� ����
			cPos = Vector3::Lerp(
				aniData->m_position[currentIdx]->m_pos,
				aniData->m_position[nextIdx]->m_pos,
				lerpVal);
		}

		t = cPos;
	}
#pragma endregion

#pragma region ȸ�� ���ϱ�
	if (aniData->m_rotation.size() > 0)
	{
		currentIdx = -1;
		nextIdx = -1;
		for (int index = 0; index < aniData->m_rotation.size(); index++)
		{
			// ���� ƽ�������� �ִϸ��̼� Ű������ ���� ������
			if (tickFrame < aniData->m_rotation[index]->m_time)
			{
				currentIdx = index - 1;
				nextIdx = index;
				break;
			}
			// ������ 0 ���� 1���̰� ���ؾ���
			// (�и�)���� - 2880 (����) 3880 - 2880 > ����� ���� ���������� ����ؾ��� 
			// Slerp�� ����
			// �̸� �����̼� ���� �����ؼ� ������ ������ ��ü
			// �ƴ϶�� ���� (���ϱ�)
		}
		if (nextIdx + currentIdx < 0)
		{
			currentIdx = (int)aniData->m_rotation.size() - 1;
			cRot = aniData->m_rotation[currentIdx]->m_rotQT_accumulation;
		}
		else
		{
			// ������ 0 ���� 1���̰� ���ؾ���
			float cTime = (float)aniData->m_rotation[currentIdx]->m_time;
			float nTime = (float)aniData->m_rotation[nextIdx]->m_time;
			// (�и�)���� - �α� / (����) �α� - ���� 	
			float lerpVal = ((float)tickFrame - cTime) / (nTime - cTime);
			// lerp�� ����
			Quaternion::Slerp(
				aniData->m_rotation[currentIdx]->m_rotQT_accumulation,
				aniData->m_rotation[nextIdx]->m_rotQT_accumulation,
				lerpVal, cRot);
		}
		r = cRot;
	}
#pragma endregion

	mUseMatrix = Matrix::CreateScale(s) * Matrix::CreateFromQuaternion(r) * Matrix::CreateTranslation(t);
}

void ModelData::SetBoneMatrix()
{
	mFinalBoneTMVec.clear();
	Bone* tempBone;

	for (int index = 0; index < (int)mBoneList.size(); ++index)
	{
		tempBone = &mBoneList[index];
		Matrix boneWorldTM = *tempBone->m_boneTM_WorldTM;	//�ִϸ��̼� ���� WorldTM
		Matrix boneNodeTM  = *tempBone->m_boneTM_NodeTM;	//ó�� NodeTM

		Matrix skinWorldTM = XMLoadFloat4x4(&mWorldMatrix);
		Matrix skinWorldTM_Inverse = XMMatrixInverse(nullptr, skinWorldTM);

		Matrix boneOffsetTM = boneNodeTM * skinWorldTM_Inverse;
		Matrix boneOffsetTM_Inverse = boneOffsetTM.Invert();

		Matrix finalBoneTM = boneOffsetTM_Inverse * boneWorldTM;

		XMFLOAT4X4 finalTM;
		XMStoreFloat4x4(&finalTM, finalBoneTM);
		mFinalBoneTMVec.push_back(finalBoneTM);
	}
}

// TM ���
Matrix ModelData::GetInvertWorldMatrix()
{
	return mWorldMatrix.Invert();
}

Matrix ModelData::GetWorldMatrix()
{
	if (mParentMeshData == nullptr)
	{
		return mLocalMatrix;
	}
	return mLocalMatrix * (mParentMeshData->GetWorldMatrix());
}

Matrix ModelData::GetUseMatrixTM()
{
	if (mParentMeshData == nullptr)
	{
		return mUseMatrix;
	}
	return mUseMatrix * mParentMeshData->GetUseMatrixTM();
}

void ModelData::SetEffectAndInputLayout(Effect** refEffect, ID3D11InputLayout** refInputLayout)
{
	if (mBoneList.size() <= 0)
	{
		if (mMaterial->mNormalMap != nullptr)
		{
			*refInputLayout = InputLayouts::NormalMap;
			*refEffect = Effects::NormalFX;
			return;
		}
	}
	else
	{
		*refInputLayout = InputLayouts::Skin;
		*refEffect = Effects::SkinFX;
		return;
	}

	*refInputLayout = InputLayouts::Basic32;
	*refEffect = Effects::BasicFX;
}
