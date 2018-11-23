//======================================================================
//	3Dポリゴンヘッダ[scene3D]
//
//======================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include <string>

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 NV; // 法線
	DWORD color;
	D3DXVECTOR2 texcoord;
} VERTEX_3D;

class CScene3D : public CScene
{
public:
	//======================================================================
	//	コンストラクタ
	//======================================================================
	CScene3D::CScene3D(int priority) : CScene(priority)
	{
		m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_VertexBuffer = NULL;
		m_IndexBuffer = NULL;
		m_Vertex = NULL;
		D3DXMatrixIdentity(&m_World);
	}

	//======================================================================
	//	デストラクタ
	//======================================================================
	CScene3D::~CScene3D()
	{
	}

	void Init(int texId, float meshSize, int sizeX, int sizeY, int numPrimitive, int numVertex, int numIndex);
	void Init(int texId, VERTEX_3D* vertex, WORD* index, int numPrimitive, int numVertex, int numIndex);
	void Uninit();
	void Update();
	void Draw();
	void DrawWithShader()override;
	void Change(VERTEX_3D* vertex, WORD* index);
	VERTEX_3D* GetVertex() { return m_Vertex; }
	static CScene3D* Create(int texId, float meshSize, int sizeX, int sizeY, int numPrimitive, int numVertex, int numIndex);
	static CScene3D* Create(int texId, VERTEX_3D* vertex, WORD* index, int numPrimitive, int numVertex, int numIndex);
	static CScene3D* Create(int texId, float meshSize, int sizeX, int sizeY, int numPrimitive, int numVertex, int numIndex, LAYER layer);
	static CScene3D* Create(int texId, VERTEX_3D* vertex, WORD* index, int numPrimitive, int numVertex, int numIndex, LAYER layer);

private:
	D3DMATERIAL9			m_Mat;			// マテリアル
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer; // 頂点バッファ
	LPDIRECT3DINDEXBUFFER9  m_IndexBuffer;  // インデックスバッファ
	D3DXMATRIX				m_World;

	int m_NumVertex;
	int m_NumIndex;
	int m_NumPrimitive;
	int m_TexId;

	VERTEX_3D* m_Vertex;
};

#endif //!_SCENE3D_H_
