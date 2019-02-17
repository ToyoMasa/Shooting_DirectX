//======================================================================
//	�e�N�X�`���Ǘ��w�b�_[texture]
//
//======================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <map>

// �e�N�X�`���Ǘ��pID
typedef enum
{
	TEX_ID_FIELD001 = 0,			// �t�B�[���h
	TEX_ID_CURSOR,					// �J�[�\��
	TEX_ID_TREE,					// �c���[
	TEX_ID_SKY,						// ��
	TEX_ID_SKY002,					// ���
	TEX_ID_NUMBER001,				// ����
	TEX_ID_TITLE,					// �^�C�g��
	TEX_ID_RESULT,					// ���U���g
	TEX_ID_CIRCLE,					// �ۂ��p�[�e�B�N��
	TEX_ID_NORMAL_CIRCLE,			// �ۂ��p�[�e�B�N��
	TEX_ID_STAR,					// ��
	TEX_ID_ATTACK,					// �U���\�e�L�X�g
	TEX_ID_WHITE,					// �^����
	TEX_ID_WALL01,					// ��
	TEX_ID_NOWLOADING,				// ���[�h��ʕ���
	TEX_ID_LOADFRAME,				// ���[�h��ʃQ�[�W�t���[��
	TEX_ID_LOADGAGE,				// ���[�h��ʃQ�[�W���g
	TEX_ID_PRESS_SPACE,				// �X�y�[�X������
	TEX_ID_PRESS_START,				// �X�^�[�g������
	TEX_ID_RANK_A,					// A
	TEX_ID_RANK_B,					// B
	TEX_ID_RANK_C,					// C
	TEX_ID_MISSION_COMPLETE,		// �Q�[���N���A
	TEX_ID_MISSION_FAILED,			// �Q�[���I�[�o�[
	TEX_ID_MISSION,					// �~�b�V����
	TEX_ID_WANTED,					// �w����z
	TEX_ID_TUTORIAL,				// �������
	TEX_ID_EXCLAMATION,				// �r�b�N���}�[�N
	TEX_ID_CAUTION,					// �x����
	TEX_ID_PAUSE,					// pause
	TEX_ID_TURORIAL_PAUSE,			// pause�̃`���[�g���A��
	TEX_ID_HOWTOUSE,				// ����m�F
	TEX_ID_CROSSHAIR_CIRCLE,		// �N���X�w�A
	TEX_ID_CROSSHAIR_RIFLE,			// ���C�t���̃N���X�w�A
	TEX_ID_CROSSHAIR_SHOTGUN,		// �V���b�g�K���̃N���X�w�A
	TEX_ID_SMOKE,					// ��
	TEX_ID_FLASH,					// �}�Y���t���b�V��
	TEX_ID_CIRCLE_2D,				// 2D��ʗp�~
	TEX_ID_DAMAGE_SCREEN,			// �_���[�W���󂯂����̏o���G�t�F�N�g
	TEX_ID_KILL_ENEMY_NUM,			// �|�����G�̐�
	TEX_ID_GRASS,					// ����
	TEX_ID_SOIL,					// �y
	TEX_ID_ROCK,					// ��
	TEX_ID_BULLETICON,				// �e�e�A�C�R��
	TEX_ID_DASH_TEXT,				// �`���[�g���A���e�L�X�g�u����v
	TEX_ID_GUNSHOT_TEXT,			// �`���[�g���A���e�L�X�g�u�e�Ō��v
	TEX_ID_GUNUP_TEXT,				// �`���[�g���A���e�L�X�g�u�e���\����v
	TEX_ID_MOVE_TEXT,				// �`���[�g���A���e�L�X�g�u�ړ��v
	TEX_ID_CAMERA_ROTATE_TEXT,		// �`���[�g���A���e�L�X�g�u�J������]�v
	TEX_ID_CHANGE_WEAPON_TEXT,		// �`���[�g���A���e�L�X�g�u�����ؑցv
	TEX_ID_RELOAD_TEXT,				// �`���[�g���A���e�L�X�g�u�����[�h�v
	TEX_ID_MOUSE_LEFT,				// �`���[�g���A������u�}�E�X�̍��N���b�N�v
	TEX_ID_MOUSE_RIGHT,				// �`���[�g���A������u�}�E�X�̉E�N���b�N�v
	TEX_ID_MOUSE_WHEEL,				// �`���[�g���A������u�}�E�X�̃z�C�[���v
	TEX_ID_MOUSE_MOVE,				// �`���[�g���A������u�}�E�X�ړ��v
	TEX_ID_WASD_RED,				// �`���[�g���A������uWASD�L�[�v
	TEX_ID_SHIFT,					// �`���[�g���A������uShift�L�[�v
	TEX_ID_KEY_R,					// �`���[�g���A������uR�L�[�v
	TEX_ID_PAD_BUTTONS,				// �`���[�g���A������u�p�b�h��ABXY�{�^���v
	TEX_ID_PAD_R,					// �`���[�g���A������u�p�b�h��R�X�e�B�b�N�v
	TEX_ID_PAD_R3,					// �`���[�g���A������u�p�b�h��R3�v
	TEX_ID_PAD_RTRB,				// �`���[�g���A������u�p�b�h��RT or RB�v
	TEX_ID_PAD_L,					// �`���[�g���A������u�p�b�h��L�X�e�B�b�N�v
	TEX_ID_PAD_L3,					// �`���[�g���A������u�p�b�h��L3�v
	TEX_ID_PAD_LTLB,				// �`���[�g���A������u�p�b�h��LT or LB�v
	TEX_ID_CAMERA_SENSI,			// �|�[�Y��ʁu�J�������x�v
	TEX_ID_CAMERA_REVERSE,			// �|�[�Y��ʁu�J�������]�v
	TEX_ID_PLUS,					// �|�[�Y��ʁu+�v
	TEX_ID_MINUS,					// �|�[�Y��ʁu-�v
	TEX_ID_RADER_FRAME,				// ���[�_�[�̘g�g��
	TEX_ID_RADIO_WAVE,				// �d�g�e�N�X�`��
	TEX_ID_RADIO_WAVE1,				// �d�g�e�N�X�`���P
	TEX_ID_RADIO_WAVE2,				// �d�g�e�N�X�`���Q
	TEX_ID_RADIO_WAVE3,				// �d�g�e�N�X�`���R
	TEX_ID_MAX,
} TEXTURE_ID;

// �e�N�X�`���ǂݍ��ݗp�̃\�[�X
static const std::string TEXTURE_SOURCE[] =
{
	"data/textures/field000.png",
	"data/textures/yajirusi.png",
	"data/textures/tree.png",
	"data/textures/sky001.jpg",
	"data/textures/sky002.jpg",
	"data/textures/number001.png",
	"data/textures/title_logo.png",
	"data/textures/result.png",
	"data/textures/circle.jpg",
	"data/textures/normal_circle.png",
	"data/textures/star.png",
	"data/textures/attack.png",
	"data/textures/white.png",
	"data/textures/wall001.png",
	"data/textures/NowLoading.png",
	"data/textures/LoadFrame.png",
	"data/textures/LoadGage.png",
	"data/textures/press_space.png",
	"data/textures/press_start.png",
	"data/textures/rank_a.png",
	"data/textures/rank_b.png",
	"data/textures/rank_c.png",
	"data/textures/mission_complete.png",
	"data/textures/mission_failed.png",
	"data/textures/mission.png",
	"data/textures/wanted.png",
	"data/textures/tutorial.png",
	"data/textures/exclamation.png",
	"data/textures/caution.png",
	"data/textures/pause.png",
	"data/textures/tutorial_pause.png",
	"data/textures/howtouse.png",
	"data/textures/crosshair_circle.png",
	"data/textures/rifle_crosshair.png",
	"data/textures/shotgun_crosshair.png",
	"data/textures/smoke.jpg",
	"data/textures/flash.png",
	"data/textures/debug_circle.png",
	"data/textures/damage_screen.png",
	"data/textures/kill_enemy_num.png",
	"data/textures/grass.jpg",
	"data/textures/soil.jpg",
	"data/textures/rock.jpg",
	"data/textures/bulleticon.png",
	"data/textures/dash.png",
	"data/textures/gunshot.png",
	"data/textures/gunup.png",
	"data/textures/move.png",
	"data/textures/camerarotate.png",
	"data/textures/changeweapon.png",
	"data/textures/reload.png",
	"data/textures/oldmouse_l.png",
	"data/textures/oldmouse_r.png",
	"data/textures/oldmouse_w.png",
	"data/textures/mouse_move.png",
	"data/textures/wasd_red.png",
	"data/textures/shift.png",
	"data/textures/key_r.png",
	"data/textures/pad_buttons.png",
	"data/textures/pad_r.png",
	"data/textures/pad_r3.png",
	"data/textures/pad_rt_or_rb.png",
	"data/textures/pad_l.png",
	"data/textures/pad_l3.png",
	"data/textures/pad_lt_or_lb.png",
	"data/textures/camera_sensi.png",
	"data/textures/camera_reverse.png",
	"data/textures/plus.png",
	"data/textures/minus.png",
	"data/textures/rader_frame.png",
	"data/textures/radio_wave.png",
	"data/textures/radio_wave1.png",
	"data/textures/radio_wave2.png",
	"data/textures/radio_wave3.png",
};

// �e�N�X�`�������[�h����Ă��邩�������邽�߂̃}�b�v
static std::map<int, std::string> MAP_TEXTURES;				// �e�N�X�`���̃t�@�C���p�X�Ǘ�
static std::map<std::string, bool> MAP_LOADING_TEXTURES;	// ���łɓ����e�N�X�`�����ǂݍ��܂�Ă��Ȃ���
static std::map<int, int> MAP_TEXTURE_USE_NUM;				// ���̃e�N�X�`�����g���Ă��鐔

class CTexture
{
public:
	CTexture()
	{
		m_Texture = NULL;
		m_Height = 0;
		m_Width = 0;
	}
	~CTexture();

	static void Init();
	static void Load(int id);
	static void Release(int id);
	static void LoadAll();
	static void ReleaseAll();
	static LPDIRECT3DTEXTURE9 GetTexture(int id);

	LPDIRECT3DTEXTURE9 GetTexture() { return m_Texture; }
	void LoadTexture(int id);
	void ReleaseTexture();

private:
	LPDIRECT3DTEXTURE9 m_Texture;
	float m_Width;
	float m_Height;
};

static CTexture TEXTURES[TEX_ID_MAX];

#endif // !_TEXTURE_H_
