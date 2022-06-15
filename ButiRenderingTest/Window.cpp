#include"Window.h"
#include"ButiUtil/ButiUtil/Util.h"
ButiEngine::Window::Window()
{
}

ButiEngine::Window::~Window()
{
}

void ButiEngine::Window::Initialize(const std::string arg_windowName, const bool arg_popType, const bool isFullScreen, HINSTANCE& arg_hinstance, const std::uint32_t width, const std::uint32_t height)
{
	wName = Util::StringToWString(arg_windowName);


	windowClass.lpfnWndProc = ProceedMessage;
	
	windowClass.hInstance = arg_hinstance? arg_hinstance: GetModuleHandleW(nullptr);
	windowClass.hCursor = (HCURSOR)LoadImageW(nullptr, MAKEINTRESOURCEW(32512), IMAGE_CURSOR, 0, 0, LR_SHARED);

	windowClass.lpszClassName = wName.c_str();

	RegisterClassW(&windowClass);
	auto windowState = WS_OVERLAPPEDWINDOW;

	if (isFullScreen) {
		windowState = WS_POPUP;
	}

	handle = CreateWindowExW(0, wName.c_str(), wName.c_str(), windowState, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  nullptr, nullptr, windowClass.hInstance, nullptr);
	auto e=GetLastError();
	SetSize(width, height);
	auto res = ShowWindow(handle, 3);
	IsWindowEnabled(handle);
}

HWND ButiEngine::Window::GetHandle()
{
	return handle;
}

ButiEngine::UInt2 ButiEngine::Window::GetSize()
{
	RECT clienRect = {};
	GetClientRect(handle, &clienRect);

	return UInt2(clienRect.right - clienRect.left, clienRect.bottom - clienRect.top);
}

ButiEngine::Vector2 ButiEngine::Window::GetWindowCenterPosition()
{
	auto windowSize = GetSize();
	auto winPos = GetWindowPosition();

	return Vector2(winPos.x + 2 + windowSize.x / 2, winPos.y + 27.0f + windowSize.y / 2);
}

void ButiEngine::Window::SetSize(const std::int32_t width, const std::int32_t height)
{
	RECT windowRect = {};
	RECT clientRect = {};
	GetWindowRect(handle, &windowRect);
	GetClientRect(handle, &clientRect);

	std::int32_t w = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + width;
	std::int32_t h = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + height;
	std::int32_t x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	std::int32_t y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
	SetWindowPos(handle, HWND_TOP, x, y, w, h, SWP_FRAMECHANGED);
}

bool ButiEngine::Window::Update()
{
	MSG message = {};
	while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
	{
		if (message.message == WM_QUIT) {
			//GameDevice::SetReturnCode(0);
			isBefSelect = isCurrentSelect;

			auto activeHandle = GetActiveWindow();
			isCurrentSelect = activeHandle == handle;
			return false;
		}
		TranslateMessage(&message);
		DispatchMessageW(&message);

	}

	isBefSelect = isCurrentSelect;

	auto activeHandle = GetActiveWindow();
	isCurrentSelect = activeHandle == handle;
	return true;
}

ButiEngine::Vector2 ButiEngine::Window::GetWindowPosition()
{
	RECT rect;
	GetWindowRect(handle, &rect);
	return Vector2((float)rect.left, (float)rect.top);
}

void ButiEngine::Window::Release()
{
	std::int32_t res = DestroyWindow(handle);
	if (!res) {
		auto er = GetLastError();
		std::int32_t i = 0;
	}
	res = UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
}

LRESULT ButiEngine::Window::ProceedMessage(HWND window, std::uint32_t message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(window, message, wParam, lParam);
}
