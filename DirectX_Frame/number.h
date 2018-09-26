//======================================================================
//	”š•`‰æˆ—ƒwƒbƒ_[number.h]
//
//======================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#define NUMBER_WIDTH (90)
#define NUMBER_HEIGHT (140)
#define NUMBER2_WIDTH (128)
#define NUMBER2_HEIGHT (137)

class CNumber
{
public:
	CNumber() {}
	~CNumber() {}

	static void Init();
	static void Uninit();
	static void Draw(int n, float x, float y);

private:
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
