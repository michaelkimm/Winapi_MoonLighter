#include "MapToolScene.h"

#include "AssistScene.h"

#include "InGameScene.h"

#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Tile.h"

#include "..\Core\Core.h"
#include "..\Core\SourceManager.h"
#include "..\Core\TfManager.h"
#include "..\Core\InputManager.h"
#include "..\Core\Texture.h"

string CMapToolScene::edit_layer_ = FLOOR_LAYER;


CMapToolScene::CMapToolScene()
{
}

CMapToolScene::~CMapToolScene()
{
}

bool CMapToolScene::ChangeBackTileSheet(HWND _hwnd, const string & _str_key)
{
	// 텍스쳐 테그 설정
	texture_tag_ = _str_key;

	// 마우스 다운 관련 변수 초기화
	mouse_down_pose_ = MY_POSE(-1.f, -1.f);
	past_mouse_down_ = false;
	mouse_up_ = false;

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
	pt_layer->CreateTileSpriteSheet(pose_, tile_width_, tile_height_, texture_key, TEXTURE_PATH);

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
