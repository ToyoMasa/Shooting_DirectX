//======================================================================
//	”š•`‰æˆ—ƒwƒbƒ_[number.h]
//
//======================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

class CNumber : public CScene
{
public:
	CNumber() : CScene(LAYER_OBJECT2D) {}
	~CNumber() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void DrawWithShader()override {}

	void SetNum(int num) { m_Num = num; }
	void SetColor(D3DCOLOR color);
	void SetSize(float size);
	static void DrawNum(int n, float x, float y, float size = 1.0f);

	static CNumber* Create();

private:
	int				m_Num;
	float			m_Size;
	static CScene2D *m_Scene2D;
};

#endif // !_NUMBER_H_
