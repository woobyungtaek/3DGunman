#include <d3d11.h>
#include <SimpleMath.h>

#include <vector>
#include <unordered_map>
#include <string>

#include "ParsingDataDefine.h"
#include "Animation.h"
#include "BTASEParser.h"

using namespace ASEParser;

#pragma region BTParserData

BTParseData::BTParseData()
	: mAsciiExport(0), mMaterialCount(0)
{
}

BTParseData::~BTParseData()
{
	for (int index = 0; index < (int)mMaterialDataVec.size(); index++)
	{
		delete mMaterialDataVec[index];
	}
	mMaterialDataVec.clear();

	for (int index = 0; index < (int)mMeshVec.size(); index++)
	{
		delete mMeshVec[index];
	}
	mMeshVec.clear();
}

void BTParseData::SetParentNode()
{
	int count = (int)mMeshVec.size();

	for (int index = 0; index < count; index++)
	{
		mMeshVec[index]->m_parentMesh = nullptr;

		for (int tIdx = 0; tIdx < count; tIdx++)
		{
			if (tIdx == index) { continue; }
			if (mMeshVec[index]->m_nodeparent == mMeshVec[tIdx]->m_nodename)
			{
				mMeshVec[index]->m_parentMesh = mMeshVec[tIdx];
				break;
			}
		}
	}
}

#pragma endregion

#pragma region BTAseParser

ASE::CASELexer* BTASEParser::mLexer;
char			BTASEParser::mTokenStr[256];
bool			BTASEParser::mbAnimation;
Animation*		BTASEParser::mAnimationTemp;

BTParseData* BTASEParser::Load(LPSTR pFile)
{
	if (mLexer == nullptr) { mLexer = new ASE::CASELexer(); }

	// ���� �ε�
	if (!mLexer->Open(pFile))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!\n");
		return nullptr;
	}

	// ������� Return �޾Ƽ� �����ؾ���
	BTParseData*  resultData = ParsingAll();
	if (resultData == nullptr)
	{
		TRACE("������ �Ľ��ϴ� �� ������ �߻��߽��ϴ�!");
		return nullptr;
	}

	//Vertex����ġ ����
	for (int index = 0; index < (int)resultData->mMeshVec.size(); index++)
	{
		int count = resultData->mMeshVec[index]->m_vector_wvertexs.size();
		for (int vIdx = 0; vIdx < count; vIdx++)
		{
			int meshVerIdx = resultData->mMeshVec[index]->m_vector_wvertexs[vIdx]->m_wvertex_number;
			for (int bbWeightIdx = 0; bbWeightIdx < resultData->mMeshVec[index]->m_vector_wvertexs[vIdx]->m_bone_blending_weight.size(); bbWeightIdx++)
			{
				resultData->mMeshVec[index]->m_meshvertex[meshVerIdx]->m_bone_blending_weight.push_back(resultData->mMeshVec[index]->m_vector_wvertexs[vIdx]->m_bone_blending_weight[bbWeightIdx]);
			}
		}
	}

	// Optimize
	for(int index =0; index < (int)resultData->mMeshVec.size(); ++index)
	{
		Optimize(resultData->mMeshVec[index]);
	}

	// NodeTm
	for (int index = 0; index < (int)resultData->mMeshVec.size(); ++index)
	{
		ConvertNodeTM(resultData->mMeshVec[index]);
	}

	// SetParent
	resultData->SetParentNode();

	// �ε��� ������ ��ȯ
	return resultData;
}

AnimationClip* BTASEParser::LoadAni(LPSTR pFile)
{
	if (mLexer == nullptr) { mLexer = new ASE::CASELexer(); }

	// ���� �ε�
	if (!mLexer->Open(pFile))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!\n");
		return nullptr;
	}

	// ������� Return �޾Ƽ� �����ؾ���
	AnimationClip* resultData = ParsingAnimation();
	if (resultData == nullptr)
	{
		TRACE("������ �Ľ��ϴ� �� ������ �߻��߽��ϴ�!");
		return nullptr;
	}

	return resultData;
}

// �Ľ� �� �����Ϳ� ��� ��ȯ
BTParseData* BTASEParser::ParsingAll()
{
	// ���� ���� ��ū
	LONG currentToken;

	// �ӽ� ��ü
	int   tempIdx	= 0;
	Mesh* pTempMesh = nullptr;
	Face* pTempFace = nullptr;
	ASEMaterial* pTempMat = nullptr;

	// ��ȯ�� ������ ��ü
	BTParseData* pParserData = new BTParseData();

	while ( true )
	{
		currentToken = mLexer->GetToken(mTokenStr);

		switch (currentToken)
		{

#pragma region Info

		case TOKENR_HELPER_CLASS:
		{

		}
		break;

		case TOKENR_3DSMAX_ASCIIEXPORT:
		{
			pParserData->mAsciiExport = Parsing_NumberLong();
		}
		break;

		case TOKENR_COMMENT:
		{
		}
		break;

#pragma endregion

#pragma region Material List

		case TOKENR_MATERIAL_LIST :
		{
		}
			break;
		case TOKENR_MATERIAL_COUNT :
		{			
		}
			break;
		case TOKENR_MATERIAL :
		{
			pTempMat = new ASEMaterial();
			pParserData->mMaterialDataVec.push_back(pTempMat);
			pTempMat->m_materialnumber = Parsing_NumberInt();
		}
		break;
		case TOKENR_MATERIAL_AMBIENT :
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_material_ambient = Parsing_NumberVector3();
		}
		break;
		case TOKENR_MATERIAL_DIFFUSE :
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_material_diffuse = Parsing_NumberVector3();
		}
		break;
		case TOKENR_MATERIAL_SPECULAR :
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_material_specular = Parsing_NumberVector3();
		}
		break;
		case TOKENR_MATERIAL_SHINE:
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_material_shine = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_SHINESTRENGTH:
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_material_shinestrength  = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_TRANSPARENCY:
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_material_transparency = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MATERIAL_WIRESIZE:
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_material_wiresize = Parsing_NumberFloat();
		}
		break;
		case TOKENR_MAP_DIFFUSE:
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_map_diffuse = new MaterialMap();
			pTempMat->m_map_temp = pTempMat->m_map_diffuse;
		}
		break;
		case TOKENR_MAP_GENERIC:
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_map_generic = new MaterialMap();
			pTempMat->m_map_temp = pTempMat->m_map_generic;
		}
		break;
		case TOKENR_BITMAP:
		{
			if (pTempMat == nullptr) { break; }
			pTempMat->m_map_temp->m_bitmap = Parsing_String();
		}
		break;

#pragma endregion

#pragma region Create Object

		// Mesh�߰� �Ǵ� ������Ʈ�� ����

		case TOKENR_GROUP:
		{
		}
		break;

		case TOKENR_HELPEROBJECT:
		{
			pTempMesh = new Mesh();
			pParserData->mMeshVec.push_back(pTempMesh);
		}
		break;


		case TOKENR_GEOMOBJECT:
		{
			pTempMesh = new Mesh();
			pParserData->mMeshVec.push_back(pTempMesh);
		}
		break;

		case TOKENR_SHAPEOBJECT:
		{
			pTempMesh = new Mesh();
			pParserData->mMeshVec.push_back(pTempMesh);
		}
		break;

#pragma endregion

#pragma region Geometry Object

		case TOKENR_NODE_NAME:
		{
			// if Animation
			if (mbAnimation == true)
			{
				mAnimationTemp->m_nodename = Parsing_String();

				mbAnimation = false;
				break;
			}

			// if Mesh
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_nodename = Parsing_String();
		}
		break;

		case TOKENR_NODE_PARENT:
		{
			// �θ� ��� ����
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_nodeparent = Parsing_String();
		}
		break;

		case TOKENR_NODE_TM:
		{
		}
		break;

		case TOKENR_INHERIT_POS:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_inherit_pos = Parsing_NumberVector3();
		}
		break;
		case TOKENR_INHERIT_ROT:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_inherit_rot = Parsing_NumberVector3();
		}
		break;
		case TOKENR_INHERIT_SCL:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_inherit_scl = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_ROW0:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_row0 = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_ROW1:
		{
			// row1�� row2 ������ �ٲ�
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_row2 = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_ROW2:
		{
			// row1�� row2 ������ �ٲ�
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_row1 = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_ROW3:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_row3 = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_POS:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_pos = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_ROTAXIS:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_rotaxis = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_ROTANGLE:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_rotangle = Parsing_NumberFloat();
		}
		break;
		case TOKENR_TM_SCALE:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_scale = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_SCALEAXIS:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_scaleaxis = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_SCALEAXISANG:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_tm_scaleaxisang = Parsing_NumberFloat();
		}
		break;

#pragma region Mesh

		case TOKENR_MESH:
		{
		}
		break;
		case TOKENR_TIMEVALUE:
		{
		}
		break;
		case TOKENR_MESH_NUMBONE:
		{
			// ���� ��Ű�׿�����Ʈ�̰� ��Ű�׿� �ʿ��� �� ������ ���� ����
			// ���� ���� ������Ʈ�� ��Ű�� ������Ʈ�� �����Ѵ�.
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_is_skinningobject = true;
		}
		break;
		case TOKENR_MESH_NUMSKINWEIGHT:
		{
		}
		break;
		case TOKENR_MESH_NUMVERTEX:
		{
			// Vertex ���� ��ŭ �߰�
			if (pTempMesh == nullptr) { break; }
			int count = Parsing_NumberInt();

			pTempMesh->m_mesh_numvertex = count;
			for (int index = 0; index < count; ++index)
			{
				pTempMesh->m_meshvertex.push_back(new Vertex());
			}
		}
		break;
		case TOKENR_MESH_NUMFACES:
		{
			// Face ������ŭ �߰�
			if (pTempMesh == nullptr) { break; }
			int count = Parsing_NumberInt();

			pTempMesh->m_mesh_numfaces = count;
			for (int index = 0; index < count; ++index)
			{
				pTempMesh->m_meshface.push_back(new Face());
			}
		}
		break;

		case TOKENR_MESH_VERTEX_LIST:
		{
		}
		break;
		case TOKENR_MESH_VERTEX:
		{
			// Mesh Vertex �б�
			if (pTempMesh == nullptr) { break; }
			int index = Parsing_NumberInt();
			pTempMesh->m_meshvertex[index]->m_pos = Parsing_NumberVector3();
		}
		break;
		
#pragma endregion

#pragma region Bone

		case TOKENR_SKIN_INITTM:
		{
		}
		break;

		case TOKENR_BONE_LIST:
		{
		}
		break;

		case TOKENR_BONE:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_bone = new Bone();
			pTempMesh->m_vector_bone_list.push_back(pTempMesh->m_bone);
			pTempMesh->m_bone->m_bone_number = Parsing_NumberInt();
		}
		break;

		case TOKENR_BONE_NAME:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_bone->m_bone_name = Parsing_String();
		}
		break;

		case TOKENR_BONE_PROPERTY:
		{

		}
		break;

		case TOKENR_MESH_WVERTEXS:
		{

		}
		break;

		case TOKENR_MESH_WEIGHT:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_wvertex = new VertexWeight();
			pTempMesh->m_wvertex->m_wvertex_number = Parsing_NumberInt();
			pTempMesh->m_vector_wvertexs.push_back(pTempMesh->m_wvertex);
		}
		break;

		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			if (pTempMesh == nullptr) { break; }
			Weight* weight = new Weight();
			weight->m_bone_number = Parsing_NumberInt();
			weight->m_bone_weight = Parsing_NumberFloat();

			pTempMesh->m_wvertex->m_bone_blending_weight.push_back(weight);
		}
		break;

#pragma endregion

#pragma region FaceList

		case TOKENR_MESH_FACE_LIST:
		{
		}
		break;
		case TOKENR_MESH_FACE:
		{
			if (pTempMesh == nullptr) { break; }

			int index = Parsing_NumberInt();

			// ABC �б�
			for (int count = 0; count < 3; ++count)
			{				
				Parsing_String(); // ���ĺ� �о ����
				pTempMesh->m_meshface[index]->m_vertexindex[count] = Parsing_NumberInt();
			}
		}
		break;

		case TOKENR_MESH_NORMALS:
		{
		}
		break;
		case TOKENR_MESH_FACENORMAL:
		{
			if (pTempMesh == nullptr) { break; }

			int index = Parsing_NumberInt();
			pTempFace = pTempMesh->m_meshface[index];
			pTempFace->m_normal = Parsing_NumberVector3();

			tempIdx = 0;
		}
		break;

		case TOKENR_MESH_VERTEXNORMAL:
		{
			if (pTempFace == nullptr) { break; }

			// index�� ������ �ƴ� ����� ���ε� �ϴ�.
			int index = Parsing_NumberInt(); 
			pTempFace->m_normalvertex[tempIdx].x = Parsing_NumberFloat();
			pTempFace->m_normalvertex[tempIdx].z = Parsing_NumberFloat();
			pTempFace->m_normalvertex[tempIdx].y = Parsing_NumberFloat();
			tempIdx++;
		}
		break;

#pragma endregion

#pragma region TVertex List

		case TOKENR_MESH_NUMTVERTEX:
		{
			if (pTempMesh == nullptr) { break; }

			int count = Parsing_NumberInt();
			pTempMesh->m_mesh_numtvertex = count;

			for (int index = 0; index < count; index++)
			{
				pTempMesh->m_mesh_tvertex.push_back(new COneTVertex());
			}

		}
		break;
		
		case TOKENR_MESH_TVERTLIST:
		{
		}
		break;

		case TOKENR_MESH_TVERT:
		{
			if (pTempMesh == nullptr) { break; }

			int index = Parsing_NumberInt();

			pTempMesh->m_mesh_tvertex[index]->m_u = Parsing_NumberFloat();
			pTempMesh->m_mesh_tvertex[index]->m_v = Parsing_NumberFloat();
		}
		break;

		case TOKENR_MESH_NUMTVFACES:
		{
		}
		break;

		case TOKENR_MESH_TFACE:
		{
			if (pTempMesh == nullptr) { break; }

			int index = Parsing_NumberInt();
			pTempMesh->m_meshface[index]->m_TFace[0] = Parsing_NumberInt();
			pTempMesh->m_meshface[index]->m_TFace[1] = Parsing_NumberInt();
			pTempMesh->m_meshface[index]->m_TFace[2] = Parsing_NumberInt();
		}
		break;

		case TOKENR_MATERIAL_REF:
		{
			if (pTempMesh == nullptr) { break; }
			pTempMesh->m_material_ref = Parsing_NumberInt();
		}
		break;

#pragma endregion


#pragma endregion

		case TOKEND_END:
		{
			// ���� ��
			TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
		}
		return pParserData;

		default:
			break;

		}
	}

	return nullptr;
}

AnimationClip* BTASEParser::ParsingAnimation()
{
	// ���� ���� ��ū
	LONG currentToken;

	// ��ȯ�� ������ ��ü
	AnimationClip* pAniClipData = new AnimationClip();

	while (true)
	{
		currentToken = mLexer->GetToken(mTokenStr);

		switch (currentToken)
		{

#pragma region Scene
		// SceneData�� ���� �ε����� �ʴ´�.
		case TOKENR_SCENE:
		{
		}
		break;
		case TOKENR_SCENE_FILENAME:
		{
			pAniClipData->mSceneData.m_filename = Parsing_String();
		}
		break;
		case TOKENR_SCENE_FIRSTFRAME:
		{
			pAniClipData->mSceneData.m_firstframe = Parsing_NumberLong();
		}
		break;
		case TOKENR_SCENE_LASTFRAME:
		{
			pAniClipData->mSceneData.m_lastframe = Parsing_NumberLong();
		}
		break;
		case TOKENR_SCENE_FRAMESPEED:
		{
			pAniClipData->mSceneData.m_framespeed = Parsing_NumberLong();
		}
		break;
		case TOKENR_SCENE_TICKSPERFRAME:
		{
			pAniClipData->mSceneData.m_ticksperframe = Parsing_NumberLong();
		}
		break;
		case TOKENR_SCENE_MESHFRAMESTEP:
		{
			pAniClipData->mSceneData.m_meshframestep = Parsing_NumberLong();
		}
		break;
		case TOKENR_SCENE_KEYFRAMESTEP:
		{
			pAniClipData->mSceneData.m_keyframestep = Parsing_NumberLong();
		}
		break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
		{
			pAniClipData->mSceneData.m_scene_background_static.x = Parsing_NumberFloat();
			pAniClipData->mSceneData.m_scene_background_static.y = Parsing_NumberFloat();
			pAniClipData->mSceneData.m_scene_background_static.z = Parsing_NumberFloat();
		}
		break;
		case TOKENR_SCENE_AMBIENT_STATIC:
		{
			pAniClipData->mSceneData.m_scene_ambient_static.x = Parsing_NumberFloat();
			pAniClipData->mSceneData.m_scene_ambient_static.y = Parsing_NumberFloat();
			pAniClipData->mSceneData.m_scene_ambient_static.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_SCENE_ENVMAP:
		{
		}
		break;
#pragma endregion

#pragma region Animation
		// �ִϸ��̼� �ʿ� �� �� �ۼ�

		case TOKENR_TM_ANIMATION:
		{
			mAnimationTemp = new Animation();
			mbAnimation = true;
		}
		break;

		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
		}
		break;

		case TOKENR_CONTROL_POS_SAMPLE:
		{
			CAnimation_pos* posAni = new CAnimation_pos();
			mAnimationTemp->m_position.push_back(posAni);

			posAni->m_time = Parsing_NumberInt();
			posAni->m_pos = Parsing_NumberVector3();
		}
		break;

		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			CAnimation_rot* rotAni = new CAnimation_rot();
			mAnimationTemp->m_rotation.push_back(rotAni);
			int prevIdx = (int)mAnimationTemp->m_rotation.size() - 2;

			rotAni->m_time = Parsing_NumberInt();
			rotAni->m_rot = Parsing_NumberVector3();
			rotAni->m_angle = Parsing_NumberFloat();

			//���ʹϾ����� �ٲ�
			rotAni->m_rotQT_accumulation = Quaternion::CreateFromAxisAngle(rotAni->m_rot, rotAni->m_angle);

			//���� �������� Rot�� �������Ѿ��Ѵ�.
			if (prevIdx >= 0)
			{
				rotAni->m_rotQT_accumulation =
					mAnimationTemp->m_rotation[prevIdx]->m_rotQT_accumulation * rotAni->m_rotQT_accumulation;
			}
		}
		break;

#pragma endregion

		case TOKENR_NODE_NAME:
		{
			if (mbAnimation == true)
			{
				mAnimationTemp->m_nodename = Parsing_String();
				wstring* wNodeName = new wstring(mAnimationTemp->m_nodename.begin(), mAnimationTemp->m_nodename.end());
				pAniClipData->mAnimationMap.insert(make_pair(*wNodeName, mAnimationTemp));

				mbAnimation = false;
				break;
			}
		}
		break;

		case TOKEND_END:
		{
			// ���� ��
			TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
		}
		return pAniClipData;

		default:
			break;

		}
	}

	return nullptr;
}

// Optimize
void BTASEParser::Optimize(ASEParser::Mesh* pMesh)
{
	//Face�� ����ŭ �ݺ�
	int faceCount = pMesh->m_mesh_numfaces;
	for (int index = 0; index < faceCount; ++index)
	{
		// Vertex�� ����ŭ �ݺ�
		for (int verCount = 0; verCount < 3; ++verCount)
		{
			// Vertex Index ȹ��
			int verIdx = pMesh->m_meshface[index]->m_vertexindex[verCount];

			// �� Vertex ����
			Vertex* _pVertex = new Vertex();
			
			// Vertex�� Pos�� �ֱ�
			_pVertex->m_pos = pMesh->m_meshvertex[verIdx]->m_pos;
			
			// Face�� �������ִ� Vertex�� Normal �� �ֱ�
			_pVertex->m_normal = pMesh->m_meshface[index]->m_normalvertex[verCount];

			// Texture�� UV�� �ֱ�
			int tfaceIdx = pMesh->m_meshface[index]->m_TFace[verCount];
			if (pMesh->m_mesh_tvertex.size() > 0)
			{
				_pVertex->u = pMesh->m_mesh_tvertex[tfaceIdx]->m_u;
				_pVertex->v = pMesh->m_mesh_tvertex[tfaceIdx]->m_v;
			}
			
			// �� ����ġ
			int bbCount = pMesh->m_meshvertex[verIdx]->m_bone_blending_weight.size();
			for (int bbIdx = 0; bbIdx < bbCount; bbIdx++)
			{
				_pVertex->m_bone_blending_weight.push_back(pMesh->m_meshvertex[verIdx]->m_bone_blending_weight[bbIdx]);
			}

			// �ߺ� Index �ʱ�ȭ
			int dupIdx = -1;
			for (int optIdx = 0; optIdx < (int)pMesh->m_opt_vertex.size(); ++optIdx)
			{
				if (pMesh->m_opt_vertex[optIdx]->m_pos != _pVertex->m_pos)
				{
					continue;
				}
				if (pMesh->m_opt_vertex[optIdx]->m_normal != _pVertex->m_normal)
				{
					continue;
				}
				if (!(pMesh->m_opt_vertex[optIdx]->u == _pVertex->u
					&& pMesh->m_opt_vertex[optIdx]->v == _pVertex->v))
				{
					continue;
				}

				dupIdx = optIdx;
			}

			// �ߺ��� �ȵ� ��� �߰�
			if (dupIdx == -1)
			{
				pMesh->m_opt_vertex.push_back(_pVertex);
				int corrIdx = (int)pMesh->m_opt_vertex.size() - 1;
				pMesh->m_meshface[index]->m_vertexindex[verCount] = corrIdx;
			}

			// �ߺ��� �� ��� ���� �ִ� Idx�� ����
			else
			{
				pMesh->m_meshface[index]->m_vertexindex[verCount] = dupIdx;
			}
		}
	}

	pMesh->m_opt_index = new IndexList[(int)pMesh->m_meshface.size()];
	for (int i = 0; i < pMesh->m_meshface.size(); ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}
}

// NodeTm
void BTASEParser::ConvertNodeTM(ASEParser::Mesh* pMesh)
{
	Matrix* pMatrix = new Matrix(pMesh->m_tm_row0, pMesh->m_tm_row1, pMesh->m_tm_row2);

	pMatrix->_41 = pMesh->m_tm_pos.x;
	pMatrix->_42 = pMesh->m_tm_pos.y;
	pMatrix->_43 = pMesh->m_tm_pos.z;
	pMatrix->_44 = 1;

	Vector3 cross = pMesh->m_tm_row0.Cross(pMesh->m_tm_row1);
	float dot = cross.Dot(pMesh->m_tm_row2);
	if (dot < 0)
	{
		// Is �װ�Ƽ��
		Vector3		s;
		Quaternion	r;
		Vector3		t;

		// ��Һ� ����
		pMatrix->Decompose(s, r, t);

		s *= -1;

		for (int index = 0; index < pMesh->m_opt_vertex.size(); ++index)
		{
			pMesh->m_opt_vertex[index]->m_normal *= -1;
		}
		*pMatrix = Matrix::CreateScale(s) * Matrix::CreateFromQuaternion(r) * Matrix::CreateTranslation(t);
	}

	pMesh->m_WorldTM = *pMatrix;

	Matrix inMatrix = pMatrix->Invert();

	// ���ؽ��鿡 ���ؼ� ���� ���ؽ��� �����.
	for (int index = 0; index < (int)pMesh->m_opt_vertex.size(); index++)
	{
		float x = (pMesh->m_opt_vertex[index]->m_pos.x * inMatrix._11 +
			pMesh->m_opt_vertex[index]->m_pos.y * inMatrix._21 +
			pMesh->m_opt_vertex[index]->m_pos.z * inMatrix._31 + inMatrix._41);

		float y = (pMesh->m_opt_vertex[index]->m_pos.x * inMatrix._12 +
			pMesh->m_opt_vertex[index]->m_pos.y * inMatrix._22 +
			pMesh->m_opt_vertex[index]->m_pos.z * inMatrix._32 + inMatrix._42);

		float z = (pMesh->m_opt_vertex[index]->m_pos.x * inMatrix._13 +
			pMesh->m_opt_vertex[index]->m_pos.y * inMatrix._23 +
			pMesh->m_opt_vertex[index]->m_pos.z * inMatrix._33 + inMatrix._43);

		pMesh->m_opt_vertex[index]->m_pos.x = x;
		pMesh->m_opt_vertex[index]->m_pos.y = y;
		pMesh->m_opt_vertex[index]->m_pos.z = z;
	}
}


#pragma region Parsing Value

// Long
long BTASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = mLexer->GetToken(mTokenStr);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(mTokenStr, NULL, 10);

	return			tempNumber;
}

// float
float BTASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = mLexer->GetToken(mTokenStr);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(mTokenStr);

	return			tempNumber;
}

// String
LPSTR BTASEParser::Parsing_String()
{
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
	mLexer->GetToken(mTokenStr);

	return mTokenStr;
}

// int
int BTASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = mLexer->GetToken(mTokenStr);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(mTokenStr);

	return			tempNumber;
}

// Vector (z, y ���� ��ü)
Vector3 BTASEParser::Parsing_NumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = mLexer->GetToken(mTokenStr);
	tempVector3.x = (float)atof(mTokenStr);
	token = mLexer->GetToken(mTokenStr);
	tempVector3.z = (float)atof(mTokenStr);
	token = mLexer->GetToken(mTokenStr);
	tempVector3.y = (float)atof(mTokenStr);

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

#pragma endregion

#pragma endregion
