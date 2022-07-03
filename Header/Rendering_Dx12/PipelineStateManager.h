#ifndef PIPELINESTATEMANAGER_H
#define PIPELINESTATEMANAGER_H
#include"../RenderingStatus.h"
namespace ButiEngine
{
namespace ButiRendering {
class GraphicDevice_Dx12;
class Resource_Shader;

class PipelineStateManager {
public:
	PipelineStateManager(Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	void Release();
	void ClearPipelineState();
	std::int32_t GetPipelineStatePSOutputCount(const std::string& arg_key);
	Value_ptr<IPiplineState> GetPipelineState(const std::string& arg_key);
	Value_ptr<IPiplineState> GetPipelineState(const DrawSettings& arg_drawSettings, Value_ptr<IRootSignature> arg_vlp_rootSignature, Value_weak_ptr<IResource_Shader> arg_vwp_shader);
private:
	std::unordered_map<std::string, Value_ptr<IPiplineState>> m_map_pipeLineState;
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
};
}
}
#endif // !PIPELINESTATEMANAGER_H
