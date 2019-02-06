//------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------

float4x4	g_world;					// ���[���h�ϊ��s��
float4x4 	g_view;						// �J�����ϊ��s��
float4x4 	g_projection;				// �v���W�F�N�V�����ϊ��s��

//------------------------------------------------
// ���_�V�F�[�_
//------------------------------------------------
void main(
	float3 in_pos		: POSITION,
	out float4 out_pos : POSITION)
{
	// ���W�ϊ�
	out_pos = mul(float4(in_pos, 1.0f), g_world);
	out_pos = mul(out_pos, g_view);
	out_pos = mul(out_pos, g_projection);
}

//------------------------------------------------
// �s�N�Z���V�F�[�_
//------------------------------------------------
void PS(out float4 out_color : COLOR0)
{
	out_color = float4(1, 1, 1, 0);
}