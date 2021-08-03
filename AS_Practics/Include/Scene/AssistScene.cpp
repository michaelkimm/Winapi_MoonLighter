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

	// 마우스 다운 관련 변수 초기화
	mouse_down_pose_ = MY_POSE(-1.f, -1.f);
	past_mouse_down_ = false;
	mouse_up_ = false;

	CCore::Instance()->SetMapEditMode(true);

	// Assist_scene_mouse_rect

	// ---------------------------------------------------------------------- : >> 스테이지 생성

	// 레이어 찾기
	CLayer* pt_layer = FindLayer("Background");
	if (pt_layer == nullptr) return false;

	// CStage 클래스 오브젝트를 생성해서 pt_layer에 넣는다. 태그 = "stage"
	// 타일 시작 점은 씬의 pose.
	CStage* pt_stage = CObject::CreateObj<CStage>("stage", pt_layer);

	if (!pt_stage->CreateTile(pose_, 20, 20, 16, 16, SAND1, _T("sand1.bmp"), TEXTURE_PATH))
	{
		SAFE_RELEASE(pt_stage);
		return false;
	}

	// 배경 사이즈 정해질 때 카메라 내 월드 사이즈도 설정
	MY_SIZE stage_size = pt_stage->GetSize();
	SetWorldSize(stage_size);

	// 씬의 카메라 초기화
	hWnd_ = _hWnd;
	RECT rectView;
	GetClientRect(hWnd_, &rectView);
	SetWndSize(rectView.right, rectView.bottom);

	// main 화면 기준 위치, 월드 사이즈 & 씬 사이즈 & 속도 초기화
	
	camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	SAFE_RELEASE(pt_stage);

	// ---------------------------------------------------------------------- : <<

	return true;
}



void CAssistScene::Input(float _time)
{
	CScene::Input(_time);

	// 마우스 위치가 Assist 씬 위일 경우만 실행
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

	// 현재 마우스 위치 get
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

	// 해당 레이어에서 "stage" 오브젝트를 가져와 타일 집합의 x, y 방향 갯수를 알아낸다.
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



	
	// 레이어 찾기
	CLayer* pt_layer = FindLayer("Assist_scene_mouse_rect");
	if (pt_layer == NULL)
	{
		SAFE_RELEASE(pt_stage2);
		return;
	}
	// 레이어에 오브젝트 있으면 삭제
	pt_layer->Clear();

	CStage* pt_stage = CObject::CreateObj<CStage>("Assist_scene_mouse_rect", pt_layer);


	// 마우스 처음 다운된 곳 보다 현재 마우스 위치가 작다면 혹은 화면상 높은 경우의 수 제외
	if (mouse_pos.x < mouse_down_pose_.x || mouse_pos.y < mouse_down_pose_.y)
	{
		SAFE_RELEASE(pt_stage);
		SAFE_RELEASE(pt_stage2);
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
			CTile* pt_tile = pt_stage2->GetTile(idx);
			CMapEditScene::AddTile(pt_tile);

			SAFE_RELEASE(pt_tile);
		}
	}
	cout << endl << endl;
	// <<



	// 타일 생성
	// 실패 시, 리턴
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
