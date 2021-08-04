#include "Tile.h"
#include "StaticObj.h"
#include "..\Core\Texture.h"
#include "..\Core\Texture.h"
#include "..\Core\SourceManager.h"
#include "..\Core\CameraManager.h"

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

//bool CTile::SetTileTexture(int _srcx, int _srcy, int _srcwidth, int _srcheight, const string& _texture_key,
//								const wchar_t* _pFileName, const string& _str_path_key, const Color& _color_key)
//{
//	SAFE_RELEASE(texture_);
//	texture_ = CSourceManager::Instance()->LoadTexture(_texture_key, _pFileName, _str_path_key);
//	if (texture_ == NULL) return false;
//
//	texture_->SetImgInfo( texture_->GetImg(), _srcx, _srcy, _srcwidth, _srcheight);
//
//	// �ؽ�ó ���� �� ����� ���� ����
//	// SetPose(_srcx, _srcy);
//	SetSize(_srcwidth, _srcheight);
//
//	return true;
//}

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

	// ------------------------------------------------------------------------- //

	// Ÿ���� pose�� �ε����� ������ �ٸ��� ����

	// ------------------------------------------------------------------------- //

	if (texture_)
	{
		if (texture_->GetImg() == NULL)
			return;

		// MY_POSE pose_in_cam = pose_ - CCameraManager::Instance()->GetPose();	 // ī�޶� ���� ��� ��ǥ
		// MY_POSE pose_in_cam = pose_ - scene_->camera_->GetPose();	 // ī�޶� ���� ��� ��ǥ

		// ī�޶� ���� ��� ��ǥ
		MY_POSE pose_in_cam = pose_ - scene_->camera_->GetPose();

		// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);
		Graphics graphics(_hdc);

		int w = size_.x;
		int h = size_.y;

		// texture �� �ε��� ��ġ�� �ؽ�ó �� ��ǥ�� ��ȯ
		MY_POSE tmp_pose = idx_int_texture_ * size_;
		int x = tmp_pose.x;
		int y = tmp_pose.y;

		ImageAttributes imgAttr;
		imgAttr.SetColorKey(Color(200, 0, 200), Color(255, 10, 255));

		graphics.DrawImage(texture_->GetImg(), Rect{ (int)pose_in_cam.x, (int)pose_in_cam.y, (int)size_.x, (int)size_.y }, x, y, w, h, UnitPixel, &imgAttr);

		// TransparentBlt(_hdc, pose_in_cam.x, pose_in_cam.y, size_.x, size_.y, texture_->GetDC(), 0, 0, texture_->GetWidth(), texture_->GetHeight(), texture_->GetColorKey());
		// ������Ʈ�� ī�޶� �󿡼� ��ġ = �������� ��ġ - ������ ��ġ

		// cout << "pose_ : (" << pose_.x << ", " << pose_.y << ")\n";
		// cout << "pose_in_cam : (" << pose_in_cam.x << ", " << pose_in_cam.y << ")\n\n\n";
	}
}

CTile * CTile::clone()
{
	return new CTile(*this);
}
