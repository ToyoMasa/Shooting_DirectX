//------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------
// ���W�ϊ��p
float4x4	g_world;					// ���[���h�ϊ��s��
float4x4 	g_view;						// �J�����ϊ��s��
float4x4 	g_projection;				// �v���W�F�N�V�����ϊ��s��

// �e�N�X�`��
bool		g_tex;						// �e�N�X�`���̂���Ȃ��@false:�Ȃ��@true:����

// ���C�g�p�����[�^
float4		g_light_pos;				// ���W(�r���[���)
float3		g_light_dir;				// ����(�r���[���)
float4		g_light_diff;               // �f�B�t���[�Y�J���[
float4		g_light_specular;			// �X�y�L�����J���[
float4		g_light_ambient;			// �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
float4		g_falloff_param;			// x:����  y:FallOff  z:���������p�����[�^1  w:���������p�����[�^2
float4		g_light_param;				// x:���������p�����[�^3  y:�X�|�b�g���C�g�p�����[�^1( cos( Phi / 2.0f ) )  z:�X�|�b�g���C�g�p�p�����[�^2( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

float4		g_mat_ambient;
float4		g_mat_emissive;
float4		g_mat_diffuse;
float4		g_mat_specular;
float4		g_mat_power;
float		g_alpha;

float4      g_camera_pos;				// �J�������W

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

// main�֐�
void main(
	float3 in_pos : POSITION,
	float3 in_normal : NORMAL,
	float4 in_diff : COLOR0,
	float2 in_tex : TEXCOORD0,
	out float4 out_pos : POSITION,
	out float4 out_diff : COLOR0,
	out float2 out_tex : TEXCOORD0,
	out float3 out_normal : TEXCOORD1,
	out float3 out_posforps : TEXCOORD2)
{
	// ���W�ϊ����ďo�́i���[���h�ϊ���̒��_���o�́j
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// ���_���W�i���[�J�����W�n�j�����[���h���W�n�ɕϊ�	
	out_posforps = out_pos.xyz;							// ���[���h�ϊ��ςݍ��W
	out_pos = mul(out_pos, g_view);						// ���_���W�i���[���h���W�n�j���J�������W�n�ɕϊ�
	out_pos = mul(out_pos, g_projection);				// ���_���W�i�J�������W�n�j���X�N���[�����W�n�ɕϊ�

	// �@�������[���h��ԏ�̃x�N�g���ɕϊ����āA�P�ʃx�N�g�������Ă���o��
	out_normal = normalize(mul(in_normal, (float3x3)g_world));

	out_diff = in_diff;
	out_tex = in_tex;
}

// main�֐�
void PS(
	float2 in_tex		: TEXCOORD0,
	float4 in_diff : COLOR0,
	float3 in_normal : TEXCOORD1,    // �@��( �r���[��� )
	float3 in_posforps : TEXCOORD2,    // ���W( �r���[��� ))
	out float4 out_color : COLOR0)
{
	float4 tex_color;
	float4 specular_color;
	float3 normal;
	float diffuse_angle_gen;
	float4 diffuse;
	float4 specular;
	float3 vtx_to_camera;
	float3 half_vec;
	float power;
	float3 light_to_vtx;
	float light_gen;
	float3 light_dir;
	float light_dir_cosa;

	// �@���̏���
	normal = normalize(in_normal);

	// ���_���W���王�_�ւ̃x�N�g���𐳋K��
	vtx_to_camera = normalize(g_camera_pos.xyz - in_posforps);

	// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̏�����
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ���C�g�����x�N�g���̌v�Z
	light_dir = normalize(in_posforps - g_light_pos.xyz);

	// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
	light_to_vtx = in_posforps - g_light_pos.xyz;
	float temp = length(light_to_vtx) * length(light_to_vtx);

	// �������̌v�Z
	light_gen = 1.0f / (g_falloff_param.z + g_falloff_param.w * length(light_to_vtx) + g_light_param.x * temp);

	// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓��ς��v�Z 
	light_dir_cosa = dot(light_dir, g_light_dir);

	// �X�|�b�g���C�g�����v�Z
	light_gen *= saturate(pow(abs(max(light_dir_cosa - g_light_param.y, 0.0f) * g_light_param.z), g_falloff_param.y));

	// �L�������O�̎��͌��������ő�ɂ���
	light_gen *= step(length(light_to_vtx), g_falloff_param.x);

	// �f�B�t���[�Y�p�x�������v�Z
	diffuse_angle_gen = saturate(dot(normal, -light_dir));

	// �f�B�t���[�Y�Ɍ����v�Z��ǉ�
	diffuse = (g_light_diff * g_mat_diffuse * diffuse_angle_gen/* + g_light_ambient*/) * light_gen;

	// �n�[�t�x�N�g���̌v�Z
	half_vec = normalize(vtx_to_camera - light_dir);

	// �p���[�̌v�Z
	power = pow(max(0.0f, dot(normal, half_vec)), g_mat_power.x);

	// �X�y�L�����J���[�̌v�Z
	specular += power * light_gen.x * g_light_specular;

	// �f�B�t���[�Y�J���[�ɃA���r�G���g�J���[�����Z
	diffuse = max(diffuse, g_mat_ambient);

	// �}�e���A���̃X�y�L�����J���[
	specular_color = specular * g_mat_specular;

	// �o�̓J���[
	tex_color = tex2D(Sampler1, in_tex);
	out_color.rgb = tex_color.rgb * diffuse.rgb/* + specular_color.rgb*/;

	// �����x
	out_color.a = g_alpha;
}
