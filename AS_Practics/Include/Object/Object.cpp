#include "Object.h"
#include "..\Core\Texture.h"
#include "..\Core\SourceManager.h"
#include "..\Core\CameraManager.h"
#include "..\Core\PathManager.h"

bool CObject::SetTexture(CTexture * _t)
{
	SAFE_RELEASE(texture_);
	texture_ = _t;

	if (texture_)
		texture_->AddRef();

	return true;
}

bool CObject::SetTexture(const string & _texture_key, const wchar_t * _pFileName, const string & _str_path_key, const Color& _color_key)
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
	texture_(NULL),
	pivot_(0.f, 0.f)
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

		// MY_POSE pose_in_cam = pose_ - CCameraManager::Instance()->GetPose();	 // 카메라 기준 상대 좌표
		MY_POSE pose_in_cam = pose_ - scene_->camera_->GetPose();	 // 카메라 기준 상대 좌표

		// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);
		TransparentBlt(_hdc, pose_in_cam.x, pose_in_cam.y, size_.x, size_.y, texture_->GetDC(), 0, 0, texture_->GetWidth(), texture_->GetHeight(), texture_->GetColorKey());
		// 오브젝트의 카메라 상에서 위치 = 스테이지 위치 - 윈도우 위치
		
		// cout << "pose_ : (" << pose_.x << ", " << pose_.y << ")\n";
		// cout << "pose_in_cam : (" << pose_in_cam.x << ", " << pose_in_cam.y << ")\n\n\n";
	}
}

void CObject::Save(FILE * _pt_file)
{
	/*
	
	저장이 필요할 수 있는 변수들

	string	str_tag_;
	MY_POSE pose_;	// 절대 좌표
	MY_POSE pose_idx;
	MY_SIZE size_;
	MY_POSE pivot_;

	class CTexture* texture_;

	// CreateObj 하면서 자동으로 저장되는거라 안해도 될것으로 우선 예상
	class CScene* scene_;
	class CLayer* layer_;
	
	*/

	// Tag 길이 정보 설정
	int str_tag_length = str_tag_.length();

	// Tage 저장
	fwrite(&str_tag_length, 4, 1, _pt_file);
	fwrite(str_tag_.c_str(), 1, str_tag_length, _pt_file);

	// pose 저장
	fwrite(&pose_, sizeof(pose_), 1, _pt_file);

	// pose_idx 저장
	// fwrite(&pose_idx, sizeof(pose_idx), 1, _pt_file);

	// size 저장
	fwrite(&size_, sizeof(size_), 1, _pt_file);

	// pivot 저장
	fwrite(&pivot_, sizeof(pivot_), 1, _pt_file);

	// 텍스쳐 저장
	bool texture_exist = false;
	if (texture_)
	{
		texture_exist = true;
		fwrite(&texture_exist, 1, 1, _pt_file);
		texture_->Save(_pt_file);
	}
	else
		fwrite(&texture_exist, 1, 1, _pt_file);
}


void CObject::Load(FILE * _pt_file)
{
	// Tag 길이 정보 설정
	int len;

	char char_tag[MAX_PATH] = {};

	// Tag 읽기
	fread(&len, 4, 1, _pt_file);
	fread(char_tag, 1, len, _pt_file);
	char_tag[len] = '\0';
	str_tag_ = char_tag;

	// pose 읽기
	fread(&pose_, sizeof(pose_), 1, _pt_file);

	// pose_idx 읽기
	// fread(&pose_idx, sizeof(pose_idx), 1, _pt_file);

	// size 읽기
	fread(&size_, sizeof(size_), 1, _pt_file);

	// pivot 읽기
	fread(&pivot_, sizeof(pivot_), 1, _pt_file);

	// 텍스쳐 읽기
	bool texture_exist = false;
	fread(&texture_exist, 1, 1, _pt_file);
	SAFE_RELEASE(texture_);

	if (texture_exist)
		texture_ = CSourceManager::Instance()->LoadTexture(_pt_file);
}
