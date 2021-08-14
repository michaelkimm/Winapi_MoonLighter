#include "Tile.h"
#include "StaticObj.h"
#include "..\Core\Texture.h"
#include "..\Core\Core.h"

CTile::CTile()	:
	idx_pose_(0, 0),
	idx_int_texture_(0, 0)
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

CTile * CTile::Clone()
{
	CTile* pt_temp = new CTile(*this);
	pt_temp->InitRef();

	return pt_temp;
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


		// 부모 위치(pivot 적용된)를 고려하여 나의 위치(pivot 적용 안됨)을 구한다.
		MY_POSE parent_pose = parent_obj_->GetPose();
		MY_POSE parent_size = parent_obj_->GetSize() / TEXTURE_SIZE;
		pose_ = MY_POSE(parent_pose.x - (parent_size.x / 2.f - idx_pose_.x - 0.5) * size_.x,
							parent_pose.y - (parent_size.y - idx_pose_.y - 1) * size_.y);

		MY_POSE pose_in_cam(0, 0);
		MY_POSE cir_pose;

		// 씬이 있으면 씬기준, 씬 없으면 부모 씬 기준
		if (scene_)
		{
			cir_pose = pose_ - scene_->camera_->GetPose();
			pose_in_cam = pose_ - scene_->camera_->GetPose() -size_ * pivot_;	// 카메라 기준 상대 좌표
		}
		else
		{
			cir_pose = pose_ - parent_obj_->GetScene()->camera_->GetPose();
			pose_in_cam = pose_ - (parent_obj_->GetScene()->camera_->GetPose()) -size_ * pivot_;
		}

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
		DrawEllipse(_hdc, POINT{ (int)cir_pose.x, (int)cir_pose.y }, 3);
	}
}

void CTile::Save(FILE * _pt_file)
{
	CStaticObj::Save(_pt_file);

	// 텍스쳐에서 자신의 위치 인덱스 저장
	fwrite(&idx_int_texture_, sizeof(idx_int_texture_), 1, _pt_file);
	fwrite(&idx_pose_, sizeof(idx_pose_), 1, _pt_file);
}

void CTile::Load(FILE * _pt_file)
{
	CStaticObj::Load(_pt_file);

	// 텍스쳐에서 자신의 위치 인덱스 불러오기
	fread(&idx_int_texture_, sizeof(idx_int_texture_), 1, _pt_file);
	fread(&idx_pose_, sizeof(idx_pose_), 1, _pt_file);
}
