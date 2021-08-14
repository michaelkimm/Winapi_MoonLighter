#include "Object.h"
#include <algorithm>
#include "Tile.h"

#include "..\Core\Texture.h"
#include "..\Core\SourceManager.h"
#include "..\Core\CameraManager.h"
#include "..\Core\PathManager.h"

void CObject::AddObjToTileVec(CObject* _add_obj, bool _do_sort)
{
	_add_obj->AddRef();
	tile_vec_.push_back(_add_obj);

	// std::sort(tile_vec_.begin(), tile_vec_.end(), CmpObjY);

	return;
}
// 타일 벡터 추가 함수. 벡터<타일>을 이용
// 벡터 내 타일들을 오브젝트의 타일 벡터에 넣는다. 벡터 내 타일들의 idx_pose_를 오브젝트 기준으로 변경한다.
// _initialize: 처음 만들 경우 true, 이미 만들어진 곳에 넣을 경우 false
// _to_idx: 이미 만들어진 곳의 어느 인덱스에 넣을지 
void CObject::AddTiles(vector<CTile*> _rect_tile_vec, int _x_length, int _y_length, MY_POSE _to_idx, bool _initialize, bool _do_sort)
{
	int cnt = 0;
	CTile* tmp_tile = NULL;
	for (int i = 0; i < _y_length; i++)
	{
		for (int j = 0; j < _x_length; j++)
		{
			// Clone은 자동 참조 카운트 추가
			tmp_tile = _rect_tile_vec[cnt++]->Clone();

			// 텍스쳐 내 위치 설정
			// tmp_tile->SetIdxInTexture(j, i);

			// 부모 오브젝트가 w/2 - 1, h -1 인덱스일 때, 내 인덱스 위치
			if (!_initialize)
				tmp_tile->SetIdxPose(_to_idx.x + j, _to_idx.y + i);
			else
				tmp_tile->SetIdxPose(j, i);

			// 위치 설정
			// tmp_tile->SetPose(pose_.x + j * TEXTURE_SIZE + _plus_pose.x, pose_.y + i * TEXTURE_SIZE + _plus_pose.y);

			tmp_tile->SetScene(NULL);
			tmp_tile->SetLayer(NULL);
			tmp_tile->SetParentObj(this);

			AddObjToTileVec(tmp_tile, _do_sort);

			SAFE_RELEASE(tmp_tile);
		}
	}

	// (_x_length * _y_length) 타일로 이뤄진 객체로 정의
	
	if (_initialize)
	{
		// 사이즈 설정
		SetSize(_x_length * TEXTURE_SIZE, _y_length * TEXTURE_SIZE);

		// 위치 설정
		pose_ += (size_ * pivot_);
	}

	cout << "size: " << tile_vec_.size() << endl;
}

// 타일 벡터 추가 함수. 단일 텍스쳐 이용
// _start_pose: 객체 좌상단 좌표
// _tile_one_type: true이면 한 타일로만 깔기, false면 큰 텍스쳐(다수 타일) 하나로 객체 생성 
bool CObject::AddTiles(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
							const string & _texture_key, const string & _root_str, bool _tile_one_type)
{
	// 사이즈 설정
	size_.x = _size_x * _num_x;
	size_.y = _size_y * _num_y;

	// pivot 고려하여 위치 설정
	pose_ = (_start_pose + size_ * pivot_);

	for (int i = 0; i < _num_y; i++)
	{
		for (int j = 0; j < _num_x; j++)
		{
			CTile* pt_tile = CObject::CreateObj<CTile>(_texture_key, NULL);
			if (pt_tile == NULL) return false;

			// 텍스처 설정
			if (!pt_tile->SetTexture(_texture_key, TEXTURE_PATH))
				return false;

			// 사이즈 설정
			pt_tile->SetSize(_size_x, _size_y);

			// 텍스쳐 내 위치 설정
			if (_tile_one_type)
				pt_tile->SetIdxInTexture(0, 0);
			else
				pt_tile->SetIdxInTexture(j, i);

			// 부모 오브젝트가 w/2 - 1, h -1 인덱스일 때, 내 인덱스 위치
			pt_tile->SetIdxPose(j, i);

			// 부모 오브젝트 설정
			pt_tile->SetParentObj(this);

			// 타일 벡터에 대입
			pt_tile->AddRef();
			tile_vec_.push_back(pt_tile);

			SAFE_RELEASE(pt_tile);
		}
	}
	return true;
}

bool CObject::AddTiles2(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
							const string& _texture_key, const string& _target_key, const string& _root_str, bool _tile_one_type)
{
	// 사이즈 설정
	size_.x = _size_x * _num_x;
	size_.y = _size_y * _num_y;

	// pivot 고려하여 위치 설정
	pose_ = (_start_pose + size_ * pivot_);

	CTexture* org_texture = CSourceManager::Instance()->FindTexture(_texture_key);
	if (org_texture == NULL) return false;

 	for (int i = 0; i < _num_y; i++)
	{
		for (int j = 0; j < _num_x; j++)
		{
			// 삽입할 타일 만들기
			CTile* pt_tile = CObject::CreateObj<CTile>(_target_key, NULL);
			if (pt_tile == NULL)
			{
				SAFE_RELEASE(org_texture);
				return false;
			}

			// 타일에 텍스처 설정
			if (!pt_tile->SetTexture(_target_key, TEXTURE_PATH))
			{
				SAFE_RELEASE(org_texture);
				SAFE_RELEASE(pt_tile);
				return false;
			}

			// 타일이 텍스쳐 내 어디 위치인지 설정
			MY_SIZE idx_in_texture;

			switch (org_texture->GetOptionVec(j, i))
			{
			case TILE_NONE:
				idx_in_texture = MY_POSE(0, 0);
				break;
			case TILE_NOMOVE:
				idx_in_texture = MY_POSE(1, 0);
				break;
			case TILE_TRANSPORT:
				idx_in_texture = MY_POSE(2, 0);
				break;
			}

			pt_tile->SetIdxInTexture(idx_in_texture.x, idx_in_texture.y);


			// 사이즈 설정
			pt_tile->SetSize(_size_x, _size_y);

			//


			//CTexture* tmp_texture = pt_tile->GetTexture();
			//
			//// 현재 타일 위치에 해당하는 타일 옵션을 가져온다.
			//TILE_OPTION tmp_option = org_texture->GetOptionVec(idx_in_texture.x, idx_in_texture.y);

			//SAFE_RELEASE(tmp_texture);

			// 부모 오브젝트가 w/2 - 1, h -1 인덱스일 때, 내 인덱스 위치
			pt_tile->SetIdxPose(j, i);

			// 부모 오브젝트 설정
			pt_tile->SetParentObj(this);

			// 타일 벡터에 대입
			pt_tile->AddRef();
			tile_vec_.push_back(pt_tile);

			SAFE_RELEASE(pt_tile);
		}
	}

	SAFE_RELEASE(org_texture);

	return true;
}

void CObject::DeleteTileInVec(float _x, float _y)
{
	cout << "erase idx: (" << _x << ", " << _y << ")\n";

	CTile* tmp_tile = NULL;

	vector<class CObject*>::iterator target_iter = tile_vec_.begin();//  + _x + _y * (size_.x / TEXTURE_SIZE);
	for (target_iter; target_iter != tile_vec_.end(); target_iter++)
	{
		tmp_tile = static_cast<CTile*>(*target_iter);
		if (tmp_tile->GetIdxPose() == MY_POSE(_x, _y))
			break;
	}
	if (target_iter == tile_vec_.end())
	{
		cout << " 지울 것 못찾음!\n";
		return;
	}
	tile_vec_.erase(target_iter);
}

void CObject::SetParentObj(CObject * _obj)
{
	SAFE_RELEASE(parent_obj_);
	_obj->AddRef();
	parent_obj_ = _obj;
}

CObject * CObject::GetParentObj() const
{
	parent_obj_->AddRef();
	return parent_obj_;
}

CObject* CObject::GetTileFromVec(int _idx) const
{
	if (_idx >= tile_vec_.size())
	{
		cout << "idx out of range in GetTileFromVec!\n";
		return NULL;
	}
	tile_vec_[_idx]->AddRef();
	return tile_vec_[_idx];
}

bool CObject::SetTexture(CTexture * _t)
{
	SAFE_RELEASE(texture_);
	texture_ = _t;

	if (texture_)
		texture_->AddRef();

	return true;
}

bool CObject::SetTexture(const string & _texture_key, const string & _str_path_key, const Color& _color_key)
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

bool CObject::CollidePt(MY_POSE _pose)
{
	RECT tmp_rect{ (int)(pose_.x - size_.x / 2), (int)(pose_.y - size_.y), (int)(pose_.x + size_.x / 2), (int)pose_.y };
	POINT tmp_pt{ (int)_pose.x, (int)_pose.y };
	if (PtInRect(tmp_rect, tmp_pt))
	{
		cout << "tmp_pt: " << tmp_pt.x << ", " << tmp_pt.y << ")\n";
		cout << "안!\n\n";
		return true;
	}
	return false;
}

CObject::CObject()	:
	texture_(NULL),
	scene_(NULL),
	layer_(NULL),
	parent_obj_(NULL),
	pivot_(0.5f, 1.f)
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
	SAFE_RELEASE(texture_);
	SafeReleaseList(tile_vec_);
	SAFE_RELEASE(parent_obj_);
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
		MY_POSE pose_in_cam = pose_ - scene_->camera_->GetPose() - size_ * pivot_;	 // 카메라 기준 상대 좌표

		// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);
		TransparentBlt(_hdc, pose_in_cam.x, pose_in_cam.y, size_.x, size_.y, texture_->GetDC(), 0, 0, texture_->GetWidth(), texture_->GetHeight(), texture_->GetColorKey());
		// 오브젝트의 카메라 상에서 위치 = 스테이지 위치 - 윈도우 위치
		
		// cout << "pose_ : (" << pose_.x << ", " << pose_.y << ")\n";
		// cout << "pose_in_cam : (" << pose_in_cam.x << ", " << pose_in_cam.y << ")\n\n\n";
	}

	if (tile_vec_.size())
	{
		vector<class CObject*>::iterator iter;
		vector<class CObject*>::iterator iter_end = tile_vec_.end();

		for (iter = tile_vec_.begin(); iter != iter_end; iter++)
		{
			(*iter)->Render(_hdc, _time);
		}
		DrawEllipse(_hdc, POINT{ (int)pose_.x, (int)pose_.y }, 6);
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
