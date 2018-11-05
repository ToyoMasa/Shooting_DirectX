//======================================================================
//	テクスチャ管理ヘッダ[texture]
//
//======================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <map>

// テクスチャ管理用ID
typedef enum
{
	TEX_ID_FIELD001 = 0,			// フィールド
	TEX_ID_CURSOR,					// カーソル
	TEX_ID_TREE,					// ツリー
	TEX_ID_SKY,						// 空
	TEX_ID_SKY002,					// 夜空
	TEX_ID_NUMBER001,				// 数字
	TEX_ID_TITLE,					// タイトル
	TEX_ID_RESULT,					// リザルト
	TEX_ID_CIRCLE,					// 丸いパーティクル
	TEX_ID_STAR,					// 星
	TEX_ID_ATTACK,					// 攻撃可能テキスト
	TEX_ID_BLACK,					// 真っ黒
	TEX_ID_WALL01,					// 壁
	TEX_ID_NOWLOADING,				// ロード画面文字
	TEX_ID_LOADFRAME,				// ロード画面ゲージフレーム
	TEX_ID_LOADGAGE,				// ロード画面ゲージ中身
	TEX_ID_PRESS_SPACE,				// スペースを押す
	TEX_ID_RANK_A,					// A
	TEX_ID_RANK_B,					// B
	TEX_ID_RANK_C,					// C
	TEX_ID_MISSION_COMPLETE,		// ゲームクリア
	TEX_ID_MISSION_FAILED,			// ゲームオーバー
	TEX_ID_MISSION,					// ミッション
	TEX_ID_WANTED,					// 指名手配
	TEX_ID_TUTORIAL,				// 操作説明
	TEX_ID_EXCLAMATION,				// ビックリマーク
	TEX_ID_CAUTION,					// 警告文
	TEX_ID_PAUSE,					// pause
	TEX_ID_TURORIAL_PAUSE,			// pauseのチュートリアル
	TEX_ID_HOWTOUSE,				// 操作確認
	TEX_ID_CROSSHAIR_CIRCLE,		// クロスヘア
	TEX_ID_SMOKE,					// 煙
	TEX_ID_FLASH,					// マズルフラッシュ
	TEX_ID_CIRCLE_2D,					// マズルフラッシュ
	TEX_ID_MAX,
} TEXTURE_ID;

// テクスチャ読み込み用のソース
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

// テクスチャがロードされているか検索するためのマップ
static std::map<int, std::string> MAP_TEXTURES;				// テクスチャのファイルパス管理
static std::map<std::string, bool> MAP_LOADING_TEXTURES;	// すでに同じテクスチャが読み込まれていないか
static std::map<int, int> MAP_TEXTURE_USE_NUM;				// このテクスチャを使っている数

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
