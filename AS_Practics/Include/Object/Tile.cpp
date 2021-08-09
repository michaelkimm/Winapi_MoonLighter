#include "Tile.h"
#include "StaticObj.h"
#include "..\Core\Texture.h"
#include "..\Core\Core.h"

CTile::CTile()	:
	option_(TILE_NONE)
{
}

CTile::CTile(const CTile& _tile)	:
	CStaticObj(_tile)
{
	*this = _tile;
}

CTile::~CTile()
{
}

bool CTile::Init()
{
	return true;
}

void CTile::Input(float _time)
{
	CStaticObj::Input(_time);
}

void CTile::Update(float _time)
{
	CStaticObj::Update(_time);
}

void CTile::LateUpdate(float _time)
{
	CStaticObj::LateUpdate(_time);
}

void CTile::Collision(float _time)
{
	CStaticObj::Collision(_time);
}

void CTile::Render(HDC _hdc, float _time)
{
	// CStaticObj::Render(_hdc, _time);
	if (texture_)
	{
		if (texture_->GetDC() == NULL)
			return;

		// MY_POSE pose_in_cam = pose_ - CCameraManager::Instance()->GetPose();	 // 카메라 기준 상대 좌표
		MY_POSE pose_in_cam = pose_ - scene_->camera_->GetPose();	 // 카메라 기준 상대 좌표

		// texture 내 인덱스 위치를 텍스처 내 좌표로 변환
		MY_POSE tmp_pose = idx_int_texture_ * size_;
		int x = tmp_pose.x;
		int y = tmp_pose.y;

		// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);

		if (pose_in_cam.x >= CCore::Instance()->GetResolution().w || pose_in_cam.x + size_.x <= 0)
			return;
		else if (pose_in_cam.y >= CCore::Instance()->GetResolution().h || pose_in_cam.y + size_.y <= 0)
			return;

		TransparentBlt(_hdc, pose_in_cam.x, pose_in_cam.y, size_.x, size_.y, texture_->GetDC(), x, y, size_.x, size_.y, texture_->GetColorKey());
		// 오브젝트의 카메라 상에서 위치 = 스테이지 위치 - 윈도우 위치

		// cout << "pose_ : (" << pose_.x << ", " << pose_.y << ")\n";
		// cout << "pose_in_cam : (" << pose_in_cam.x << ", " << pose_in_cam.y << ")\n\n\n";
	}
}

CTile * CTile::Clone()
{
	CTile* pt_temp = new CTile(*this);
	pt_temp->InitRef();

	return pt_temp;
}
