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
	void SetSize(float size);
	static void DrawNum(int n, float x, float y, float size = 1.0f);

	static CNumber* Create();

private:
	int				m_Num;
	float			m_Size;
	static CScene2D *m_Scene2D;
};

// 1Œ…‚Ì”š•`‰æ
void DrawNumber(int n, float x, float y, float width, float height, D3DCOLOR color);
// 2Œ…ˆÈã‚Ì”š•`‰æ	(x‚Í•`‰æ‚·‚é‚Ì’†SÀ•W)
void DrawNumbers(int n, float x, float y, float width, float height, D3DCOLOR color);

void DrawScore(int score, int fig, float x, float y, float width, float height, D3DCOLOR color);

// ƒtƒHƒ“ƒgˆá‚¢
void DrawNumber2(int n, float x, float y, float width, float height, D3DCOLOR color);
void DrawNumbers2(int n, float x, float y, float width, float height, D3DCOLOR color);
void DrawScore2(int score, int fig, float x, float y, float width, float height, D3DCOLOR color);

#endif // !_NUMBER_H_
