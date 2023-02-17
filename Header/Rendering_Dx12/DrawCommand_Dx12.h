#ifndef DRAWCOMMAND_DX12_H
#define DRAWCOMMAND_DX12_H
#include"../DrawCommand.h"
namespace ButiEngine {
namespace ButiRendering {
class GraphicDevice_Dx12;
class DrawCommand_Dx12 :public IDrawCommand{
public:
	DrawCommand_Dx12(const std::int32_t arg_offset, const std::int32_t arg_step, Value_weak_ptr<IDrawObject> arg_vwp_drawObject,Value_weak_ptr<IResource_Mesh> arg_vwp_mesh, Value_weak_ptr<IResource_Material> arg_vwp_material, Value_ptr < CBuffer<Matrices>> arg_vlp_matrixBuffer, std::vector<Value_ptr<ICBuffer>>& arg_ref_vec_exCBuffer,Value_weak_ptr<IRenderer> arg_vwp_renderer,Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	~DrawCommand_Dx12() {
		std::int32_t i = 0;
	}
	void Execute()override;
	void Execute_afterRendering()override;
	void Initialize()override;
	Value_weak_ptr<IResource_Mesh> GetMesh()const override { return m_vwp_mesh; }
	Value_weak_ptr<IResource_Material> GetMaterial()const override { return m_vwp_material; }
	Value_ptr< CBuffer<Matrices>> GetMatrixCBuffer()const override { return m_vlp_matrixBuffer; }
	std::int32_t GetMeshDrawOffset()override { return m_offset; }
	std::int32_t GetMeshDrawStep() override { return m_step; }
	Value_weak_ptr<IDrawObject> GetDrawObject()const override { return m_vwp_drawObject; }
	bool IsAlpha()const { return m_vwp_material.lock()->IsAlpha(); }
private:
	std::int32_t m_offset, m_step, m_cBufferSlotOffset;
	std::uint32_t m_vertexShaderType;
	Value_weak_ptr<IDrawObject> m_vwp_drawObject;
	Value_weak_ptr<IResource_Mesh> m_vwp_mesh;
	Value_weak_ptr<IResource_Material> m_vwp_material;
	Value_ptr < CBuffer<Matrices>> m_vlp_matrixBuffer;
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
	List<Value_ptr<ICBuffer>> m_list_useExCBuffer;
};
}
}

#endif // !DRAWCOMMAND_DX12_H