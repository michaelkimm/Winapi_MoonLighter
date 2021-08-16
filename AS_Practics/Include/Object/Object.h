#pragma once
#ifndef COBJECT_H_
#define COBJECT_H_

#include "..\Game.h"
#include "..\Core\Ref.h"
#include "..\Scene\Layer.h"



class CObject
	: public CRef
{
	friend class CLayer;

protected:
	string	str_tag_;
	string class_tag_;
	MY_POSE pose_;		// 절대 좌표
	// MY_POSE pose_idx;	// 맵에서 인덱스 위치
	MY_SIZE size_;
	MY_POSE pivot_;

	// 텍스쳐
	class CTexture* texture_;

	// 타일 벡터
	vector<class CObject*> tile_vec_;

	// 부모 오브젝트
	CObject* parent_obj_;

public:
	void AddObjToTileVec(CObject* _add_obj, bool _do_sort = false);

	// 타일 벡터 추가 함수. 벡터<타일>을 이용
	void AddTiles(vector<class CTile*> _rect_tile_vec, int _x_length, int _y_length, MY_POSE _to_idx = MY_POSE(0, 0), bool _initialize = false, bool _do_sort = false);
	
	// 타일 벡터 추가 함수. 단일 텍스쳐 이용
	bool AddTiles(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
						const string& _texture_key, const string& _root_str = TEXTURE_PATH, bool _tile_one_type = false);

	// 타일 벡터 추가 함수. 단일 텍스쳐 이용
	bool AddTiles2(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
						const string& _org_key, const string& _target_key, const string& _root_str = TEXTURE_PATH, bool _tile_one_type = false);

	void DeleteTileInVec(float _x, float _y);

	void		SetParentObj(CObject* _obj);
	CObject*	GetParentObj() const;
	CObject*	GetTileFromVec(int _idx) const;
	int			GetTileVecCnt() const { return tile_vec_.size(); }

public:
	void SetTag(const string& _str_tag) { str_tag_ = _str_tag; }
	void SetClassTag(const string& _str_tag) { class_tag_ = _str_tag; }
	void SetPose(const MY_POSE& _pose)		{ pose_ = _pose; }
	void SetPose(float _x, float _y)	{ pose_ = MY_POSE(_x, _y); }
	void SetPose(int _x, int _y)		{ pose_ = MY_POSE(_x, _y); }
	void SetSize(const MY_SIZE& _size)		{ size_ = _size; }
	void SetSize(float _x, float _y)	{ size_ = MY_SIZE(_x, _y); }
	void SetSize(int _x, int _y)		{ size_ = MY_SIZE(_x, _y); }
	void SetPivot(const MY_POSE& _pivot)		{ pivot_ = _pivot; }
	bool SetTexture(class CTexture* _t);
	bool SetTexture(const string& _texture_key, const string& _str_path_key = TEXTURE_PATH,
							const Color& _color_key = Color(255, 0, 255));


	string GetTag() const { return str_tag_; }
	MY_POSE GetPose() const { return pose_; }
	MY_SIZE GetSize() const { return size_; }
	MY_POSE GetPivot() const { return pivot_; }
	CTexture* GetTexture() const;

public:
	bool CollidePt(MY_POSE _pose);

protected:
	class CScene* scene_;
	class CLayer* layer_;

public:
	void	SetScene(class CScene* _scene)	{ scene_ = _scene; }
	void	SetLayer(class CLayer* _layer)	{ layer_ = _layer; }
	CScene* GetScene()	const				{ return scene_; }
	CLayer* GetLayer()	const				{ return layer_; }

protected:
	CObject();
	CObject(const CObject& _obj);
	virtual ~CObject();
	 
	virtual CObject* Clone() = 0;

public:
	virtual bool Init() = 0;
	virtual void Input(float _time);
	virtual void Update(float _time);
	virtual void LateUpdate(float _time);
	virtual void Collision(float _time);
	virtual void Render(HDC _hdc, float _time);

public:
	virtual void Save(FILE* _pt_file);
	virtual void Load(FILE* _pt_file);

public:
	template <typename T>
	static T* CreateObj(const string& _name_tag, const string& _class_tag, CLayer* _layer, MY_POSE _pose = MY_POSE(0, 0), bool _do_sort = false)
	{
		T* pt_obj = new T; 

		if (!pt_obj->Init())
		{
			SAFE_DELETE(pt_obj);
			return nullptr;
		}

		if (_layer)
		{
			_layer->AddObj(pt_obj, _do_sort);
		}

		pt_obj->SetPose(_pose);

		pt_obj->SetTag(_name_tag);

		pt_obj->SetClassTag(_class_tag);

		return pt_obj;
	}

	static void LoadObjectVec(vector<CObject*>& _obj_vec, FILE* _pt_file, CLayer* _layer, CObject* _parent = NULL);
};



#endif


