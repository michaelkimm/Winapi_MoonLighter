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

void CLayer::AddObj(class CObject* _obj)
{
	_obj->SetScene(pt_scene_);
	_obj->SetLayer(this);
	_obj->AddRef();

	obj_list_.push_back(_obj);

	// cout << "sort ����!\n";

	// y ���� ū ������� ����
	// std::sort(obj_list_.begin(), obj_list_.end(), CObject::CmpObjY);
	
	// cout << "sort ��!\n";
}

CObject* CLayer::GetObj(int _idx) const
{
	if (_idx >= obj_list_.size())
		return NULL;

	obj_list_[_idx]->AddRef();

	return obj_list_[_idx];
}

void CLayer::Clear()
{
	SafeReleaseList(obj_list_);
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

bool CLayer::CreateTile(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
	const string & _texture_key, const string & _root_str, bool _no_tile_only_size)
{
	tile_x_num_ = _num_x;
	tile_y_num_ = _num_y;
	tile_width_ = _size_x;
	tile_height_ = _size_y;

	// ������ ����
	MY_SIZE size_;
	size_.x = tile_width_ * tile_x_num_;
	size_.y = tile_height_ * tile_y_num_;

	// Ÿ�� ���� ���� ����� ���� ���
	if (_no_tile_only_size)
		return true;

	for (int i = 0; i < _num_y; i++)
	{
		for (int j = 0; j < _num_x; j++)
		{
			// Ÿ�� �̸��� ���� ���̾� �Ѱܼ� Ÿ�� ����
			CTile* pt_tile = CObject::CreateObj<CTile>(_texture_key, this);
			if (pt_tile == NULL) return false;

			// �ؽ�ó ����
			if (!pt_tile->SetTexture(_texture_key))
			{
				SAFE_RELEASE(pt_tile);
				return false;
			}

			// Ÿ���� ���� ��ġ ����
			pt_tile->SetPose(_start_pose.x + j * _size_x, _start_pose.y + i * _size_y);

			// img������ �ε��� �� Ÿ�� �Ѱ� ������ ����
			// �ؽ��İ� �ϳ��� ������ (0, 0)���� ����
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


	// ������ ����
	MY_SIZE size_;
	size_.x = tile_x_num_;
	size_.y = tile_y_num_;

	// ��ġ ����
	for (int i = 0; i < tile_y_num_; i++)
	{
		for (int j = 0; j < tile_x_num_; j++)
		{
			// Ÿ�� �̸��� ���� ���̾� �Ѱܼ� Ÿ�� ����
			CTile* pt_tile = CObject::CreateObj<CTile>(_texture_key, this);
			if (pt_tile == NULL) return false;

			// �ؽ�ó ����
			// img, ������ ����, �̹� source �����ڿ� ����Ǿ� �ִ� �ؽ��ĸ� ����Ű�� ���
			if (!pt_tile->SetTexture(_texture_key))
			{
				SAFE_RELEASE(pt_tile);
				return false;
			}

			// Ÿ���� ���� ��ġ ����
			pt_tile->SetPose(_start_pose.x + j * _size_x, _start_pose.y + i * _size_y);

			// img������ �ε��� �� Ÿ�� �Ѱ� ������ ����
			pt_tile->SetIdxInTexture(j, i);
			pt_tile->SetSize(_size_x, _size_y);

			SAFE_RELEASE(pt_tile);
		}
	}
	return true;
}


