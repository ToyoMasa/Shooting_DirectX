//======================================================================
//	ƒ^ƒCƒgƒ‹[title.h]
//
//======================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "mode.h"
#include "sound.h"

class CFog;
class CField;
class CLight;

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
	static CScene2D* TextPressSpace;
	static CSceneSkinMesh* m_Mesh;
	static CSound* BGM;
	static CSound* SE;
	static CScene2D* Load;
	static CScene2D* LoadFrame;
	static CScene2D* LoadGage;
	static CCamera* Camera;
	static CFog* Fog;
	static CField* Field;
	static CLight* Light;
	static int Count;

};

#endif // !_TITLE_H_
