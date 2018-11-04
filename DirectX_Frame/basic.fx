//------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------
float4x4 	g_world;		// ���[���h�s��
float4x4	g_view;			// �J�����ϊ��s��
float4x4	g_projection;		// �v���W�F�N�V�����ϊ��s��
bool		g_tex;			// �e�N�X�`������Ȃ��t���O�@true:����

// ���֘A
float3		g_light_dir;	// ���s�����̕���
float4		g_diffuse;		// �f�t���[�Y
float4		g_ambient;		// �A���r�G���g
float4		g_specular;		// �X�y�L�����\

// �}�e���A��
float4		g_diffuse_mat;		// �f�t���[�Y
float4		g_emmisive_mat;		// �G�~�b�V�u
float4		g_ambient_mat;		// �A���r�G���g
float4		g_specular_mat;		// �X�y�L�����[
float		g_power;		// �X�y�L�����p���[

//------------------------------------------------
// �T���v���[1
//------------------------------------------------
sampler 	Sampler1 =
	sampler_state{
		MinFilter = LINEAR;
		MagFilter = LINEAR;
	};

//------------------------------------------------
// ���_�V�F�[�_
//------------------------------------------------
void main(	float3 in_pos : POSITION,
			float3 in_normal : NORMAL,
			float2 in_tex1 : TEXCOORD0,
 			out float4 out_pos : POSITION,
			out float4 out_color : COLOR0,
			out float2 out_tex : TEXCOORD0)
{
	float3 N;	// ���[���h�ϊ��ςݖ@��
	float3 L;	// ���̃x�N�g��

	// ���W�ϊ�
	//out_pos = mul(in_pos, mul(mul(g_world, g_view), g_projection));	// ���_�ƕϊ��s����|����
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// ���_���W�i���[�J�����W�n�j�����[���h���W�n�ɕϊ�	
	out_pos = mul(out_pos,g_view);				// ���_���W�i���[���h���W�n�j���J�������W�n�ɕϊ�
	out_pos = mul(out_pos,g_projection);			// ���_���W�i�J�������W�n�j���X�N���[�����W�n�ɕϊ�
	
	// �e�N�X�`�����W�����̂܂܏o��
	out_tex = in_tex1;					// �e�N�X�`�����W�̏o��

	// �@�������[���h�ϊ����Đ��K��
	N = normalize(mul(in_normal, (float3x3)g_world));

	// ���s�����̌��𐳋K��
	L = normalize(-g_light_dir);	// �����𔽓]���Ă���

	// �o�̓J���[�����߂�
	out_color = g_ambient * g_ambient_mat + g_diffuse * g_diffuse_mat * max(0.0, dot(L, N));
}

//------------------------------------------------
// �s�N�Z���V�F�[�_
//------------------------------------------------
void PS(	float4 in_color : COLOR0,
			float2 in_tex1 : TEXCOORD0,
			out float4 out_color : COLOR0)
{
	if (g_tex)
	{
		float4 tex_color = tex2D(Sampler1, in_tex1);
		// �e�N�X�`���̐F�ƒ��_�̐F���|�����킹��
		out_color = in_color * tex_color;
	}
	else 
	{
		out_color = in_color;
	}
	out_color.a = 1;
}








