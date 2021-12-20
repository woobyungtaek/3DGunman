/*
	���� �̸��� �Է��ϸ�
	Mesh, Animation���� �����͸� ����� ����.
	������ �����ʹ� ������ �Ŵ����� ���� ����ǵ��� �Ѵ�.
*/

#pragma once

enum eObjectType
{
	eObjectType_ObjectRoot, eObjectType_Geomobject, eObjectType_Shape,
	eObjectType_Camera, eObjectType_Light, eObjectType_HelperObject,
	eObjectType_MarkerObject, eObjectType_SkinnedMesh, eObjectType_Bone,
	eObjectType_CMeshPlane, eObjectType_CBullet_Trace,
	//
	eObjectType_BasicSphere, eObjectType_Missile,
	eObjectType_MeshBillboard, eObjectType_PointSprite,			// CMesh���..
	eObjectType_End,
};

#include "ASEFile.h"

#include "ParsingDataDefine.h"

using namespace ASEParser;

class Animation;

// �ִϸ��̼�(��庰)�� ���� Clip
class AnimationClip
{
public:
	AnimationClip() {}
	~AnimationClip() {}

public:
	// �����Ϳ� �ִ� SceneData
	Scenedata mSceneData;

	std::wstring mClipName;

	// Key[ ����̸� ], Value[ �ִϸ��̼� ]
	unordered_map<std::wstring, Animation*> mAnimationMap;
};

class BTParseData
{
public:
	BTParseData();
	~BTParseData();

public:

	// 3D Max Parsing�� �پ� ������ ��
	int mAsciiExport;

	// Material ����Ʈ
	int mMaterialCount;
	vector< ASEParser::ASEMaterial*> mMaterialDataVec;

	// Mesh ����Ʈ
	vector< ASEParser::Mesh*> mMeshVec;

public:
	void SetParentNode();
};

class BTASEParser
{
private:
	static ASE::CASELexer* mLexer;
	static char mTokenStr[256];

	static 	bool mbAnimation; // �ִϸ��̼� �д� ��
	static  Animation* mAnimationTemp; // ���� �д� �ִϸ��̼�

public:
	static BTParseData* Load(LPSTR pFile);
	static AnimationClip* LoadAni(LPSTR pFile);

private:
	static BTParseData* ParsingAll();
	static AnimationClip* ParsingAnimation();

	static void		Optimize(ASEParser::Mesh* pMesh);
	static void		ConvertNodeTM(ASEParser::Mesh* pMesh);

	static int		Parsing_NumberInt();	// int
	static long		Parsing_NumberLong();	// long�� �о �������ش�.
	static float	Parsing_NumberFloat();	// float
	static LPSTR	Parsing_String();		// string
	static Vector3	Parsing_NumberVector3();// 3���� Float�� ���� �ϳ��� �о �������ش�
	
};

