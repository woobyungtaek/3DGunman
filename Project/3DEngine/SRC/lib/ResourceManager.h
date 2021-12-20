#pragma once
class ResourceManager
{
private:
	static ResourceManager* mInstance;
	ResourceManager();

public:
	static ResourceManager* GetInstance();
	~ResourceManager();

private:
	// Key[ ������ �̸� ], value[ Key[ ��� �̸� ], value[ ������Ʈ�� ] ]
	unordered_map<wstring, ModelDataSet*> mModelDataSetMap;

	// Key[ Ŭ�� �̸� ], value[ �ִϸ��̼Ǹ��� ( Animation�� ���1���� ������ ǥ��) ]
	unordered_map<wstring, AnimationClip* > mAnimationClipMap;

	// Key[ ���� �� ], value[ ������� ]
	unordered_map<wstring, vector<BTMaterial*>> mBTMaterialVecMap;

	// Key[ �ؽ��� �̸� ], value[ ���ҽ� ]
	unordered_map<wstring, ID3D11ShaderResourceView*> mTextureMap;

public:
	void Init();

	void LoadTextureFromDDS	(wstring dataName, wstring pFilePath);
	void LoadTextureFromPng(wstring dataName, wstring pFilePath);
	void LoadModelDataSet (wstring dataName, wstring pFilePath);
	void LoadAnimation	(wstring dataName, wstring pFilePath);

public:

	// Model���� �����
	template<typename T>
	ModelData* CreateModelData(MeshData<T>* data);

	// Helper
	ModelData* LoadAxis();
	ModelData* LoadGrid();
	ModelData* LoadSpriteSquare();

	// Basic
	ModelData* LoadBox();
	ModelData* LoadSphere(float radius, UINT sliceCount, UINT stackCount);

	// Sky
	ModelData* LoadSkySphere(float radius, UINT sliceCount, UINT stackCount);

	// Mesh���� �����
	MeshData<BTStruct::Vertex>* CreateMeshData(ASEParser::Mesh* data);
	MeshData<BTStruct::VertexSkin>* CreateMeshSkinData(ASEParser::Mesh* data);

	MeshData<BTStruct::Vertex>* GetMeshData_Sphere(float radius, UINT sliceCount, UINT stackCount);

public:
	ModelDataSet*	GetModelDatasOrginal(wstring name);
	ModelDataSet*	GetModelDatas(wstring name);

	AnimationClip*	GetAniClipData(wstring name);

	BTMaterial*		GetMaterial(wstring name, int refNum);

	ID3D11ShaderResourceView* GetTexture(wstring name);
};