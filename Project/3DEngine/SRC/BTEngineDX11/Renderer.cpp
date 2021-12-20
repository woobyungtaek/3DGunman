#include "pch.h"
#include "Renderer.h"

bool compare(Renderer* a, Renderer* b)
{
	return (a->mSortOrder < b->mSortOrder);
}

Renderer::Renderer()
{
	Object::Scene->mRendererVec.push_back(this);
	sort(Object::Scene->mRendererVec.begin(),
		Object::Scene->mRendererVec.end(),
		compare);
}

Renderer::~Renderer()
{
	ReleaseCOM(mInputLayout);
}

void Renderer::UpdateRenderer(Camera* pCamera)
{
	mWorld = Matrix::Identity;
	mView = pCamera->GetView();
	mProj = pCamera->GetProj();

	mEyePos = pCamera->GetPos();

}

void Renderer::Update(float deltaTime)
{
	if (mModelDataSet == nullptr) { return; }

	mModelDataSet->UpdateUseTMByLocalTM();

	if (mModelDataSet->IsWorldTMUpdated == false)
	{
		mModelDataSet->UpdateWorldMatrix();
	}
}

void Renderer::SettingBuffers(ModelData* meshData)
{
	// �ε������ۿ� ���ؽ����� ����
	mDeviceContext->IASetVertexBuffers(0, 1, &meshData->VB, &meshData->Stride, &mOffset);
	mDeviceContext->IASetIndexBuffer(meshData->IB, DXGI_FORMAT_R32_UINT, 0);
}

void Renderer::SetMeshDatasByStr(wstring dataName)
{
	// ������ MeshDatas�� ������ �ȴ�.
	mModelDataSet = ResourceManager::GetInstance()->GetModelDatas(dataName)->GetCopy();
}

void Renderer::CommonSetting()
{
	// �Է� ��ġ ��ü ����
	mDeviceContext->IASetPrimitiveTopology(mPrimitiveTopo);
	mDeviceContext->RSSetState(mpRenderState);
}

void Renderer::SetSortOrder(int order)
{
	mSortOrder = order;
	sort(Object::Scene->mRendererVec.begin(),
		Object::Scene->mRendererVec.end(),
		compare);
}
