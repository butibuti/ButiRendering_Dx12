#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/CBuffer.h"
namespace ButiEngine {
namespace ButiRendering {
namespace CbufferDetail {

class CBufferUpdater_Dx12 :public ICBufferUpdater, public GPUResource
{
public:
	CBufferUpdater_Dx12( const std::uint32_t arg_size) :m_size((arg_size + 255) & ~255) {}
	CBufferUpdater_Dx12() {}
	void Release()override {
		if (vwp_heapManager.lock()) {
			vwp_heapManager.lock()->Release(BlankSpace{ static_cast<std::uint32_t>(m_index),static_cast<std::uint32_t>(m_size) / 0x100 });
		}
	}
	void CreateBuffer(void* arg_p_data, const bool arg_mapKeep)override {
		auto out = vwp_heapManager.lock()->CreateConstantBufferView(arg_p_data, arg_mapKeep, m_size);
		m_index = out.index;
		gpuDescriptorHandle = out.GPUHandle;
	}
	void SetGraphicDevice( Value_weak_ptr<GraphicDevice> arg_graphicDevice) override {
		vwp_graphicDevice = arg_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>();
		vwp_heapManager = vwp_graphicDevice.lock()->GetDescriptorHeapManager();
	}
	void PreInitialize() {

	}
	void Attach(const std::uint32_t arg_slotOffset)const override {
		vwp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(arg_slotOffset , gpuDescriptorHandle);
	}
	void Update(void* arg_p_data)override {

		vwp_heapManager.lock()->ConstantBufferUpdate(arg_p_data, m_index, m_size);
	}

private:
	void  UpdateResourceRelease()override {

	}
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHandle;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	Value_weak_ptr<DescriptorHeapManager> vwp_heapManager;
	std::int32_t m_size = 0, m_index = 0;
};
}
}
}
ButiEngine::ButiRendering::CbufferDetail::ICBufferUpdater* ButiEngine::ButiRendering::CbufferDetail::CreateCBufferUpdater(const std::int32_t arg_size)
{
	return ButiMemorySystem::Allocator::allocate<CBufferUpdater_Dx12>(arg_size);
}