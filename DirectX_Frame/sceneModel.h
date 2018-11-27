//======================================================================
//	3Dモデルヘッダ[sceneModel]
//
//======================================================================
#ifndef _SCENEMODEL_H_
#define _SCENEMODEL_H_

#include <string>
#include <fbxsdk.h>
#pragma comment (lib, "libfbxsdk-mt.lib")

// モデル管理用ID
typedef enum
{
	MODEL_ID_SKYDOME,
	MODEL_ID_RIFLE,
	MODEL_ID_SHOTGUN,
	MODEL_ID_CAPSULE,
	MODEL_ID_MAX,
} MODEL_ID;

// モデル読み込み用のソース
static const std::string MODEL_SOURCE[] =
{
	"data/models/sky.x",
	"data/models/rifle.x",
	"data/models/shotgun.x",
	"data/models/scifi_capsule.x",
};

class CSceneModel : public CScene
{
public:
	//======================================================================
	//	コンストラクタ
	//======================================================================
	CSceneModel::CSceneModel(int priority) : CScene(priority)
	{
		D3DXMatrixIdentity(&m_Target);
		D3DXMatrixIdentity(&m_World);
		D3DXMatrixIdentity(&m_Move);
		D3DXMatrixIdentity(&m_Rotate);
		D3DXMatrixIdentity(&m_RotX);
		D3DXMatrixIdentity(&m_RotY);
		D3DXMatrixIdentity(&m_RotZ);
		D3DXMatrixIdentity(&m_Scale);
		m_MaterialNum = 0;
		m_TextureNum = 0;
		m_Texture = NULL;
		m_NormalmapTexture = NULL;
		m_Material = NULL;
		m_isIgnoreLight = false;
	}

	//======================================================================
	//	デストラクタ
	//======================================================================
	CSceneModel::~CSceneModel()
	{
	}

	void Init(const std::string& modelName);
	void Uninit();
	void Update();
	void Draw();
	void DrawWithShader()override;
	void SetWorld(D3DXMATRIX move);
	void SetNormalMapTexture(const std::string& texName);
	void Move(D3DXVECTOR3 pos);
	void Rotate(D3DXVECTOR3 rot);
	void Rotate(D3DXMATRIX rot);
	void Scale(D3DXVECTOR3 scale);
	bool AddTangentSpace();
	D3DXMATRIX& GetScale() { return m_Scale; }
	D3DXMATRIX& GetWorld() { return m_World; }
	LPD3DXMESH GetMesh() { return m_Mesh; }
	static CSceneModel* Create(const std::string& modelName, int layer = LAYER_OBJECT3D);
	static CSceneModel* Create(const std::string& modelName, bool isIgnore, int layer = LAYER_OBJECT3D);

private:
	D3DXMATRIX			m_Target;
	D3DXMATRIX			m_World;		// ワールド変換行列
	D3DXMATRIX			m_Move;			// 平行移動行列
	D3DXMATRIX			m_Rotate;		// 回転行列
	D3DXMATRIX			m_RotX;			// X軸回転行列
	D3DXMATRIX			m_RotY;			// Y軸回転行列
	D3DXMATRIX			m_RotZ;			// Z軸回転行列
	D3DXMATRIX			m_Scale;		// 拡大縮小行列
	LPD3DXMESH			m_Mesh;			// メッシュ
	DWORD				m_MaterialNum;	// マテリアル数
	int					m_TextureNum;	// テクスチャ数
	LPD3DXBUFFER		m_Material;		// マテリアル
	LPDIRECT3DTEXTURE9 *m_Texture;		// テクスチャ
	LPDIRECT3DTEXTURE9  m_NormalmapTexture; // 法線マップテクスチャ
	bool				m_isIgnoreLight;// 影を付けるかどうか
};

#endif //!_SCENEMODEL_H_