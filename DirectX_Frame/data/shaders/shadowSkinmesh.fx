//------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------
float4x4 	g_matWorld[4] : WORLD;  // ���[���h�ϊ��s��z��
float4x4	g_world;				// ���[���h�ϊ��s��
float4x4 	g_view;					// �J�����ϊ��s��
float4x4 	g_projection;			// �v���W�F�N�V�����ϊ��s��
int 		g_blendNum;				// �u�����h����z��̐�

//------------------------------------------------
// ���_�V�F�[�_
//------------------------------------------------
void main(
	float3 in_pos		: POSITION,
	float4 in_weight : BLENDWEIGHT,
	out float4 out_pos : POSITION)
{
	float Weight[4] = (float[4])in_weight;      	// �d�݂�float�ɕ������܂�
	float LastBlendWeight = 0.0f;        		// �Ō�̍s��Ɋ|������d��
	float4x4 matCombWorld = 0.0f;           	// �������[���h�ϊ��s��

												// ���[���h�ϊ��s����u�����h
	for (int i = 0; i < g_blendNum; i++)
	{
		LastBlendWeight += Weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
		matCombWorld += g_matWorld[i] * Weight[i];
	}

	// �Ō�̏d�݂𑫂��Z
	matCombWorld += g_matWorld[g_blendNum - 1] * (1.0f - LastBlendWeight);

	out_pos = mul(float4(in_pos, 1.0f), matCombWorld);		// ���_���W�i���[�J�����W�n�j�����[���h���W�n�ɕϊ�
	out_pos = mul(out_pos, g_view);							// ���_���W�i���[���h���W�n�j���J�������W�n�ɕϊ�
	out_pos = mul(out_pos, g_projection);					// ���_���W�i�J�������W�n�j���X�N���[�����W�n�ɕϊ�
}

//------------------------------------------------
// �s�N�Z���V�F�[�_
//------------------------------------------------
void PS(out float4 out_color : COLOR0)
{
	out_color = float4(1, 1, 1, 0);
}
