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

void CNumber::Init()
{
	CTexture::Load(TEX_ID_NUMBER001);

	CNumber::m_Scene2D = new CScene2D(LAYER_OBJECT2D);
	CNumber::m_Scene2D->Init(TEX_ID_NUMBER001, 65, 93);
}

void CNumber::Uninit()
{
	if (CNumber::m_Scene2D != NULL)
	{
		CNumber::m_Scene2D->Release();
	}

	CTexture::Release(TEX_ID_NUMBER001);
}

void CNumber::Draw(int n, float x, float y)
{
	CNumber::m_Scene2D->SetTexCoord(68 * (n % 5) / 340.0f, (68 * (n % 5) + 68) / 340.0f, 0.5f * (n / 5), 0.5f * (n / 5) + 0.5f);
	CNumber::m_Scene2D->Set(D3DXVECTOR3(x, y, 0));
	CNumber::m_Scene2D->Draw();
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
