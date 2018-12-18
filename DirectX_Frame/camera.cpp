//======================================================================
//	�J�������� [camera.cpp]�@�i2018/4/17�j
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "camera.h"
#include "manager.h"

CCamera::CCamera()
{
	m_Pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_At    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Projection;
	m_View;
	m_Fov = 90.0f;
	m_Dot = 0;

	m_ArmLength = 0.0f;
	m_ArmLength2D = 0.0f;

	// *--�E�����x�N�g�������߂�--*
	// ������x�N�g���ƑO�����x�N�g���̊O�ς����߂��2�̃x�N�g���ɑ΂��Ē��p�̃x�N�g��
	// 2�̃x�N�g���̂Ȃ����ʂ̌����Ă�������@��
	D3DXVec3Cross(&m_Right, &m_Up, &m_Front);	// �O��
}

CCamera::~CCamera()
{
}

void CCamera::Init()
{

}

void CCamera::Init(D3DXVECTOR3 pos, D3DXVECTOR3 at)
{
	m_Pos = pos;
	m_At = at;

	m_Front = at - pos;
	D3DXVec3Normalize(&m_Front, &m_Front); 

	D3DXVECTOR3 vec = m_At - m_Pos;
	m_ArmLength = D3DXVec3Length(&vec);

	D3DXVECTOR3 at2 = m_At;
	at2.y = 0.0f;
	D3DXVECTOR3 position = m_Pos;
	position.y = 0.0f;
	vec = at2 - position;
	m_ArmLength2D = D3DXVec3Length(&vec);
}

void CCamera::Uninit()
{

}

void CCamera::Update()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	//	--�J�����̏���--
	// ���݂̍��W��ۑ�

	//if (CManager::GetDebug())
	{
		ImGui::Begin("Camera", 0);
		ImGui::Text("Front:X = %.2f Y = %.2f Z = %.2f", m_Front.x, m_Front.y, m_Front.z);
		ImGui::Text("Right:X = %.2f Y = %.2f Z = %.2f", m_Right.x, m_Right.y, m_Right.z);
		ImGui::Text("Pos  :X = %.2f Y = %.2f Z = %.2f", m_Pos.x, m_Pos.y, m_Pos.z);
		ImGui::End();
	}

	// �r���[�s��̍쐬
	D3DXMatrixLookAtLH(&m_View, &m_Pos, &m_At, &m_Up);	//�r���[�s�������Ă����֗��֐�

	// �v���W�F�N�V�����i�p�[�X�y�N�e�B�u�j�s��̍쐬
	// ������(�s��|�C���^, ��p, �A�X�y�N�g��, �j�A�N���b�v(0���傫���l�A1��1m���炢), �t�@�[�N���b�v)
	D3DXMatrixPerspectiveFovLH(&m_Projection, D3DXToRadian(m_Fov) /* �܂���D3DX_PI / 3�@*/, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP, FAR_CLIP);
}

void CCamera::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

void CCamera::SetAt(D3DXVECTOR3 at)
{
	m_At = at;
}

void CCamera::SetFov(float fov)
{
	m_Fov = fov;
}

void CCamera::Move(D3DXVECTOR3 pos)
{
	m_Pos += pos;
	m_At += pos;
}

void CCamera::Move(float horizontal, float vertical)
{
	//*********************************************************
	//	���E�ړ�
	//*********************************************************
	{
		D3DXVec3Normalize(&m_Right, &m_Right);
		m_Pos += m_Right * CAMERA_MOOVE_SPEED * horizontal;
		m_At += m_Right * CAMERA_MOOVE_SPEED * horizontal;

	}

	//*********************************************************
	//	�O��ړ�
	//*********************************************************
	{
		// �O���������߂�
		m_Front = m_At - m_Pos;

		// m_Front�𐳋K������
		D3DXVec3Normalize(&m_Front, &m_Front);

		D3DXVECTOR3 vec = m_Front; // �ړ�����
		vec.y = 0;

		// vec�𐳋K������
		D3DXVec3Normalize(&vec, &vec);

		m_Pos += vec * CAMERA_MOOVE_SPEED * vertical;
		m_At += vec * CAMERA_MOOVE_SPEED * vertical;
	}
}

void CCamera::Rotation(float horizontal, float vertical)
{
	//*********************************************************
	//	���E��]
	//*********************************************************
	{
		// ��]�s���錾
		D3DXMATRIX mtxRotation;

		// ��]�s��(Y����])�����(��]���x)
		D3DXMatrixRotationY(&mtxRotation, CAMERA_AT_RAD * horizontal);

		D3DXVECTOR3 dir = m_At - m_Pos;

		D3DXVec3TransformNormal(&dir, &dir, &mtxRotation);	// �x�N�g���̍��W�ϊ�(�o��, ����, �ϊ��s��)

		m_At = dir + m_Pos;

		D3DXVec3TransformNormal(&m_Front, &m_Front, &mtxRotation);	// �x�N�g���̍��W�ϊ�(�o��, ����, �ϊ��s��)
		D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);	// �x�N�g���̍��W�ϊ�(�o��, ����, �ϊ��s��)
		D3DXVec3Normalize(&m_Front, &m_Front);
		D3DXVec3Normalize(&m_Right, &m_Right);
	}

	//*********************************************************
	//	�㉺��]
	//*********************************************************
	{
		// ��]�s���錾
		D3DXMATRIX mtxRotation;

		// ��]�s������(��]���x)
		D3DXMatrixRotationAxis(&mtxRotation, &m_Right, CAMERA_AT_RAD * vertical);

		D3DXVECTOR3 dir = m_At - m_Pos;

		D3DXVec3TransformNormal(&dir, &dir, &mtxRotation);	// �x�N�g���̍��W�ϊ�(�o��, ����, �ϊ��s��)

		D3DXVECTOR3 front;	

		D3DXVec3TransformNormal(&front, &m_Front, &mtxRotation);	// �x�N�g���̍��W�ϊ�(�o��, ����, �ϊ��s��)

		D3DXVECTOR3 vec = front; // �ړ�����
		vec.y = 0;

		// vec�𐳋K������
		D3DXVec3Normalize(&vec, &vec);

		m_Dot = D3DXVec3Dot(&front, &vec);

		// �㉺�̌��E��݂���
		if (m_Dot > 0.5f)
		{
			m_At = dir + m_Pos;
			//	m_Pos = m_At - dir;
			D3DXVec3TransformNormal(&m_Front, &m_Front, &mtxRotation);	// �x�N�g���̍��W�ϊ�(�o��, ����, �ϊ��s��)
			D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);	// �x�N�g���̍��W�ϊ�(�o��, ����, �ϊ��s��)
			D3DXVec3Normalize(&m_Front, &m_Front);
			D3DXVec3Normalize(&m_Right, &m_Right);
		}
	}
}

CCamera* CCamera::Create()
{
	CCamera* camera= new CCamera();
	camera->Init(D3DXVECTOR3(0.0f, 3.0f, -3.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f));

	return camera;
}

CCamera* CCamera::Create(D3DXVECTOR3 pos, D3DXVECTOR3 at)
{
	CCamera* camera = new CCamera();
	camera->Init(pos, at);

	return camera;
}

D3DXMATRIX CCamera::GetView()
{
	return m_View;
}

void CCamera::SetRot(D3DXVECTOR3 rot)
{

}
