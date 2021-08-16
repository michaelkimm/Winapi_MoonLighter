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

	// 마우스 위치가 assist 씬 위일 경우만 실행
	if (CInputManager::Instance()->GetHwnd() != hWnd_)
		return;

	// 마우스가 눌려진게 아니면 업데이트할 필요 없음!
	if (!CInputManager::Instance()->GetMouseLeftDown())
		return;

	// 마우스 안누르다가 누른건지 확인
	if (CInputManager::Instance()->GetPastMouseLeftDown() == false)
	{
		mouse_down_pose_ = CInputManager::Instance()->GetMousePose();

		past_mouse_down_ = true;
	}

	if (CInputManager::Instance()->GetMouseLeftUp())
		return;


	// 과거에 마우스 다운 한 적 없으면 아웃
	if (!past_mouse_down_)
		return;

	// cout << "2\n";

	// 현재 마우스 위치 Get
	MY_POSE mouse_pos = CInputManager::Instance()->GetMousePose();

	// pivot & 카메라 고려해서 조정
	mouse_pos = ChangeMousePoseWithCam(mouse_pos);

	// 마우스 다운 위치 수정 by camera
	MY_POSE mouse_down_pose_cam = mouse_down_pose_;
	mouse_down_pose_cam += camera_->GetPose();

	// : >> >> 새 마우스 사각형 그리기

	// 준비물: 과거 마우스 다운 위치, 현재 마우스 위치, 씬의 텍스쳐 가로 갯수, 타일 하나 가로 & 세로 길이

	

	//--------------------------------------------------------------------------------------------//

	// 만들 마우스 사각형 집합 갯수가 타일 집합 경계 안에 있나?

	//--------------------------------------------------------------------------------------------//

	// 1. mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 
	// MY_POSE tmp_mouse_down_pose = (mouse_down_pose_ + camera_->GetPose()) / TEXTURE_SIZE;
	MY_POSE tmp_mouse_down_pose = (mouse_down_pose_cam) / TEXTURE_SIZE;
	int tmp_mouse_down_pose_x_idx = floor(tmp_mouse_down_pose.x);
	int tmp_mouse_down_pose_y_idx = floor(tmp_mouse_down_pose.y);
	mouse_down_pose_cam.x = tmp_mouse_down_pose_x_idx;
	mouse_down_pose_cam.y = tmp_mouse_down_pose_y_idx;
	mouse_down_pose_cam *= TEXTURE_SIZE;

	// mouse_down_pose_cam.x += TEXTURE_SIZE * 0.5;
	// mouse_down_pose_cam.y += TEXTURE_SIZE * 1;


	// mouse_down_pose_ 위치에 마우스 사각형 집합 생성
	int rect_num_x_ = abs(mouse_pos.x - mouse_down_pose_cam.x) / TEXTURE_SIZE + 1;
	int rect_num_y_ = abs(mouse_pos.y - mouse_down_pose_cam.y) / TEXTURE_SIZE + 1;

	// 2. rect_num_x_ & rect_num_y_ 조정 (전체 타일 집합 크기에 맞게) 
	// 레이어 찾기
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

	// 레이어 찾기
	CLayer* mouse_rect_layer = FindLayer(MOUSE_RECT_LAYER);
	if (mouse_rect_layer == NULL)
		return;

	// 레이어에 오브젝트 있으면 삭제
	mouse_rect_layer->Clear();

	// 마우스 처음 다운된 곳 보다 현재 마우스 위치가 작다면 혹은 화면상 높은 경우의 수 제외
	if (mouse_pos.x < mouse_down_pose_cam.x || mouse_pos.y < mouse_down_pose_cam.y)
		return;

	// : >> 값 조정
	// 1. mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 

	UpdateMousePoseWithCam();

	// cout << "4\n";

	// : >> MapEditScene 관리

	// MapEditScene 정적 변수 변경
	CMapEditScene* pt_map_edit_scene = static_cast<CMapEditScene*>(CSceneManager::Instance()->pt_map_edit_scene_);
	if (pt_map_edit_scene) return;
	pt_map_edit_scene->SetRectNumX(rect_num_x_);
	pt_map_edit_scene->SetRectNumY(rect_num_y_);

	// MapEditScene 본래 있던 타일 비우기
	pt_map_edit_scene->ClearTile();

	// MapEditScene 타일 업데이트
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

	// 타일 생성
	// 실패 시, 리턴
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
