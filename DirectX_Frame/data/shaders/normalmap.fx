//------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------
// ���W�ϊ��p
float4x4	g_world;					// ���[���h�ϊ��s��
float4x4 	g_view;						// �J�����ϊ��s��
float4x4 	g_projection;				// �v���W�F�N�V�����ϊ��s��
// �e�N�X�`��
texture		g_texture;					// �e�N�X�`��
bool		g_tex;						// �e�N�X�`���̂���Ȃ��@false:�Ȃ��@true:����
// ��
float4		g_specular;					// �X�y�L�����[��
float3		g_light_dir;				// ���s�����̕���
// �}�e���A��
float4		g_specular_mat;				// �X�y�L�����[
float		g_power;					// �X�y�L�����[���̃p���[�l

float4      g_camerapos;				// �J�������W

//------------------------------------------------
// �T���v���[1
//------------------------------------------------
sampler Sampler1 =
sampler_state {
	Texture = <g_texture>;
	MinFilter = LINEAR;		// ���j�A�t�B���^�i�k�����j
	MagFilter = LINEAR;		// ���j�A�t�B���^�i�g�厞�j
};

//------------------------------------------------
// �T���v���[2
//------------------------------------------------
sampler NormalmapSampler1 =
sampler_state {
	Texture = <g_normaltexture>;
	MinFilter = LINEAR;		// ���j�A�t�B���^�i�k�����j
	MagFilter = LINEAR;		// ���j�A�t�B���^�i�g�厞�j
};

//------------------------------------------------
// ���_�V�F�[�_
//------------------------------------------------
void main(float3 in_pos : POSITION,
	float3 in_normal : NORMAL,
	float4 in_col1 : COLOR0,
	float2 in_tex1 : TEXCOORD0,
	float3 in_binormal : BINORMAL,
	float3 in_tangent : TANGENT,
	out float4 out_pos : POSITION,
	out float2 out_tex1 : TEXCOORD0,
	out float3 out_lightvector : TEXCOORD1,
	out float3 out_posforps : TEXCOORD2,
	out float3 out_cameraposforps : TEXCOORD3)
{
	float3	N;		// ���[���h��ԏ�̖@���x�N�g��
	float3  P;      // ���[���h���W
	float3  V;      // �����x�N�g��
	float3	L;		// ���̍������ޕ���
	float3  B;		// ���[���h��Ԃ̏]�@���x�N�g��
	float3  T;		// ���[���h��Ԃ̐ڃx�N�g��

					// ���W�ϊ�
	out_pos = mul(float4(in_pos, 1.0f), g_world);
	P = (float3)out_pos;
	out_pos = mul(out_pos, g_view);
	out_pos = mul(out_pos, g_projection);

	// �]�@�������[���h��Ԃɕϊ�
	B = mul(in_binormal, (float3x3)g_world);

	// �ڃx�N�g�������[���h��Ԃɕϊ�
	T = mul(in_tangent, (float3x3)g_world);

	// �@���x�N�g�������[���h��Ԃɕϊ�
	N = mul(in_normal, (float3x3)g_world);

	// ���K��
	B = normalize(B);
	T = normalize(T);
	N = normalize(N);

	// ���s���̍������ޕ���	�P�ʃx�N�g����
	//out_lightvector = g_light_dir;
	L = normalize(-g_light_dir);

	// ���s���̍������ޕ����@�ڃx�N�g����Ԃɕϊ����邽�߂�
	// �ڃx�N�g����ԍs��
	//    TTT0
	//    BBB0
	//    NNN0
	//    0001
	//     �̋t�s������߂�
	//       (�]�u�łn�j�j
	//    TBN0
	//    TBN0
	//    TBN0
	//    0001
	out_lightvector.x = dot(L, T);
	out_lightvector.y = dot(L, B);
	out_lightvector.z = dot(L, N);

	// �J�������W�i�ڃx�N�g����ԁj
	//out_cameraposforps = (float3)g_camerapos;
	out_cameraposforps.x = dot((float3)g_camerapos, T);
	out_cameraposforps.y = dot((float3)g_camerapos, B);
	out_cameraposforps.z = dot((float3)g_camerapos, N);

	// ���[���h���W�i�ڃx�N�g����ԁj
	//out_posforps = P;
	out_posforps = dot(P, T);
	out_posforps = dot(P, B);
	out_posforps = dot(P, N);

	// �e�N�X�`�����W�����̂܂܏o�͂���
	out_tex1 = in_tex1;

}
//------------------------------------------------
// �s�N�Z���V�F�[�_
//------------------------------------------------
void PS(float4 in_specular		: COLOR1,
	float2 in_tex1				: TEXCOORD0,
	float3 in_lightvector		: TEXCOORD1,
	float3 in_pos				: TEXCOORD2,
	float3 in_camerapos			: TEXCOORD3,
	out float4 out_color : COLOR0)
{
	float4 tex_color = tex2D(Sampler1, in_tex1);

	// �m�[�}���}�b�v����@�������擾
	float4 Normal = tex2D(NormalmapSampler1, in_tex1);
	Normal = Normal * 2.0f - 1.0f;	// -1����1�͈̔͂ɕϊ�
	Normal = normalize(Normal);

	// �ڃx�N�g����Ԃ̌��̓��˃x�N�g���𐳋K������
	float3 Light = normalize(in_lightvector);

	float4 diffuse;

	// �@���}�b�v���g�p���ăf�B�t���[�Y�v�Z
	diffuse = max(0, dot(Light, Normal.xyz));

	// �X�y�L�������v�Z
	//float4 specular = float4(0.2, 0.2, 0.2, 1.0);

	// �����x�N�g���v�Z
	float3 V = in_camerapos - in_pos;
	V = normalize(V);

	// �n�[�t�x�N�g���v�Z
	float3 H = normalize(Light + V);

	// �u�����t�H���ŃX�y�L���������v�Z
	float4 specular = pow(max(0, dot(H, Normal.xyz)), 200);
	//float4 specular = g_specular * g_specular_mat *
	//	pow(max(0, dot((float3)Normal, H)), g_power);

	//�F�̏o��
	if (g_tex)
	{
		// �e�N�X�`���̐F�ƒ��_�̐F���|�����킹��
		out_color = diffuse * tex_color + specular;
		out_color.a = 1.0f;
	}
	else 
	{
		out_color = diffuse + specular;
	}
}
