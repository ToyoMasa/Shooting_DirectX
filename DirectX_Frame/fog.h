#ifndef _FOG_H_
#define _FOG_H_

#include <d3dx9.h>

class CFog
{
public:
	CFog() {}
	~CFog() {}

	void Set(D3DCOLOR const&color, float const&start, float const&end);
	void Set(D3DCOLOR const&color, float const&density);
	void End();
private:
	D3DCAPS9 m_Caps;
};

#endif // !_FOG_H_
