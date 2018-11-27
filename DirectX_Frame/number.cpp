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

void CNumber::SetColor(D3DCOLOR color)
{
	m_Scene2D->SetColor(color);
}
