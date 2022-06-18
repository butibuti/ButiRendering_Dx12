#ifndef CBUFFER_H
#define CBUFFER_H

#pragma once
#include<string>
#include "ButiUtil/ButiUtil/ObjectFactory.h"
#include"GraphicDevice.h"
#include<functional>
namespace ButiEngine {
namespace ButiRendering {
struct BlankSpace {
	std::uint32_t index;
	std::uint32_t size;
};
class ICBuffer :public IObject {
public:

	void SetSlot(const std::uint32_t arg_slot) {
		slot = arg_slot;
	}
	const std::uint32_t GetSlot()const {
		return slot;
	}
	const std::string& GetExName()const {
		return exName;
	}
	void SetExName(const std::string& arg_cbufferName) {
		exName = arg_cbufferName;
	}
	virtual void Attach(const std::uint32_t slotOffset) const = 0;
	virtual void Update()const {}
	virtual void CreateBuffer(const bool arg_mapKeep = true) = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;

	virtual bool OnControl() = 0;
	virtual Value_ptr<ICBuffer> Clone() = 0;

	std::uint32_t slot = 0;
	std::string exName;
protected:
};

template <typename T>
class CBuffer :public ICBuffer
{
public:
	CBuffer() {}
	~CBuffer() {}
	virtual T& Get() = 0;
	void SetValueControlFunction(std::function<bool(T&)> arg_func_control) { m_func_control = arg_func_control; }
	bool OnControl() override{return m_func_control(Get()); }
private:
	std::function<bool(T&)> m_func_control;
};
template<typename T> Value_ptr<CBuffer<T>> CreateCBuffer(const std::int32_t arg_slot, const std::string& arg_bufferName) { return nullptr; }
}
}

#endif // !CBUFFER_H