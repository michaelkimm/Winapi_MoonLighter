#include "Object.h"
#include "..\Core\Texture.h"
#include "..\Core\SourceManager.h"
#include "..\Core\CameraManager.h"

bool CObject::SetTexture(CTexture * _t)
{
	SAFE_RELEASE(texture_);
	texture_ = _t;

	if (texture_)
		texture_->AddRef();

	return true;
}

bool CObject::SetTexture(const string & _texture_key, const Color& _color_key)
{
	SAFE_RELEASE(texture_);
	texture_ = CSourceManager::Instance()->FindTexture(_texture_key);
	if (texture_ == NULL) return false;

	float world_w = texture_->GetWidth();
	float world_h = texture_->GetHeight();

	// 텍스처 정할 때 사이즈도 맞춰 설정
	SetSize(world_w, world_h);

	return true;
}

CTexture * CObject::GetTexture() const
{
	texture_->AddRef();
	return texture_;
}

CObject::CObject()	:
	texture_(NULL)
{
}

CObject::CObject(const CObject& _obj)
{
	*this = _obj;
	if (texture_)
		texture_->AddRef();
}

CObject::~CObject()
{
	SAFE_RELEASE(texture_)
}

bool CObject::Init()
{
	return true;
}

void CObject::Input(float _time)
{
}

void CObject::Update(float _time)
{
}

void CObject::LateUpdate(float _time)
{
}

void CObject::Collision(float _time)
{
}

void CObject::Render(HDC _hdc, float _time)
{
	//if (texture_)
	//{
	//	if (texture_->GetImg() == NULL)
	//		return;

	//	// MY_POSE pose_in_cam = pose_ - CCameraManager::Instance()->GetPose();	 // 카메라 기준 상대 좌표
	//	MY_POSE pose_in_cam = pose_ - scene_->camera_->GetPose();	 // 카메라 기준 상대 좌표

	//	// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);
	//	Graphics graphics(_hdc);

	//	int w = texture_->GetWidth();
	//	int h = texture_->GetHeight();
	//	int x = texture_->GetX() * w;
	//	int y = texture_->GetY() * h;
	//	
	//	ImageAttributes imgAttr;
	//	imgAttr.SetColorKey(Color(200, 0, 200), Color(255, 10, 255));

	//	graphics.DrawImage(texture_->GetImg(), Rect{ (int)pose_in_cam.x, (int)pose_in_cam.y, (int)size_.x, (int)size_.y }, x, y, w, h, UnitPixel, &imgAttr);

	//	// TransparentBlt(_hdc, pose_in_cam.x, pose_in_cam.y, size_.x, size_.y, texture_->GetDC(), 0, 0, texture_->GetWidth(), texture_->GetHeight(), texture_->GetColorKey());
	//	// 오브젝트의 카메라 상에서 위치 = 스테이지 위치 - 윈도우 위치
	//	
	//	// cout << "pose_ : (" << pose_.x << ", " << pose_.y << ")\n";
	//	// cout << "pose_in_cam : (" << pose_in_cam.x << ", " << pose_in_cam.y << ")\n\n\n";
	//}
}