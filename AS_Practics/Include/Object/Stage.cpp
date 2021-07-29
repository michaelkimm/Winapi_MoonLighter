#include "Stage.h"
#include "Tile.h"
#include "..\Core\Texture.h"
#include "..\Core\CameraManager.h"
#include "..\Core\InputManager.h"

bool CStage::CreateTile(int _num_x, int _num_y, int _size_x, int _size_y, 
							const string & _texture_key, const wchar_t * _file_name, const string & _root_str)
{
	tile_x_num_ = _num_x;
	tile_y_num_ = _num_y;
	tile_width_ = _size_x;
	tile_height_ = _size_y;

	for (int i = 0; i < _num_y; i++)
	{
		for (int j = 0; j < _num_x; j++)
		{
			CTile* pt_tile = CObject::CreateObj<CTile>("water1", layer_);
			if (pt_tile == NULL) return false;

			// 텍스처 설정
			if (!pt_tile->SetTexture(_texture_key, _file_name, TEXTURE_PATH))
				return false;
			
			// 사이즈 설정
			pt_tile->SetSize(_size_x, _size_y);

			// 위치 설정
			pt_tile->SetPose(j * _size_x, i * _size_y);

			tile_vec_.push_back(pt_tile);

			SAFE_RELEASE(pt_tile);
		}
	}
	return true;
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
	SetPose(0.f, 0.f);
	tile_edit_name_ = TILE_WATER1;
	return true;
}

void CStage::Input(float _time)
{
	// 타일 이름 설정
	if (CInputManager::Instance()->GetKey1())
		tile_edit_name_ = TILE_WATER1;
	else if (CInputManager::Instance()->GetKey2())
		tile_edit_name_ = TILE_WATER2;
	else if (CInputManager::Instance()->GetKey3())
		tile_edit_name_ = TILE_SAND1;

	// : >> 클릭한 곳의 타일 정보를 알아오기
	if (CInputManager::Instance()->GetMouseLeftDown())
	{
		// 카메라 내 마우스 클릭한 곳 좌표
		MY_POSE m_pose = CInputManager::Instance()->GetMousePose();

		// 마우스 클릭한 곳의 절대 좌표
		m_pose += CCameraManager::Instance()->GetPose();

		// 2차원 타일 집합 내, 해당 타일 위치(타일 크기 고려)
		m_pose /= tile_vec_[0]->GetSize();

		int idx = floor(m_pose.y) * tile_x_num_ + floor(m_pose.x);

		// 특정 레이어 & 특정 상태일 때, 해당 타일 내 텍스쳐만 교환할 수 있어야함
		switch (tile_edit_name_)
		{
		case TILE_WATER1:
			if (!tile_vec_[idx]->SetTexture(WATER1, _T("water1.bmp"), TEXTURE_PATH))
				cout << "tile set texture 실패!\n";
			break;
		case TILE_WATER2:
			if (!tile_vec_[idx]->SetTexture(WATER2, _T("water2.bmp"), TEXTURE_PATH))
				cout << "tile set texture 실패!\n";
			break;
		case TILE_SAND1:
			if (!tile_vec_[idx]->SetTexture(SAND1, _T("sand1.bmp"), TEXTURE_PATH))
				cout << "tile set texture 실패!\n";
			break;
		}
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
	// CStaticObj::Render(_hdc, _time);
	if (texture_)
	{
		if (texture_->GetDC() == NULL)
			return;

		MY_POSE cam_size = CCameraManager::Instance()->GetWndSize();
		MY_POSE cam_pose = CCameraManager::Instance()->GetPose();

		// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);
		// TransparentBlt(_hdc, 0, 0, size_.x, size_.y, texture_->GetDC(), cam_pose.x, cam_pose.y, texture_->GetWidth(), texture_->GetHeight(), RGB(255, 0, 255));
		TransparentBlt(_hdc, pose_.x, pose_.y, cam_size.x, cam_size.y, texture_->GetDC(), cam_pose.x, cam_pose.y, cam_size.x, cam_size.y, RGB(255, 0, 255));
		// TransparentBlt(_hdc, cam_pose.x, cam_pose.y, cam_size.x, cam_size.y, texture_->GetDC(), 0, 0, cam_size.x, cam_size.y, RGB(255, 0, 255));
		// 오브젝트의 카메라 상에서 위치 = 스테이지 위치 - 윈도우 위치

		// cout << "cam_pose_ : (" << cam_pose.x << ", " << cam_pose.y << ")\n";
	}

	for (int i = 0; i < tile_y_num_; i++)
	{
		for (int j = 0; j < tile_x_num_; j++)
		{
			tile_vec_[i * tile_x_num_ + j]->Render(_hdc, _time);
		}
	}
	
}
