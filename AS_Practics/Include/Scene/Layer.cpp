#include "Layer.h"

#include "..\Object\Object.h"
#include "..\Object\Tile.h"
#include "..\Core\SourceManager.h"
#include "..\Core\Texture.h"
#include <algorithm>

CLayer::CLayer() :
	pt_scene_(nullptr),
	str_tag_(""),
	z_order_(0)
{
}

CLayer::~CLayer()
{
	SafeReleaseList(obj_list_);
}

void CLayer::PushObj(CObject *_obj)
{
	_obj->AddRef();
	obj_list_.push_back(_obj);
	return;
}

void CLayer::DeleteObj(float _x, float _y)
{
	cout << "erase idx: (" << _x << ", " << _y << ")\n";

	vector<class CObject*>::iterator target_iter = FindObj(MY_POSE(_x, _y));
	if (target_iter == obj_list_.end())
	{
		cout << "못 찾음!\n";
		return;
	}
	cout << "남은 갯수: " << obj_list_.size() << endl;
	obj_list_.erase(target_iter);
}

void CLayer::DeleteAll()
{
	SafeReleaseList(obj_list_);
}

vector<class CObject*>::iterator CLayer::FindObj(MY_POSE _pose_idx)
{
	vector<class CObject*>::iterator iter;
	vector<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		if (((*iter)->GetPose() / TEXTURE_SIZE) == _pose_idx )
			break;
	}
	return iter;
}

bool CmpObjY(class CObject* a, class CObject *b)
{
	return a->GetPose().y < b->GetPose().y;
}


void CLayer::AddObj(class CObject* _obj, bool _do_sort)
{
	_obj->SetScene(pt_scene_);
	_obj->SetLayer(this);
	_obj->AddRef();

	obj_list_.push_back(_obj);

	// cout << "sort 시작!\n";

	// y 값이 큰 순서대로 정렬
	if (_do_sort)
		std::sort(obj_list_.begin(), obj_list_.end(), CmpObjY);
	
	// cout << "sort 끝!\n";
}

CObject* CLayer::GetObj(int _idx) const
{
	if (_idx >= obj_list_.size())
		return NULL;

	obj_list_[_idx]->AddRef();

	return obj_list_[_idx];
}

CObject* CLayer::GetObj(const string& _name_tag)
{
	vector<class CObject*>::iterator iter;
	vector<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		if ((*iter)->GetTag() == _name_tag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

void CLayer::Clear()
{
	SafeReleaseList(obj_list_);
}

void CLayer::SortObjListZOrder()
{
	// y 값이 큰 순서대로 정렬
	std::sort(obj_list_.begin(), obj_list_.end(), CmpObjY);
}

bool CLayer::Init()
{
	return true;
}

void CLayer::Input(float _time)
{
	vector<class CObject*>::iterator iter;
	vector<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Input(_time);
	}
}

void CLayer::Update(float _time)
{
	vector<class CObject*>::iterator iter;
	vector<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Update(_time);
	}
}

void CLayer::LateUpdate(float _time)
{
	vector<class CObject*>::iterator iter;
	vector<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->LateUpdate(_time);
	}
}

void CLayer::Collision(float _time)
{
	vector<class CObject*>::iterator iter;
	vector<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Collision(_time);
	}
}

void CLayer::Render(HDC _hdc, float _time)
{
	vector<class CObject*>::iterator iter;
	vector<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Render(_hdc, _time);
	}
}

void CLayer::Save(FILE * _pt_file)
{
	// : >> 레이어 정보 저장

	// 전체 타일 갯수
	int tile_cnt = obj_list_.size();
	fwrite(&tile_cnt, 4, 1, _pt_file);

	// 레이어 멤버 변수
	fwrite(&tile_x_num_, 4, 1, _pt_file);
	fwrite(&tile_y_num_, 4, 1, _pt_file);
	fwrite(&tile_width_, 4, 1, _pt_file);
	fwrite(&tile_height_, 4, 1, _pt_file);

	vector<class CObject*>::iterator iter;
	vector<class CObject*>::iterator iter_end = obj_list_.end();

	for (iter = obj_list_.begin(); iter != iter_end; iter++)
	{
		(*iter)->Save(_pt_file);
	}
	// <<
}

void CLayer::Load(FILE * _pt_file)
{
	// : >> 레이어 정보 불러오기

	// 전체 타일 갯수
	int tile_cnt;
	fread(&tile_cnt, 4, 1, _pt_file);

	// 레이어 멤버 변수
	fread(&tile_x_num_, 4, 1, _pt_file);
	fread(&tile_y_num_, 4, 1, _pt_file);
	fread(&tile_width_, 4, 1, _pt_file);
	fread(&tile_height_, 4, 1, _pt_file);

	// 기존 벡터 제거
	SafeReleaseList(obj_list_);

	for (int i = 0; i < tile_cnt; i++)
	{
		// 타일 동적할당
		CTile* pt_tile = CObject::CreateObj<CTile>("Tile", this);

		// 타일 정보 로드
		pt_tile->Load(_pt_file);

		SAFE_RELEASE(pt_tile);
	}
	// <<
}

bool CLayer::CreateTile(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
	const string & _texture_key, const string & _root_str, bool _no_tile_only_size)
{
	tile_x_num_ = _num_x;
	tile_y_num_ = _num_y;
	tile_width_ = _size_x;
	tile_height_ = _size_y;

	// 사이즈 설정
	MY_SIZE size_;
	size_.x = tile_width_ * tile_x_num_;
	size_.y = tile_height_ * tile_y_num_;

	// 타일 생성 없이 사이즈만 만들 경우
	if (_no_tile_only_size)
		return true;

	for (int i = 0; i < _num_y; i++)
	{
		for (int j = 0; j < _num_x; j++)
		{
			// 타일 이름과 속한 레이어 넘겨서 타일 생성
			CTile* pt_tile = CObject::CreateObj<CTile>(_texture_key, this);
			if (pt_tile == NULL) return false;

			// 텍스처 설정
			if (!pt_tile->SetTexture(_texture_key))
			{
				SAFE_RELEASE(pt_tile);
				return false;
			}

			// 타일의 절대 위치 설정
			pt_tile->SetPose(_start_pose.x + j * _size_x, _start_pose.y + i * _size_y);

			// img에서의 인덱스 및 타일 한개 사이즈 설정
			// 텍스쳐가 하나기 때문에 (0, 0)으로 설정
			pt_tile->SetIdxInTexture(0, 0);
			pt_tile->SetSize(_size_x, _size_y);

			SAFE_RELEASE(pt_tile);
		}
	}
	return true;
}


bool CLayer::CreateTileSpriteSheet(const MY_POSE& _start_pose, int _size_x, int _size_y,
	const string& _texture_key, const string& _root_str)
{

	CTexture* tmp_texture = CSourceManager::Instance()->FindTexture(_texture_key);
	tile_x_num_ = tmp_texture->GetWidth() / _size_x;
	tile_y_num_ = tmp_texture->GetHeight() / _size_y;
	tile_width_ = _size_x;
	tile_height_ = _size_y;

	SAFE_RELEASE(tmp_texture);


	// 사이즈 설정
	MY_SIZE size_;
	size_.x = tile_x_num_;
	size_.y = tile_y_num_;

	// 위치 설정
	for (int i = 0; i < tile_y_num_; i++)
	{
		for (int j = 0; j < tile_x_num_; j++)
		{
			// 타일 이름과 속한 레이어 넘겨서 타일 생성
			CTile* pt_tile = CObject::CreateObj<CTile>(_texture_key, this);
			if (pt_tile == NULL) return false;

			// 텍스처 설정
			// img, 사이즈 설정, 이미 source 관리자에 저장되어 있던 텍스쳐를 가리키는 방식
			if (!pt_tile->SetTexture(_texture_key))
			{
				SAFE_RELEASE(pt_tile);
				return false;
			}

			// 타일의 절대 위치 설정
			// pt_tile->SetPose(_start_pose.x + j * _size_x, _start_pose.y + i * _size_y);

			// img에서의 인덱스 및 타일 한개 사이즈 설정
			pt_tile->SetIdxInTexture(j, i);
			pt_tile->SetSize(_size_x, _size_y);

			SAFE_RELEASE(pt_tile);
		}
	}
	return true;
}


