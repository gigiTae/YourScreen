#include "GUIManager.h"
#include "../ImpGraphicsEngine/IImpGraphicsEngine.h"
using namespace ImpGraphics;

GUIManager::GUIManager()
{
	
}

GUIManager::~GUIManager()
{

}

void GUIManager::Init(size_t hWnd, void* d3dDevice, void* d3dDeviceContext)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(reinterpret_cast<HWND>(hWnd));
	ImGui_ImplDX11_Init(reinterpret_cast<ID3D11Device*>(d3dDevice), 
		reinterpret_cast<ID3D11DeviceContext*>(d3dDeviceContext));
}

void GUIManager::Update(IImpGraphicsEngine* graphicsEngine)
{
	// (Your code process and dispatch Win32 messages)
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow(); // Show demo window! :)
	/// -------------------- Custom --------------------
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	bool open_ptr = true;

	ImGui::Begin("Debug", &open_ptr, window_flags);

	static bool isOnWireFrame = false;
	if (ImGui::Checkbox("WireFrame", &isOnWireFrame))
	{
		graphicsEngine->SetWireFrame(isOnWireFrame);
	}

	static bool isOnGrid = true;
	if (ImGui::Checkbox("OnGrid", &isOnGrid))
	{
		graphicsEngine->SetGrid(isOnGrid);
	}

	static bool isOnAxis = true;
	if (ImGui::Checkbox("OnAxis", &isOnAxis))
	{
		graphicsEngine->SetAxis(isOnAxis);
	}

	ImGui::End();
// 	ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.
//  
// 	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
// 	ImGui::Text("Position");               // Display some text (you can use a format strings too)
// 	ImGui::PopStyleColor();
// 
// 	ImGui::Text("X : %f, Y : %f, Z : %f", *xPos, *yPos, *zPos);
// 
// 	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
// 	ImGui::Text("\nScale");               // Display some text (you can use a format strings too)
// 	ImGui::PopStyleColor();
// 
// 	ImGui::Text("X : %f, Y : %f, Z : %f", *xScale, *yScale, *zScale);
// 
// 	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
// 	ImGui::Text("\nCur Mode: ");               // Display some text (you can use a format strings too)
// 	ImGui::PopStyleColor();
// 	
// 	ImGui::SameLine();
// 		ImGui::Text("\nMove");
// 		ImGui::Text("\nScale");
// 
// 		if (ImGui::Button("Change Mode"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
// 		{
// 
// 	}
// 	if (ImGui::Button("Reset"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
// 	{
// 	}
// 		
// 	if (ImGui::Button("+"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
// 	{
// 
// 	}
// 	static int d0 = 3;
// 	ImGui::InputInt("input double", &d0);
// 
// 	ImGui::Text("%d", d0);
// 
// // 	ImGui::InputText("##Name", txt_def, sizeof(txt_def));
// // 	ImGui::SameLine();
// 	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
// 	//ImGui::Checkbox("Another Window", &show_another_window);
// 
// 	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
// 	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
// 
// // 	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
// // 		counter++;
// // 	ImGui::SameLine();
// // 	ImGui::Text("counter = %d", counter);
// 
// 	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
// 	ImGui::End();
}

void GUIManager::Render()
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls swapchain's Present() function)
}

void GUIManager::Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}