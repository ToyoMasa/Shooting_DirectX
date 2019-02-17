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
	TEX_ID_NORMAL_CIRCLE,			// 丸いパーティクル
	TEX_ID_STAR,					// 星
	TEX_ID_ATTACK,					// 攻撃可能テキスト
	TEX_ID_WHITE,					// 真っ白
	TEX_ID_WALL01,					// 壁
	TEX_ID_NOWLOADING,				// ロード画面文字
	TEX_ID_LOADFRAME,				// ロード画面ゲージフレーム
	TEX_ID_LOADGAGE,				// ロード画面ゲージ中身
	TEX_ID_PRESS_SPACE,				// スペースを押す
	TEX_ID_PRESS_START,				// スタートを押す
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
	TEX_ID_CROSSHAIR_RIFLE,			// ライフルのクロスヘア
	TEX_ID_CROSSHAIR_SHOTGUN,		// ショットガンのクロスヘア
	TEX_ID_SMOKE,					// 煙
	TEX_ID_FLASH,					// マズルフラッシュ
	TEX_ID_CIRCLE_2D,				// 2D画面用円
	TEX_ID_DAMAGE_SCREEN,			// ダメージを受けた時の出血エフェクト
	TEX_ID_KILL_ENEMY_NUM,			// 倒した敵の数
	TEX_ID_GRASS,					// 草原
	TEX_ID_SOIL,					// 土
	TEX_ID_ROCK,					// 岩
	TEX_ID_BULLETICON,				// 銃弾アイコン
	TEX_ID_DASH_TEXT,				// チュートリアルテキスト「走る」
	TEX_ID_GUNSHOT_TEXT,			// チュートリアルテキスト「銃で撃つ」
	TEX_ID_GUNUP_TEXT,				// チュートリアルテキスト「銃を構える」
	TEX_ID_MOVE_TEXT,				// チュートリアルテキスト「移動」
	TEX_ID_CAMERA_ROTATE_TEXT,		// チュートリアルテキスト「カメラ回転」
	TEX_ID_CHANGE_WEAPON_TEXT,		// チュートリアルテキスト「武器を切替」
	TEX_ID_RELOAD_TEXT,				// チュートリアルテキスト「リロード」
	TEX_ID_MOUSE_LEFT,				// チュートリアル操作「マウスの左クリック」
	TEX_ID_MOUSE_RIGHT,				// チュートリアル操作「マウスの右クリック」
	TEX_ID_MOUSE_WHEEL,				// チュートリアル操作「マウスのホイール」
	TEX_ID_MOUSE_MOVE,				// チュートリアル操作「マウス移動」
	TEX_ID_WASD_RED,				// チュートリアル操作「WASDキー」
	TEX_ID_SHIFT,					// チュートリアル操作「Shiftキー」
	TEX_ID_KEY_R,					// チュートリアル操作「Rキー」
	TEX_ID_PAD_BUTTONS,				// チュートリアル操作「パッドのABXYボタン」
	TEX_ID_PAD_R,					// チュートリアル操作「パッドのRスティック」
	TEX_ID_PAD_R3,					// チュートリアル操作「パッドのR3」
	TEX_ID_PAD_RTRB,				// チュートリアル操作「パッドのRT or RB」
	TEX_ID_PAD_L,					// チュートリアル操作「パッドのLスティック」
	TEX_ID_PAD_L3,					// チュートリアル操作「パッドのL3」
	TEX_ID_PAD_LTLB,				// チュートリアル操作「パッドのLT or LB」
	TEX_ID_CAMERA_SENSI,			// ポーズ画面「カメラ感度」
	TEX_ID_CAMERA_REVERSE,			// ポーズ画面「カメラ反転」
	TEX_ID_PLUS,					// ポーズ画面「+」
	TEX_ID_MINUS,					// ポーズ画面「-」
	TEX_ID_RADER_FRAME,				// レーダーの枠組み
	TEX_ID_RADIO_WAVE,				// 電波テクスチャ
	TEX_ID_RADIO_WAVE1,				// 電波テクスチャ１
	TEX_ID_RADIO_WAVE2,				// 電波テクスチャ２
	TEX_ID_RADIO_WAVE3,				// 電波テクスチャ３
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
