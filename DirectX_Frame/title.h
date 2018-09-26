//======================================================================
//	ƒ^ƒCƒgƒ‹[title.h]
//
//======================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "mode.h"
#include "sound.h"

class CModeTitle : public CMode
{
public:
	CModeTitle() : CMode() {}
	~CModeTitle() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static CScene2D* m_TitleLogo;
	static CScene2D* m_Text_PressSpace;
	static CSceneSkinMesh* m_Mesh;
	static CCamera* m_Camera;
	static CSound* m_BGM;
	static CSound* m_SE;
	static int m_Count;

	static CScene2D* Load;
	static CScene2D* LoadFrame;
	static CScene2D* LoadGage;
};

#endif // !_TITLE_H_
