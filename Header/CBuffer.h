#ifndef CBUFFER_H
#define CBUFFER_H

#pragma once
#include<string>
#include "ButiUtil/ButiUtil/ObjectFactory.h"
#include"GraphicDevice.h"
#include<functional>
#include<typeinfo>
namespace ButiEngine {
namespace ButiRendering {
struct BlankSpace {
	std::uint32_t index;
	std::uint32_t size;
};
class ICBuffer :public IObject {
public:
	virtual const std::string& GetBufferName()const =0;

	virtual void Attach(const std::uint32_t slotOffset) const = 0;
	virtual void Update() = 0;
	virtual void CreateBuffer(const bool arg_mapKeep = true) = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;

	virtual bool OnControl() = 0;
	virtual Value_ptr<ICBuffer> Clone() = 0;
protected:
};
namespace CbufferDetail {
class ICBufferUpdater {
public:
	virtual void Attach(const std::uint32_t arg_slotOffset)const = 0;
	virtual void Update(void* arg_p_data) = 0;
	virtual void Release() = 0;
	virtual void CreateBuffer(void* arg_p_data, const bool arg_mapKeep = true) = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
private:
};
BUTIRENDERING_API ICBufferUpdater* CreateCBufferUpdater(const std::int32_t arg_size);
}
template <typename T>
class CBuffer :public ICBuffer
{
public:
	CBuffer(){
		CreateCBufferUpdater();
	}
	~CBuffer() {
		if (m_p_updater) {
			m_p_updater->Release();
			ButiMemorySystem::Allocator::deallocate(m_p_updater);
		}
	}
	void CreateCBufferUpdater() {
		if (!m_p_updater) {
			m_p_updater = CbufferDetail::CreateCBufferUpdater(sizeof(T));
		}		
	}
	T& Get() { return m_instance; }
	void Attach(const std::uint32_t arg_slotOffset) const {
		m_p_updater->Attach(arg_slotOffset);
	}
	void Update()override{ 
		m_p_updater->Update(&m_instance);
	}
	void CreateBuffer(const bool arg_mapKeep=true ) {
		m_mapKeep = arg_mapKeep;
		m_p_updater->CreateBuffer(&m_instance,arg_mapKeep);
	}
	void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) { m_p_updater->SetGraphicDevice(arg_graphicDevice); }
	void SetValueControlFunction(std::function<bool(T&)> arg_func_control) { m_func_control = arg_func_control; }
	bool OnControl() override{return m_func_control(Get()); }
	const std::string& GetBufferName()const {
		return T::GetConstantBufferName();
	}
	Value_ptr<ICBuffer> Clone()override {
		auto output = ObjectFactory::Create<CBuffer<T>>();
		(output->m_instance) = (m_instance);
		return output;
	}
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(m_instance);
		CreateCBufferUpdater();
	}
private:
	std::function<bool(T&)> m_func_control;
	T m_instance;
	std::uint32_t m_size=0;
	bool m_mapKeep = false;
	CbufferDetail::ICBufferUpdater* m_p_updater=nullptr;
};
template<typename T> Value_ptr<CBuffer<T>> CreateCBuffer() { 
	return ObjectFactory::Create<CBuffer<T>>();
}
}
}

#endif // !CBUFFER_H