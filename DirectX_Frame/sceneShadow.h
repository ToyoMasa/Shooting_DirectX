//======================================================================
//	影描画処理ヘッダ[shadow.h]
//
//======================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

class CSceneShadow : public CScene
{
public:
	//======================================================================
	//	コンストラクタ
	//======================================================================
	CSceneShadow::CSceneShadow() : CScene(LAYER_EFFEKSEER)
	{

	}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawWithShader()override {}
	static CSceneShadow* Create();

	void Move(D3DXVECTOR3 pos);
	void Rotate(D3DXVECTOR3 rot);
	void Rotate(D3DXMATRIX rot);
	void Scale(D3DXVECTOR3 scale);

private:
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer; // 頂点バッファ
	LPDIRECT3DINDEXBUFFER9  m_IndexBuffer;  // インデックスバッファ
	D3DXMATRIX				m_World;
	D3DXMATRIX				m_Move;			// 平行移動行列
	D3DXMATRIX				m_Rotate;		// 回転行列
	D3DXMATRIX				m_RotX;			// X軸回転行列
	D3DXMATRIX				m_RotY;			// Y軸回転行列
	D3DXMATRIX				m_RotZ;			// Z軸回転行列
	D3DXMATRIX				m_Scale;		// 拡大縮小行列

};

#endif // !_SHADOW_H_
