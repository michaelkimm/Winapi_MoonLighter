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

	// �ؽ�ó ���� �� ����� ���� ����
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

		// MY_POSE pose_in_cam = pose_ - CCameraManager::Instance()->GetPose();	 // ī�޶� ���� ��� ��ǥ
		MY_POSE pose_in_cam = pose_ - scene_->camera_->GetPose();	 // ī�޶� ���� ��� ��ǥ

		// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);
		TransparentBlt(_hdc, pose_in_cam.x, pose_in_cam.y, size_.x, size_.y, texture_->GetDC(), 0, 0, texture_->GetWidth(), texture_->GetHeight(), texture_->GetColorKey());
		// ������Ʈ�� ī�޶� �󿡼� ��ġ = �������� ��ġ - ������ ��ġ
		
		// cout << "pose_ : (" << pose_.x << ", " << pose_.y << ")\n";
		// cout << "pose_in_cam : (" << pose_in_cam.x << ", " << pose_in_cam.y << ")\n\n\n";
	}
}

void CObject::Save(FILE * _pt_file)
{
	/*
	
	������ �ʿ��� �� �ִ� ������

	string	str_tag_;
	MY_POSE pose_;	// ���� ��ǥ
	MY_POSE pose_idx;
	MY_SIZE size_;
	MY_POSE pivot_;

	class CTexture* texture_;

	// CreateObj �ϸ鼭 �ڵ����� ����Ǵ°Ŷ� ���ص� �ɰ����� �켱 ����
	class CScene* scene_;
	class CLayer* layer_;
	
	*/

	// Tag ���� ���� ����
	int str_tag_length = str_tag_.length();

	// Tage ����
	fwrite(&str_tag_length, 4, 1, _pt_file);
	fwrite(str_tag_.c_str(), 1, str_tag_length, _pt_file);

	// pose ����
	fwrite(&pose_, sizeof(pose_), 1, _pt_file);

	// pose_idx ����
	// fwrite(&pose_idx, sizeof(pose_idx), 1, _pt_file);

	// size ����
	fwrite(&size_, sizeof(size_), 1, _pt_file);

	// pivot ����
	fwrite(&pivot_, sizeof(pivot_), 1, _pt_file);

	// �ؽ��� ����
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
	// Tag ���� ���� ����
	int len;

	char char_tag[MAX_PATH] = {};

	// Tag �б�
	fread(&len, 4, 1, _pt_file);
	fread(char_tag, 1, len, _pt_file);
	char_tag[len] = '\0';
	str_tag_ = char_tag;

	// pose �б�
	fread(&pose_, sizeof(pose_), 1, _pt_file);

	// pose_idx �б�
	// fread(&pose_idx, sizeof(pose_idx), 1, _pt_file);

	// size �б�
	fread(&size_, sizeof(size_), 1, _pt_file);

	// pivot �б�
	fread(&pivot_, sizeof(pivot_), 1, _pt_file);

	// �ؽ��� �б�
	bool texture_exist = false;
	fread(&texture_exist, 1, 1, _pt_file);
	SAFE_RELEASE(texture_);

	if (texture_exist)
		texture_ = CSourceManager::Instance()->LoadTexture(_pt_file);
}
