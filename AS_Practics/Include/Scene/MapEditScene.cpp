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
	// 포인터 생성으로 인해 추가
	_t->AddRef();

	// Clone은 자동 참조 카운트 추가
	CTile* tmp_tile = _t->Clone();

	// push_back으로 추가
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
}

CMapEditScene::CMapEditScene()	:
	edit_layer_(FLOOR_LAYER),
	prev_mouse_pose_with_cam_idx_(0, 0)
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

	// ---------------------------------------------------------------------- : >> 스테이지 생성

	int tile_x_num_ = 30;
	int tile_y_num_ = 30;
	int tile_width_ = TEXTURE_SIZE;
	int tile_height_ = TEXTURE_SIZE;
	string texture_key = WATER1;

	// 월드 사이즈 설정
	world_size_.x = tile_width_ * tile_x_num_;
	world_size_.y = tile_height_ * tile_y_num_;

	// : >> 바닥 레이어 텍스쳐 있는 상태로 초기화 하고 싶은 경우 사용

	// 첫번째 레이어 찾기
	CLayer* pt_layer = FindLayer(FLOOR_LAYER);
	if (pt_layer == nullptr) return false;

	// 레이어 내 크기, 갯수, 텍스처 종류로 타일 생성
	pt_layer->CreateTile(pose_, tile_x_num_, tile_y_num_, tile_width_, tile_height_, texture_key, TEXTURE_PATH);

	// <<

	// : >> 모든 레이어 빈 상태로 초기화 (크기 등 내부 상태 설정)
	pt_layer = NULL;

	list<class CLayer*>::iterator iter;
	list<class CLayer*>::iterator iter_end = layer_list_.end();
	for (iter = layer_list_.begin(); iter != iter_end; iter++)
	{
		// 모든 레이어 빈 상태로 초기화
		pt_layer = FindLayer((*iter)->GetTag());
		if (pt_layer == nullptr) return false;

		// 레이어 내 크기, 갯수 설정
		pt_layer->CreateTile(pose_, tile_x_num_, tile_y_num_, tile_width_, tile_height_, texture_key, TEXTURE_PATH, true);

	}
	// <<

	// 씬의 카메라 초기화
	hWnd_ = _hWnd;
	RECT rectView;
	GetClientRect(hWnd_, &rectView);
	SetWndSize(rectView.right, rectView.bottom);

	// main 화면 기준 위치, 월드 사이즈 & 씬 사이즈 & 속도 초기화
	camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	// ---------------------------------------------------------------------- : <<

	return true;
}


void CMapEditScene::Input(float _time)
{
	CMapToolScene::Input(_time);
	// CCameraManager::Instance()->Input(_time);

	CLayer* pt_layer = FindLayer(MOUSE_RECT_LAYER);
	if (pt_layer == NULL) return;

	// 레이어에 오브젝트 있으면 삭제
	pt_layer->Clear();

	// 마우스 위치가 Assist 씬 위일 경우만 실행
	if (CInputManager::Instance()->GetHwnd() != hWnd_)
		return;


	// 현재 마우스 위치 get
	MY_POSE mouse_pos = CInputManager::Instance()->GetMousePose();

	// : >> >> 새 마우스 사각형 그리기

	// 준비물: 과거 마우스 다운 위치, 현재 마우스 위치, 씬의 텍스쳐 가로 갯수, 타일 하나 가로 & 세로 길이

	// mouse_down_pose_ 위치에 마우스 사각형 집합 생성
	int rect_num_x = rect_num_x_;
	int rect_num_y = rect_num_y_;


	//--------------------------------------------------------------------------------------------//

	// 만들 마우스 사각형 집합 갯수가 타일 집합 경계 안에 있나?

	//--------------------------------------------------------------------------------------------//

	// 1. mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 
	MY_POSE tmp_mouse_pose = mouse_pos / TEXTURE_SIZE;
	int tmp_mouse_pose_x_idx = floor(tmp_mouse_pose.x);
	int tmp_mouse_pose_y_idx = floor(tmp_mouse_pose.y);

	// 2. rect_num_x_ & rect_num_y_ 조정 (전체 타일 집합 크기에 맞게) 
	// 레이어 찾기
	CLayer* pt_layer2 = FindLayer(edit_layer_);
	if (pt_layer2 == nullptr) return;


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
		return;

	//--------------------------------------------------------------------------------------------//

	// End

	//--------------------------------------------------------------------------------------------//


	// mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 
	UpdateMousePoseWithCam();


	//--------------------------------------------------------------------------------------------//

	// Delete 기능 생성

	//--------------------------------------------------------------------------------------------//
	CLayer* pt_targer_layer = NULL;
	if (CInputManager::Instance()->GetKeyDel() && CInputManager::Instance()->GetKeyA() && CInputManager::Instance()->GetKeyCtrl())
	{
		// Delete + 마우스 + 스페이스바 다운일 경우 모두 제거
		cout << "다 지워!\n";
		pt_layer2->DeleteAll();
	}

	// Delete 키가 눌러진 상태로 마우스 누르면 발동
	if (CInputManager::Instance()->GetKeyDel() && CInputManager::Instance()->GetMouseLeftDown())
	{
		
		// 방금 제거했던 위치면
		if (prev_mouse_pose_with_cam_idx_ == mouse_pose_with_cam_idx_)
			return;

		// 현재 레이어에서 마우스 좌표쪽에 해당되는 인덱스의 타일 제거
		pt_layer2->DeleteObj(mouse_pose_with_cam_idx_.x, mouse_pose_with_cam_idx_.y);

		prev_mouse_pose_with_cam_idx_ = mouse_pose_with_cam_idx_;

		return;
	}

   //--------------------------------------------------------------------------------------------//

   // End

   //--------------------------------------------------------------------------------------------//


	// 타일 생성
	// 실패 시, 리턴
	if (!pt_layer->CreateTile(mouse_pose_with_cam_, rect_num_x, rect_num_y, TEXTURE_SIZE, TEXTURE_SIZE, EMPTY_BW_32, TEXTURE_PATH))
		return;


	//--------------------------------------------------------------------------------------------//

	// 마우스 클릭 시 타일 값 붙여넣기

	//--------------------------------------------------------------------------------------------//

	// 마우스가 눌려진게 아니면 업데이트할 필요 없음!
	if (!CInputManager::Instance()->GetMouseLeftDown())
		return;

	static MY_POSE prev_add_pose(0, 0);

	// 방금 추가했던 위치면 다시 추가x
	if (prev_add_pose == mouse_pose_with_cam_)
		return;

	// 해당 레이어에 rect_tile_vec_ 내에 저장 돼있던 타일 붙여넣기
	PaintTiles(pt_layer2, mouse_pose_with_cam_, rect_tile_vec_, rect_num_x_, rect_num_y_);

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
