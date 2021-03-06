//======================================================================
//	シーンオブジェクトヘッダ[scene.h]
//
//======================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

static const int OBJECT_MAX = 200;
static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

#include "shader.h"

//*********************************************************
//	シーンオブジェクトの種類
//*********************************************************
enum SCENE_TYPE
{
	SCENE_TYPE_NONE,
	SCENE_TYPE_2D,
	SCENE_TYPE_3D,
	SCENE_TYPE_PLAYER,
	SCENE_TYPE_ENEMY,
	SCENE_TYPE_BULLET,
	SCENE_TYPE_BILLBOARD,
	SCENE_TYPE_WALL,
};

//*********************************************************
//	描画レイヤー
//*********************************************************
enum LAYER
{
	LAYER_BACKGROUND,
	LAYER_SKINMESH,
	LAYER_EFFEKSEER,
	LAYER_OBJECT3D,
	LAYER_OBJECT2D,
	LAYER_OBJECT2D_AFTER,
	LAYER_EFFEKSEER_AFTER,
	PRIORITY_MAX
};

class CScene
{
protected:
	D3DXVECTOR3 m_Pos;
	SCENE_TYPE m_Type;
	bool m_Visible;
	bool m_isUseShadow;
	CShader *m_Shader;
public:
	CScene(int priority);
	virtual ~CScene() {}
	virtual void Init() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawShadow() {};
	virtual void DrawWithShader() = 0;
	virtual void SetWorld(D3DXMATRIX move) {}
	virtual void Move(D3DXVECTOR3 pos) {}
	virtual void Rotate(D3DXVECTOR3 rot) {}
	virtual void Scale(D3DXVECTOR3 scale) {}
	void Set(const D3DXVECTOR3& pos);
	void SetVisible(bool value) { m_Visible = value; }
	bool& GetVisible() { return m_Visible; }
	void SetUseShadow(bool value) { m_isUseShadow = value; }
	bool& GetUseShadow() { return m_isUseShadow; }
	void SetShader(CShader* shader) { m_Shader = shader; }
	CShader* GetShader() { return m_Shader; }
	D3DXVECTOR3 GetPos() { return m_Pos; }
	void Release();
	static void UpdateAll();
	static void DrawAll();
	static void DrawShadowAll();
	static void ReleaseAll();
	static CScene* GetScene(int priority, int id) { return m_Scene[priority][id]; }
	SCENE_TYPE& GetType() { return m_Type; }
private:
	static CScene* m_Scene[PRIORITY_MAX][OBJECT_MAX];
};

#endif // !_SCENE_H_
