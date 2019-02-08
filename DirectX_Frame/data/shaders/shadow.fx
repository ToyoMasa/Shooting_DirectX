//------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------
float4x4	g_world;					// ���[���h�ϊ��s��
float4x4 	g_view;						// �J�����ϊ��s��
float4x4 	g_projection;				// �v���W�F�N�V�����ϊ��s��
float4		g_light_pos;
float		g_far;
float		g_near;

//------------------------------------------------
// ���_�V�F�[�_
//------------------------------------------------
void main(
	float3 in_pos : POSITION,
	//float3 in_normal : NORMAL,
	out float4 out_pos : POSITION,
	out float4 out_ztex : TEXCOORD0)
{
	// ���ʂɃ��[���h�r���[�ˉe�s�������
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// ���_���W�i���[�J�����W�n�j�����[���h���W�n�ɕϊ�
	out_pos = mul(out_pos, g_view);							// ���_���W�i���[���h���W�n�j���J�������W�n�ɕϊ�
	out_pos = mul(out_pos, g_projection);

	// �e�N�X�`�����W�𒸓_�ɍ��킹��
	out_ztex = out_pos;
}

//------------------------------------------------
// �s�N�Z���V�F�[�_
//------------------------------------------------
void PS(
	float4 in_ztex : TEXCOORD0,
	out float4 out_color : COLOR0)
{
	out_color = in_ztex.z / in_ztex.w;
}
