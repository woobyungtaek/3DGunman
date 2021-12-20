#include "pch.h"
#include "Animator.h"

Animator::Animator() :
	mFrame(0.0f), mFrameIdx(0),
	mDelay(0.0f), mTempDelay(0.0f),
	mbPlaying(false), mbPause(false), mbLoop(false),
	mpCurrentRenderer(nullptr), mpCurrentClip(nullptr)

{
}

void Animator::Start()
{
}

void Animator::Update(float deltaTime)
{
	if (mbPlaying == false) { return; }	// ��� ���°� �ƴ� 

	if (mDelay > 0)	// ���ð��� ����
	{
		mTempDelay += deltaTime;
		if (mTempDelay < mDelay) { return; }
	}

	if (mpCurrentClip == nullptr) { return; } // Ŭ���� ����
	if (mpCurrentRenderer == nullptr)		  // �������� ����
	{
		mpCurrentRenderer = gameObject->GetComponent<Renderer>();
		if (mpCurrentRenderer == nullptr) { return; }
	}

	// ���� ���°� �ƴ� �� Frame ���
	if (mbPause == false) 
	{
		// Frame / FrameIdx ���
		mFrame += (deltaTime * 1000);
		if (mFrame > mpCurrentClip->mSceneData.m_framespeed)
		{
			mFrame = 0;
			mFrameIdx += 1;

			if (mFrameIdx > mpCurrentClip->mSceneData.m_lastframe)
			{
				if (mbLoop)
				{
					// �ݺ� ���
					mFrameIdx = 0;
				}
				else
				{
					// �ִϸ��̼� ����
					Stop();
				}
			}
		}
	}

	// tickFrame ���
	int tickFrame = mFrameIdx * mpCurrentClip->mSceneData.m_ticksperframe;

	// MeshData�� LocalTM�� �̿��� �� ��� TM�� �����Ѵ�.
	ModelDataSet* meshDatas = mpCurrentRenderer->mModelDataSet;

	int count = (int)meshDatas->mModelDataVec.size();
	for (int index = 0; index < count; ++index)
	{
		// ���� �� MeshData
		ModelData* instMeshData = meshDatas->mModelDataVec[index];

		// ������ AniData �˻�
		if (mpCurrentClip->mAnimationMap.find(instMeshData->mNodeName)
			== mpCurrentClip->mAnimationMap.end())
		{
			continue;
		}

		// Ani�����Ϳ� Renderer�� �ǻ�� Matrix���� �����Ƿ� �ִϸ��̼� ����
		Animation* instAniData = mpCurrentClip->mAnimationMap[instMeshData->mNodeName];

		instMeshData->SetAnimationMatrix(instAniData, tickFrame);
	}

	// �������� WorldMatrix�� ����
	meshDatas->UpdateWorldMatrix();
}

// Clip �߰�

void Animator::AddAnimationClipByStr(wstring clipName)
{
	AnimationClip* animationClip = ResourceManager::GetInstance()->GetAniClipData(clipName);

	mAnimationClipVec.push_back(animationClip);
}


// Clip ���� 

void Animator::SetAnimationClipByStr(wstring clipName)
{
	int count = (int)mAnimationClipVec.size();
	for (int index = 0; index < count; ++index)
	{
		if (clipName.compare(mAnimationClipVec[index]->mClipName))
		{
			mpCurrentClip = mAnimationClipVec[index];
			return;
		}
	}

	mpCurrentClip = nullptr;
}

void Animator::SetAnimationClipByIdx(int index)
{
	int count = (int)mAnimationClipVec.size();
	if (index > count && index < 0)
	{
		mpCurrentClip = nullptr;
		return;
	}

	mpCurrentClip = mAnimationClipVec[index];
}


// ���

void Animator::Play(bool isLoop)
{
	if (mbPlaying == true) { return; }
	mbLoop = isLoop;

	mFrame = 0.0f;
	mFrameIdx = 0;

	mTempDelay = 0.0f;

	mbPlaying = true;
	mbPause = false;
}

void Animator::PlayWithDelay(float delay, bool isLoop)
{
	if (mbPlaying == true) { return; }
	Play(isLoop);

	mDelay = delay;
}

void Animator::Pause()
{
	if (mbPlaying == false) { return; }
	mbPause = true;
}

void Animator::Resume()
{
	mbPause = false;
}

void Animator::Stop()
{
	mbPlaying = false;
	mbPause = false;
}

