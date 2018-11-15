//======================================================================
//	ライティング処理 [Light.cpp]　（2017/11/13）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "camera.h"
#include "light.h"

//======================================================================
//								※注意
//		ワールド座標で拡大縮小をすると法線ベクトルもずれるので注意
//======================================================================

CLight::CLight()
{
}

CLight::~CLight()
{
}

void CLight::Init(int id)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_LightID = id;

	m_Light.Type = D3DLIGHT_DIRECTIONAL;	// ディレクショナルライト
			
	// ライトのベクトル
	D3DXVECTOR3 vecDir(LIGHT_NMR_X, LIGHT_NMR_Y, LIGHT_NMR_Z);

	// vecDirを正規化してlightに代入する
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, &vecDir);

	// ディフューズカラーをセット（白）　ディレクショナル＋アンビエントになる
	m_Light.Diffuse.r = DEFAULT_DEFUSE_COLOR;
	m_Light.Diffuse.g = DEFAULT_DEFUSE_COLOR;
	m_Light.Diffuse.b = DEFAULT_DEFUSE_COLOR;
	m_Light.Diffuse.a = DEFAULT_DEFUSE_COLOR;
	m_Light.Ambient.r = DEFAULT_AMBIENT_COLOR;
	m_Light.Ambient.g = DEFAULT_AMBIENT_COLOR;
	m_Light.Ambient.b = DEFAULT_AMBIENT_COLOR;
	m_Light.Ambient.a = DEFAULT_AMBIENT_COLOR;

	// ライト番号のセット(0～3)まで使える
	pDevice->SetLight(m_LightID, &m_Light);
	// 0番のライトをON
	pDevice->LightEnable(m_LightID, TRUE);
	// グローバルカラーの設定
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(GLOBAL_AMBIENT, GLOBAL_AMBIENT, GLOBAL_AMBIENT, 1.0f));
}

void CLight::Uninit()
{

}

void CLight::SetLight(float diff1, float diff2, float diff3, float diff4, float amb1, float amb2, float amb3, float amb4)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	ZeroMemory(&m_Light, sizeof(m_Light));
	m_Light.Type = D3DLIGHT_DIRECTIONAL;	// ディレクショナルライト

	// ライトのベクトル
	D3DXVECTOR3 vecDir(LIGHT_NMR_X, LIGHT_NMR_Y, LIGHT_NMR_Z);

	// vecDirを正規化してlightに代入する
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, &vecDir);

	m_Light.Diffuse.r = diff1;
	m_Light.Diffuse.g = diff2;
	m_Light.Diffuse.b = diff3;
	m_Light.Diffuse.a = diff4;
	m_Light.Ambient.r = amb1;
	m_Light.Ambient.g = amb2;
	m_Light.Ambient.b = amb3;
	m_Light.Ambient.a = amb4;

	// ライト番号のセット(0～3)まで使える
	pDevice->SetLight(m_LightID, &m_Light);
}

CLight* CLight::Create(int id)
{
	CLight* light = new CLight();
	light->Init(id);

	return light;
}
