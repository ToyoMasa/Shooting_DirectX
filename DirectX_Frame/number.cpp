//======================================================================
//	ナンバー処理　（2018/1/7）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"
#include "scene2D.h"
#include "number.h"

CScene2D *CNumber::m_Scene2D = NULL;

static const float NUMBER_TEX_WIDTH = 41.0f;
static const float NUMBER_TEX_HEIGHT = 64.0f;

void CNumber::Init()
{
	m_Size = 1.0f;
	m_Num = 0;

	if (m_Scene2D == NULL)
	{
		CTexture::Load(TEX_ID_NUMBER001);

		m_Scene2D = new CScene2D(LAYER_OBJECT2D);
		m_Scene2D->Init(TEX_ID_NUMBER001, NUMBER_TEX_WIDTH, NUMBER_TEX_HEIGHT);
	}
}

void CNumber::Uninit()
{
	if (m_Scene2D != NULL)
	{
		m_Scene2D->Release();
		m_Scene2D = NULL;
	}

	CTexture::Release(TEX_ID_NUMBER001);
}

void CNumber::Update()
{

}

void CNumber::Draw()
{
	DrawNum(m_Num, m_Pos.x, m_Pos.y, m_Size);
}

void CNumber::SetSize(float size)
{
	m_Size = size;
}

void CNumber::DrawNum(int n, float x, float y, float size)
{
	int digit = 0;
	int work = n;

	while (1)
	{
		digit++;
		work /= 10;
		if (work == 0)
		{
			break;
		}
	}

	work = n;

	m_Scene2D->SetVisible(true);
	m_Scene2D->SetSize(D3DXVECTOR2(NUMBER_TEX_WIDTH * size, NUMBER_TEX_HEIGHT * size));

	const float drawLeft = x - ((NUMBER_TEX_WIDTH + 30.0f) * size) * digit / 4.0f;

	while (digit > 0)
	{
		int num = work % 10;
		m_Scene2D->SetTexCoord(NUMBER_TEX_WIDTH * (num % 10) / (NUMBER_TEX_WIDTH * 10.0f),
			(NUMBER_TEX_WIDTH * (num % 10) + NUMBER_TEX_WIDTH) / (NUMBER_TEX_WIDTH * 10.0f),
			0.0f,
			1.0f);
		m_Scene2D->Set(D3DXVECTOR3(drawLeft	+ ((NUMBER_TEX_WIDTH + 30.0f) * size) * digit / 2.0f - (15.0f * size), y, 0));
		m_Scene2D->Draw();
		work /= 10;
		digit--;
	}

	m_Scene2D->SetVisible(false);
}

CNumber* CNumber::Create()
{
	CNumber* number = new CNumber();
	number->Init();

	return number;
}

//
//void DrawNumber(int n, float x, float y, float width, float height, D3DCOLOR color)
//{
//	UIDraw(TEXTURE_UI_NUMBER, x, y, width, height, color, (n % 10) * NUMBER2_WIDTH, 0, NUMBER2_WIDTH, NUMBER2_HEIGHT);
//}
//
//void DrawNumbers(int n, float x, float y, float width, float height, D3DCOLOR color)
//{
//	if (n == 0)
//	{
//		UIDraw(TEXTURE_UI_NUMBER, x - width / 2 - 0.5f, y, width, height, color, (n % 10) * NUMBER2_WIDTH, 0, NUMBER2_WIDTH - 8, NUMBER2_HEIGHT);
//		return;
//	}
//
//	int num = n;
//	int i = 0;
//
//	while (num > 0)
//	{
//		num /= 10;
//		i++;
//	}
//
//	num = n;
//	float dx = x - width * i / 2;
//
//	while (i > 0)
//	{
//		UIDraw(TEXTURE_UI_NUMBER, dx + ((width - 20.0f) * (i - 1)) - 0.5f, y, width, height, color, ((num % 10)) * NUMBER2_WIDTH, 0, NUMBER2_WIDTH - 8, NUMBER2_HEIGHT);
//
//		num /= 10;
//		i--;
//	}
//}
//
//void DrawScore(int score, int fig, float x, float y, float width, float height, D3DCOLOR color)
//{
//	int scorex = 1;
//
//	for (int i = 0; i < fig; i++)
//	{
//		scorex *= 10;
//	}
//
//	// カンストの処理
//	if (score >= scorex)
//	{
//		for (int i = 0; i < fig; i++)
//		{
//			DrawNumber(9, x + 1.0f * fig + (width - 20.0f) * (fig - (i + 1)), y, width, height, color);
//		}
//	}
//	else
//	{
//		for (int i = 0; i < fig; i++)
//		{
//			DrawNumber(score % 10, x + 1.0f * fig + (width - 20.0f) * (fig - (i + 1)), y, width, height, color);
//			score /= 10;
//		}
//	}
//}
//
//void DrawNumber2(int n, float x, float y, float width, float height, D3DCOLOR color)
//{
//	//	UIDraw(TEXTURE_UI_NUMBER2, x, y, width, height, color, (n % 5) * NUMBER2_WIDTH, (n / 5) * (NUMBER2_HEIGHT + 10.0f), NUMBER2_WIDTH, NUMBER2_HEIGHT);
//	UIDraw(TEXTURE_UI_NUMBER2, x, y, width, height, color, (n % 10) * NUMBER2_WIDTH, 0, NUMBER2_WIDTH, NUMBER2_HEIGHT);
//}
//
//void DrawNumbers2(int n, float x, float y, float width, float height, D3DCOLOR color)
//{
//	if (n == 0)
//	{
//		UIDraw(TEXTURE_UI_NUMBER2, x - width / 2 - 0.5f, y, width, height, color, (n % 10) * NUMBER2_WIDTH, 0, NUMBER2_WIDTH - 8, NUMBER2_HEIGHT);
//		return;
//	}
//
//	int num = n;
//	int i = 0;
//
//	while (num > 0)
//	{
//		num /= 10;
//		i++;
//	}
//
//	num = n;
//	float dx = x - width * i / 2;
//
//	while (i > 0)
//	{
//		UIDraw(TEXTURE_UI_NUMBER2, dx + ((width - 20.0f) * (i - 1)) - 0.5f, y, width, height, color, ((num % 10)) * NUMBER2_WIDTH, 0, NUMBER2_WIDTH - 8, NUMBER2_HEIGHT);
//
//		num /= 10;
//		i--;
//	}
//}
//
//void DrawScore2(int score, int fig, float x, float y, float width, float height, D3DCOLOR color)
//{
//	int scorex = 1;
//
//	for (int i = 0; i < fig; i++)
//	{
//		scorex *= 10;
//	}
//
//	// カンストの処理
//	if (score >= scorex)
//	{
//		for (int i = 0; i < fig; i++)
//		{
//			DrawNumber2(9, x + 1.0f * fig + (width - 20.0f) * (fig - (i + 1)), y, width, height, color);
//		}
//	}
//	else
//	{
//		for (int i = 0; i < fig; i++)
//		{
//			DrawNumber2(score % 10, x + 1.0f * fig + (width - 20.0f) * (fig - (i + 1)), y, width, height, color);
//			score /= 10;
//		}
//	}
//}
