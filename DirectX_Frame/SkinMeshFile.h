//======================================================================
//	スキンメッシュファイルヘッダ[SkinMeshFile.h]
//
//======================================================================
#ifndef SKINMESH_FILE_H_
#define SKINMESH_FILE_H_

#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "skinMeshData.h"

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

	bool Load(std::string file_name, SkinMeshFileAnimation* anim);

	HRESULT AllocateAllBoneMatrix(LPD3DXFRAME  );	

	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER  );

	void Draw(LPD3DXMATRIX matrix);
	void DrawWithShader(LPD3DXMATRIX matrix, CShader* shader);

	void DrawFrame(LPD3DXFRAME frame);
	void DrawFrameWithShader(LPD3DXFRAME frame, CShader* shader);

	void DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container);
	void DrawMeshContainerWithShader(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container, CShader* shader);

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