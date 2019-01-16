// �O���[�o���ϐ�
float4x4 	g_matWorld[4] : WORLD;  // ���[���h�ϊ��s��z��
float4x4	g_world;		// �ϊ��s��
float4x4	g_view;			// �ϊ��s��
float4x4	g_projection;		// �ϊ��s��
int 		g_blendNum;       	// �u�����h����z��̐�
bool		g_tex;			// �e�N�X�`������Ȃ��t���O�@true:����

// ���֘A
float3		g_light_dir;		// ���s�����̕���
float4		g_diffuse;		// �f�t���[�Y
float4		g_ambient;		// �A���r�G���g
float4		g_specular;		// �X�y�L�����\

// �}�e���A��
float4		g_diffuse_mat;		// �f�t���[�Y
float4		g_emmisive_mat;		// �G�~�b�V�u
float4		g_ambient_mat;		// �A���r�G���g
float4		g_specular_mat;		// �X�y�L�����[
float		g_power;		// �X�y�L�����p���[

sampler		Sampler1 =
sampler_state {
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// ���_�V�F�[�_�̊֐�
void main(
	float4 inPos		: POSITION,		// [����] ���W�i���f����ԁj
	float4 inDiff		: COLOR0,		// [����] �F
	float4 inWeight 	: BLENDWEIGHT,
	float3 inNormal 	: NORMAL,
	float2 inTex 		: TEXCOORD0,

	out float4 outPos	: POSITION,		// [�o��] ���W�i�ˉe��ԁj
	out float4 outDiff	: COLOR0,
	out float2 outTex 	: TEXCOORD0)		// [�o��] �F
{
	float Weight[4] = (float[4])inWeight;      	// �d�݂�float�ɕ������܂�
	float LastBlendWeight = 0.0f;        		// �Ō�̍s��Ɋ|������d��
	float4x4 matCombWorld = 0.0f;           	// �������[���h�ϊ��s��

	// ���[���h�ϊ��s����u�����h
	int i;
	for(i = 0; i < 4; i++)
	{
		LastBlendWeight += Weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
		matCombWorld += g_matWorld[i] * Weight[i];
	}

	// �Ō�̏d�݂𑫂��Z
	matCombWorld += g_matWorld[g_blendNum-1] * (1.0f-LastBlendWeight);

	// �@���x�N�g����ϊ�
	float3 transNormal = mul(inNormal, (float3x3)matCombWorld);
	//outNormal = normalize(transNormal);

	outPos  = mul(inPos, mul(mul(matCombWorld, g_view), g_projection));	// ���_�ƕϊ��s����|����
	outDiff = inDiff;						// �F�͂��̂܂�	
	outTex = inTex;					// �e�N�X�`�����W�̏o��

	float3 N;	// ���[���h�ϊ��ςݖ@��
	float3 L;	// ���̃x�N�g��

	// �@�������[���h�ϊ����Đ��K��
	N = normalize(transNormal);

	// ���s�����̌��𐳋K��
	L = normalize(-g_light_dir);	// �����𔽓]���Ă���

	// �o�̓J���[�����߂�
	outDiff = g_ambient * g_ambient_mat + g_diffuse * g_diffuse_mat * max(0.0, dot(L, N));
	outDiff.w = 1.0f;
}

// �s�N�Z���V�F�[�_�̊֐�
void PS(
	float4 inDiff	: COLOR0,	// [����] �F
	float2 inTex : TEXCOORD0,		// �s�N�Z����UV���W
	out float4 outDiff : COLOR0)		// [�o��] �F
{
	if (g_tex)
	{
		// �T���v���[����e�N�Z�����擾
		float4 texcol = tex2D(Sampler1, inTex);

		outDiff = inDiff * texcol;	// �e�N�Z���F���o��
		outDiff.a = 0;
	}
	else
	{
		outDiff = inDiff;	// �e�N�Z���F���o��
	}
}
