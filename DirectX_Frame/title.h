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
	static CScene2D* TitleLogo;
	static CScene2D* Text_PressSpace;
	static CSceneSkinMesh* m_Mesh;
	static CCamera* Camera;
	static CSound* BGM;
	static CSound* SE;
	static int Count;

	static CScene2D* Load;
	static CScene2D* LoadFrame;
	static CScene2D* LoadGage;
};

#endif // !_TITLE_H_
