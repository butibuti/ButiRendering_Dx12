#pragma once
#include"CBuffer.h"
namespace ButiEngine {
namespace ButiRendering {
template <class T>
class CArrayBuffer :public ICBuffer
{
public:
	virtual T& Get(const std::uint32_t arg_index) = 0;
protected:
};
}
}