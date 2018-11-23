#include "common.h"
#include "renderer.h"

//======================================================================
//	静的メンバ変数の初期化
//======================================================================
CScene *CScene::m_Scene[PRIORITY_MAX][OBJECT_MAX] = { NULL };

CScene::CScene(int priority)
{
	for (int i = 0; i < OBJECT_MAX; i++)
	{
		if (m_Scene[priority][i] == NULL)
		{
			m_Scene[priority][i] = this;
			m_Type = SCENE_TYPE_NONE;
			m_Visible = true;
			m_Shader = NULL;
			break;
		}
	}
}

void CScene::Set(const D3DXVECTOR3& pos)
{
	m_Pos = pos;
}

void CScene::Release()
{
	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			if (m_Scene[j][i] == this)
			{
				Uninit();
				m_Scene[j][i] = NULL;
				delete this;
				break;
			}
		}
	}
}

void CScene::UpdateAll()
{
	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			if (m_Scene[j][i] != NULL)
			{
				m_Scene[j][i]->Update();
			}
		}
	}
}

void CScene::DrawAll()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// αテスト(3つセット)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// αテストのON/OFF
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);					// 第2引数は0～255の好きな値
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 第2引数は不等号(GREATERは大なり)、上の値より大きければ合格

	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		switch (j)
		{
		case LAYER_BACKGROUND:
			// FVF(今から使用する頂点情報)の設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			break;
		case LAYER_OBJECT2D:
			pDevice->SetFVF(FVF_VERTEX_2D);
			break;

		default:
			// FVF(今から使用する頂点情報)の設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			break;
		}
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			if (m_Scene[j][i] != NULL)
			{
				if (m_Scene[j][i]->m_Visible)
				{  
					if (m_Scene[j][i]->m_Shader != NULL)
					{
						m_Scene[j][i]->DrawWithShader();
					}
					else
					{
						m_Scene[j][i]->Draw();
					}
				}
			}
		}
	}

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストのON/OFF
}

void CScene::ReleaseAll()
{
	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			if (m_Scene[j][i] != NULL)
			{
				m_Scene[j][i]->Release();
			}
		}
	}
}
