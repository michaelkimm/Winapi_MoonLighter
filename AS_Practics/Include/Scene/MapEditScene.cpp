#include "MapEditScene.h"

#include "InGameScene.h"
#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Stage.h"
#include "..\Core\Core.h"
#include "..\Core\SourceManager.h"
#include "..\Core\CameraManager.h"
#include "..\Core\InputManager.h"

// ���� ���� �ʱ�ȭ
int CMapEditScene::rect_num_x_ = 0;
int CMapEditScene::rect_num_y_ = 0;
vector<class CTile*> CMapEditScene::rect_tile_vec_;\

void CMapEditScene::AddTile(class CTile* _t)
{
	// ������ �������� ���� �߰�
	_t->AddRef();

	// push_back���� �߰�
	_t->AddRef();
	rect_tile_vec_.push_back(_t);

	SAFE_RELEASE(_t);
}

void CMapEditScene::ClearTile()
{
	SafeReleaseList(rect_tile_vec_);
}

CMapEditScene::CMapEditScene()
{
}

CMapEditScene::~CMapEditScene()
{
	CCore::Instance()->SetMapEditMode(false);
	SafeReleaseList(rect_tile_vec_);
}

bool CMapEditScene::Init(HWND _hWnd)
{
	if (!CScene::Init(_hWnd))
		return false;

	CCore::Instance()->SetMapEditMode(true);

	// ---------------------------------------------------------------------- : >> �������� ����

	// ���̾� ã��
	CLayer* pt_layer = FindLayer("Background");
	if (pt_layer == nullptr) return false;

	// CStage Ŭ���� ������Ʈ�� �����ؼ� pt_layer�� �ִ´�. �±� = "stage" 
	CStage* pt_stage = CObject::CreateObj<CStage>("stage", pt_layer);

	if (!pt_stage->CreateTile(pose_, 100, 100, 16, 16, WATER1, _T("water1.bmp"), TEXTURE_PATH))
	{
		SAFE_RELEASE(pt_stage);
		return false;
	}

	/*if (!pt_stage->CreateTileImg(pose_, 16, 16, SV_BEACH_SUMMER, _T("SV_Beach_Summer.png"), TEXTURE_PATH))
	{
		SAFE_RELEASE(pt_stage);
		return false;
	}*/

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


void CMapEditScene::Input(float _time)
{
	CScene::Input(_time);
	// CCameraManager::Instance()->Input(_time);

	cout << "rect_num_x_, rect_num_y_: (" << rect_num_x_ << ", " << rect_num_y_ << ")\n";

	// ���콺 ��ġ�� Assist �� ���� ��츸 ����
	if (CInputManager::Instance()->GetHwnd() != hWnd_)
	{
		// ���̾� ã��
		CLayer* pt_layer = FindLayer("Assist_scene_mouse_rect");
		if (pt_layer == NULL) return;

		// ���̾ ������Ʈ ������ ����
		pt_layer->Clear();

		return;
	}

	// ���� ���콺 ��ġ get
	MY_POSE mouse_pos = CInputManager::Instance()->GetMousePose();

	// : >> >> �� ���콺 �簢�� �׸���

	// �غ�: ���� ���콺 �ٿ� ��ġ, ���� ���콺 ��ġ, ���� �ؽ��� ���� ����, Ÿ�� �ϳ� ���� & ���� ����

	// mouse_down_pose_ ��ġ�� ���콺 �簢�� ���� ����
	int rect_num_x = rect_num_x_;
	int rect_num_y = rect_num_y_;



	//--------------------------------------------------------------------------------------------//

	// ���� ���콺 �簢�� ���� ������ Ÿ�� ���� ��� �ȿ� �ֳ�?

	//--------------------------------------------------------------------------------------------//

	// 1. mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
	MY_POSE tmp_mouse_pose = mouse_pos / 16;
	int tmp_mouse_pose_x_idx = floor(tmp_mouse_pose.x);
	int tmp_mouse_pose_y_idx = floor(tmp_mouse_pose.y);

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
	if (tileset_x_length < tmp_mouse_pose_x_idx + rect_num_x)
	{
		rect_num_x = tileset_x_length - tmp_mouse_pose_x_idx;
	}
	if (tileset_y_length < tmp_mouse_pose_y_idx + rect_num_y)
	{
		rect_num_y = tileset_y_length - tmp_mouse_pose_x_idx;
	}

	if (rect_num_x <= 0 || rect_num_y <= 0)
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


	//// ���콺 ó�� �ٿ�� �� ���� ���� ���콺 ��ġ�� �۴ٸ� Ȥ�� ȭ��� ���� ����� �� ����
	//if (mouse_pos.x < mouse_down_pose_.x || mouse_pos.y < mouse_down_pose_.y)
	//{
	//	SAFE_RELEASE(pt_stage);
	//	return;
	//}

	// : >> �� ����
	// 1. mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
	mouse_pos /= 16;
	int mouse_pose_x_idx = floor(mouse_pos.x);
	int mouse_pose_y_idx = floor(mouse_pos.y);
	mouse_pos.x = mouse_pose_x_idx;
	mouse_pos.y = mouse_pose_y_idx;
	mouse_pos *= 16;

	// <<

	// Ÿ�� ����
	// ���� ��, ����
	if (!pt_stage->CreateTile(mouse_pos, rect_num_x, rect_num_y, 16, 16, EMPTY_BLACK_16, _T("empty_b&w_16.bmp"), TEXTURE_PATH))
	{
		SAFE_RELEASE(pt_stage);
		SAFE_RELEASE(pt_stage2);
		return;
	}



	//--------------------------------------------------------------------------------------------//

	// ���콺 Ŭ�� �� Ÿ�� �� �ٿ��ֱ�

	//--------------------------------------------------------------------------------------------//

	// ���콺�� �������� �ƴϸ� ������Ʈ�� �ʿ� ����!
	if (!CInputManager::Instance()->GetMouseLeftDown())
	{
		cout << "\nhaha4\n";
		SAFE_RELEASE(pt_stage);
		SAFE_RELEASE(pt_stage2);
		return;
	}
	cout << "\nhaha5\n";
	// MapEditScene Ÿ�� ������Ʈ
	int cnt = 0;
	int init_i = tmp_mouse_pose_x_idx + tmp_mouse_pose_y_idx * tileset_x_length;
	for (int i = 0; i < rect_num_y_; i++)
	{
		for (int j = 0; j < rect_num_x_; j++)
		{
			int idx = init_i + tileset_x_length * i;
			idx += j;
			cout << "idx : " << idx << endl;
			pt_stage2->ChangeTile(idx, rect_tile_vec_[cnt++]);
			cout << "idx haha : " << idx << endl;
		}
	}
	cout << endl << endl;
	 // <<

	//--------------------------------------------------------------------------------------------//

	// End

	//--------------------------------------------------------------------------------------------//

	SAFE_RELEASE(pt_stage);
	SAFE_RELEASE(pt_stage2);
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
