//======================================================================
//	2Dポリゴンヘッダ[scene2D.h]
//
//======================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

typedef struct
{
	D3DXVECTOR4 pos;
	D3DCOLOR	color;
	D3DXVECTOR2 texcoord;
}VERTEX_2D;

class CScene2D :public CScene
{
public:
	//======================================================================
	//	コンストラクタ
	//======================================================================
	CScene2D::CScene2D(int prioriy) :CScene(prioriy)
	{
		m_Pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		m_TexCoordX1 = 0.0f;
		m_TexCoordX2 = 1.0f;
		m_TexCoordY1 = 0.0f;
		m_TexCoordY2 = 1.0f;
		m_TexId = 0;
		m_Color = 0xffffffff;
	}

	//======================================================================
	//	デストラクタ
	//======================================================================
	~CScene2D() {}

	void Init(int texid, float texW, float texH);
	void Uninit();
	void Update();
	void Draw();

	void SetColor(D3DCOLOR color) { m_Color = color; }
	void SetTexCoord(float texCoordx1, float texCoordx2, float texCoordy1, float texCoordy2);
	void SetSize(D3DXVECTOR2 size) { m_TexSize = size; }

	static CScene2D* Create(int texid, float texW, float texH);
private:
	D3DXVECTOR2 m_TexSize;
	D3DCOLOR m_Color;
	float m_TexCoordX1;
	float m_TexCoordX2;
	float m_TexCoordY1;
	float m_TexCoordY2;
	int m_TexId;
};

#endif //!_SCENE2D_H_