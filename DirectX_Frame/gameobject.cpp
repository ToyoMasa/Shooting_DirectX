#include "common.h"

void CGameObject::Release()
{
	Uninit();
	delete this;
}
