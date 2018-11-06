//======================================================================
//	ボックス描画処理ヘッダ[box.h]
//
//======================================================================
#ifndef _BOX_H_
#define _BOX_H_

#define BOX_TYPE_NUM (1)

class CShader;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 NV; // 法線
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
} VERTEX3D_BOX;

class CBox
{
public:
	CBox();
	~CBox();

	bool Init(float X, float Y, float Z, int texID);
	void Uninit(void);

	void Draw(D3DXMATRIX mtxWorld);
	void DrawWithShader(D3DXMATRIX mtxWorld, CShader* shader);

private:
	VERTEX3D_BOX m_v[24];
	WORD m_index[36];

	D3DMATERIAL9 m_mat; // モデル1部分につき1個
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer; //頂点バッファ
	LPDIRECT3DINDEXBUFFER9  m_pIndexBuffer;  //インデックスバッファ

	int m_TexID;
};

#endif // !_BOX_H_
