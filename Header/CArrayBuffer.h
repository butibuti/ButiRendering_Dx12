#pragma once
#include"CBuffer.h"
namespace ButiEngine {
namespace ButiRendering {
template <class T>
class CArrayBuffer :public ICBuffer
{
public:
	virtual T& Get(const std::uint32_t arg_index) = 0;

	void SetValueControlFunction(std::function<bool(T&)> arg_func_control) { m_func_control = arg_func_control; }
	bool OnControl()override { return m_func_control(Get(0)); }
private:
	std::function<bool(T&)> m_func_control;
};
}
}