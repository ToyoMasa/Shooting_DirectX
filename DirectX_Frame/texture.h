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
	TEX_ID_STAR,					// ��
	TEX_ID_ATTACK,					// �U���\�e�L�X�g
	TEX_ID_BLACK,					// �^����
	TEX_ID_WALL01,					// ��
	TEX_ID_NOWLOADING,				// ���[�h��ʕ���
	TEX_ID_LOADFRAME,				// ���[�h��ʃQ�[�W�t���[��
	TEX_ID_LOADGAGE,				// ���[�h��ʃQ�[�W���g
	TEX_ID_PRESS_SPACE,				// �X�y�[�X������
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
	TEX_ID_SMOKE,					// ��
	TEX_ID_FLASH,					// �}�Y���t���b�V��
	TEX_ID_CIRCLE_2D,					// �}�Y���t���b�V��
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
	"data/textures/star.png",
	"data/textures/attack.png",
	"data/textures/black.png",
	"data/textures/wall001.png",
	"data/textures/NowLoading.png",
	"data/textures/LoadFrame.png",
	"data/textures/LoadGage.png",
	"data/textures/press_space.png",
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
	"data/textures/smoke.jpg",
	"data/textures/flash.png",
	"data/textures/debug_circle.png",
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
