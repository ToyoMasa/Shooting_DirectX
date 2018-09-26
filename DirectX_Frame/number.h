//======================================================================
//	�����`�揈���w�b�_[number.h]
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

// 1���̐����`��
void DrawNumber(int n, float x, float y, float width, float height, D3DCOLOR color);
// 2���ȏ�̐����`��	(x�͕`�悷�鎞�̒��S���W)
void DrawNumbers(int n, float x, float y, float width, float height, D3DCOLOR color);

void DrawScore(int score, int fig, float x, float y, float width, float height, D3DCOLOR color);

// �t�H���g�Ⴂ
void DrawNumber2(int n, float x, float y, float width, float height, D3DCOLOR color);
void DrawNumbers2(int n, float x, float y, float width, float height, D3DCOLOR color);
void DrawScore2(int score, int fig, float x, float y, float width, float height, D3DCOLOR color);

#endif // !_NUMBER_H_
