//======================================================================
//	マップ作成ツール[mapmake.h]
//
//======================================================================
#ifndef _MAPMAKE_H_
#define _MAPMAKE_H_

#include "mode.h"

class CCamera;

//*********************************************************
//	ツール上で選択できるモード
//*********************************************************
enum MAKE_MODE
{
	MAP_MAKE,
	WAYPOINT_MAKE,
	NONE
};

//*********************************************************
//	マップ作成ツール
//*********************************************************
class CModeMapMake : public CMode
{
public:
	CModeMapMake() : CMode() {}
	~CModeMapMake() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ChangeMap();

	void MapMakeUpdate();
	void WaypointMakeUpdate();

private:
	static CCamera*	m_Camera;
	CScene2D*		m_Brush;
	CLight*			m_Light;
	CField*			m_Field;
	float**			m_Heights;
	DWORD**			m_isHit;
	int				m_NumBlock;
	int				m_NextNumBlock;
	int				m_BrushType;
	int				m_WaypointCursolType;
	float			m_Size;
	float			m_BrushRange;
	float			m_BrushValue;
	float			m_TempColor[4];
	DWORD			m_BrushColor;
	double			m_TestText;
	char			m_FileName[256];
	bool			m_VertexMode;
	bool			m_Wire;

	int				m_SelectMode;
	int				m_SelectWayPoint;
};

#endif // !_MAPMAKE_H_
