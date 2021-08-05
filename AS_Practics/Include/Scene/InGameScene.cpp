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

bool CInGameScene::Init(HWND _hWnd)
{
	//// 위에서 Init한 것을 기반으로 초기화 하기 때문에 밑에 선언
	//if (!CScene::Init(_hWnd))
	//	return false;

	//class CLayer*	pt_layer;

	//// ---------------------------------------------------------------------- : >> 배경 추가
	//
	//// 배경 레이어 찾기
	//pt_layer = FindLayer("Background");
	//if (pt_layer == NULL) return false;

	//// 물체 생상하여 레이어에 추가
	//class CStage* pt_stage = CObject::CreateObj<CStage>("Map1", pt_layer);
	//if (pt_layer == NULL) return false;

	//// 텍스처 & 물체 size 설정
	//pt_stage->SetTexture("Background", _T("3000.bmp"), TEXTURE_PATH);


	//// 배경 사이즈 정해질 때 카메라 내 월드 사이즈도 설정
	//MY_SIZE stage_size = pt_stage->GetSize();
	//SetWorldSize(stage_size);

	//// 씬의 카메라 초기화
	//hWnd_ = _hWnd;
	//RECT rectView;
	//GetClientRect(hWnd_, &rectView);
	//SetWndSize(rectView.right, rectView.bottom);

	//// main 화면 기준 위치, 월드 사이즈 & 씬 사이즈 & 속도 초기화
	//camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	//SAFE_RELEASE(pt_stage);


	//// ---------------------------------------------------------------------- : <<

	//

	//// ---------------------------------------------------------------------- : >> 플레이어 추가

	//// 오브젝트를 추가할 레이어를 찾는다.
	//pt_layer = FindLayer("Player");
	//if (pt_layer == NULL) return false;

	//// 추가할 오브젝트를 생성한다. 또한 추가할 레이어에 추가한다.
	//class CPlayer*	pt_player = CObject::CreateObj<CPlayer>("Player", pt_layer);

	//// 텍스처 & 물체 size 설정
	//pt_player->SetTexture(PLAYER_TAG, _T("sigong.bmp"), TEXTURE_PATH);

	//// 플레이어에 카메라 고정
	//camera_->SetMaster(pt_player);

	//// CreateObj해서 레이어에 추가까지 하면 참조 카운트가 2개가 된다. 따라서 1개로 만들기 위해 pt_layer 해제
	//SAFE_RELEASE(pt_player);

	//// ---------------------------------------------------------------------- : <<



	//// ---------------------------------------------------------------------- : >> 몬스터 추가

	////// 오브젝트를 추가할 레이어를 찾는다.
	////pt_layer = FindLayer("Monster");
	////if (pt_layer == NULL) return false;

	////class CMonster* pt_monster = CObject::CreateObj<CMonster>("Monster", pt_layer);
	////
	////SAFE_RELEASE(pt_monster);

	//// ---------------------------------------------------------------------- : <<


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
	// CScene::Render(_hdc, _time);
}
