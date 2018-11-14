//------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------

float4x4	g_world;					// ���[���h�ϊ��s��
float4x4 	g_view;						// �J�����ϊ��s��
float4x4 	g_projection;				// �v���W�F�N�V�����ϊ��s��

bool		g_tex;						// �e�N�X�`���̂���Ȃ��@false:�Ȃ��@true:����

										// ��
float4		g_diffuse;					// �f�B�t���[�Y
float4		g_emmisive;					// �G�~�b�V�u
float4		g_ambient;					// ����
float4		g_specular;					// �X�y�L�����[��

float3		g_light_dir;				// ���s�����̕���

										// �}�e���A��
float4		g_diffuse_mat;				// �f�B�t���[�Y��
float4		g_emmissive_mat;				// �G�~�b�V�u��
float4		g_ambient_mat;				// ����
float4		g_specular_mat;				// �X�y�L�����[
float		g_power;					// �X�y�L�����[���̃p���[�l

										// �X�y�L�������̌v�Z�p
float4		g_camerapos;				// �J�����ʒu

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
// ���_�V�F�[�_
//------------------------------------------------
void main(float3 in_pos		: POSITION,
	float3 in_normal : NORMAL,
	float2 in_tex1 : TEXCOORD0,
	out float4 out_pos : POSITION,		//�ϊ��㒸�_�o��	
	out float2 out_tex1 : TEXCOORD0,	//�e�N�X�`�����W�o��
	out float3 out_normal : TEXCOORD1,	//���[���h�ϊ���@���o��
	out float3 out_posforps : TEXCOORD2)	//���[���h�ϊ��㒸�_�o��
{

	// ���W�ϊ����ďo�́i���[���h�ϊ���̒��_���o�́j
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// ���_���W�i���[�J�����W�n�j�����[���h���W�n�ɕϊ�	
	out_posforps = out_pos.xyz;							// ���[���h�ϊ��ςݍ��W
	out_pos = mul(out_pos, g_view);						// ���_���W�i���[���h���W�n�j���J�������W�n�ɕϊ�
	out_pos = mul(out_pos, g_projection);				// ���_���W�i�J�������W�n�j���X�N���[�����W�n�ɕϊ�


	// �e�N�X�`�����W�����̂܂܏o�͂���
	out_tex1 = in_tex1;			// �e�N�X�`�����W�̏o��


	// �@�������[���h��ԏ�̃x�N�g���ɕϊ����āA�P�ʃx�N�g�������Ă���o��
	out_normal = normalize(mul(in_normal, (float3x3)g_world));

}

//------------------------------------------------
// �s�N�Z���V�F�[�_
//------------------------------------------------
void PS(float2 in_tex1		: TEXCOORD0,	//�e�N�X�`�����W����
	float3 in_normal : TEXCOORD1,	//�s�N�Z���̖@������
	float3 in_posforps : TEXCOORD2,	//�s�N�Z���̍��W����
	out float4 out_color : COLOR0)			//�s�N�Z���J���[�o��
{
	float3	N;		// ���[���h��ԏ�̖@���x�N�g��
	float3	L;		// ���̍������ޕ���
	float3	P;		// ���[���h��ԏ�̒��_���W
	float3	V;		// �i�J�������W�[���_���W�j�x�N�g��
	float3  H;		//  �n�[�t�x�N�g���i�����x�N�g���ƌ��̕����x�N�g���j

					// ���[���h��Ԃł̍��W
	P = in_posforps;

	// ���[���h��Ԃł̖@���x�N�g���i���K�����K�v�j
	N = normalize(in_normal);

	// ���s���̍������ޕ���	�P�ʃx�N�g����
	L = normalize(-(g_light_dir));

	// �����x�N�g�������߂違���K��
	V = g_camerapos.xyz - P;
	V = normalize(V);

	// �n�[�t�x�N�g�������߂�
	H = normalize(L + V);

	// �o�̓J���[�����߂�
	float4 diffuse = g_ambient * g_ambient_mat +
		g_diffuse * g_diffuse_mat *
		max(0.0, dot(L, N));
	diffuse.w = 1.0f; 

	// �X�y�L�����[���ɂ��F���v�Z
	float4 specular = g_specular * g_specular_mat *
		pow(max(0, dot(N, H)), 50);



	//�F�̏o��
	if (g_tex) {
		float4 tex_color = tex2D(Sampler1, in_tex1);
		// �e�N�X�`���̐F�ƒ��_�̐F���|�����킹��
		out_color = saturate(diffuse * tex_color + specular);
	}
	else {
		out_color = saturate(diffuse + specular);
	}
}
