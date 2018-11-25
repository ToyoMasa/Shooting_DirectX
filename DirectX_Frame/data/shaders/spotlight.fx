// �O���[�o���ϐ�
//float4x4 	g_matWorld[4] : WORLD;  // ���[���h�ϊ��s��z��
float4x4	g_world;		// �ϊ��s��
float4x4	g_view;			// �ϊ��s��
float4x4	g_projection;		// �ϊ��s��
//int 		g_blendNum;       	// �u�����h����z��̐�
bool		g_tex;			// �e�N�X�`������Ȃ��t���O�@true:����

float3		g_lightposition;
float		g_lightstrength;
float3		g_lightdirection;
float		g_lightcos; // �X�|�b�g���C�g�̊p�x�̃R�T�C���l
float		g_lightdecay; // ����

//------------------------------------------------
// �T���v���[1
//------------------------------------------------
sampler Sampler1 =
sampler_state {
	Texture = <g_texture>;
	MinFilter = LINEAR;		// ���j�A�t�B���^�i�k�����j
	MagFilter = LINEAR;		// ���j�A�t�B���^�i�g�厞�j
};

void main(float3 in_pos 	: POSITION,
	float3 in_normal 		: NORMAL,
	float2 in_tex1			: TEXCOORD0,
	out float4 out_pos			: POSITION,
	out float2 out_tex1			: TEXCOORD0,
	out float3 out_normal 		: TEXCOORD1,
	out float3 out_lightdir 	: TEXCOORD2)
{

	// ���W�ϊ����ďo�́i���[���h�ϊ���̒��_���o�́j
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// ���_���W�i���[�J�����W�n�j�����[���h���W�n�ɕϊ�	
	out_pos = mul(out_pos, g_view);						// ���_���W�i���[���h���W�n�j���J�������W�n�ɕϊ�
	out_pos = mul(out_pos, g_projection);				// ���_���W�i�J�������W�n�j���X�N���[�����W�n�ɕϊ�

	// �@�������[���h��ԏ�̃x�N�g���ɕϊ����āA�P�ʃx�N�g�������Ă���o��
	out_normal = normalize(mul(in_normal, (float3x3)g_world));

	out_lightdir = out_pos - g_lightposition;

	// �e�N�X�`�����W�����̂܂܏o�͂���
	out_tex1 = in_tex1;			// �e�N�X�`�����W�̏o��

}

void PS(float2 in_tex1		: TEXCOORD0,	//�e�N�X�`�����W����
	float3 in_normal 		: TEXCOORD1,	//�s�N�Z���̖@������
	float3 in_lightdir 		: TEXCOORD2,
	out float4 out_color 	: COLOR0)			//�s�N�Z���J���[�o��
{
	float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	float ambient = 0.1f;

	float3 ldir = normalize(in_lightdir);
	float shade = 0;
	float coneFactor = dot(ldir, normalize(g_lightdirection)); //< �X�|�b�g���C�g�͈͓̔����ǂ���
	if(coneFactor > g_lightcos) 
	{
		shade = dot(normalize(in_normal), - ldir) * pow(coneFactor, g_lightdecay) * g_lightstrength; //< �����ƃ��C�g�̋������l��
 	}

	//�F�̏o��
	if (g_tex)
	{
		float4 tex_color = tex2D(Sampler1, in_tex1);
		out_color = tex_color * (shade + ambient);
	}
	else
	{
		out_color = color * (shade + ambient);
	}
}