#include "AssistScene.h"

#include "InGameScene.h"
#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
// #include "..\Object\Stage.h"
#include "..\Core\Core.h"
#include "..\Core\SourceManager.h"
// #include "..\Core\CameraManager.h"
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

	// assist_scene_mouse_rect

	// ---------------------------------------------------------------------- : >> �������� ����

	// ���̾� ã��
	CLayer* pt_layer = FindLayer("Background");
	if (pt_layer == nullptr) return false;

	int tile_x_num_ = 1;
	int tile_y_num_ = 1;
	int tile_width_ = 16;
	int tile_height_ = 16;
	string texture_key = SV_BEACH_SUMMER;

	// ���� ������ ����
	CTexture* tmp_texture = CSourceManager::Instance()->FindTexture(texture_key);
	world_size_.x = tmp_texture->GetWidth();
	world_size_.y = tmp_texture->GetHeight();
	SAFE_RELEASE(tmp_texture);

	// ���̾� �� ũ��, ����, �ؽ�ó ������ Ÿ�� ����
	pt_layer->CreateTileImg(pose_, tile_width_, tile_height_, texture_key, _T("SV_Beach_Summer.png"), TEXTURE_PATH);

	// ���� ī�޶� �ʱ�ȭ
	hWnd_ = _hWnd;
	RECT rectview;
	GetClientRect(hWnd_, &rectview);
	SetWndSize(rectview.right, rectview.bottom);

	// main ȭ�� ���� ��ġ, ���� ������ & �� ������ & �ӵ� �ʱ�ȭ
	
	camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	// ---------------------------------------------------------------------- : <<

	return true;
}



void CAssistScene::Input(float _time)
{
	CScene::Input(_time);

	// ���콺 ��ġ�� assist �� ���� ��츸 ����
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

	// ���� ���콺 ��ġ Get
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

	int tileset_x_length = pt_layer2->GetTileXNum();
	int tileset_y_length = pt_layer2->GetTileYNum();
	if (tileset_x_length < tmp_mouse_down_pose_x_idx + rect_num_x_)
	{
		rect_num_x_ = tileset_x_length - tmp_mouse_down_pose_x_idx;
	}
	if (tileset_y_length < tmp_mouse_down_pose_y_idx + rect_num_y_)
	{
		rect_num_y_ = tileset_y_length - tmp_mouse_down_pose_y_idx;
	}

	if (rect_num_x_ <= 0 || rect_num_y_ <= 0)
		return;


	//--------------------------------------------------------------------------------------------//

	// end

	//--------------------------------------------------------------------------------------------//


	// ���̾� ã��
	CLayer* pt_layer = FindLayer("Assist_scene_mouse_rect");
	if (pt_layer == NULL)
		return;

	// ���̾ ������Ʈ ������ ����
	pt_layer->Clear();

	// CStage* pt_stage = CObject::CreateObj<CStage>("assist_scene_mouse_rect", pt_layer);


	// ���콺 ó�� �ٿ�� �� ���� ���� ���콺 ��ġ�� �۴ٸ� Ȥ�� ȭ��� ���� ����� �� ����
	if (mouse_pos.x < mouse_down_pose_.x || mouse_pos.y < mouse_down_pose_.y)
	{
		// safe_release(pt_stage);
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

	cout << "kiki9\n";

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
			CTile* pt_tile = static_cast<CTile*>(pt_layer2->GetObj(idx));
			CMapEditScene::AddTile(pt_tile);

			SAFE_RELEASE(pt_tile);
		}
	}
	cout << endl << endl;
	// <<

	// Ÿ�� ����
	// ���� ��, ����
	if (!pt_layer->CreateTile(mouse_down_pose_, rect_num_x_, rect_num_y_, 16, 16, EMPTY_BW_16, _T("empty_bw_16.bmp"), TEXTURE_PATH))
	 	return;

	cout << "kiki10\n";

	// << <<
	return;
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
