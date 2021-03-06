//======================================================================
//	2Dポリゴンヘッダ[scene2D.h]
//
//======================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

typedef struct
{
	D3DXVECTOR4 pos;
	D3DCOLOR	color;
	D3DXVECTOR2 texcoord;
}VERTEX_2D;

//*********************************************************
//	2Dオブジェクト
//*********************************************************
class CScene2D :public CScene
{
public:
	CScene2D::CScene2D(int priority) :CScene(priority)
	{
		m_Pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		m_TexCoordX1 = 0.0f;
		m_TexCoordX2 = 1.0f;
		m_TexCoordY1 = 0.0f;
		m_TexCoordY2 = 1.0f;
		m_RotationAngle = 0.0f;
		m_RotationOffsetX = 0.0f;
		m_RotationOffsetY = 0.0f;
		m_TexId = 0;
		m_Color = 0xffffffff;
	}

	~CScene2D() {}

	void Init(int texid, float texW, float texH);
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void DrawWithShader()override {}

	void Rotate(float rot) { m_RotationAngle = rot; }
	void SetColor(D3DCOLOR color) { m_Color = color; }
	void SetTexCoord(float texCoordx1, float texCoordx2, float texCoordy1, float texCoordy2);
	void SetSize(D3DXVECTOR2 size) { m_TexSize = size; }

	static CScene2D* Create(int texid, float texW, float texH, int priority = LAYER_OBJECT2D);
private:
	D3DXVECTOR2 m_TexSize;
	D3DCOLOR m_Color;
	float m_TexCoordX1;
	float m_TexCoordX2;
	float m_TexCoordY1;
	float m_TexCoordY2;
	float m_RotationAngle;
	float m_RotationOffsetX;
	float m_RotationOffsetY;
	int m_TexId;
};

#endif //!_SCENE2D_H_