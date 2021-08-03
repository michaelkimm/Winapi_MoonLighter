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

bool CTile::SetTileTexture(int _srcx, int _srcy, int _srcwidth, int _srcheight, const string& _texture_key,
								const wchar_t* _pFileName, const string& _str_path_key, const Color& _color_key)
{
	SAFE_RELEASE(texture_);
	texture_ = CSourceManager::Instance()->LoadTexture(_texture_key, _pFileName, _str_path_key);
	if (texture_ == NULL) return false;

	texture_->SetImgInfo( texture_->GetImg(), _srcx, _srcy, _srcwidth, _srcheight);

	// 텍스처 정할 때 사이즈도 맞춰 설정
	// SetPose(_srcx, _srcy);
	SetSize(_srcwidth, _srcheight);

	return true;
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
	CStaticObj::Render(_hdc, _time);
}

CTile * CTile::clone()
{
	return new CTile(*this);
}
