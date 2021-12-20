#pragma once
class Renderer;

class MeshRenderer : public Renderer
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

public:
	virtual void Render(Camera* pCamera) override;

private:
	ID3D11ShaderResourceView*   mTexture;	// �ؽ���
	ID3D11ShaderResourceView*   mNormalMap; // ��ָ�

	Matrix mTexTransform; 

	bool isNormalMap;
};

