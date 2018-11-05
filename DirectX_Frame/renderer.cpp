#include "common.h"
#include "main.h"
#include "renderer.h"

//======================================================================
//	�ÓI�����o�ϐ��̏�����
//======================================================================
LPDIRECT3D9 CRenderer::m_D3D = NULL;
LPDIRECT3DDEVICE9 CRenderer::m_D3DDevice = NULL;

CRenderer::CRenderer()
{
	CRenderer::m_D3D = NULL;
	CRenderer::m_D3DDevice = NULL;
}

CRenderer::~CRenderer()
{
}

bool CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;

	//Direct3D�C���^�[�t�F�[�X�̎擾
	CRenderer::m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (CRenderer::m_D3D == NULL)
	{
		return false;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(CRenderer::m_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//[�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs��
	if (FAILED(CRenderer::m_D3D->CreateDevice(
		D3DADAPTER_DEFAULT,						//�f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,							//�f�o�C�X�^�C�v
		hWnd,									//�t�H�[�J�X����E�B���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,	//�f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,									//�f�o�C�X�̃v���[���e�[�V�����ւ̃|�C���^
		&CRenderer::m_D3DDevice)))						//�f�o�C�X�C���^�t�F�[�X�ւ̃|�C���^�@���ŏd�v
	{
		//��L�̐ݒ肪���s������
		MessageBox(hWnd, "Direct3D�f�o�C�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	//�����_�\�X�e�[�g�p�����[�^�̐ݒ�
	//SRC�c������`�����́B�܂�|���S���Ƀe�N�X�`����\�������́B
	//DEST�c���łɕ`�悳��Ă����ʂ̂��ƁB
	//SRC_RGB * SRC_�� + DEST_RGB * (1 - SRC_��)
	CRenderer::m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	CRenderer::m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CRenderer::m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//CRenderer::m_D3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	// �Ɩ�
	CRenderer::m_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�|���S���ƃe�N�X�`���̐F�̏�Z
	CRenderer::m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	CRenderer::m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);			//���Ӓl
	CRenderer::m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);			//�E�Ӓl

	//�e�N�X�`���̃s�N�Z���Ɋւ���ݒ�
	//�e�N�X�`���A�h���X�O���Q�Ƃ������ɂǂ����邩
	//WRAP = �����i�J��Ԃ��j
	//CLAMP = ��ԍŌ�̃s�N�Z�����������΂�
	//MIRROR = ���]�iU�����̂݁AV�����݂̂��ł���j
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//D3DTEXF_POINT�@���@����̓_�𖳗���莝���Ă��遨�h�b�g�G���������鎞�Ɏg��
	//D3DTEXF_LINEAR�@���@�A���`�G�C���A�V���O�̂悤�ɂȂ߂炩�ɂ���(��{������)
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);			//���ۂ̃e�N�X�`���T�C�Y��菬�������̃t�B���^�����O
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);			//���ۂ̃e�N�X�`���T�C�Y���傫�����̃t�B���^�����O
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);			//�~�b�v�}�b�v�i���̉摜�́��Q�̒P�ʂŎ����ō����摜�j�Ԃ�␳����

	return true;
}

void CRenderer::Uninit()
{
	if (CRenderer::m_D3DDevice != NULL)
	{//�f�o�C�X�̊J��
		CRenderer::m_D3DDevice->Release();
		CRenderer::m_D3DDevice = NULL;
	}

	if (CRenderer::m_D3D != NULL)
	{//Direct3D�I�u�W�F�N�g�̊J��
		CRenderer::m_D3D->Release();
		CRenderer::m_D3D = NULL;
	}
}

LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return CRenderer::m_D3DDevice;
}

HRESULT CRenderer::DrawBegin()
{
	//�N���A����
	CRenderer::GetDevice()->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(64, 96, 255, 255), 1.0f, 0);
	//CRenderer::GetDevice()->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	return CRenderer::GetDevice()->BeginScene();
}

void CRenderer::DrawEnd()
{
	//Direct3D�ɂ��`��̏I��
	CRenderer::GetDevice()->EndScene();

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	CRenderer::GetDevice()->Present(NULL, NULL, NULL, NULL);	//��O������hWnd�ł���
}