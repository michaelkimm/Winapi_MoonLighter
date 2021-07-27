#include "InGameScene.h"
#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Core\SourceManager.h"


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

	// 오브젝트를 추가할 레이어를 찾는다.
	class CLayer*	pt_layer = FindLayer("Default");



	// : >> 플레이어 추가

	// 추가할 오브젝트를 생성한다. 또한 추가할 레이어에 추가한다.
	class CPlayer*	pt_player = CObject::CreateObj<CPlayer>("Player", pt_layer);

	// 플레이어 텍스처 추가
	pt_player->SetTexture(PLAYER_TAG, _T("sigong.bmp"), TEXTURE_PATH);
	if (pt_player->GetTexture() == NULL)
		return false;

	// CreateObj해서 레이어에 추가까지 하면 참조 카운트가 2개가 된다. 따라서 1개로 만들기 위해 pt_layer 해제
	SAFE_RELEASE(pt_player);

	// <<



	// : >> 몬스터 추가

	class CMonster* pt_monster = CObject::CreateObj<CMonster>("Monster", pt_layer);
	
	SAFE_RELEASE(pt_monster);

	// <<

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
