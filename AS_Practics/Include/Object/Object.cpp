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
// Ÿ�� ���� �߰� �Լ�. ����<Ÿ��>�� �̿�
// ���� �� Ÿ�ϵ��� ������Ʈ�� Ÿ�� ���Ϳ� �ִ´�. ���� �� Ÿ�ϵ��� idx_pose_�� ������Ʈ �������� �����Ѵ�.
// _initialize: ó�� ���� ��� true, �̹� ������� ���� ���� ��� false
// _to_idx: �̹� ������� ���� ��� �ε����� ������ 
void CObject::AddTiles(vector<CTile*> _rect_tile_vec, int _x_length, int _y_length, MY_POSE _to_idx, bool _initialize, bool _do_sort)
{
	int cnt = 0;
	CTile* tmp_tile = NULL;
	for (int i = 0; i < _y_length; i++)
	{
		for (int j = 0; j < _x_length; j++)
		{
			// Clone�� �ڵ� ���� ī��Ʈ �߰�
			tmp_tile = _rect_tile_vec[cnt++]->Clone();

			// �ؽ��� �� ��ġ ����
			// tmp_tile->SetIdxInTexture(j, i);

			// �θ� ������Ʈ�� w/2 - 1, h -1 �ε����� ��, �� �ε��� ��ġ
			if (!_initialize)
				tmp_tile->SetIdxPose(_to_idx.x + j, _to_idx.y + i);
			else
				tmp_tile->SetIdxPose(j, i);

			// ��ġ ����
			// tmp_tile->SetPose(pose_.x + j * TEXTURE_SIZE + _plus_pose.x, pose_.y + i * TEXTURE_SIZE + _plus_pose.y);

			tmp_tile->SetScene(NULL);
			tmp_tile->SetLayer(NULL);
			tmp_tile->SetParentObj(this);

			AddObjToTileVec(tmp_tile, _do_sort);

			SAFE_RELEASE(tmp_tile);
		}
	}

	// (_x_length * _y_length) Ÿ�Ϸ� �̷��� ��ü�� ����
	
	if (_initialize)
	{
		// ������ ����
		SetSize(_x_length * TEXTURE_SIZE, _y_length * TEXTURE_SIZE);

		// ��ġ ����
		pose_ += (size_ * pivot_);
	}

	cout << "size: " << tile_vec_.size() << endl;
}

// Ÿ�� ���� �߰� �Լ�. ���� �ؽ��� �̿�
// _start_pose: ��ü �»�� ��ǥ
// _tile_one_type: true�̸� �� Ÿ�Ϸθ� ���, false�� ū �ؽ���(�ټ� Ÿ��) �ϳ��� ��ü ���� 
bool CObject::AddTiles(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
							const string & _texture_key, const string & _root_str, bool _tile_one_type)
{
	// ������ ����
	size_.x = _size_x * _num_x;
	size_.y = _size_y * _num_y;

	// pivot ����Ͽ� ��ġ ����
	pose_ = (_start_pose + size_ * pivot_);

	for (int i = 0; i < _num_y; i++)
	{
		for (int j = 0; j < _num_x; j++)
		{
			CTile* pt_tile = CObject::CreateObj<CTile>(_texture_key, NULL);
			if (pt_tile == NULL) return false;

			// �ؽ�ó ����
			if (!pt_tile->SetTexture(_texture_key, TEXTURE_PATH))
				return false;

			// ������ ����
			pt_tile->SetSize(_size_x, _size_y);

			// �ؽ��� �� ��ġ ����
			if (_tile_one_type)
				pt_tile->SetIdxInTexture(0, 0);
			else
				pt_tile->SetIdxInTexture(j, i);

			// �θ� ������Ʈ�� w/2 - 1, h -1 �ε����� ��, �� �ε��� ��ġ
			pt_tile->SetIdxPose(j, i);

			// �θ� ������Ʈ ����
			pt_tile->SetParentObj(this);

			// Ÿ�� ���Ϳ� ����
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
	// ������ ����
	size_.x = _size_x * _num_x;
	size_.y = _size_y * _num_y;

	// pivot ����Ͽ� ��ġ ����
	pose_ = (_start_pose + size_ * pivot_);

	CTexture* org_texture = CSourceManager::Instance()->FindTexture(_texture_key);
	if (org_texture == NULL) return false;

 	for (int i = 0; i < _num_y; i++)
	{
		for (int j = 0; j < _num_x; j++)
		{
			// ������ Ÿ�� �����
			CTile* pt_tile = CObject::CreateObj<CTile>(_target_key, NULL);
			if (pt_tile == NULL)
			{
				SAFE_RELEASE(org_texture);
				return false;
			}

			// Ÿ�Ͽ� �ؽ�ó ����
			if (!pt_tile->SetTexture(_target_key, TEXTURE_PATH))
			{
				SAFE_RELEASE(org_texture);
				SAFE_RELEASE(pt_tile);
				return false;
			}

			// Ÿ���� �ؽ��� �� ��� ��ġ���� ����
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


			// ������ ����
			pt_tile->SetSize(_size_x, _size_y);

			//


			//CTexture* tmp_texture = pt_tile->GetTexture();
			//
			//// ���� Ÿ�� ��ġ�� �ش��ϴ� Ÿ�� �ɼ��� �����´�.
			//TILE_OPTION tmp_option = org_texture->GetOptionVec(idx_in_texture.x, idx_in_texture.y);

			//SAFE_RELEASE(tmp_texture);

			// �θ� ������Ʈ�� w/2 - 1, h -1 �ε����� ��, �� �ε��� ��ġ
			pt_tile->SetIdxPose(j, i);

			// �θ� ������Ʈ ����
			pt_tile->SetParentObj(this);

			// Ÿ�� ���Ϳ� ����
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
		cout << " ���� �� ��ã��!\n";
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

	// �ؽ�ó ���� �� ����� ���� ����
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
		cout << "��!\n\n";
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

		// MY_POSE pose_in_cam = pose_ - CCameraManager::Instance()->GetPose();	 // ī�޶� ���� ��� ��ǥ
		MY_POSE pose_in_cam = pose_ - scene_->camera_->GetPose() - size_ * pivot_;	 // ī�޶� ���� ��� ��ǥ

		// BitBlt(_hdc, pose_.x, pose_.y, size_.x, size_.y, texture_->GetDC(), 0, 0, SRCCOPY);
		TransparentBlt(_hdc, pose_in_cam.x, pose_in_cam.y, size_.x, size_.y, texture_->GetDC(), 0, 0, texture_->GetWidth(), texture_->GetHeight(), texture_->GetColorKey());
		// ������Ʈ�� ī�޶� �󿡼� ��ġ = �������� ��ġ - ������ ��ġ
		
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
