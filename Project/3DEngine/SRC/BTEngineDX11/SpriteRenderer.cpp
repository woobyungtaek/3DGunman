#include "pch.h"
#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
	D3D11Render* instRender = BTGameEngine::GetInstance()->GetRender();
	mDeviceContext = instRender->GetDeviceContext();
	mpRenderState = instRender->GetSolidRS();

	mPrimitiveTopo = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mInputLayout = InputLayouts::Sprite2D;

	mTexture = nullptr;

	mModelDataSet = ResourceManager::GetInstance()->GetModelDatas(L"Sprite2D");
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Init()
{
	RectTransform* rectTransform = new RectTransform();
	rectTransform->Position = gameObject->mTransform->Position;
	rectTransform->Rotation = gameObject->mTransform->Rotation;
	rectTransform->Scale = gameObject->mTransform->Scale;

	rectTransform->mScreenSize = BTGameEngine::GetInstance()->GetRender()->GetScreenSize();

	delete gameObject->mTransform;
	
	gameObject->mRectTransform = rectTransform;
	gameObject->mTransform = rectTransform;
}

void SpriteRenderer::Render(Camera* pCamera)
{
	if (mModelDataSet == nullptr) { return; }

	mOffset = 0;
	RectTransform* rectTransform = static_cast<RectTransform*>(gameObject->mTransform);

	rectTransform->mFarHeight = pCamera->GetFarHeight();
	rectTransform->mScreenSize = BTGameEngine::GetInstance()->GetRender()->GetScreenSize();

	if (mTexture == nullptr) { return; }
	rectTransform->mImageSize =  mImageSize;

	mDeviceContext->OMSetDepthStencilState(BTGameEngine::GetInstance()->GetRender()->GetDisableDS(), 20);
	mDeviceContext->IASetInputLayout(mInputLayout);
	CommonSetting();

	// 이 함수 내부를 밖에서 해줘야하네

	mWorld = rectTransform->GetTransformMatrix();
	mView = Matrix::Identity;
	Matrix worldViewProj = mWorld * mView * mProj;

	ID3DX11EffectTechnique* tech = Effects::Sprite2DFX->mTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);

	Effects::Sprite2DFX->SetWorldViewProj(worldViewProj);
	Effects::Sprite2DFX->SetDiffuseMap(mTexture);

	for (int index = 0; index < mModelDataSet->mModelDataVec.size(); ++index)
	{
		if (mModelDataSet->mModelDataVec[index]->mbNoBuffer == true) { continue; }
		SettingBuffers(mModelDataSet->mModelDataVec[index]);

		for (UINT pass = 0; pass < techDesc.Passes; ++pass)
		{
			tech->GetPassByIndex(pass)->Apply(0, mDeviceContext);
			mDeviceContext->DrawIndexed(mModelDataSet->mModelDataVec[index]->IndexCount, 0, 0);
		}
	}

	mDeviceContext->OMSetDepthStencilState(BTGameEngine::GetInstance()->GetRender()->GetDS(), 1);
}

void SpriteRenderer::SetSpriteByName(std::wstring dataName)
{
	mTexture = ResourceManager::GetInstance()->GetTexture(dataName);

	D3D11_TEXTURE2D_DESC* desc = new D3D11_TEXTURE2D_DESC();
	ID3D11Resource* pResource;
	mTexture->GetResource(&pResource);

	ID3D11Texture2D* tex2D = static_cast<ID3D11Texture2D*>(pResource);

	tex2D->GetDesc(desc);

	mImageSize.x = desc->Width;
	mImageSize.y = desc->Height;

	delete desc;
}

void SpriteRenderer::SetSprite(ID3D11ShaderResourceView* data, bool bSizeSet)
{
	mTexture = data;
	if (bSizeSet == true)
	{
		D3D11_TEXTURE2D_DESC* desc = new D3D11_TEXTURE2D_DESC();
		ID3D11Resource* pResource;
		mTexture->GetResource(&pResource);

		ID3D11Texture2D* tex2D = static_cast<ID3D11Texture2D*>(pResource);

		tex2D->GetDesc(desc);

		mImageSize.x = desc->Width;
		mImageSize.y = desc->Height;

		delete desc;
	}
}
