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
	//// ������ Init�� ���� ������� �ʱ�ȭ �ϱ� ������ �ؿ� ����
	//if (!CScene::Init(_hWnd))
	//	return false;

	//class CLayer*	pt_layer;

	//// ---------------------------------------------------------------------- : >> ��� �߰�
	//
	//// ��� ���̾� ã��
	//pt_layer = FindLayer("Background");
	//if (pt_layer == NULL) return false;

	//// ��ü �����Ͽ� ���̾ �߰�
	//class CStage* pt_stage = CObject::CreateObj<CStage>("Map1", pt_layer);
	//if (pt_layer == NULL) return false;

	//// �ؽ�ó & ��ü size ����
	//pt_stage->SetTexture("Background", _T("3000.bmp"), TEXTURE_PATH);


	//// ��� ������ ������ �� ī�޶� �� ���� ����� ����
	//MY_SIZE stage_size = pt_stage->GetSize();
	//SetWorldSize(stage_size);

	//// ���� ī�޶� �ʱ�ȭ
	//hWnd_ = _hWnd;
	//RECT rectView;
	//GetClientRect(hWnd_, &rectView);
	//SetWndSize(rectView.right, rectView.bottom);

	//// main ȭ�� ���� ��ġ, ���� ������ & �� ������ & �ӵ� �ʱ�ȭ
	//camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	//SAFE_RELEASE(pt_stage);


	//// ---------------------------------------------------------------------- : <<

	//

	//// ---------------------------------------------------------------------- : >> �÷��̾� �߰�

	//// ������Ʈ�� �߰��� ���̾ ã�´�.
	//pt_layer = FindLayer("Player");
	//if (pt_layer == NULL) return false;

	//// �߰��� ������Ʈ�� �����Ѵ�. ���� �߰��� ���̾ �߰��Ѵ�.
	//class CPlayer*	pt_player = CObject::CreateObj<CPlayer>("Player", pt_layer);

	//// �ؽ�ó & ��ü size ����
	//pt_player->SetTexture(PLAYER_TAG, _T("sigong.bmp"), TEXTURE_PATH);

	//// �÷��̾ ī�޶� ����
	//camera_->SetMaster(pt_player);

	//// CreateObj�ؼ� ���̾ �߰����� �ϸ� ���� ī��Ʈ�� 2���� �ȴ�. ���� 1���� ����� ���� pt_layer ����
	//SAFE_RELEASE(pt_player);

	//// ---------------------------------------------------------------------- : <<



	//// ---------------------------------------------------------------------- : >> ���� �߰�

	////// ������Ʈ�� �߰��� ���̾ ã�´�.
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
