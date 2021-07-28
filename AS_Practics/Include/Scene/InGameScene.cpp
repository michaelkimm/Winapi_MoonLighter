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
	// �θ� Scene Ŭ������ �ʱ�ȭ �Լ��� ȣ�����ش�.
	// ����??
	if (!CScene::Init())
		return false;

	class CLayer*	pt_layer;

	// ---------------------------------------------------------------------- : >> ��� �߰�
	
	// ��� ���̾� ã��
	pt_layer = FindLayer("Background");
	if (pt_layer == NULL) return false;

	// ��ü �����Ͽ� ���̾ �߰�
	class CStage* pt_stage = CObject::CreateObj<CStage>("Map1", pt_layer);
	if (pt_layer == NULL) return false;

	// �ؽ�ó & ��ü size ����
	pt_stage->SetTexture("Background", _T("3000.bmp"), TEXTURE_PATH);
	if (pt_stage->GetTexture() == NULL) return false;

	SAFE_RELEASE(pt_stage);

	// ---------------------------------------------------------------------- : <<



	

	// ---------------------------------------------------------------------- : >> �÷��̾� �߰�

	// ������Ʈ�� �߰��� ���̾ ã�´�.
	pt_layer = FindLayer("Player");
	if (pt_layer == NULL) return false;

	// �߰��� ������Ʈ�� �����Ѵ�. ���� �߰��� ���̾ �߰��Ѵ�.
	class CPlayer*	pt_player = CObject::CreateObj<CPlayer>("Player", pt_layer);

	// �ؽ�ó & ��ü size ����
	pt_player->SetTexture(PLAYER_TAG, _T("sigong.bmp"), TEXTURE_PATH);
	if (pt_player->GetTexture() == NULL)
		return false;

	// CreateObj�ؼ� ���̾ �߰����� �ϸ� ���� ī��Ʈ�� 2���� �ȴ�. ���� 1���� ����� ���� pt_layer ����
	SAFE_RELEASE(pt_player);

	// ---------------------------------------------------------------------- : <<



	// ---------------------------------------------------------------------- : >> ���� �߰�

	// ������Ʈ�� �߰��� ���̾ ã�´�.
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
