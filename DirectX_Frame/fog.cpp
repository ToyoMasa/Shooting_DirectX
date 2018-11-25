//======================================================================
//	フォグの設定[fog.cpp]　（2018/11/14）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "fog.h"

void CFog::Set(D3DCOLOR const&color, float const&start, float const&end)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	//フォグの設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
	pDevice->SetRenderState(D3DRS_FOGCOLOR, color); //白色で不透明
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);      //頂点モード
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);     //テーブルモード
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start)); //開始位置
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));     //終了位置
}

void CFog::Set(D3DCOLOR const&color, float const&density)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	//フォグの設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
	pDevice->SetRenderState(D3DRS_FOGCOLOR, color); //白色で不透明
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);      //頂点モード
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);     //テーブルモード
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&density)); //開始位置
}

void CFog::End()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
}
