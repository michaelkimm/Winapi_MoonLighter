#include "AssistScene.h"

#include "InGameScene.h"
#include "MapEditScene.h"
#include "SceneManager.h"

#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Tile.h"
#include "..\Object\Stage.h"

#include "..\Core\Core.h"
#include "..\Core\SourceManager.h"
#include "..\Core\TfManager.h"
#include "..\Core\InputManager.h"
#include "..\Core\Texture.h"



CAssistScene::CAssistScene()
{
}

CAssistScene::~CAssistScene()
{
	// SafeReleaseList(rect_tile_vec_);
}

bool CAssistScene::Init(HWND _hWnd)
{
	if (!CMapToolScene::Init(_hWnd))
		return false;

	ChangeBackTileSheet(_hWnd, SV_BEACH_SUMMER);
	return true;
}



void CAssistScene::Input(float _time)
{
	CMapToolScene::Input(_time);
	
	// cout << "1\n";

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
		return;


	// ���ſ� ���콺 �ٿ� �� �� ������ �ƿ�
	if (!past_mouse_down_)
		return;

	// cout << "2\n";

	// ���� ���콺 ��ġ Get
	MY_POSE mouse_pos = CInputManager::Instance()->GetMousePose();

	// pivot & ī�޶� ����ؼ� ����
	mouse_pos = ChangeMousePoseWithCam(mouse_pos);

	// ���콺 �ٿ� ��ġ ���� by camera
	MY_POSE mouse_down_pose_cam = mouse_down_pose_;
	mouse_down_pose_cam += camera_->GetPose();

	// : >> >> �� ���콺 �簢�� �׸���

	// �غ�: ���� ���콺 �ٿ� ��ġ, ���� ���콺 ��ġ, ���� �ؽ��� ���� ����, Ÿ�� �ϳ� ���� & ���� ����

	

	//--------------------------------------------------------------------------------------------//

	// ���� ���콺 �簢�� ���� ������ Ÿ�� ���� ��� �ȿ� �ֳ�?

	//--------------------------------------------------------------------------------------------//

	// 1. mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 
	// MY_POSE tmp_mouse_down_pose = (mouse_down_pose_ + camera_->GetPose()) / TEXTURE_SIZE;
	MY_POSE tmp_mouse_down_pose = (mouse_down_pose_cam) / TEXTURE_SIZE;
	int tmp_mouse_down_pose_x_idx = floor(tmp_mouse_down_pose.x);
	int tmp_mouse_down_pose_y_idx = floor(tmp_mouse_down_pose.y);
	mouse_down_pose_cam.x = tmp_mouse_down_pose_x_idx;
	mouse_down_pose_cam.y = tmp_mouse_down_pose_y_idx;
	mouse_down_pose_cam *= TEXTURE_SIZE;

	// mouse_down_pose_cam.x += TEXTURE_SIZE * 0.5;
	// mouse_down_pose_cam.y += TEXTURE_SIZE * 1;


	// mouse_down_pose_ ��ġ�� ���콺 �簢�� ���� ����
	int rect_num_x_ = abs(mouse_pos.x - mouse_down_pose_cam.x) / TEXTURE_SIZE + 1;
	int rect_num_y_ = abs(mouse_pos.y - mouse_down_pose_cam.y) / TEXTURE_SIZE + 1;

	// 2. rect_num_x_ & rect_num_y_ ���� (��ü Ÿ�� ���� ũ�⿡ �°�) 
	// ���̾� ã��
	CLayer* floor_layer = FindLayer(FLOOR_LAYER);
	if (floor_layer == nullptr) return;

	CObject* tile_sheet = floor_layer->GetObj("tileset");
	if (tile_sheet == NULL) return;


	int tileset_x_length = tile_sheet->GetSize().x / TEXTURE_SIZE;
	int tileset_y_length = tile_sheet->GetSize().y / TEXTURE_SIZE;
	SAFE_RELEASE(tile_sheet);

	if (tileset_x_length < tmp_mouse_down_pose_x_idx + rect_num_x_)
	{
		rect_num_x_ = tileset_x_length - tmp_mouse_down_pose_x_idx;
	}
	if (tileset_y_length < tmp_mouse_down_pose_y_idx + rect_num_y_)
	{
		rect_num_y_ = tileset_y_length - tmp_mouse_down_pose_y_idx;
	}

	// cout << "2.5\n";

	if (rect_num_x_ <= 0 || rect_num_y_ <= 0)
		return;

	// cout << "3\n";

	//--------------------------------------------------------------------------------------------//

	// end

	//--------------------------------------------------------------------------------------------//

	// ���̾� ã��
	CLayer* mouse_rect_layer = FindLayer(MOUSE_RECT_LAYER);
	if (mouse_rect_layer == NULL)
		return;

	// ���̾ ������Ʈ ������ ����
	mouse_rect_layer->Clear();

	// ���콺 ó�� �ٿ�� �� ���� ���� ���콺 ��ġ�� �۴ٸ� Ȥ�� ȭ��� ���� ����� �� ����
	if (mouse_pos.x < mouse_down_pose_cam.x || mouse_pos.y < mouse_down_pose_cam.y)
		return;

	// : >> �� ����
	// 1. mouse_down_pose_ ��ġ ���� (��ü Ÿ�� ���տ� �°�) 

	UpdateMousePoseWithCam();

	// cout << "4\n";

	// : >> MapEditScene ����

	// MapEditScene ���� ���� ����
	CMapEditScene* pt_map_edit_scene = static_cast<CMapEditScene*>(CSceneManager::Instance()->pt_map_edit_scene_);
	if (pt_map_edit_scene) return;
	pt_map_edit_scene->SetRectNumX(rect_num_x_);
	pt_map_edit_scene->SetRectNumY(rect_num_y_);

	// MapEditScene ���� �ִ� Ÿ�� ����
	pt_map_edit_scene->ClearTile();

	// MapEditScene Ÿ�� ������Ʈ
	int init_i = tmp_mouse_down_pose_x_idx + tmp_mouse_down_pose_y_idx * tileset_x_length;
	for (int i = 0; i < rect_num_y_; i++)
	{
		for (int j = 0; j < rect_num_x_; j++)
		{
			int idx = init_i + tileset_x_length * i;
			idx += j;
			CObject* tmp_obj = floor_layer->GetObj("tileset");
			if (tmp_obj == NULL) return;

			CTile* pt_tile = static_cast<CTile*>(tmp_obj->GetTileFromVec(idx));
			if (pt_tile)
				pt_map_edit_scene->AddTile(pt_tile);
			else
			{
				SAFE_RELEASE(tmp_obj);
				return;
			}

			SAFE_RELEASE(pt_tile);
			SAFE_RELEASE(tmp_obj);
		}
	}
	// <<

	// cout << "5\n";

	// Ÿ�� ����
	// ���� ��, ����
	CStage* pt_user_rect = CObject::CreateObj<CStage>("user_rect", STAGE_CLASS, mouse_rect_layer);
	pt_user_rect->AddTiles(mouse_down_pose_cam, rect_num_x_, rect_num_y_, TEXTURE_SIZE, TEXTURE_SIZE, EMPTY_BW_32, TEXTURE_PATH, true);
	SAFE_RELEASE(pt_user_rect);
	// if (!mouse_rect_layer->CreateTile(mouse_down_pose_cam, rect_num_x_, rect_num_y_, TEXTURE_SIZE, TEXTURE_SIZE, EMPTY_BW_32, TEXTURE_PATH))
	// 	return;

	// cout << "6\n";

	// << <<
	return;
}


void CAssistScene::Update(float _time)
{
	CMapToolScene::Update(_time);
}

void CAssistScene::LateUpdate(float _time)
{
	CMapToolScene::LateUpdate(_time);
}

void CAssistScene::Collision(float _time)
{
	CMapToolScene::Collision(_time);
}

void CAssistScene::Render(HDC _hdc, float _time)
{
	CMapToolScene::Render(_hdc, _time);
}

void CAssistScene::Save(FILE * _pt_file)
{
	CMapToolScene::Save(_pt_file);
}

void CAssistScene::Load(FILE * _pt_file)
{
	CMapToolScene::Load(_pt_file);
}
