#include<iostream>
#include"../ButiRendering/Common.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"Window.h"
std::int32_t main() {
	auto vlp_window = ButiEngine::make_value<ButiEngine::Window>();
	HINSTANCE instance=nullptr;
	vlp_window->Initialize("RenderingTest", false, false, instance, 960, 540);
	
	while (vlp_window->Update()) {

	}
	vlp_window->Release();
	return 0;
}