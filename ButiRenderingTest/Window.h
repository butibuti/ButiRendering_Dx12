#ifndef WINDOW_H
#define WINDOW_H
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include"ButiMath/ButiMath.h"
#include<string>
namespace ButiEngine {


class Window 
{
public:
	Window();
	~Window();
	void Initialize(const std::string arg_windowName, const bool arg_popType, const bool isFullScreen, HINSTANCE& arg_hinstance, const std::uint32_t width = 0, const std::uint32_t height = 0);
	HWND GetHandle();
	UInt2 GetSize();
	Vector2 GetWindowCenterPosition();
	void SetSize(const std::int32_t width, const std::int32_t height);
	bool Update();
	Vector2 GetWindowPosition();
	void Release();
private:
	HWND handle;
	UInt2 size;
	WNDCLASSW windowClass= WNDCLASSW();
	std::wstring wName;
	bool isBefSelect = false, isCurrentSelect = false;
	static LRESULT CALLBACK ProceedMessage(HWND window, std::uint32_t message, WPARAM wParam, LPARAM lParam);
};
}

#endif