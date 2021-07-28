#include "InGameScene.h"
#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Core\SourceManager.h"
#include "..\Object\Stage.h"


CInGameScene::CInGameScene()
{
}

CInGameScene::~CInGameScene()
{
}

bool CInGameScene::Init()
{
	// 부모 Scene 클래스의 초기화 함수를 호출해준다.
	// 왜지??
	if (!CScene::Init())
		return false;

	class CLayer*	pt_layer;

	// ---------------------------------------------------------------------- : >> 배경 추가
	
	// 배경 레이어 찾기
	pt_layer = FindLayer("Background");
	if (pt_layer == NULL) return false;

	// 물체 생상하여 레이어에 추가
	class CStage* pt_stage = CObject::CreateObj<CStage>("Map1", pt_layer);
	if (pt_layer == NULL) return false;

	// 텍스처 & 물체 size 설정
	pt_stage->SetTexture("Background", _T("3000.bmp"), TEXTURE_PATH);
	if (pt_stage->GetTexture() == NULL) return false;

	SAFE_RELEASE(pt_stage);

	// ---------------------------------------------------------------------- : <<



	

	// ---------------------------------------------------------------------- : >> 플레이어 추가

	// 오브젝트를 추가할 레이어를 찾는다.
	pt_layer = FindLayer("Player");
	if (pt_layer == NULL) return false;

	// 추가할 오브젝트를 생성한다. 또한 추가할 레이어에 추가한다.
	class CPlayer*	pt_player = CObject::CreateObj<CPlayer>("Player", pt_layer);

	// 텍스처 & 물체 size 설정
	pt_player->SetTexture(PLAYER_TAG, _T("sigong.bmp"), TEXTURE_PATH);
	if (pt_player->GetTexture() == NULL)
		return false;

	// CreateObj해서 레이어에 추가까지 하면 참조 카운트가 2개가 된다. 따라서 1개로 만들기 위해 pt_layer 해제
	SAFE_RELEASE(pt_player);

	// ---------------------------------------------------------------------- : <<



	// ---------------------------------------------------------------------- : >> 몬스터 추가

	// 오브젝트를 추가할 레이어를 찾는다.
	pt_layer = FindLayer("Monster");
	if (pt_layer == NULL) return false;

	class CMonster* pt_monster = CObject::CreateObj<CMonster>("Monster", pt_layer);
	
	SAFE_RELEASE(pt_monster);

	// ---------------------------------------------------------------------- : <<

	return true;
}

void CInGameScene::Input(float _time)
{
	CScene::Input(_time);
}

void CInGameScene::Update(float _time)
{
	CScene::Update(_time);
}

void CInGameScene::LateUpdate(float _time)
{
	CScene::LateUpdate(_time);
}

void CInGameScene::Collision(float _time)
{
	CScene::Collision(_time);
}

void CInGameScene::Render(HDC _hdc, float _time)
{
	CScene::Render(_hdc, _time);
}
