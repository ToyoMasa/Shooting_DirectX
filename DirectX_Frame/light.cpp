//======================================================================
//	���C�e�B���O���� [Light.cpp]�@�i2017/11/13�j
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "camera.h"
#include "light.h"

//======================================================================
//								������
//		���[���h���W�Ŋg��k��������Ɩ@���x�N�g���������̂Œ���
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

	m_Light.Type = D3DLIGHT_DIRECTIONAL;	// �f�B���N�V���i�����C�g
			
	// ���C�g�̃x�N�g��
	D3DXVECTOR3 vecDir(LIGHT_NMR_X, LIGHT_NMR_Y, LIGHT_NMR_Z);

	// vecDir�𐳋K������light�ɑ������
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, &vecDir);

	// �f�B�t���[�Y�J���[���Z�b�g�i���j�@�f�B���N�V���i���{�A���r�G���g�ɂȂ�
	m_Light.Diffuse.r = DEFAULT_DEFUSE_COLOR;
	m_Light.Diffuse.g = DEFAULT_DEFUSE_COLOR;
	m_Light.Diffuse.b = DEFAULT_DEFUSE_COLOR;
	m_Light.Diffuse.a = DEFAULT_DEFUSE_COLOR;
	m_Light.Ambient.r = DEFAULT_AMBIENT_COLOR;
	m_Light.Ambient.g = DEFAULT_AMBIENT_COLOR;
	m_Light.Ambient.b = DEFAULT_AMBIENT_COLOR;
	m_Light.Ambient.a = DEFAULT_AMBIENT_COLOR;

	// ���C�g�ԍ��̃Z�b�g(0�`3)�܂Ŏg����
	pDevice->SetLight(m_LightID, &m_Light);
	// 0�Ԃ̃��C�g��ON
	pDevice->LightEnable(m_LightID, TRUE);
	// �O���[�o���J���[�̐ݒ�
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(GLOBAL_AMBIENT, GLOBAL_AMBIENT, GLOBAL_AMBIENT, 1.0f));
}

void CLight::Uninit()
{

}

void CLight::SetLightColor(float diff1, float diff2, float diff3, float diff4, float amb1, float amb2, float amb3, float amb4)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_Light.Diffuse.r = diff1;
	m_Light.Diffuse.g = diff2;
	m_Light.Diffuse.b = diff3;
	m_Light.Diffuse.a = diff4;
	m_Light.Ambient.r = amb1;
	m_Light.Ambient.g = amb2;
	m_Light.Ambient.b = amb3;
	m_Light.Ambient.a = amb4;

	// ���C�g�ԍ��̃Z�b�g(0�`3)�܂Ŏg����
	pDevice->SetLight(m_LightID, &m_Light);
}

void CLight::SetEnable(BOOL enable)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	pDevice->LightEnable(m_LightID, enable);
}

CLight* CLight::Create(int id)
{
	CLight* light = new CLight();
	light->Init(id);

	return light;
}


CSpotLight::CSpotLight()
{
}

CSpotLight::~CSpotLight()
{
}

void CSpotLight::Init(
	int id,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 dir,
	float range,
	float theta,
	float phi)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_LightID = id;

	m_Light.Type = D3DLIGHT_SPOT;	// �X�|�b�g���C�g

	// ���C�g���W
	m_Light.Position = pos;

	// ���C�g�̃x�N�g��
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, &dir);

	// ���C�g�̗L������
	m_Light.Range = range;

	// ���C�g�̌����x
	m_Light.Falloff = 1.0f;

	// ���C�g�̌����̕ω�
	m_Light.Attenuation0 = 0.0f;
	m_Light.Attenuation1 = 1.0f;
	m_Light.Attenuation2 = 0.0f;

	// �����̌��̍L����͈́i���W�A���p�j
	m_Light.Theta = theta;

	// �O���̌��̍L����͈́i���W�A���p�j
	m_Light.Phi = phi;

	// �f�B�t���[�Y�J���[���Z�b�g�i���j�@�f�B���N�V���i���{�A���r�G���g�ɂȂ�
	m_Light.Diffuse.r = DEFAULT_DEFUSE_COLOR;
	m_Light.Diffuse.g = DEFAULT_DEFUSE_COLOR;
	m_Light.Diffuse.b = DEFAULT_DEFUSE_COLOR;
	m_Light.Diffuse.a = DEFAULT_DEFUSE_COLOR;
	m_Light.Ambient.r = DEFAULT_AMBIENT_COLOR;
	m_Light.Ambient.g = DEFAULT_AMBIENT_COLOR;
	m_Light.Ambient.b = DEFAULT_AMBIENT_COLOR;
	m_Light.Ambient.a = DEFAULT_AMBIENT_COLOR;

	// ���C�g�ԍ��̃Z�b�g(0�`3)�܂Ŏg����
	pDevice->SetLight(m_LightID, &m_Light);
	// 0�Ԃ̃��C�g��ON
	pDevice->LightEnable(m_LightID, TRUE);
	// �O���[�o���J���[�̐ݒ�
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(GLOBAL_AMBIENT, GLOBAL_AMBIENT, GLOBAL_AMBIENT, 1.0f));
}

void CSpotLight::SetPos(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	m_Light.Position = pos;
	pDevice->SetLight(m_LightID, &m_Light);
}

void CSpotLight::SetDir(D3DXVECTOR3 dir)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	m_Light.Direction = dir;
	pDevice->SetLight(m_LightID, &m_Light);
}

void CSpotLight::SetRange(float range)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	m_Light.Range = range;
	pDevice->SetLight(m_LightID, &m_Light);
}

void CSpotLight::SetTheta(float theta)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	m_Light.Theta = theta;
	pDevice->SetLight(m_LightID, &m_Light);
}

void CSpotLight::SetPhi(float phi)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	m_Light.Phi = phi;
	pDevice->SetLight(m_LightID, &m_Light);
}

void CSpotLight::Uninit()
{

}

CSpotLight* CSpotLight::Create(
	int id,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 dir,
	float range,
	float theta,
	float phi)
{
	CSpotLight* light = new CSpotLight();
	light->Init(id, pos, dir, range, theta, phi);

	return light;
}
