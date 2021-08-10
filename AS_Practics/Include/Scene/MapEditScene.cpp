#include "MapEditScene.h"

#include "InGameScene.h"
#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Tile.h"
#include "..\Core\Core.h"
#include "..\Core\SourceManager.h"
#include "..\Core\InputManager.h"

void CMapEditScene::AddTile(class CTile* _t)
{
	// ������ �������� ���� �߰�
	_t->AddRef();

	// Clone�� �ڵ� ���� ī��Ʈ �߰�
	CTile* tmp_tile = _t->Clone();

	// push_back���� �߰�
	tmp_tile->AddRef();
	rect_tile_vec_.push_back(tmp_tile);
	SAFE_RELEASE(tmp_tile);
	SAFE_RELEASE(_t);
}

void CMapEditScene::ClearTile()
{
	SafeReleaseList(rect_tile_vec_);
}

void CMapEditScene::UpdateMousePoseWithCam()
{
	// ���� ���콺 ��ġ get
	mouse_pose_with_cam_ = CInputManager::Instance()->GetMousePose();

	// : >> �� ����
	// 1. mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
	mouse_pose_with_cam_ += camera_->GetPose();
	mouse_pose_with_cam_ /= TEXTURE_SIZE;
	int mouse_pose_x_idx = floor(mouse_pose_with_cam_.x);
	int mouse_pose_y_idx = floor(mouse_pose_with_cam_.y);
	mouse_pose_with_cam_.x = mouse_pose_x_idx;
	mouse_pose_with_cam_.y = mouse_pose_y_idx;
	mouse_pose_with_cam_ *= TEXTURE_SIZE;
}

CMapEditScene::CMapEditScene()	:
	edit_layer_(FLOOR_LAYER)
{
}

CMapEditScene::~CMapEditScene()
{
	CCore::Instance()->SetMapEditMode(false);
	SafeReleaseList(rect_tile_vec_);
}

bool CMapEditScene::Init(HWND _hWnd)
{
	if (!CMapToolScene::Init(_hWnd))
		return false;

	CCore::Instance()->SetMapEditMode(true);

	// ---------------------------------------------------------------------- : >> �������� ����

	int tile_x_num_ = 30;
	int tile_y_num_ = 30;
	int tile_width_ = TEXTURE_SIZE;
	int tile_height_ = TEXTURE_SIZE;
	string texture_key = WATER1;

	// ���� ������ ����
	world_size_.x = tile_width_ * tile_x_num_;
	world_size_.y = tile_height_ * tile_y_num_;

	// : >> �ٴ� ���̾� �ؽ��� �ִ� ���·� �ʱ�ȭ �ϰ� ���� ��� ���

	// ù��° ���̾� ã��
	CLayer* pt_layer = FindLayer(FLOOR_LAYER);
	if (pt_layer == nullptr) return false;

	// ���̾� �� ũ��, ����, �ؽ�ó ������ Ÿ�� ����
	pt_layer->CreateTile(pose_, tile_x_num_, tile_y_num_, tile_width_, tile_height_, texture_key, TEXTURE_PATH);

	// <<

	// : >> ��� ���̾� �� ���·� �ʱ�ȭ (ũ�� �� ���� ���� ����)
	pt_layer = NULL;

	list<class CLayer*>::iterator iter;
	list<class CLayer*>::iterator iter_end = layer_list_.end();
	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		// ��� ���̾� �� ���·� �ʱ�ȭ
		pt_layer = FindLayer((*iter)->GetTag());
		if (pt_layer == nullptr) return false;

		// ���̾� �� ũ��, ���� ����
		pt_layer->CreateTile(pose_, tile_x_num_, tile_y_num_, tile_width_, tile_height_, texture_key, TEXTURE_PATH, true);

	}
	// <<

	// ���� ī�޶� �ʱ�ȭ
	hWnd_ = _hWnd;
	RECT rectView;
	GetClientRect(hWnd_, &rectView);
	SetWndSize(rectView.right, rectView.bottom);

	// main ȭ�� ���� ��ġ, ���� ������ & �� ������ & �ӵ� �ʱ�ȭ
	camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	// ---------------------------------------------------------------------- : <<

	return true;
}


void CMapEditScene::Input(float _time)
{
	// cout << "m11\n";

	CMapToolScene::Input(_time);
	// CCameraManager::Instance()->Input(_time);

	// ���콺 ��ġ�� Assist �� ���� ��츸 ����
	if (CInputManager::Instance()->GetHwnd() != hWnd_)
	{
		// ���̾� ã��
		CLayer* pt_layer = FindLayer(MOUSE_RECT_LAYER);
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

	// cout << "m22\n";


	//--------------------------------------------------------------------------------------------//

	// ���� ���콺 �簢�� ���� ������ Ÿ�� ���� ��� �ȿ� �ֳ�?

	//--------------------------------------------------------------------------------------------//

	// 1. mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
	MY_POSE tmp_mouse_pose = mouse_pos / TEXTURE_SIZE;
	int tmp_mouse_pose_x_idx = floor(tmp_mouse_pose.x);
	int tmp_mouse_pose_y_idx = floor(tmp_mouse_pose.y);

	// 2. rect_num_x_ & rect_num_y_ ���� (��ü Ÿ�� ���� ũ�⿡ �°�) 
	// ���̾� ã��
	CLayer* pt_layer2 = FindLayer(edit_layer_);
	if (pt_layer2 == nullptr) return;

	// cout << "��׶��� Ÿ�� ����: " << pt_layer2->GetObjCnt() << endl;
	// cout << edit_layer_.c_str() << endl;

	int tileset_x_length = pt_layer2->GetTileXNum();
	int tileset_y_length = pt_layer2->GetTileYNum();
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
		// SAFE_RELEASE(pt_stage2);
		return;
	}

	// cout << "m33\n";

	//--------------------------------------------------------------------------------------------//

	// End

	//--------------------------------------------------------------------------------------------//




	// ���̾� ã��
	CLayer* pt_layer = FindLayer(MOUSE_RECT_LAYER);
	if (pt_layer == NULL)
	{
		// SAFE_RELEASE(pt_stage2);
		return;
	}

	// ���̾ ������Ʈ ������ ����
	pt_layer->Clear();


	// 1. mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
	UpdateMousePoseWithCam();


	// Ÿ�� ����
	// ���� ��, ����
	if (!pt_layer->CreateTile(mouse_pose_with_cam_, rect_num_x, rect_num_y, TEXTURE_SIZE, TEXTURE_SIZE, EMPTY_BW_32, TEXTURE_PATH))
		return;

	// cout << "m44\n";

	//--------------------------------------------------------------------------------------------//

	// ���콺 Ŭ�� �� Ÿ�� �� �ٿ��ֱ�

	//--------------------------------------------------------------------------------------------//

	// return;

	// ���콺�� �������� �ƴϸ� ������Ʈ�� �ʿ� ����!
	if (!CInputManager::Instance()->GetMouseLeftDown())
	{
		// cout << "\nhaha4\n";
		return;
	}
	// cout << "\nhaha5\n";
	
	// �ش� ���̾ rect_tile_vec_ ���� ���� ���ִ� Ÿ�� �ٿ��ֱ�
	PaintTiles(pt_layer2, mouse_pose_with_cam_, rect_tile_vec_, rect_num_x_, rect_num_y_);

	// UI ���̾ ����Ʈ ���ϵ�, �ش� �ε����� �����ϴ� �ؽ��� �ɼ��� �����ͼ� ĥ�ϱ�.
	// PaintMap(vector<class CTile*> _vec_tile, int _rect_num_x, int _rect_num_y, const string& _target_layer, MY_SIZE _world_size);

	cout << endl << endl;
	 // <<

	// cout << "m55\n";

	//--------------------------------------------------------------------------------------------//

	// End

	//--------------------------------------------------------------------------------------------//
}

void CMapEditScene::Update(float _time)
{
	CMapToolScene::Update(_time);
}

void CMapEditScene::LateUpdate(float _time)
{
	CMapToolScene::LateUpdate(_time);
}

void CMapEditScene::Collision(float _time)
{
	CMapToolScene::Collision(_time);
}

void CMapEditScene::Render(HDC _hdc, float _time)
{
	CMapToolScene::Render(_hdc, _time);
}
