//*****************************************************************************
//!	@file	matuutil.cpp
//!	@brief	
//!	@note	���w�n�̃��[�e�B���e�B�֐��Q
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<d3dx9.h>

//==============================================================================
//!	@fn		MakeWorldMatrix
//!	@brief	�p�x�ƕ��s�ړ��ʂ���s������
//!	@param�@�s��i�o�́j�@�@	
//!	@param�@	�p�x�i���́j
//!	@param�@	���s�ړ��ʁi���́j
//!	@retval	�Ȃ�
//==============================================================================
void MakeWorldMatrix(D3DXMATRIX& mat,const D3DXVECTOR3& angle,const D3DXVECTOR3& trans){
	D3DXMATRIX matx, maty, matz, mattrans;;
	D3DXMatrixRotationX(&matx, D3DX_PI*angle.x / 180.0f);
	D3DXMatrixRotationY(&maty, D3DX_PI*angle.y / 180.0f);
	D3DXMatrixRotationZ(&matz, D3DX_PI*angle.z / 180.0f);
	mat = matx*maty*matz;
	D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);
	
	mat = mat*mattrans;
}

//==============================================================================
//!	@fn		MakeWorldMatrix
//!	@brief	�p�x�ƕ��s�ړ��ʂ���s������(�ώZ�s����v�Z)
//!	@param�@�s��i�o�́j�@�@	
//!	@param�@�ώZ�s��i�o�́j�@�@	
//!	@param�@�p�x�i���́j
//!	@param�@���s�ړ��ʁi���́j
//!	@retval	�Ȃ�
//==============================================================================
void MakeWorldMatrix(D3DXMATRIX& mat, D3DXMATRIX& mattotal,const D3DXVECTOR3& angle, const D3DXVECTOR3& trans){
	D3DXMATRIX matx, maty, matz, mattrans;;

	D3DXMatrixRotationX(&matx, D3DX_PI*angle.x / 180.0f);			// X����]�s��𐶐�
	D3DXMatrixRotationY(&maty, D3DX_PI*angle.y / 180.0f);			// Y����]�s��𐶐�
	D3DXMatrixRotationZ(&matz, D3DX_PI*angle.z / 180.0f);			// Z����]�s��𐶐�
	mat = matx*maty*matz;											// ����

	D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);	// ���s�ړ��s��𐶐�

	mat = mat*mattrans;
	mattotal = mat*mattotal;
}

//==============================================================================
//!	@fn		IsCanIsee
//!	@brief	��ʓ��ɑ��݂��Ă��邩���`�F�b�N
//!	@param�@�ʒu���W�i���[���h�j�@�@	
//!	@param�@�J�����s��@	
//!	@param�@�v���W�F�N�V�����ϊ��s��
//!	@param�@�X�N���[�����W
//!	@retval	true ��ʓ��@false ��ʊO
//==============================================================================
bool IsCanIsee(const D3DXVECTOR3& pos,const D3DXMATRIX& matcamera,const D3DXMATRIX& matprojection,D3DXVECTOR3& ans){

	D3DXMATRIX mat;

	// �J�����ϊ��s��ƃv���W�F�N�V�����ϊ��s��̍���
	mat = matcamera*matprojection;

	// �X�N���[�����W�ɕϊ�
	D3DXVec3TransformCoord(&ans, &pos, &mat);

	if (ans.z < 0.0f){
		return false;
	}
	if (ans.z > 1.0f){
		return false;
	}
	if (ans.x < -1.0f)
	{
		return false;
	}
	if (ans.x > 1.0f)
	{
		return false;
	}
	if (ans.y < -1.0f)
	{
		return false;
	}
	if (ans.y > 1.0f)
	{
		return false;
	}
	return true;
}

//==============================================================================
//!	@fn		WorldToScreen
//!	@brief	���[���h���W���X�N���[�����W�ɕϊ�����
//!	@param�@�f�o�C�X�I�u�W�F�N�g�@�@	
//!	@param�@�J�����s��@�@	
//!	@param�@�v���W�F�N�V�����ϊ��s��@	
//!	@param�@���[���h���W�n�ł̍��W�i�ԊґO�j
//!	@param�@�X�N���[�����W
//!	@retval	�Ȃ�
//==============================================================================
void WorldToScreen(
	const LPDIRECT3DDEVICE9 lpdev,
	const D3DXMATRIX& matCamera,
	const D3DXMATRIX& matProjection,
	const D3DXVECTOR3& wp,
	D3DXVECTOR3& sp){

	D3DXMATRIX		mat;
	D3DXMATRIX		matViewPort;		// �r���[�|�[�g�s��

	D3DVIEWPORT9	vp;

	// �r���[�|�[�g�擾
	lpdev->GetViewport(&vp);

	// �X�N���[�����A�����擾
	DWORD screen_width = vp.Width;
	DWORD screen_height = vp.Height;

	// �r���[�|�[�g�s��쐬
	D3DXMatrixIdentity(&matViewPort);
	matViewPort._11 = screen_width / 2.0f;
	matViewPort._22 = -static_cast<float>(screen_height) / 2.0f;
	matViewPort._41 = screen_width / 2.0f;
	matViewPort._42 = screen_height / 2.0f;

	// �s��������i�J�����s�񁖃v���W�F�N�V�����ϊ��s�񁖃r���[�|�[�g�s��j
	mat = matCamera*matProjection*matViewPort;
	// �s��~�x�N�g��
	D3DXVec3TransformCoord(&sp, &wp, &mat);
}

//******************************************************************************
//	End of file.
//******************************************************************************
