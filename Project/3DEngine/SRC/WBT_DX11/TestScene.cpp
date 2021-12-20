#include "pch.h"
#include "TestScene.h"


TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

void TestScene::Start()
{
	// 리소스 로드
	//BTLoader::LoadModelDataSet(L"Genji", L"../ASE/genji_max.ASE");
	BTLoader::LoadModelDataSet(L"Joe_Mesh", L"../ASE/03IK-Joe.ASE");
	BTLoader::LoadModelDataSet(L"teapot", L"../ASE/teapot.ASE");
	BTLoader::LoadModelDataSet(L"Ant3", L"../ASE/ant3.ase");
	BTLoader::LoadModelDataSet(L"Cylinder", L"../ASE/Cylinder.ASE");
	//BTLoader::LoadModelDataSet(L"untitled", L"../ASE/untitled3.ASE");

	BTLoader::LoadAnimation(L"Joe_Ani", L"../ASE/03IK-Joe.ASE");
	BTLoader::LoadAnimation(L"Ant3Ani", L"../ASE/ant3.ase");
	BTLoader::LoadAnimation(L"CylinderAni", L"../ASE/Cylinder.ASE");

	BTLoader::LoadTextureFromDDS(L"BoxDDS", L"../Textures/WoodCrate02.dds");
	BTLoader::LoadTextureFromDDS(L"GenjiTex", L"../Textures/000000002405.dds");
	BTLoader::LoadTextureFromDDS(L"GenjiSubTex", L"../Textures/000000002476.dds");
	BTLoader::LoadTextureFromDDS(L"GenjiNormal", L"../Textures/Genji_nmap.dds");
	BTLoader::LoadTextureFromDDS(L"brick", L"../Textures/darkbrickdxt1.dds");
	BTLoader::LoadTextureFromDDS(L"grasscube", L"../Textures/grasscube1024.dds");
	BTLoader::LoadTexturePng(L"testBrickNormal", L"../Textures/testBrickNormal.png");
	BTLoader::LoadTexturePng(L"TileMapImage", L"../Textures/TileMapImage.png");
	BTLoader::LoadTexturePng(L"Character_Hair", L"../Textures/Character_Hair.png");

	mActiveCamera->SetCubeMap(L"grasscube");

	GameObject* testSceneManager = new GameObject();
	testSceneManager->AddComponent<TestSceneManager>();
}
