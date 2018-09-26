//======================================================================
//	3Dモデルヘッダ[sceneModelFBX]
//
//======================================================================
#ifndef _SCENEMODELFBX_H_
#define _SCENEMODELFBX_H_

#include <string>
#include <list>
#include <fbxsdk.h>

#pragma comment (lib, "libfbxsdk-mt.lib")

//-------------------------------------------------
// 構造体宣言
//-------------------------------------------------
typedef struct VERTEX
{
	float posX, posY, posZ;
	float normalX, normalY, normalZ;
} Vertex;

// UV
typedef struct Point2DF 
{
	float x, y;

	bool operator==(Point2DF& val) 
	{
		if (this->x == val.x && this->y == val.y) 
		{
			return true;
		}
		return false;
	}
} point2;

typedef struct __UV_SET__ 
{
	std::string uvSetName;
	std::list<std::string> textures;
	std::list<point2> uvBuffer;
} UvSet;

typedef struct COLOR_RGBAF {
	float r;
	float g;
	float b;
	float a;
} ColorRGBA;

// モデル管理用ID
typedef enum
{
	FBX_ID_XBOT = 0,		// 
} FBX_MODEL_ID;

// モデル読み込み用のソース
static const std::string FBX_MODEL_SOURCE[] =
{
	"data/models/Running.fbx",
};

class CSceneModelFBX : public CScene
{
public:
	//======================================================================
	//	コンストラクタ
	//======================================================================
	CSceneModelFBX::CSceneModelFBX(int priority) : CScene(priority)
	{
		D3DXMatrixIdentity(&m_World);
		D3DXMatrixIdentity(&m_Move);
		D3DXMatrixIdentity(&m_Rotate);
		D3DXMatrixIdentity(&m_RotX);
		D3DXMatrixIdentity(&m_RotY);
		D3DXMatrixIdentity(&m_RotZ);
		D3DXMatrixIdentity(&m_Scale);

	}

	//======================================================================
	//	デストラクタ
	//======================================================================
	CSceneModelFBX::~CSceneModelFBX()
	{
	}

	static void InitFbxManager();
	static void UninitFbxManager();
	void Init(const std::string& modelName);
	void Uninit();
	void Update();
	void Draw();
	void SetWorld(D3DXMATRIX move);
	void Move(D3DXVECTOR3 pos);
	void Rotate(D3DXVECTOR3 rot);
	void Rotate(D3DXMATRIX rot);
	void Scale(D3DXVECTOR3 scale);
	static CSceneModelFBX* Create(const std::string& modelName);
	/*static CSceneModelFBX* Create(const std::string& modelName, bool isIgnore);*/
	void GetMesh(FbxNode* node);			// メッシュ情報取得
	void GetPosition(FbxMesh* mesh);		// 頂点座標取得
	void GetNormal(FbxMesh* mesh);			// 法線情報取得
	void GetVertexUV(FbxMesh* mesh);		// UV情報取得
	void GetTextureNames(FbxMesh* mesh);	// テクスチャ情報取得
	void GetVertexColor(FbxMesh* mesh);		// 頂点カラー取得

private:
	D3DXMATRIX		m_World;		// ワールド変換行列
	D3DXMATRIX		m_Move;			// 平行移動行列
	D3DXMATRIX		m_Rotate;		// 回転行列
	D3DXMATRIX		m_RotX;			// X軸回転行列
	D3DXMATRIX		m_RotY;			// Y軸回転行列
	D3DXMATRIX		m_RotZ;			// Z軸回転行列
	D3DXMATRIX		m_Scale;		// 拡大縮小行列
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer; // 頂点バッファ
	LPDIRECT3DINDEXBUFFER9  m_IndexBuffer;  // インデックスバッファ

	FbxManager*		m_FbxManager;	// FbxManagerの生成
	FbxImporter*	m_FbxImporter;	// Importerの生成
	FbxIOSettings*	m_IOSettings;	// 入出力設定
	FbxScene*		m_Scene;		// シーンオブジェクト
	FbxNode*		m_RootNode;		// ルートノード
	FbxNode*		m_ChildNode;	// 子ノード
	int				m_NodeCount;	// ノード数
	int				m_PolygonCount;	// ポリゴン数
	int*			m_PolygonSize;	// ポリゴンサイズ
	VERTEX*			m_Vertex;		// 頂点バッファ	
	int				m_VertexCount;	// 頂点数
	int				m_IndexCount;	// インデックスバッファ数
	DWORD*			m_Index;		// インデックスバッファ
	UvSet			m_UVset;		// UV
	ColorRGBA*		m_VertexColor;	// 頂点カラー
};

#endif //!_SCENEMODELFBX_H_