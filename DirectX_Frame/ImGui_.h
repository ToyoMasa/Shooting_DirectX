#pragma once

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include	<d3d9.h>
#include	<d3dx9.h>



class CImGui
{
public:
	static void Init(HWND hwnd, LPDIRECT3DDEVICE9 device);
	static void Uninit();
	static void Update();
	static void BeginDraw();
	static void EndDraw();
private:

};
