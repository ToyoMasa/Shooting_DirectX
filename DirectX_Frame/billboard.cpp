//======================================================================
//	ビルボード描画　（2018/04/25）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "camera.h"
#include "scene3D.h"
#include "texture.h"
#include "billboard.h"

//======================================================================
//	静的メンバ変数の初期化
//======================================================================
D3DMATERIAL9			CBillBoard::m_Mat;					// モデル1部分につき1個
LPDIRECT3DVERTEXBUFFER9	CBillBoard::m_VertexBuffer = NULL;	// 頂点バッファ
LPDIRECT3DINDEXBUFFER9	CBillBoard::m_IndexBuffer = NULL;	// インデックスバッファ
D3DXMATRIX				CBillBoard::m_World;				// ワールド変換行列
D3DXMATRIX				CBillBoard::m_Move;					// 座標変換行列
D3DXMATRIX				CBillBoard::m_Scale;				// 拡大縮小行列
CBillBoard*				CBillBoard::m_BillBoards[BILLBOARD_MAX] = { NULL };

//======================================================================
//	グローバル変数
//======================================================================

void CBillBoard::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	HRESULT hr;

	CTexture::Load(TEX_ID_CIRCLE);

	// 頂点バッファ							↓大きい分には問題ない
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);
	
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}
	
	// インデックスバッファ					↓大きい分には問題ない					↓sizeがDWORDなら32
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * 4, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);
	
	if (FAILED(hr))
	{
		MessageBox(NULL, "インデックスバッファの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}
	
	VERTEX_3D* pV;
	
	// 頂点バッファ
	m_VertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
	
	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
	pV[1] = { D3DXVECTOR3( 0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) };
	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
	pV[3] = { D3DXVECTOR3( 0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) };
	
	m_VertexBuffer->Unlock();
	
	// インデックスバッファ
	LPWORD pIndex;
	m_IndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
	
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 3;
	
	m_IndexBuffer->Unlock();
	
	// ワールド座標行列の初期化
	D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_Move);
	D3DXMatrixIdentity(&m_Scale);
}

void CBillBoard::Uninit()
{
	CTexture::Release(TEX_ID_CIRCLE);
	CBillBoard::ReleaseAll();

	//頂点バッファの解放
	if (m_VertexBuffer != NULL)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = NULL;
	}
	
	//インデックスバッファの解放
	if (m_IndexBuffer != NULL)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = NULL;
	}
}

void CBillBoard::UpdateAll()
{

}

void CBillBoard::Draw(int textureId, D3DXVECTOR3 vPos, float scale, CCamera* camera)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ワールド座標行列をセット
	D3DXMatrixTranslation(&m_Move, vPos.x, vPos.y, vPos.z);
	D3DXMatrixScaling(&m_Scale, scale, scale, scale);

	m_World = m_Scale * m_Move;

	D3DXMATRIX mtxViewRotInv = camera->GetView();

	// ビュー行列の逆行列を作成
	// 平行移動を無効にする
	mtxViewRotInv._41 = 0.0f;
	mtxViewRotInv._42 = 0.0f;
	mtxViewRotInv._43 = 0.0f;

	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);

	m_World = mtxViewRotInv * m_World;

	pDevice->SetTexture(0, CTexture::GetTexture(textureId));

	//各種行列の設定(自分のやりたいところでやる)
	pDevice->SetTransform(D3DTS_WORLD, &m_World);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

void CBillBoard::DrawOne(CCamera* camera)
{
	if (m_isVisible)
	{
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
		if (pDevice == NULL)
		{
			return;
		}

		// ワールド座標行列をセット
		D3DXMatrixTranslation(&m_Move, m_Pos.x, m_Pos.y, m_Pos.z);
		D3DXMatrixScaling(&m_Scale, m_ScaleX, m_ScaleY, m_ScaleZ);

		m_World = m_Scale * m_Move;

		D3DXMATRIX mtxViewRotInv = camera->GetView();

		// ビュー行列の逆行列を作成
		// m_DrawTypeで切替
		switch (m_DrawType)
		{
		case 0:
			mtxViewRotInv._41 = 0.0f;
			mtxViewRotInv._42 = 0.0f;
			mtxViewRotInv._43 = 0.0f;
			break;

		case 1:
			mtxViewRotInv._21 = 0.0f;
			mtxViewRotInv._32 = 0.0f;
			mtxViewRotInv._12 = 0.0f;
			mtxViewRotInv._23 = 0.0f;
			mtxViewRotInv._41 = 0.0f;
			mtxViewRotInv._42 = 0.0f;
			mtxViewRotInv._43 = 0.0f;

		default:
			break;
		}

		VERTEX_3D* pV;

		m_VertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

		pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), m_Color, D3DXVECTOR2(0.0f, 0.0f) };
		pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), m_Color, D3DXVECTOR2(1.0f, 0.0f) };
		pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), m_Color, D3DXVECTOR2(0.0f, 1.0f) };
		pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), m_Color, D3DXVECTOR2(1.0f, 1.0f) };

		m_VertexBuffer->Unlock();

		D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);

		// 頂点バッファとインデックスバッファの設定
		pDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_3D));
		pDevice->SetIndices(m_IndexBuffer);

		m_World = mtxViewRotInv * m_World;

		pDevice->SetTexture(0, CTexture::GetTexture(m_TextureId));

		//各種行列の設定(自分のやりたいところでやる)
		pDevice->SetTransform(D3DTS_WORLD, &m_World);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
	}
}

void CBillBoard::DrawFixedY(int textureId, D3DXVECTOR3 vPos, float scale, CCamera* camera)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ワールド座標行列をセット
	D3DXMatrixTranslation(&m_Move, vPos.x, vPos.y, vPos.z);
	D3DXMatrixScaling(&m_Scale, scale, scale, scale);

	m_World = m_Scale * m_Move;

	D3DXMATRIX mtxViewRotInv = camera->GetView();

	// ビュー行列の逆行列を作成
	// 平行移動とY軸以外の回転を無効にする
	mtxViewRotInv._21 = 0.0f;
	mtxViewRotInv._32 = 0.0f;
	mtxViewRotInv._12 = 0.0f;
	mtxViewRotInv._23 = 0.0f;
	mtxViewRotInv._41 = 0.0f;
	mtxViewRotInv._42 = 0.0f;
	mtxViewRotInv._43 = 0.0f;

	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);

	m_World = mtxViewRotInv * m_World;

	pDevice->SetTexture(0, CTexture::GetTexture(textureId));

	//各種行列の設定(自分のやりたいところでやる)
	pDevice->SetTransform(D3DTS_WORLD, &m_World);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

void CBillBoard::DrawBegin()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ライティング使うときに外す
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// αテスト(3つセット)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// αテストのON/OFF
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);					// 第2引数は0～255の好きな値
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 第2引数は不等号(GREATERは大なり)、上の値より大きければ合格

	// 加算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// FVF(今から使用する頂点情報)の設定
	pDevice->SetFVF(FVF_VERTEX_3D);
}

void CBillBoard::DrawEnd()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CBillBoard::DrawAll(CCamera* camera)
{
	CBillBoard::DrawBegin();

	for (int i = 0; i < BILLBOARD_MAX; i++)
	{
		if (CBillBoard::m_BillBoards[i] != NULL)
		{
			CBillBoard::m_BillBoards[i]->DrawOne(camera);
		}
	}
	CBillBoard::DrawEnd();
}

void CBillBoard::Set(int id, int texId, D3DXVECTOR3 pos, float scale, int drawtype)
{
	CBillBoard::m_BillBoards[id]->m_TextureId = texId;
	CBillBoard::m_BillBoards[id]->m_Pos = pos;
	CBillBoard::m_BillBoards[id]->m_ScaleX = scale;
	CBillBoard::m_BillBoards[id]->m_ScaleY = scale;
	CBillBoard::m_BillBoards[id]->m_ScaleZ = scale;
	CBillBoard::m_BillBoards[id]->m_DrawType = drawtype;

	if (drawtype == FIXED_Y)
	{
		CBillBoard::m_BillBoards[id]->m_Pos.y += 0.5f * scale;
	}
}

void CBillBoard::Set(int texId, D3DXVECTOR3 pos, float scale, int drawtype)
{
	m_TextureId = texId;
	m_Pos = pos;
	m_ScaleX = scale;
	m_ScaleY = scale;
	m_ScaleZ = scale;
	m_DrawType = drawtype;

	if (drawtype == FIXED_Y)
	{
		m_Pos.y += 0.5f * scale;
	}
}

void CBillBoard::Set(int texId, D3DXVECTOR3 pos, float scale, int drawtype, D3DCOLOR color)
{
	m_TextureId = texId;
	m_Pos = pos;
	m_ScaleX = scale;
	m_ScaleY = scale;
	m_ScaleZ = scale;
	m_DrawType = drawtype;
	m_Color = color;

	if (drawtype == FIXED_Y)
	{
		m_Pos.y += 0.5f * scale;
	}
}

void CBillBoard::Release()
{
	for (int i = 0; i < BILLBOARD_MAX; i++)
	{
		if (m_BillBoards[i] == this)
		{
			m_BillBoards[i] = NULL;
			delete this;
			break;
		}
	}
}

void CBillBoard::ReleaseAll()
{
	for (int i = 0; i < BILLBOARD_MAX; i++)
	{
		if (m_BillBoards[i] != NULL)
		{
			m_BillBoards[i]->Release();
		}
	}
}

CBillBoard* CBillBoard::Create(int texId)
{
	CBillBoard* billboard = new CBillBoard();
	billboard->Set(texId, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, 0);

	CTexture::Load(texId);


	return billboard;
}

//typedef struct
//{
//	D3DXVECTOR3 pos;
//	D3DXVECTOR3 NV; // 法線
//	D3DCOLOR color;
//	D3DXVECTOR2 texcoord;
//} VERTEX3D;
//
//static LPDIRECT3DTEXTURE9 g_pTextures[BB_TEXTURE_MAX];
//static D3DMATERIAL9 g_mat; // モデル1部分につき1個
//static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; //頂点バッファ
//static LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer = NULL;  //インデックスバッファ
//static D3DXMATRIX g_mtxWorld;			//ワールド変換行列
//static D3DXMATRIX g_mtxMove;			//座標変換行列
//static D3DXMATRIX g_mtxScale;			//拡大縮小行列
//
//void BillBoardUpdate(void)			// ビルボードの更新
//{
//
//}
//
//void BillBoardDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale)			// ビルボードの描画
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	// ワールド座標行列をセット
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// ビュー行列の逆行列を作成
//	// 平行移動とY軸以外の回転を無効にする
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// テクスチャのセット
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(今から使用する頂点情報)の設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// 頂点バッファとインデックスバッファの設定
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//各種行列の設定(自分のやりたいところでやる)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//}
//void BillBoardDrawStand(int texNum, D3DXVECTOR3 vPos, float scale)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	// ワールド座標行列をセット
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y + scale * 0.5f, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// ビュー行列の逆行列を作成
//	// 平行移動とY軸以外の回転を無効にする
//	mtxViewRotInv._21 = 0.0f;
//	mtxViewRotInv._32 = 0.0f;
//	mtxViewRotInv._12 = 0.0f;
//	mtxViewRotInv._23 = 0.0f;
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ライティング使うときに外す
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// αテスト(3つセット)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// αテストのON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);					// 第2引数は0～255の好きな値
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 第2引数は不等号(GREATERは大なり)、上の値より大きければ合格
//
//																	// テクスチャのセット
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(今から使用する頂点情報)の設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// 頂点バッファとインデックスバッファの設定
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//各種行列の設定(自分のやりたいところでやる)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストのON/OFF
//}
//
//void BillBoardDrawStand(int texNum, float posX, float posY, float posZ, float scale)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	// ワールド座標行列をセット
//	D3DXMatrixTranslation(&g_mtxMove, posX, posY + scale * 0.5f, posZ);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// ビュー行列の逆行列を作成
//	// 平行移動とY軸以外の回転を無効にする
//	mtxViewRotInv._21 = 0.0f;
//	mtxViewRotInv._32 = 0.0f;
//	mtxViewRotInv._12 = 0.0f;
//	mtxViewRotInv._23 = 0.0f;
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ライティング使うときに外す
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// αテスト(3つセット)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// αテストのON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);					// 第2引数は0～255の好きな値
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 第2引数は不等号(GREATERは大なり)、上の値より大きければ合格
//
//																	// テクスチャのセット
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(今から使用する頂点情報)の設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// 頂点バッファとインデックスバッファの設定
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//各種行列の設定(自分のやりたいところでやる)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストのON/OFF
//}
//
//// アニメーションビルボードの描画
//void BillBoardAnimDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale, int tcx, int tcy, int tcw, int tch)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	int tw = g_aBBTextureFileName[texNum].width;
//	int th = g_aBBTextureFileName[texNum].height;
//	float u0 = (float)tcx / tw;					//テクスチャの切り取り
//	float v0 = (float)tcy / th;					//
//	float u1 = (float)(tcx + tcw) / tw;
//	float v1 = (float)(tcy + tch) / th;
//
//	VERTEX3D* pV;
//
//	// 頂点バッファ
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u0, v0) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u1, v0) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u0, v1) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u1, v1) };
//
//	g_pVertexBuffer->Unlock();
//
//	// ワールド座標行列をセット
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// ビュー行列の逆行列を作成
//	// 平行移動とY軸以外の回転を無効にする
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ライティング使うときに外す
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// αテスト(3つセット)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// αテストのON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 16);					// 第2引数は0～255の好きな値
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 第2引数は不等号(GREATERは大なり)、上の値より大きければ合格
//
//																	// テクスチャのセット
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(今から使用する頂点情報)の設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// 頂点バッファとインデックスバッファの設定
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//各種行列の設定(自分のやりたいところでやる)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストのON/OFF
//
//																// 頂点バッファ
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) };
//
//	g_pVertexBuffer->Unlock();
//}
//
//// 色変えビルボードの描画
//void BillBoardColorDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale, D3DCOLOR color)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	VERTEX3D* pV;
//
//	// 頂点バッファ
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), color, D3DXVECTOR2(0.0f, 0.0f) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), color, D3DXVECTOR2(1.0f, 0.0f) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), color, D3DXVECTOR2(0.0f, 1.0f) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), color, D3DXVECTOR2(1.0f, 1.0f) };
//
//	g_pVertexBuffer->Unlock();
//
//	// ワールド座標行列をセット
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// ビュー行列の逆行列を作成
//	// 平行移動とY軸以外の回転を無効にする
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ライティング使うときに外す
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// αテスト(3つセット)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// αテストのON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// 第2引数は0～255の好きな値
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 第2引数は不等号(GREATERは大なり)、上の値より大きければ合格
//
//																	// テクスチャのセット
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(今から使用する頂点情報)の設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// 頂点バッファとインデックスバッファの設定
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//各種行列の設定(自分のやりたいところでやる)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストのON/OFF
//
//																// 頂点バッファ
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) };
//
//	g_pVertexBuffer->Unlock();
//}
//
//// アニメーションビルボードの描画
//void BillBoardAnimDrawCircle(int texNum, float width, float height, D3DXVECTOR3 vPos, float scale, int tcx, int tcy, int tcw, int tch)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	int tw = g_aBBTextureFileName[texNum].width;
//	int th = g_aBBTextureFileName[texNum].height;
//	float u0 = (float)tcx / tw;					//テクスチャの切り取り
//	float v0 = (float)tcy / th;					//
//	float u1 = (float)(tcx + tcw) / tw;
//	float v1 = (float)(tcy + tch) / th;
//
//	VERTEX3D* pV;
//
//	// 頂点バッファ
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-width / 2,  height / 2, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u0, v0) };
//	pV[1] = { D3DXVECTOR3(width / 2,  height / 2, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u1, v0) };
//	pV[2] = { D3DXVECTOR3(-width / 2, -height / 2, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u0, v1) };
//	pV[3] = { D3DXVECTOR3(width / 2, -height / 2, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u1, v1) };
//
//	g_pVertexBuffer->Unlock();
//
//	// ワールド座標行列をセット
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// ビュー行列の逆行列を作成
//	// 平行移動とY軸以外の回転を無効にする
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ライティング使うときに外す
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// αテスト(3つセット)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// αテストのON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 16);					// 第2引数は0～255の好きな値
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 第2引数は不等号(GREATERは大なり)、上の値より大きければ合格
//
//																	// テクスチャのセット
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(今から使用する頂点情報)の設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// 頂点バッファとインデックスバッファの設定
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//各種行列の設定(自分のやりたいところでやる)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストのON/OFF
//
//																// 頂点バッファ
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) };
//
//	g_pVertexBuffer->Unlock();
//}
//
//// 回転しないビルボードの描画
//void BillBoardStaticDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale)
//{
//}
//
//CBillBoard::CBillBoard()
//{
//	m_Texture = NULL;
//	m_VertexBuffer = NULL;
//	m_IndexBuffer = NULL;
//}
//
//CBillBoard::~CBillBoard()
//{
//}
//
//void CBillBoard::Init()
//{
//
//}
//
//void CBillBoard::Uninit()
//{
//	//頂点バッファの解放
//	if (m_VertexBuffer != NULL)
//	{
//		m_VertexBuffer->Release();
//		m_VertexBuffer = NULL;
//	}
//
//	//インデックスバッファの解放
//	if (m_IndexBuffer != NULL)
//	{
//		m_IndexBuffer->Release();
//		m_IndexBuffer = NULL;
//	}
//}
