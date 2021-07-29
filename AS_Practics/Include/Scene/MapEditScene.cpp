#include "MapEditScene.h"

#include "InGameScene.h"
#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Stage.h"
#include "..\Core\SourceManager.h"
#include "..\Core\CameraManager.h"

CMapEditScene::CMapEditScene()
{
}

CMapEditScene::~CMapEditScene()
{
}

bool CMapEditScene::Init()
{
	if (!CScene::Init())
		return false;

	// ---------------------------------------------------------------------- : >> �������� ����

	// ���̾� ã��
	CLayer* pt_layer = FindLayer("Background");
	if (pt_layer == nullptr) return false;

	// CStage Ŭ���� ������Ʈ�� �����ؼ� pt_layer�� �ִ´�. �±� = "stage" 
	CStage* pt_stage = CObject::CreateObj<CStage>("stage", pt_layer);

	if (!pt_stage->CreateTile(100, 100, 16, 16, WATER1, _T("water1.bmp"), TEXTURE_PATH))
	{
		SAFE_RELEASE(pt_stage);
		return false;
	}

	SAFE_RELEASE(pt_stage);

	// ---------------------------------------------------------------------- : <<


	return true;
}


void CMapEditScene::Input(float _time)
{
	CScene::Input(_time);
}

void CMapEditScene::Update(float _time)
{
	CScene::Update(_time);
}

void CMapEditScene::LateUpdate(float _time)
{
	CScene::LateUpdate(_time);
}

void CMapEditScene::Collision(float _time)
{
	CScene::Collision(_time);
}

void CMapEditScene::Render(HDC _hdc, float _time)
{
	CScene::Render(_hdc, _time);
}
