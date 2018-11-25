//======================================================================
//	�t�H�O�̐ݒ�[fog.cpp]�@�i2018/11/14�j
//	Author : �L�� ���r
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

	//�t�H�O�̐ݒ�
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
	pDevice->SetRenderState(D3DRS_FOGCOLOR, color); //���F�ŕs����
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);      //���_���[�h
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);     //�e�[�u�����[�h
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start)); //�J�n�ʒu
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));     //�I���ʒu
}

void CFog::Set(D3DCOLOR const&color, float const&density)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	//�t�H�O�̐ݒ�
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
	pDevice->SetRenderState(D3DRS_FOGCOLOR, color); //���F�ŕs����
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);      //���_���[�h
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);     //�e�[�u�����[�h
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&density)); //�J�n�ʒu
}

void CFog::End()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
}
