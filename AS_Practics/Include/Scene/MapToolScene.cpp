#include "MapToolScene.h"

#include "AssistScene.h"

#include "InGameScene.h"

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



CMapToolScene::CMapToolScene()	:
	prev_mouse_pose_with_cam_idx_(0, 0)
{
}

CMapToolScene::~CMapToolScene()
{
}

void CMapToolScene::UpdateMousePoseWithCam()
{
	// 현재 마우스 위치 get
	mouse_pose_with_cam_ = CInputManager::Instance()->GetMousePose();

	// : >> 값 조정
	// 1. mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 

	mouse_pose_with_cam_ += camera_->GetPose();

	mouse_pose_with_cam_ /= TEXTURE_SIZE;
	int mouse_pose_x_idx = floor(mouse_pose_with_cam_.x);
	int mouse_pose_y_idx = floor(mouse_pose_with_cam_.y);
	mouse_pose_with_cam_.x = mouse_pose_x_idx;
	mouse_pose_with_cam_.y = mouse_pose_y_idx;
	mouse_pose_with_cam_idx_ = mouse_pose_with_cam_;
	mouse_pose_with_cam_ *= TEXTURE_SIZE;

	// mouse_pose_with_cam_.x += TEXTURE_SIZE * 0.5;
	// mouse_pose_with_cam_.y += TEXTURE_SIZE * 1;

	// 피봇으로 위치 조정
	// mouse_pose_with_cam_.x -= TEXTURE_SIZE * 0.5;
	// mouse_pose_with_cam_.y -= TEXTURE_SIZE * 1;
}

MY_POSE CMapToolScene::ChangeMousePoseWithCam(MY_POSE _pose)
{
	// : >> 값 조정
	// 1. _pose 위치 조정 (전체 타일 집합에 맞게 & pivot 고려) 

	_pose += camera_->GetPose();

	_pose /= TEXTURE_SIZE;
	int mouse_pose_x_idx = floor(_pose.x);
	int mouse_pose_y_idx = floor(_pose.y);
	_pose.x = mouse_pose_x_idx;
	_pose.y = mouse_pose_y_idx;
	_pose *= TEXTURE_SIZE;

	// _pose.x += TEXTURE_SIZE * 0.5;
	// _pose.y += TEXTURE_SIZE * 1;

	return _pose;
}

bool CMapToolScene::ChangeBackTileSheet(HWND _hwnd, const string & _str_key)
{
	// 텍스쳐 테그 설정
	texture_tag_ = _str_key;

	// 마우스 다운 관련 변수 초기화
	mouse_down_pose_ = MY_POSE(-1.f, -1.f);
	past_mouse_down_ = false;

	CCore::Instance()->SetMapEditMode(true);

	// ---------------------------------------------------------------------- : >> 스테이지 생성

	// 레이어 찾기
	CLayer* pt_layer = FindLayer(FLOOR_LAYER);
	if (pt_layer == nullptr) return false;

	// 기존 레이어 비우기
	pt_layer->Clear();

	cout << "ChangeBackTileSheet 2\n";

	int tile_width_ = TEXTURE_SIZE;
	int tile_height_ = TEXTURE_SIZE;
	string texture_key = _str_key;

	// 월드 사이즈 설정
	CTexture* tmp_texture = CSourceManager::Instance()->FindTexture(texture_key);
	world_size_.x = tmp_texture->GetWidth();
	world_size_.y = tmp_texture->GetHeight();

	SAFE_RELEASE(tmp_texture);

	cout << "ChangeBackTileSheet 3\n";

	// 레이어 내 크기, 갯수, 텍스처 종류로 타일 생성
	CStage* tile_sheet = CObject::CreateObj<CStage>("tileset", pt_layer);

	tile_sheet->AddTiles(pose_, world_size_.x / TEXTURE_SIZE, world_size_.y / TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_SIZE, texture_key);

	SAFE_RELEASE(tile_sheet);

	// pt_layer->CreateTileSpriteSheet(pose_, tile_width_, tile_height_, texture_key, TEXTURE_PATH);

	cout << "ChangeBackTileSheet 4\n";

	// 씬의 카메라 초기화
	hWnd_ = _hwnd;
	RECT rectView;
	GetClientRect(_hwnd, &rectView);
	SetWndSize(rectView.right, rectView.bottom);

	// main 화면 기준 위치, 월드 사이즈 & 씬 사이즈 & 속도 초기화
	camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	// ---------------------------------------------------------------------- : <<

	return true;
}

bool CMapToolScene::PaintMap(vector<class CTile*> _vec_tile, int _rect_num_x, int _rect_num_y, const string& _target_layer, MY_SIZE _world_size)
{
	// 선택한 사각형의 가로 세로 픽셀합 크기
	int rect_dx = _rect_num_x * TEXTURE_SIZE;
	int rect_dy = _rect_num_y * TEXTURE_SIZE;

	CLayer* pt_layer = NULL;

	// 지정된 (_rect_num_x * _rect_num_y) 사각형으로 전체 칠하기
	for (int j = 0; j < _world_size.y; j += rect_dy)
	{
		for (int i = 0; i < _world_size.x; i += rect_dx)
		{
			// 레이어 찾기
			pt_layer = FindLayer(_target_layer);
			if (pt_layer == nullptr) return false;

			MY_POSE tmp = MY_POSE(i, j);
			if (((int)(tmp.x + rect_dx) > (int)(_world_size.x)) || ((int)(tmp.y + rect_dy) > (int)(_world_size.y)))
				continue;

			// PaintTiles2(CLayer* _target_layer, MY_POSE _pose, vector<CTile*> _rect_tile_vec, int _x_length, int _y_length)
			PaintTiles(pt_layer, tmp, _vec_tile, _rect_num_x, _rect_num_y);
		}
	}
	return true;
}

bool CMapToolScene::PaintMap2(vector<class CTile*> _vec_tile, int _rect_num_x, int _rect_num_y, const string& _target_layer, MY_SIZE _world_size, const string& _texture_key)
{
	// 선택한 사각형의 가로 세로 픽셀합 크기
	int rect_dx = _rect_num_x * TEXTURE_SIZE;
	int rect_dy = _rect_num_y * TEXTURE_SIZE;

	CLayer* pt_layer = NULL;

	// 지정된 (_rect_num_x * _rect_num_y) 사각형으로 전체 칠하기
	for (int j = 0; j < _world_size.y; j += rect_dy)
	{
		for (int i = 0; i < _world_size.x; i += rect_dx)
		{
			// 레이어 찾기
			pt_layer = FindLayer(_target_layer);
			if (pt_layer == nullptr) return false;

			MY_POSE tmp = MY_POSE(i, j);
			if (((int)(tmp.x + rect_dx) > (int)(_world_size.x)) || ((int)(tmp.y + rect_dy) > (int)(_world_size.y)))
				continue;

			// PaintTiles2(CLayer* _target_layer, MY_POSE _pose, vector<CTile*> _rect_tile_vec, int _x_length, int _y_length)
			PaintTiles2(pt_layer, tmp, _vec_tile, _rect_num_x, _rect_num_y, i / rect_dx, j / rect_dy, _texture_key);
		}
	}
	return true;
}

void CMapToolScene::PaintTiles(CLayer* _target_layer, MY_POSE _pose, vector<CTile*> _rect_tile_vec, int _x_length, int _y_length)
{
	int cnt = 0;
	CTile* tmp_tile = NULL;
	for (int i = 0; i < _y_length; i++)
	{
		for (int j = 0; j < _x_length; j++)
		{
			// Clone은 자동 참조 카운트 추가
			tmp_tile = _rect_tile_vec[cnt++]->Clone();
			tmp_tile->SetPose(_pose.x + j * TEXTURE_SIZE, _pose.y + i * TEXTURE_SIZE);

			// MY_POSE tmp_pose(_pose.x + j * TEXTURE_SIZE, _pose.y + i * TEXTURE_SIZE);
			// cout << "붙여 놓는 인덱스: (" << tmp_pose.x / TEXTURE_SIZE << ", " << tmp_pose.y / TEXTURE_SIZE << ")\n";

			_target_layer->AddObj(tmp_tile);

			SAFE_RELEASE(tmp_tile);
		}
	}
}

void CMapToolScene::PaintTiles2(CLayer* _target_layer, MY_POSE _pose, vector<CTile*> _rect_tile_vec, int _x_length, int _y_length, int _x, int _y, const string& _texture_key)
{
	CTexture* _t = CSourceManager::Instance()->FindTexture(_texture_key);
	if (_t == NULL) return;

	int cnt = 0;
	CTile* tmp_tile = NULL;
	for (int i = 0; i < _y_length; i++)
	{
		for (int j = 0; j < _x_length; j++)
		{
			// Clone은 자동 참조 카운트 추가
			switch (_t->GetOptionVec(_x, _y))
			{
			case TILE_NONE:
				cnt = 0;
				break;
			case TILE_NOMOVE:
				cnt = 1;
				break;
			case TILE_TRANSPORT:
				cnt = 2;
				break;
			}

			// 타일 생성
			tmp_tile = _rect_tile_vec[cnt]->Clone();
			tmp_tile->SetPose(_pose.x + j * TEXTURE_SIZE, _pose.y + i * TEXTURE_SIZE);

			_target_layer->AddObj(tmp_tile);

			SAFE_RELEASE(tmp_tile);
		}
	}
	SAFE_RELEASE(_t);

}

bool CMapToolScene::Init(HWND _hWnd)
{
	if (!CScene::Init(_hWnd))
		return false;

	return true;
}

void CMapToolScene::Input(float _time)
{
	CScene::Input(_time);
}

void CMapToolScene::Update(float _time)
{
	CScene::Update(_time);
}

void CMapToolScene::LateUpdate(float _time)
{
	CScene::LateUpdate(_time);
}

void CMapToolScene::Collision(float _time)
{
	CScene::Collision(_time);
}

void CMapToolScene::Render(HDC _hdc, float _time)
{
	CScene::Render(_hdc, _time);
}

void CMapToolScene::Save(FILE * _pt_file)
{
	CScene::Save(_pt_file);
}

void CMapToolScene::Load(FILE * _pt_file)
{
	CScene::Load(_pt_file);
}
