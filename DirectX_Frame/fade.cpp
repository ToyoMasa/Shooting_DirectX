#include "common.h"
#include "main.h"
#include "mode.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "camera.h"
#include "light.h"
#include "manager.h"
#include "texture.h"
#include "billboard.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "skybox.h"
#include "number.h"
#include "title.h"
#include "game.h"
#include "fade.h"

int CFade::m_Alpha;
bool CFade::m_FadeIn = false;
bool CFade::m_FadeOut = false;
CMode* CFade::m_NextMode = NULL;

void CFade::Init()
{
	m_Alpha = 255;
}

void CFade::Uninit()
{

}

void CFade::Update()
{
	if (m_FadeIn)
	{
		if (m_Alpha > 3)
		{
			m_Alpha -= 255 / 120;
		}
		else
		{
			m_FadeIn = false;
			m_FadeOut = false;
		}
	}

	if (m_FadeOut)
	{
		if (m_Alpha < 252)
		{
			m_Alpha += 255 / 120;
		}
		else
		{
			m_FadeOut = false;
			m_FadeIn = false;

			CManager::SetMode(m_NextMode);
		}
	}

}

void CFade::Draw()
{
	if (m_FadeIn || m_FadeOut)
	{
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
		if (pDevice == NULL)
		{
			return;
		}

		VERTEX_2D vertex[4];
		vertex[0].pos = D3DXVECTOR4(0 - 0.5f, 0, 0, 1);
		vertex[1].pos = D3DXVECTOR4(SCREEN_WIDTH - 0.5f, 0, 0, 1);
		vertex[2].pos = D3DXVECTOR4(0 - 0.5f, SCREEN_HEIGHT, 0, 1);
		vertex[3].pos = D3DXVECTOR4(SCREEN_WIDTH - 0.5f, SCREEN_HEIGHT, 0, 1);

		vertex[0].texcoord = D3DXVECTOR2(0, 0);
		vertex[1].texcoord = D3DXVECTOR2(1, 0);
		vertex[2].texcoord = D3DXVECTOR2(0, 1);
		vertex[3].texcoord = D3DXVECTOR2(1, 1);

		vertex[0].color =
			vertex[1].color =
			vertex[2].color =
			vertex[3].color = D3DCOLOR_RGBA(0, 0, 0, m_Alpha);

		pDevice->SetTexture(0, NULL);
		pDevice->SetFVF(FVF_VERTEX_2D);

		// αテスト(3つセット)
		//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// αテストのON
		//pDevice->SetRenderState(D3DRS_ALPHAREF, 128);					// 第2引数は0～255の好きな値
		//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 第2引数は不等号(GREATERは大なり)、上の値より大きければ合格

		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vertex[0], sizeof(VERTEX_2D));

		//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// αテストのOFF
	}
}

void CFade::FadeIn()
{
	m_FadeIn = true;
	m_FadeOut = false;
}

void CFade::FadeOut(CMode* nextMode)
{
	m_FadeOut = true;
	m_FadeIn = false;

	m_NextMode = nextMode;
}
