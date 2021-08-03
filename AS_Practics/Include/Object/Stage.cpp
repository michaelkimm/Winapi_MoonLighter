#include "Stage.h"
#include "Tile.h"
#include "..\Core\Texture.h"
#include "..\Core\CameraManager.h"
#include "..\Core\InputManager.h"
#include "..\Core\TfManager.h"
#include "..\Scene\SceneManager.h"

bool CStage::CreateTile(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
							const string & _texture_key, const wchar_t * _file_name, const string & _root_str)
{
	tile_x_num_ = _num_x;
	tile_y_num_ = _num_y;
	tile_width_ = _size_x;
	tile_height_ = _size_y;

	// ������ ����
	size_.x = tile_width_ * tile_x_num_;
	size_.y = tile_height_ * tile_y_num_;

	for (int i = 0; i < _num_y; i++)
	{
		for (int j = 0; j < _num_x; j++)
		{
			CTile* pt_tile = CObject::CreateObj<CTile>(_texture_key, layer_);
			if (pt_tile == NULL) return false;

			// �ؽ�ó ����
			if (!pt_tile->SetTexture(_texture_key, _file_name, TEXTURE_PATH))
				return false;
			
			// ������ ����
			pt_tile->SetSize(_size_x, _size_y);

			// ��ġ ����
			pt_tile->SetPose(_start_pose.x + j * _size_x, _start_pose.y + i * _size_y);

			tile_vec_.push_back(pt_tile);

			SAFE_RELEASE(pt_tile);
		}
	}
	return true;
}

void CStage::ChangeTile(int _idx, CTile* _t)
{
	_t->AddRef();

	CTexture* tmp_texture = _t->GetTexture();

	tile_vec_[_idx]->SetTexture(tmp_texture);
	tile_vec_[_idx]->SetOption(_t->GetOption());

	SAFE_RELEASE(tmp_texture);
	SAFE_RELEASE(_t);
}

CTile* CStage::GetTile(int _idx) const
{ 
	tile_vec_[_idx]->AddRef();
	return tile_vec_[_idx]; 
}


void CStage::MapEditSceneInput()
{
	// Ÿ�� �̸� ����
	if (CInputManager::Instance()->GetKey1())
		tile_edit_name_ = TILE_WATER1;
	else if (CInputManager::Instance()->GetKey2())
		tile_edit_name_ = TILE_WATER2;
	else if (CInputManager::Instance()->GetKey3())
		tile_edit_name_ = TILE_SAND1;

	// : >> Ŭ���� ���� Ÿ�� ������ �˾ƿ���
	if (CInputManager::Instance()->GetMouseLeftDown())
	{
		// ī�޶� �� ���콺 Ŭ���� �� ��ǥ
		MY_POSE m_pose = CInputManager::Instance()->GetMousePose();
		// cout << "\n\n\n���콺 ������ǥ mouspose: (" << m_pose.x << ", " << m_pose.y << endl;

		// cout << "mouspose: (" << m_pose.x << ", " << m_pose.y << endl;

		// ���콺 Ŭ���� ���� ���� ��ǥ
		m_pose += CCameraManager::Instance()->GetPose();
		// cout << "ī�޶� pose: (" << CCameraManager::Instance()->GetPose().x << ", " << CCameraManager::Instance()->GetPose().y << endl;
		// cout << "������ǥ mouspose: (" << m_pose.x << ", " << m_pose.y << endl;

		// 2���� Ÿ�� ���� ��, �ش� Ÿ�� ��ġ(Ÿ�� ũ�� ���)
		m_pose /= tile_vec_[0]->GetSize();

		// cout << "tile_pose: (" << m_pose.x << ", " << m_pose.y << endl;

		int idx = floor(m_pose.y) * tile_x_num_ + floor(m_pose.x);
		if (idx >= (int)tile_vec_.size())
		{
			cout << "���� range �ٱ�! idx: (" << idx << endl;
		}

		// Ư�� ���̾� & Ư�� ������ ��, �ش� Ÿ�� �� �ؽ��ĸ� ��ȯ�� �� �־����
		switch (tile_edit_name_)
		{
		case TILE_WATER1:
			if (!tile_vec_[idx]->SetTexture(WATER1, _T("water1.bmp"), TEXTURE_PATH))
				cout << "tile set texture ����!\n";
			break;
		case TILE_WATER2:
			if (!tile_vec_[idx]->SetTexture(WATER2, _T("water2.bmp"), TEXTURE_PATH))
				cout << "tile set texture ����!\n";
			break;
		case TILE_SAND1:
			if (!tile_vec_[idx]->SetTexture(SAND1, _T("sand1.bmp"), TEXTURE_PATH))
				cout << "tile set texture ����!\n";
			break;
		}
	}
}

void CStage::AssistSceneInput()
{
}

CStage::CStage()
{
}

CStage::~CStage()
{
	SafeReleaseList(tile_vec_);
}

bool CStage::Init()
{
	// MY_SIZE wnd_pose = CTfManager::Instance()->GetTf("TileSet_proc");
	// SetPose(wnd_pose.x, wnd_pose.y);
	SetPose(0.f, 0.f);

	tile_edit_name_ = TILE_WATER1;
	return true;
}

void CStage::Input(float _time)
{
	// ���� ������Ʈ�� �����ִ� ���� map edit scene�� ��� �Ʒ� �˰��� ����
	HWND this_scene = scene_->GetHwnd();
	if (this_scene == CSceneManager::Instance()->GetHwnd(MAP_EDIT_SCENE))
	{
		// CStage::MapEditSceneInput();
	}
	
	// ���� ������Ʈ�� �����ִ� ���� assist scene�� ��� �Ʒ� �˰��� ����
	else if (this_scene == CSceneManager::Instance()->GetHwnd(ASSIST_SCENE))
	{
		CStage::AssistSceneInput();
	}
}

void CStage::Update(float _time)
{
	CStaticObj::Update(_time);
}

void CStage::LateUpdate(float _time)
{
	CStaticObj::LateUpdate(_time);
}

void CStage::Collision(float _time)
{
	CStaticObj::Collision(_time);
}

void CStage::Render(HDC _hdc, float _time)
{
	MY_POSE cam_size = scene_->camera_->GetWndSize();
	MY_POSE cam_pose = scene_->camera_->GetPose();

	// CStaticObj::Render(_hdc, _time);
	if (texture_)
	{
		if (texture_->GetDC() == NULL)
			return;

		// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);
		// TransparentBlt(_hdc, 0, 0, size_.x, size_.y, texture_->GetDC(), cam_pose.x, cam_pose.y, texture_->GetWidth(), texture_->GetHeight(), RGB(255, 0, 255));
		TransparentBlt(_hdc, pose_.x, pose_.y, cam_size.x, cam_size.y, texture_->GetDC(), cam_pose.x, cam_pose.y, cam_size.x, cam_size.y, RGB(255, 0, 255));
		// TransparentBlt(_hdc, cam_pose.x, cam_pose.y, cam_size.x, cam_size.y, texture_->GetDC(), 0, 0, cam_size.x, cam_size.y, RGB(255, 0, 255));
		// ������Ʈ�� ī�޶� �󿡼� ��ġ = �������� ��ġ - ������ ��ġ

		// cout << "cam_pose_ : (" << cam_pose.x << ", " << cam_pose.y << ")\n";
	}

	HDC hMemDC = CreateCompatibleDC(_hdc);
	HBITMAP hOldBitmap;
	HBITMAP hDoubleBufferImage_ = CreateCompatibleBitmap(_hdc, cam_size.x, cam_size.y);

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage_);

	for (int i = 0; i < tile_y_num_; i++)
	{
		for (int j = 0; j < tile_x_num_; j++)
		{
			tile_vec_[i * tile_x_num_ + j]->Render(hMemDC, _time);
		}
	}
	// cout << "BitBlt cam_pose: (" << cam_pose.x << ", " << cam_pose.y << ")\n";
	BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, hMemDC, cam_pose.x, cam_pose.y, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}
