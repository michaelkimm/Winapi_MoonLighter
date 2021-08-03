#include "AssistScene.h"

#include "InGameScene.h"
#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Stage.h"
#include "..\Core\Core.h"
#include "..\Core\SourceManager.h"
#include "..\Core\CameraManager.h"
#include "..\Core\TfManager.h"
#include "..\Core\InputManager.h"
#include "MapEditScene.h"

CAssistScene::CAssistScene()
{
}

CAssistScene::~CAssistScene()
{
	SafeReleaseList(rect_tile_vec_);
}

void CAssistScene::MakeCTileVec(MY_POSE _start_pose, int _num_x, int _num_y)
{

	return;
}

bool CAssistScene::Init(HWND _hWnd)
{
	if (!CScene::Init(_hWnd))
		return false;

	// ���콺 �ٿ� ���� ���� �ʱ�ȭ
	mouse_down_pose_ = MY_POSE(-1.f, -1.f);
	past_mouse_down_ = false;
	mouse_up_ = false;

	CCore::Instance()->SetMapEditMode(true);

	// Assist_scene_mouse_rect

	// ---------------------------------------------------------------------- : >> �������� ����

	// ���̾� ã��
	CLayer* pt_layer = FindLayer("Background");
	if (pt_layer == nullptr) return false;

	// CStage Ŭ���� ������Ʈ�� �����ؼ� pt_layer�� �ִ´�. �±� = "stage"
	// Ÿ�� ���� ���� ���� pose.
	CStage* pt_stage = CObject::CreateObj<CStage>("stage", pt_layer);

	if (!pt_stage->CreateTile(pose_, 20, 20, 16, 16, SAND1, _T("sand1.bmp"), TEXTURE_PATH))
	{
		SAFE_RELEASE(pt_stage);
		return false;
	}

	// ��� ������ ������ �� ī�޶� �� ���� ����� ����
	MY_SIZE stage_size = pt_stage->GetSize();
	SetWorldSize(stage_size);

	// ���� ī�޶� �ʱ�ȭ
	hWnd_ = _hWnd;
	RECT rectView;
	GetClientRect(hWnd_, &rectView);
	SetWndSize(rectView.right, rectView.bottom);

	// main ȭ�� ���� ��ġ, ���� ������ & �� ������ & �ӵ� �ʱ�ȭ
	
	camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	SAFE_RELEASE(pt_stage);

	// ---------------------------------------------------------------------- : <<

	return true;
}



void CAssistScene::Input(float _time)
{
	CScene::Input(_time);

	// ���콺 ��ġ�� Assist �� ���� ��츸 ����
	if (CInputManager::Instance()->GetHwnd() != hWnd_)
		return;

	// ���콺�� �������� �ƴϸ� ������Ʈ�� �ʿ� ����!
	if (!CInputManager::Instance()->GetMouseLeftDown())
		return;

	// ���콺 �ȴ����ٰ� �������� Ȯ��
	if (CInputManager::Instance()->GetPastMouseLeftDown() == false)
	{
		mouse_down_pose_ = CInputManager::Instance()->GetMousePose();
		past_mouse_down_ = true;
	}

	if (CInputManager::Instance()->GetMouseLeftUp())
	{
		mouse_up_ = true;
		return;
	}


	// ���ſ� ���콺 �ٿ� �� �� ������ �ƿ�
	if (!past_mouse_down_)
		return;

	// ���� ���콺 ��ġ get
	MY_POSE mouse_pos = CInputManager::Instance()->GetMousePose();

	// : >> >> �� ���콺 �簢�� �׸���

	// �غ�: ���� ���콺 �ٿ� ��ġ, ���� ���콺 ��ġ, ���� �ؽ��� ���� ����, Ÿ�� �ϳ� ���� & ���� ����

	// mouse_down_pose_ ��ġ�� ���콺 �簢�� ���� ����
	int rect_num_x_ = abs(mouse_pos.x - mouse_down_pose_.x) / 16 + 1;
	int rect_num_y_ = abs(mouse_pos.y - mouse_down_pose_.y) / 16 + 1;



	//--------------------------------------------------------------------------------------------//

	// ���� ���콺 �簢�� ���� ������ Ÿ�� ���� ��� �ȿ� �ֳ�?

	//--------------------------------------------------------------------------------------------//

	// 1. mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
	MY_POSE tmp_mouse_down_pose =  mouse_down_pose_ / 16;
	int tmp_mouse_down_pose_x_idx = floor(tmp_mouse_down_pose.x);
	int tmp_mouse_down_pose_y_idx = floor(tmp_mouse_down_pose.y);

	// 2. rect_num_x_ & rect_num_y_ ���� (��ü Ÿ�� ���� ũ�⿡ �°�) 
	// ���̾� ã��
	CLayer* pt_layer2 = FindLayer("Background");
	if (pt_layer2 == nullptr) return;

	// �ش� ���̾�� "stage" ������Ʈ�� ������ Ÿ�� ������ x, y ���� ������ �˾Ƴ���.
	CStage* pt_stage2 = pt_layer2->FindObj<CStage>("stage");
	if (pt_stage2 == nullptr)
	{
		SAFE_RELEASE(pt_stage2);
		return;
	}

	int tileset_x_length = pt_stage2->GetTileXNum();
	int tileset_y_length = pt_stage2->GetTileYNum();
	if (tileset_x_length < tmp_mouse_down_pose_x_idx + rect_num_x_)
	{
		rect_num_x_ = tileset_x_length - tmp_mouse_down_pose_x_idx;
	}
	if (tileset_y_length < tmp_mouse_down_pose_y_idx + rect_num_y_)
	{
		rect_num_y_ = tileset_y_length - tmp_mouse_down_pose_y_idx;
	}

	if (rect_num_x_ <= 0 || rect_num_y_ <= 0)
	{
		SAFE_RELEASE(pt_stage2);
		return;
	}


	//--------------------------------------------------------------------------------------------//

	// End

	//--------------------------------------------------------------------------------------------//



	
	// ���̾� ã��
	CLayer* pt_layer = FindLayer("Assist_scene_mouse_rect");
	if (pt_layer == NULL)
	{
		SAFE_RELEASE(pt_stage2);
		return;
	}
	// ���̾ ������Ʈ ������ ����
	pt_layer->Clear();

	CStage* pt_stage = CObject::CreateObj<CStage>("Assist_scene_mouse_rect", pt_layer);


	// ���콺 ó�� �ٿ�� �� ���� ���� ���콺 ��ġ�� �۴ٸ� Ȥ�� ȭ��� ���� ����� �� ����
	if (mouse_pos.x < mouse_down_pose_.x || mouse_pos.y < mouse_down_pose_.y)
	{
		SAFE_RELEASE(pt_stage);
		SAFE_RELEASE(pt_stage2);
		return;
	}

	// : >> �� ����
	// 1. mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
	mouse_down_pose_ /= 16;
	int mouse_down_pose_x_idx = floor(mouse_down_pose_.x);
	int mouse_down_pose_y_idx = floor(mouse_down_pose_.y);
	mouse_down_pose_.x = mouse_down_pose_x_idx;
	mouse_down_pose_.y = mouse_down_pose_y_idx;
	mouse_down_pose_ *= 16;

	// <<


	// : >> MapEditScene ����

	// MapEditScene ���� ���� ����
	CMapEditScene::rect_num_x_ = rect_num_x_;
	CMapEditScene::rect_num_y_ = rect_num_y_;

	// MapEditScene ���� �ִ� Ÿ�� ����
	CMapEditScene::ClearTile();

	// MapEditScene Ÿ�� ������Ʈ
	int init_i = mouse_down_pose_x_idx + mouse_down_pose_y_idx * tileset_x_length;
	for (int i = 0; i < rect_num_y_; i++)
	{
		int idx = init_i + tileset_x_length * i;
		for (int j = 0; j < rect_num_x_; j++)
		{
			idx += j;
			CTile* pt_tile = pt_stage2->GetTile(idx);
			CMapEditScene::AddTile(pt_tile);

			SAFE_RELEASE(pt_tile);
		}
	}
	cout << endl << endl;
	// <<



	// Ÿ�� ����
	// ���� ��, ����
	if (!pt_stage->CreateTile(mouse_down_pose_, rect_num_x_, rect_num_y_, 16, 16, EMPTY_BLACK_16, _T("empty_b&w_16.bmp"), TEXTURE_PATH))
	{
		SAFE_RELEASE(pt_stage);
		SAFE_RELEASE(pt_stage2);
		return;
	}

	SAFE_RELEASE(pt_stage);
	SAFE_RELEASE(pt_stage2);

	// << <<
}

void CAssistScene::Update(float _time)
{
	CScene::Update(_time);
}

void CAssistScene::LateUpdate(float _time)
{
	CScene::LateUpdate(_time);
}

void CAssistScene::Collision(float _time)
{
	CScene::Collision(_time);
}

void CAssistScene::Render(HDC _hdc, float _time)
{
	CScene::Render(_hdc, _time);
}
