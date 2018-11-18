//======================================================================
//	ビルボード処理ヘッダ[billboard.h]
//
//======================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include <string>

class CCamera;

static const int BILLBOARD_MAX = 10000;

enum DRAW_TYPE
{
	NORMAL = 0,
	FIXED_Y,
};

//bool BillBoardInit(void);			// ビルボードの初期化
//void BillBoardUninit(void);			// ビルボードの終了処理
//void BillBoardUpdate(void);			// ビルボードの更新
//
//									//************************************************************************
//									//	ビルボード描画（非設置物）
//									//	第一引数　使用するテクスチャ番号
//									//	第二引数　描画する場所
//									//	第三引数　サイズ
//									//************************************************************************
//void BillBoardDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale);						// 球状のビルボードの描画
//void BillBoardDrawCircle(D3DXVECTOR3 vPos, float scale);						// 球状のビルボードの描画
//void BillBoardColorDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale, D3DCOLOR color);
//
//void BillBoardAnimDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale, int tcx, int tcy, int tcw, int tch); // アニメーションビルボードの描画
//void BillBoardAnimDrawCircle(int texNum, float width, float height, D3DXVECTOR3 vPos, float scale, int tcx, int tcy, int tcw, int tch); // アニメーションビルボードの描画
//
//void BillBoardStaticDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale);					// 回転しないビルボードの描画
//
//																							//************************************************************************
//																							//	ビルボード描画（設置物）
//																							//	第一引数　使用するテクスチャ番号
//																							//	第二引数　描画する場所
//																							//	第三引数　サイズ
//																							//************************************************************************
//void BillBoardDrawStand(int texNum, D3DXVECTOR3 vPos, float scale);
//void BillBoardDrawStand(int texNum, float posX, float posY, float posZ, float scale);		// 直立するオブジェクトビルボードの描画


class CBillBoard
{
public:
	CBillBoard() 
	{
		for (int i = 0; i < BILLBOARD_MAX; i++)
		{
			if (m_BillBoards[i] == NULL)
			{
				this->SetId(i);
				m_BillBoards[i] = this;
				m_BillBoards[i]->m_DrawType = 0;
				m_BillBoards[i]->m_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
				m_BillBoards[i]->m_isVisible = true;
				break;
			}
		}
	}
	~CBillBoard() {}
	static void Init();
	static void Uninit();
	static void UpdateAll();
	static void Draw(int textureId, D3DXVECTOR3 vPos, float scale, CCamera* camera);
	void DrawOne(CCamera* camera);
	static void DrawFixedY(int textureId, D3DXVECTOR3 vPos, float scale, CCamera* camera);
	static void DrawBegin();
	static void DrawEnd();
	static void DrawAll(CCamera* camera);
	static void Set(int id, int texId, D3DXVECTOR3 pos, float scale, int drawtype);
	void Set(int texId, D3DXVECTOR3 pos, float scale, int drawtype);
	void Set(D3DXVECTOR3 pos, float scale, int drawtype);
	void Set(int texId, D3DXVECTOR3 pos, float scale, int drawtype, D3DCOLOR color);
	void SetId(int Id) { m_Id = Id; }
	void SetVisible(bool visible) { m_isVisible = visible; }
	void Release();
	static void ReleaseAll();
	static CBillBoard* Create(int texId);

private:
	int								m_Id;					// 管理番号
	int								m_DrawType;				// ビルボードの描画タイプ
	int								m_TextureId;			// テクスチャ番号
	D3DXVECTOR3						m_Pos;					// 場所
	D3DCOLOR						m_Color;				// 色
	float							m_ScaleX;				// サイズ
	float							m_ScaleY;				// サイズ
	float							m_ScaleZ;				// サイズ
	bool							m_isVisible;			// 描画が有効かどうか
	static D3DMATERIAL9				m_Mat;					// モデル1部分につき1個
	static LPDIRECT3DVERTEXBUFFER9	m_VertexBuffer;			// 頂点バッファ
	static LPDIRECT3DINDEXBUFFER9	m_IndexBuffer;			// インデックスバッファ
	static D3DXMATRIX				m_World;				// ワールド変換行列
	static D3DXMATRIX				m_Move;					// 座標変換行列
	static D3DXMATRIX				m_Scale;				// 拡大縮小行列
	static CBillBoard*				m_BillBoards[BILLBOARD_MAX];		// ビルボード
};


#endif // !_BILLBOARD_H_
