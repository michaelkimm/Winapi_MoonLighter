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



CMapToolScene::CMapToolScene()
{
}

CMapToolScene::~CMapToolScene()
{
}

bool CMapToolScene::ChangeBackTileSheet(HWND _hwnd, const string & _str_key)
{
	// �ؽ��� �ױ� ����
	texture_tag_ = _str_key;

	// ���콺 �ٿ� ���� ���� �ʱ�ȭ
	mouse_down_pose_ = MY_POSE(-1.f, -1.f);
	past_mouse_down_ = false;

	CCore::Instance()->SetMapEditMode(true);

	// ---------------------------------------------------------------------- : >> �������� ����

	// ���̾� ã��
	CLayer* pt_layer = FindLayer(FLOOR_LAYER);
	if (pt_layer == nullptr) return false;

	// ���� ���̾� ����
	pt_layer->Clear();

	cout << "ChangeBackTileSheet 2\n";

	int tile_width_ = TEXTURE_SIZE;
	int tile_height_ = TEXTURE_SIZE;
	string texture_key = _str_key;

	// ���� ������ ����
	CTexture* tmp_texture = CSourceManager::Instance()->FindTexture(texture_key);
	world_size_.x = tmp_texture->GetWidth();
	world_size_.y = tmp_texture->GetHeight();

	SAFE_RELEASE(tmp_texture);

	cout << "ChangeBackTileSheet 3\n";

	// ���̾� �� ũ��, ����, �ؽ�ó ������ Ÿ�� ����
	pt_layer->CreateTileSpriteSheet(pose_, tile_width_, tile_height_, texture_key, TEXTURE_PATH);

	cout << "ChangeBackTileSheet 4\n";

	// ���� ī�޶� �ʱ�ȭ
	hWnd_ = _hwnd;
	RECT rectView;
	GetClientRect(_hwnd, &rectView);
	SetWndSize(rectView.right, rectView.bottom);

	// main ȭ�� ���� ��ġ, ���� ������ & �� ������ & �ӵ� �ʱ�ȭ
	camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	// ---------------------------------------------------------------------- : <<

	return true;
}

bool CMapToolScene::PaintMap(vector<class CTile*> _vec_tile, int _rect_num_x, int _rect_num_y, const string& _target_layer, MY_SIZE _world_size)
{
	// ������ �簢���� ���� ���� �ȼ��� ũ��
	int rect_dx = _rect_num_x * TEXTURE_SIZE;
	int rect_dy = _rect_num_y * TEXTURE_SIZE;

	CLayer* pt_layer = NULL;

	// ������ (_rect_num_x * _rect_num_y) �簢������ ��ü ĥ�ϱ�
	for (int j = 0; j < _world_size.y; j += rect_dy)
	{
		for (int i = 0; i < _world_size.x; i += rect_dx)
		{
			// ���̾� ã��
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
	// ������ �簢���� ���� ���� �ȼ��� ũ��
	int rect_dx = _rect_num_x * TEXTURE_SIZE;
	int rect_dy = _rect_num_y * TEXTURE_SIZE;

	CLayer* pt_layer = NULL;

	// ������ (_rect_num_x * _rect_num_y) �簢������ ��ü ĥ�ϱ�
	for (int j = 0; j < _world_size.y; j += rect_dy)
	{
		for (int i = 0; i < _world_size.x; i += rect_dx)
		{
			// ���̾� ã��
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
			// Clone�� �ڵ� ���� ī��Ʈ �߰�
			tmp_tile = _rect_tile_vec[cnt++]->Clone();
			tmp_tile->SetPose(_pose.x + j * TEXTURE_SIZE, _pose.y + i * TEXTURE_SIZE);

			_target_layer->AddObj(tmp_tile, true);

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
			// Clone�� �ڵ� ���� ī��Ʈ �߰�
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

			// Ÿ�� ����
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
