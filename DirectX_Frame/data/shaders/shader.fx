// �O���[�o���ϐ�
float4x4 World;		// �ϊ��s��
float4x4 View;		// �ϊ��s��
float4x4 Proj;		// �ϊ��s��

// ���_�V�F�[�_�̊֐�
void VertexShader_Main(
	in float4 inPos		: POSITION,			// [����] ���W�i���f����ԁj
	in float4 inDiff	: COLOR0,			// [����] �F

	out float4 outPos	: POSITION,			// [�o��] ���W�i�ˉe��ԁj
	out float4 outDiff	: COLOR0)			// [�o��] �F
{
	outPos  = mul(inPos, mul(mul(World, View), Proj));	// ���_�ƕϊ��s����|����
	outDiff = inDiff;						// �F�͂��̂܂�	
}

// �s�N�Z���V�F�[�_�̊֐�
void PixelShader_Main(
	in  float4 inDiff	: COLOR0,			// [����] �F
	out float4 outDiff : COLOR0)			// [�o��] �F
{
	outDiff = inDiff;
}

// �e�N�j�b�N�̒�`
technique TVPShader		// �e�N�j�b�N���uTVPShader�v
{
	pass pass0			// 1�Ԗڂ̃p�X���upass0�v
	{
		// �X�e�[�g�ݒ�
		// �V�F�[�_�ݒ�
		VertexShader = compile vs_2_0 VertexShader_Main();	// ���_�V�F�[�_�̐ݒ�
		PixelShader = compile ps_2_0 PixelShader_Main();	// �s�N�Z���V�F�[�_�̐ݒ�
	}
}
