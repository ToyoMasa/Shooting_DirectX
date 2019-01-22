// �O���[�o���ϐ�
float4x4	g_world;		// �ϊ��s��
float4x4	g_view;		// �ϊ��s��
float4x4	g_projection;		// �ϊ��s��
bool		g_tex;			// �e�N�X�`������Ȃ��t���O�@true:����

sampler		Sampler1 =
sampler_state {
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// ���_�V�F�[�_�̊֐�
void main(
	in float4 inPos		: POSITION,			// [����] ���W�i���f����ԁj
	in float4 inDiff	: COLOR0,			// [����] �F
	float2 in_tex : TEXCOORD0,

	out float4 outPos	: POSITION,			// [�o��] ���W�i�ˉe��ԁj
	out float4 outDiff	: COLOR0,
	out float2 out_tex : TEXCOORD0)			// [�o��] �F
{
	outPos  = mul(inPos, mul(mul(g_world, g_view), g_projection));	// ���_�ƕϊ��s����|����
	outDiff = inDiff;						// �F�͂��̂܂�	
	out_tex = in_tex;					// �e�N�X�`�����W�̏o��
}

// �s�N�Z���V�F�[�_�̊֐�
void PS(
	in  float4 inDiff	: COLOR0,			// [����] �F
	float2 in_tex : TEXCOORD0,	// �s�N�Z����UV���W
	out float4 outDiff : COLOR0)			// [�o��] �F
{
	if (g_tex)
	{
		// �T���v���[����e�N�Z�����擾
		float4 texcol = tex2D(Sampler1, in_tex);

		outDiff = inDiff * texcol;	// �e�N�Z���F���o��
	}
	else
	{
		outDiff = inDiff;	// �e�N�Z���F���o��
	}
}
