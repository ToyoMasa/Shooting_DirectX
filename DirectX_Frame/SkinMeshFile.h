#ifndef SKINMESH_FILE_H_
#define SKINMESH_FILE_H_

#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "SkinMeshData.h"

static const int MAX_ANIMATION = 20;

class SkinMeshFileAnimation
{
public:
	SkinMeshFileAnimation() : m_AnimController(NULL) {}
	~SkinMeshFileAnimation();

	LPD3DXANIMATIONCONTROLLER& GetAnimController() { return m_AnimController; }
	LPD3DXANIMATIONSET GetAnimSet(int id) { return m_AnimSet[id]; }

	void Init();
	void UpdateAnim(float time);

	// 追加
	DWORD GetCurrentAnim() { return m_CurrentAnim; }
	// アニメーション切替
	void ChangeAnim(UINT animID, float shiftTime);
	// アニメーションの速度設定
	bool SetLoopTime(UINT animID, FLOAT time);
	// アニメ移行時間セット
	void SetShiftTime(float time) { m_ShiftTime = time; }
	// 1回再生のアニメ―ション
	void PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID);
	void StopMontage() { m_bPlayMontage = false; }
	bool GetPlayMontage() { return m_bPlayMontage; }
	// アニメーションの再生時間を取得
	float GetWeightTime() { return m_CurrentWeightTime; }

private:
	// アニメーション管理
	LPD3DXANIMATIONCONTROLLER m_AnimController;

	// アニメーション
	LPD3DXANIMATIONSET m_AnimSet[MAX_ANIMATION];
	//現在のアニメーション
	DWORD m_CurrentAnim;
	//現在のアニメーションデータトラック
	D3DXTRACK_DESC m_CurrentTrackDesc;
	float m_TrackSpeed;             // トラックスピード調節値
	float m_ShiftTime;              // シフトするのにかかる時間
	float m_CurrentWeightTime;      // 現在のウェイト時間

									// 1度きりアニメーション
	DWORD m_NextAnim;
	bool m_bPlayMontage;
	float m_MontageTime;

};

// 階層付きXFile
class SkinMeshFile
{
public:
	SkinMeshFile() :
		m_SkinMeshData(),
		m_RootFrame(NULL),
		m_AnimController(NULL)
	{}

	~SkinMeshFile();

	/*
		XFile読み込み
			戻り値：
				読み込み結果：
					成功：
						true
					失敗：
						false
			引数：
				std::string file_name：
					ファイル名
			内容：
				XFileの読み込みを行う
	*/
	bool Load(std::string file_name, SkinMeshFileAnimation* anim);


	/*
		ボーン行列の領域確保(全ボーン)
			戻り値：
				結果
					成功：S_OK
					失敗：E_FAIL

			引数：					
				LPD3DXFRAME  
					対象フレーム

			内容：
				対象フレームのボーン行列のメモリ領域の確保関数を実行する
				子供、兄弟に対してこの関数を再帰として使用し、
				末端まで確保を行うようにする
	*/
	HRESULT AllocateAllBoneMatrix(LPD3DXFRAME  );	

	/*
		ボーン行列の領域確保(全ボーン)
			戻り値：
				結果
					成功：S_OK
					失敗：E_FAIL

			引数：					
				LPD3DXMESHCONTAINER  
					確保対象のメッシュコンテナ

			内容：
				コンテナで使用しているボーン行列分のメモリ領域を確保する
				
	*/
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER  );

	/*
		描画
			戻り値：
				なし

			引数：
				LPD3DXMATRIX matrix
					描画で使用するワールド座標行列

			内容：
				XFileを描画する
				この関数内でルートフレームを引数とした
				DrawFrameを使用する
				その際にこのXFileを描画するオブジェクトの
				ワールド座標行列を引数で渡す
	*/
	void Draw(LPD3DXMATRIX matrix);

	/*
		フレームの描画
			戻り値：
				なし

			引数：
				LPD3DXFRAME frame：
					フレームデータ

			内容：
				引数で指定されたフレームを描画する
				この関数内でフレームが保持している
				全てのメッシュコンテナを描画(DrawMeshContainer)する
	*/
	void DrawFrame(LPD3DXFRAME frame);

	/*
		メッシュコンテナの描画
			戻り値：
				なし

			引数：
				LPD3DXFRAME frame：
					フレームデータ

				LPD3DXMESHCONTAINER container：
					メッシュコンテナ
			内容：
				指定されたメッシュコンテナを描画する
				描画の内容は通常のXFileの描画方法と同じ方法で
				描画できる
	*/
	void DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container);

	/*
		フレームの更新
			戻り値：
				なし

			引数：
				LPD3DXFRAME base：
					更新フレーム

				LPD3DXMATRIX parent_matrix：
					親の姿勢行列

			内容：
				親の姿勢行列を元にして自分の姿勢行列を更新する
				子、兄弟がいる場合はそれを再帰で呼び出す
	*/
	void UpdateFrame(LPD3DXFRAME base, LPD3DXMATRIX parent_matrix);

private:
	// 対象のボーンを検索
	FrameData* SearchBoneFrame(LPSTR _BoneName, D3DXFRAME* _pFrame);
public:
	// ボーンのマトリックス取得（ボーンの名前）
	D3DXMATRIX GetBoneMatrix(LPSTR _BoneName);

	LPD3DXFRAME GetRootFrame() { return m_RootFrame; }

private:
	// 階層データ
	SkinMeshData *m_SkinMeshData;

	// ルートフレーム
	LPD3DXFRAME m_RootFrame;

	// アニメーション管理
    LPD3DXANIMATIONCONTROLLER m_AnimController;

	// 追加
	// 全フレーム参照配列
	std::vector<FrameData*> m_FrameArray;
	// メッシュコンテナありのフレーム参照記録
	std::vector<FrameData*> m_IntoMeshFrameArray;

	// 全てのフレームポインタ格納処理関数
	void CreateFrameArray(LPD3DXFRAME _pFrame);

};

#endif