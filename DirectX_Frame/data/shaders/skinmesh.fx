// �O���[�o���ϐ�
float4x4 	g_matWorld[4] : WORLD;   // ���[���h�ϊ��s��z��
float4x4	g_world;		// �ϊ��s��
float4x4	g_view;		// �ϊ��s��
float4x4	g_projection;		// �ϊ��s��
int 		g_BlendNum;       // �u�����h����z��̐�
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
	float4 	inWeight : BLENDWEIGHT,
	float2 in_tex : TEXCOORD0,

	out float4 outPos	: POSITION,			// [�o��] ���W�i�ˉe��ԁj
	out float4 outDiff	: COLOR0,
	out float2 out_tex : TEXCOORD0)			// [�o��] �F
{
	float Weight[4] = (float[4])inWeight;      // �d�݂�float�ɕ������܂�
	float LastBlendWeight = 0.0f;        // �Ō�̍s��Ɋ|������d��
	float4x4 matCombWorld = 0.0f;           // �������[���h�ϊ��s��

	// ���[���h�ϊ��s����u�����h
	for(int i=0; i < g_BlendNum - 1; i++)
	{
	LastBlendWeight += Weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
	matCombWorld += g_matWorld[i] * Weight[i];
	}

	// �Ō�̏d�݂𑫂��Z
	matCombWorld += g_matWorld[g_BlendNum-1] * (1.0f-LastBlendWeight);


	outPos  = mul(inPos, mul(mul(matCombWorld, g_view), g_projection));	// ���_�ƕϊ��s����|����
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

		outDiff = texcol;	// �e�N�Z���F���o��
	}
	else
	{
		outDiff = inDiff;	// �e�N�Z���F���o��
	}
}
