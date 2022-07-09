#ifndef ROOTSIGNATUREMANAGER_H
#define ROOTSIGNATUREMANAGER_H
#include"../RenderingStatus.h"
#include<string>
#include<vector>
#include<unordered_map>
#include"../DrawSettings.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
struct D3D12_GPU_DESCRIPTOR_HANDLE;
namespace ButiEngine
{
namespace ButiRendering {
class GraphicDevice_Dx12;
class IRootSignature;
class RootSignatureManager {
public:
	RootSignatureManager(Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	void Release();
	Value_ptr<IRootSignature> GetRootSignature(const std::string& arg_key);
	Value_ptr<IRootSignature> GetRootSignature(Value_ptr<IResource_Shader> arg_vlp_shader);
private:
	std::unordered_map<std::string, Value_ptr<IRootSignature>> m_map_rootSignature;
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
};
}
}

#endif // !ROOTSIGNATUREMANAGER_H
