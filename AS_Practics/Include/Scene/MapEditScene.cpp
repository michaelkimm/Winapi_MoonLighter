#include "MapEditScene.h"

#include "InGameScene.h"
#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Tile.h"
#include "..\Object\Stage.h"
#include "..\Object\UIObj.h"
#include "..\Object\NatureObj.h"
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
	CLayer* pt_mouse_rect_layer = FindLayer(FLOOR_LAYER);
	if (pt_mouse_rect_layer == nullptr) return false;

	//--------------------------------------------------------------------------------------------//

	// ���̾ �������� ���� �� ���̾� ũ�� �� Ư�� ����

	//--------------------------------------------------------------------------------------------//


	// ���̾� �� ũ��, ����, �ؽ�ó ������ Ÿ�� ����
	// pt_mouse_rect_layer->CreateTile(pose_, tile_x_num_, tile_y_num_, tile_width_, tile_height_, texture_key, TEXTURE_PATH);

	// ���̾�� ������Ʈ�� �����
	CObject* stage = CObject::CreateObj<CStage>("stage1", STAGE_CLASS, pt_mouse_rect_layer);

	// ������Ʈ�� ���� ��, ������ Ÿ�� ������ ������ ������ Ÿ�� ���� ����
	stage->AddTiles(pose_, tile_x_num_, tile_y_num_, tile_width_, tile_height_, texture_key, TEXTURE_PATH, true);


	SAFE_RELEASE(stage);


	// : >> ��� ���̾� �� ���·� �ʱ�ȭ (ũ�� �� ���� ���� ����)
	pt_mouse_rect_layer = NULL;

	list<class CLayer*>::iterator iter;
	list<class CLayer*>::iterator iter_end = layer_list_.end();
	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		// ��� ���̾� �� ���·� �ʱ�ȭ
		pt_mouse_rect_layer = FindLayer((*iter)->GetTag());
		if (pt_mouse_rect_layer == nullptr) return false;

		// ���̾� �� ũ��, ���� ����
		pt_mouse_rect_layer->CreateTile(pose_, tile_x_num_, tile_y_num_, tile_width_, tile_height_, texture_key, TEXTURE_PATH, true);
	}
	
	// <<

	//--------------------------------------------------------------------------------------------//

	// End

	//--------------------------------------------------------------------------------------------//

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
	CMapToolScene::Input(_time);
	// CCameraManager::Instance()->Input(_time);

	CLayer* pt_mouse_rect_layer = FindLayer(MOUSE_RECT_LAYER);
	if (pt_mouse_rect_layer == NULL) return;

	// ���̾ ������Ʈ ������ ����
	pt_mouse_rect_layer->Clear();

	// ���콺 ��ġ�� Assist �� ���� ��츸 ����
	if (CInputManager::Instance()->GetHwnd() != hWnd_)
		return;


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
	MY_POSE tmp_mouse_pose = mouse_pos / TEXTURE_SIZE;
	int tmp_mouse_pose_x_idx = floor(tmp_mouse_pose.x);
	int tmp_mouse_pose_y_idx = floor(tmp_mouse_pose.y);

	// 2. rect_num_x_ & rect_num_y_ ���� (��ü Ÿ�� ���� ũ�⿡ �°�) 
	// ���̾� ã��
	CLayer* pt_edit_layer = FindLayer(edit_layer_);
	if (pt_edit_layer == nullptr) return;


	int tileset_x_length = pt_edit_layer->GetTileXNum();
	int tileset_y_length = pt_edit_layer->GetTileYNum();
	if (tileset_x_length < tmp_mouse_pose_x_idx + rect_num_x)
	{
		rect_num_x = tileset_x_length - tmp_mouse_pose_x_idx;
	}
	if (tileset_y_length < tmp_mouse_pose_y_idx + rect_num_y)
	{
		rect_num_y = tileset_y_length - tmp_mouse_pose_x_idx;
	}

	if (rect_num_x <= 0 || rect_num_y <= 0)
		return;

	//--------------------------------------------------------------------------------------------//

	// End

	//--------------------------------------------------------------------------------------------//


	// mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
	UpdateMousePoseWithCam();


	//--------------------------------------------------------------------------------------------//

	// Delete ��� ����

	//--------------------------------------------------------------------------------------------//
	CLayer* pt_targer_layer = NULL;
	if (CInputManager::Instance()->GetKeyDel() && CInputManager::Instance()->GetKeyA() && CInputManager::Instance()->GetKeyCtrl())
	{
		// Delete + ���콺 + �����̽��� �ٿ��� ��� ��� ����
		cout << "�� ����!\n";
		pt_edit_layer->DeleteAll();
	}

	// Delete Ű�� ������ ���·� ���콺 ������ �ߵ�
	if (CInputManager::Instance()->GetKeyDel() && CInputManager::Instance()->GetMouseLeftDown())
	{
		// ��� �����ߴ� ��ġ��
		if (prev_mouse_pose_with_cam_idx_ == mouse_pose_with_cam_idx_)
			return;


		if (pt_edit_layer->GetTag() == FLOOR_LAYER)
		{
			CObject* tmp_obj_stage = pt_edit_layer->GetObj("stage1");
			if (tmp_obj_stage == NULL) return;
			tmp_obj_stage->DeleteTileInVec(mouse_pose_with_cam_idx_.x, mouse_pose_with_cam_idx_.y);
			SAFE_RELEASE(tmp_obj_stage);
		}
		else if (pt_edit_layer->GetTag() == MAP_OBJ_LAYER)
		{
			pt_edit_layer->DeleteObj(mouse_pose_with_cam_ + MY_POSE(0.5f, 0.5f) * TEXTURE_SIZE);
		}
		
		// ���� ���̾�� ���콺 ��ǥ�ʿ� �ش�Ǵ� �ε����� Ÿ�� ����
		// pt_edit_layer->DeleteObj(mouse_pose_with_cam_idx_.x, mouse_pose_with_cam_idx_.y);

		prev_mouse_pose_with_cam_idx_ = mouse_pose_with_cam_idx_;

		return;
	}


   //--------------------------------------------------------------------------------------------//

   // End

   //--------------------------------------------------------------------------------------------//


	//// Ÿ�� ����
	//// ���� ��, ����
	//if (!pt_mouse_rect_layer->CreateTile(mouse_pose_with_cam_, rect_num_x, rect_num_y, TEXTURE_SIZE, TEXTURE_SIZE, EMPTY_BW_32, TEXTURE_PATH))
	//	return;


	// ���̾�� ������Ʈ�� �����
	CObject* ui_obj = CObject::CreateObj<CUIObj>("user_rect", UI_OBJ_CLASS, pt_mouse_rect_layer);

	// ������Ʈ�� ���� ��, ������ Ÿ�� ������ ������ ������ Ÿ�� ���� ����
	ui_obj->AddTiles(mouse_pose_with_cam_, rect_num_x, rect_num_y, TEXTURE_SIZE, TEXTURE_SIZE, EMPTY_BW_32, TEXTURE_PATH, true);

	SAFE_RELEASE(ui_obj);

	//--------------------------------------------------------------------------------------------//

	// ���콺 Ŭ�� �� Ÿ�� �� �ٿ��ֱ�

	//--------------------------------------------------------------------------------------------//

	// ���콺�� �������� �ƴϸ� ������Ʈ�� �ʿ� ����!
	if (!CInputManager::Instance()->GetMouseLeftDown())
		return;

	static MY_POSE prev_add_pose(0, 0);

	// ��� �߰��ߴ� ��ġ�� �ٽ� �߰�x
	if (prev_add_pose == mouse_pose_with_cam_)
		return;

	// �ش� ���̾ rect_tile_vec_ ���� ���� ���ִ� Ÿ�� �ٿ��ֱ�
	if (pt_edit_layer->GetTag() == FLOOR_LAYER)
	{
		// ���̾�� ������Ʈ ã�´�
		CObject* tmp_obj = pt_edit_layer->GetObj("stage1");
		if (tmp_obj == NULL) return;

		tmp_obj->AddTiles(rect_tile_vec_, rect_num_x, rect_num_y, mouse_pose_with_cam_idx_, false);

		SAFE_RELEASE(tmp_obj);
	}
	else if (pt_edit_layer->GetTag() == MAP_OBJ_LAYER)
	{
		// ���̾�� ������Ʈ�� �����
		CNatureObj* nature_obj = CObject::CreateObj<CNatureObj>("user_rect", NATURE_OBJ_CLASS, pt_edit_layer, mouse_pose_with_cam_);

		// ������Ʈ�� ���� ��, ������ Ÿ�� ������ ������ ������ Ÿ�� ���� ����
		// ���� �� ���� ��Ʈ!
		nature_obj->AddTiles(rect_tile_vec_, rect_num_x, rect_num_y, MY_POSE(0, 0), true, true);
		

		pt_edit_layer->SortObjListZOrder();
		SAFE_RELEASE(nature_obj);
	}
	
	prev_add_pose = mouse_pose_with_cam_;

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

void CMapEditScene::Save(FILE * _pt_file)
{
	CMapToolScene::Save(_pt_file);
}

void CMapEditScene::Load(FILE * _pt_file)
{
	CMapToolScene::Load(_pt_file);
}
