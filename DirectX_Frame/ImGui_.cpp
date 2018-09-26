//#include "Main_.h"
#include "ImGui_.h"

bool show_another_window;

void CImGui::Init(HWND hwnd,LPDIRECT3DDEVICE9 device)
{
	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplDX9_Init(hwnd, device);

	// Setup style
	ImGui::StyleColorsDark();

	show_another_window = false;
}
void CImGui::Uninit()
{
	ImGui_ImplDX9_Shutdown();
	ImGui::DestroyContext();
}
void CImGui::Update()
{	
	
	
	//static float f = 0.0f;
	//static int counter = 0;
	//ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
	//ImGui::Checkbox("Another Window", &show_another_window);

	//if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
	//	counter++;
	//ImGui::SameLine();
	//ImGui::Text("counter = %d", counter);

	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	

	


	//if (show_demo_window)
	//{
	//	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
	//	ImGui::ShowDemoWindow(&show_demo_window);
	//}
	 //Rendering
	
}
void CImGui::BeginDraw()
{
	ImGui_ImplDX9_NewFrame();

	// サイズ指定
	ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiSetCond_Once);
	
	ImGui::Begin("Debug Window", &show_another_window);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	
}
void CImGui::EndDraw()
{
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}