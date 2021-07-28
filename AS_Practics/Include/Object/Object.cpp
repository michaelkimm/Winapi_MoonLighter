#include "Object.h"
#include "..\Core\Texture.h"
#include "..\Core\SourceManager.h"

void CObject::SetTexture(CTexture * _t)
{
	SAFE_RELEASE(texture_);
	texture_ = _t;

	if (texture_)
		texture_->AddRef();
}

void CObject::SetTexture(const string & _str_key, const wchar_t * _pFileName, const string & _str_path_key)
{
	SAFE_RELEASE(texture_);
	texture_ = CSourceManager::Instance()->LoadTexture(_str_key, _pFileName, _str_path_key);

	// 텍스처 정할 때 사이즈도 맞춰 설정
	SetSize(texture_->GetWidth(), texture_->GetHeight());
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
	if (texture_)
	{
		if (texture_->GetDC() == NULL)
			return;
		// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);
		TransparentBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, texture_->GetWidth(), texture_->GetHeight(), RGB(255, 0, 255));
		
	}
}