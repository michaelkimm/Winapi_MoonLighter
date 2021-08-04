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

	// 마우스 다운 관련 변수 초기화
	mouse_down_pose_ = MY_POSE(-1.f, -1.f);
	past_mouse_down_ = false;
	mouse_up_ = false;

	CCore::Instance()->SetMapEditMode(true);

	// assist_scene_mouse_rect

	// ---------------------------------------------------------------------- : >> 스테이지 생성

	// 레이어 찾기
	CLayer* pt_layer = FindLayer("Background");
	if (pt_layer == nullptr) return false;

	int tile_x_num_ = 1;
	int tile_y_num_ = 1;
	int tile_width_ = 16;
	int tile_height_ = 16;
	string texture_key = SV_BEACH_SUMMER;

	// 월드 사이즈 설정
	CTexture* tmp_texture = CSourceManager::Instance()->FindTexture(texture_key);
	world_size_.x = tmp_texture->GetWidth();
	world_size_.y = tmp_texture->GetHeight();
	SAFE_RELEASE(tmp_texture);

	// 레이어 내 크기, 갯수, 텍스처 종류로 타일 생성
	pt_layer->CreateTileImg(pose_, tile_width_, tile_height_, texture_key, _T("SV_Beach_Summer.png"), TEXTURE_PATH);

	// 씬의 카메라 초기화
	hWnd_ = _hWnd;
	RECT rectview;
	GetClientRect(hWnd_, &rectview);
	SetWndSize(rectview.right, rectview.bottom);

	// main 화면 기준 위치, 월드 사이즈 & 씬 사이즈 & 속도 초기화
	
	camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	// ---------------------------------------------------------------------- : <<

	return true;
}



void CAssistScene::Input(float _time)
{
	CScene::Input(_time);

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
	{
		mouse_up_ = true;
		return;
	}

	// 과거에 마우스 다운 한 적 없으면 아웃
	if (!past_mouse_down_)
		return;

	// 현재 마우스 위치 Get
	MY_POSE mouse_pos = CInputManager::Instance()->GetMousePose();

	// : >> >> 새 마우스 사각형 그리기

	// 준비물: 과거 마우스 다운 위치, 현재 마우스 위치, 씬의 텍스쳐 가로 갯수, 타일 하나 가로 & 세로 길이

	// mouse_down_pose_ 위치에 마우스 사각형 집합 생성
	int rect_num_x_ = abs(mouse_pos.x - mouse_down_pose_.x) / 16 + 1;
	int rect_num_y_ = abs(mouse_pos.y - mouse_down_pose_.y) / 16 + 1;


	//--------------------------------------------------------------------------------------------//

	// 만들 마우스 사각형 집합 갯수가 타일 집합 경계 안에 있나?

	//--------------------------------------------------------------------------------------------//

	// 1. mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 
	MY_POSE tmp_mouse_down_pose =  mouse_down_pose_ / 16;
	int tmp_mouse_down_pose_x_idx = floor(tmp_mouse_down_pose.x);
	int tmp_mouse_down_pose_y_idx = floor(tmp_mouse_down_pose.y);

	// 2. rect_num_x_ & rect_num_y_ 조정 (전체 타일 집합 크기에 맞게) 
	// 레이어 찾기
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


	// 레이어 찾기
	CLayer* pt_layer = FindLayer("Assist_scene_mouse_rect");
	if (pt_layer == NULL)
		return;

	// 레이어에 오브젝트 있으면 삭제
	pt_layer->Clear();

	// CStage* pt_stage = CObject::CreateObj<CStage>("assist_scene_mouse_rect", pt_layer);


	// 마우스 처음 다운된 곳 보다 현재 마우스 위치가 작다면 혹은 화면상 높은 경우의 수 제외
	if (mouse_pos.x < mouse_down_pose_.x || mouse_pos.y < mouse_down_pose_.y)
	{
		// safe_release(pt_stage);
		return;
	}

	// : >> 값 조정
	// 1. mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 
	mouse_down_pose_ /= 16;
	int mouse_down_pose_x_idx = floor(mouse_down_pose_.x);
	int mouse_down_pose_y_idx = floor(mouse_down_pose_.y);
	mouse_down_pose_.x = mouse_down_pose_x_idx;
	mouse_down_pose_.y = mouse_down_pose_y_idx;
	mouse_down_pose_ *= 16;
	// <<

	cout << "kiki9\n";

	// : >> MapEditScene 관리

	// MapEditScene 정적 변수 변경
	CMapEditScene::rect_num_x_ = rect_num_x_;
	CMapEditScene::rect_num_y_ = rect_num_y_;

	// MapEditScene 본래 있던 타일 비우기
	CMapEditScene::ClearTile();

	// MapEditScene 타일 업데이트
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

	// 타일 생성
	// 실패 시, 리턴
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
