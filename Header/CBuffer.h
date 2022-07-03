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
namespace CbufferDetail {
class ICBufferUpdater {
public:
	virtual void Attach(const std::uint32_t arg_slotOffset)const = 0;
	virtual void Update(void* arg_p_data)const {}
	virtual void Release() = 0;
	virtual void CreateBuffer(void* arg_p_data, const bool arg_mapKeep = true) = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
private:
};
BUTIRENDERING_API ICBufferUpdater* CreateCBufferUpdater(const std::int32_t arg_slot, const std::int32_t arg_size);
}
template <typename T>
class CBuffer :public ICBuffer
{
public:
	CBuffer(){}
	CBuffer(const std::int32_t arg_slot, const std::string& arg_bufferName) {
		this->slot = arg_slot;
		this->exName = arg_bufferName;
		CreateCBufferUpdater();
	}
	~CBuffer() {
		if (m_instance) {
			m_p_updater->Release();
			m_instance = nullptr;
		}
		ButiMemorySystem::Allocator::deallocate(m_p_updater);
	}
	void CreateCBufferUpdater() {
		if (!m_p_updater) {
			m_p_updater = CbufferDetail::CreateCBufferUpdater(this->slot, sizeof(T));
		}		
	}
	void Initialize() {
		m_instance = (make_value<T>());
	}
	T& Get() { return *m_instance; }
	void Attach(const std::uint32_t arg_slotOffset) const {
		m_p_updater->Attach(arg_slotOffset);
	}
	void Update()const { m_p_updater->Update(m_instance.get()); }
	void CreateBuffer(const bool arg_mapKeep=true ) {
		m_mapKeep = arg_mapKeep;
		m_p_updater->CreateBuffer(m_instance.get(),arg_mapKeep);
	}
	void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) { m_p_updater->SetGraphicDevice(arg_graphicDevice); }
	void SetValueControlFunction(std::function<bool(T&)> arg_func_control) { m_func_control = arg_func_control; }
	bool OnControl() override{return m_func_control(Get()); }

	Value_ptr<ICBuffer> Clone()override {
		auto output = ObjectFactory::Create<CBuffer<T>>(this->slot, this->exName);
		*(output->m_instance) = (*m_instance);
		return output;
	}
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(this->slot);
		archive(this->exName);
		archive(m_instance);
		CreateCBufferUpdater();
	}
private:
	std::function<bool(T&)> m_func_control;
	Value_ptr<T> m_instance=nullptr;
	std::uint32_t m_size=0;
	bool m_mapKeep = false;
	CbufferDetail::ICBufferUpdater* m_p_updater=nullptr;
};
template<typename T> Value_ptr<CBuffer<T>> CreateCBuffer(const std::int32_t arg_slot, const std::string& arg_bufferName) { 
	return ObjectFactory::Create<CBuffer<T>>(arg_slot, arg_bufferName);
}
}
}

#endif // !CBUFFER_H