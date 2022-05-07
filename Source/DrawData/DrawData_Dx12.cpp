#include "stdafx.h"

#include"Header/GameParts/ResourceContainer.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"../../Header/Rendering_Dx12/PipelineStateManager.h"
#include "../../Header/DrawData/DrawData_Dx12.h"
#include"../../Header/Renderer.h"

ButiEngine::ButiRendering::MeshDrawObject_Dx12::MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const Value_weak_ptr<IResource_Shader>& arg_vwp_shader, const Value_weak_ptr<IResource_Material>& arg_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<DrawInformation> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
	:MeshDrawObject_Dx12(arg_vwp_mesh, arg_vwp_shader, List<Value_weak_ptr<IResource_Material>>{arg_vwp_material}, arg_vlp_renderer, arg_vwp_graphicDevice, arg_vlp_drawInfo, arg_vlp_transform)
{
}

ButiEngine::ButiRendering::MeshDrawObject_Dx12::MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const Value_weak_ptr<IResource_Shader>& arg_vwp_shader, const List<Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<DrawInformation> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
{
	drawData.vlp_renderer = arg_vlp_renderer;
	drawData.SetMesh(arg_vwp_mesh);
	drawData.SetShader(arg_vwp_shader);
	drawData.SetMaterial( arg_list_vwp_material);
	vwp_graphicDevice = arg_vwp_graphicDevice;
	drawData.subset.push_back(arg_vwp_mesh.lock()->GetIndexCount());
	//boxEightCorner = vwp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetMesh(meshTag).lock()->GetBackUpdata().GetBoxEightCorner();
	drawData.vlp_drawInfo = arg_vlp_drawInfo;
	drawData.transform = arg_vlp_transform->GetMatrix();
	drawData.vlp_transform = arg_vlp_transform;
}

ButiEngine::ButiRendering::MeshDrawObject_Dx12::MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const Value_weak_ptr<IResource_Shader>& arg_vwp_shader, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<DrawInformation> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
	:MeshDrawObject_Dx12(arg_vwp_model.lock()->GetMesh(), arg_vwp_shader,arg_vwp_model.lock()->GetMaterial(), arg_vlp_renderer, arg_vwp_graphicDevice, arg_vlp_drawInfo, arg_vlp_transform)
{
	drawData.vlp_drawInfo = arg_vlp_drawInfo;
	drawData.SetModel(arg_vwp_model);
	drawData.subset = arg_vwp_model.lock()->GetSubset();
}

ButiEngine::ButiRendering::MeshDrawObject_Dx12::MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const Value_weak_ptr<IResource_Shader>& arg_vwp_shader, const List<Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<DrawInformation> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
	:MeshDrawObject_Dx12(arg_vwp_model.lock()->GetMesh(), arg_vwp_shader,arg_list_vwp_material, arg_vlp_renderer, arg_vwp_graphicDevice, arg_vlp_drawInfo, arg_vlp_transform)
{
	drawData.vlp_drawInfo = arg_vlp_drawInfo;
	drawData.SetModel(arg_vwp_model);
	drawData.subset = arg_vwp_model.lock()->GetSubset();
}

void ButiEngine::ButiRendering::MeshDrawObject_Dx12::Initialize()
{
	textureRegion = 0;
	auto container = vwp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer();
	for (std::int32_t i = 0; i < drawData.GetMaterial().GetSize(); i++) {
		auto textureCount = drawData.GetMaterial()[i].lock()->GetTextureCount();
		textureRegion = max(textureRegion, textureCount);
	}

	if (drawData.vlp_drawInfo->drawSettings.isShadowMap) {
		textureRegion++;
	}
	//SetInfo_WithoutCommand();
}
void ButiEngine::ButiRendering::MeshDrawObject_Dx12::DrawBefore()
{
	drawData.transform = drawData.vlp_transform->ToMatrix();
}

void ButiEngine::ButiRendering::MeshDrawObject_Dx12::Draw() {
	BufferUpdate();
	CommandSet();
}




void ButiEngine::ButiRendering::DrawObject_Dx12::Initialize(const std::uint32_t srvCount)
{

	for (std::int32_t i = 0; i < drawData.vlp_drawInfo->drawSettings.vec_samplerMode.size(); i++) {
		vec_samplerBufferDescriptorHandle.push_back(vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetSamplerHandle((std::int32_t)drawData.vlp_drawInfo->drawSettings.vec_samplerMode[i]).GPUHandle);
	}
	CreatePipeLineState(drawData.vlp_drawInfo->vec_exCBuffer.size()+1, srvCount);

	//デスクプリタヒープのハンドルの配列の作成

	for (auto itr = drawData.vlp_drawInfo->vec_exCBuffer.begin(); itr != drawData.vlp_drawInfo->vec_exCBuffer.end(); itr++) {
		(*itr)->SetGraphicDevice(vwp_graphicDevice.lock());
		(*itr)->CreateBuffer();
	}



	auto cbuffer_Dx12 = ObjectFactory::Create<CBuffer_Dx12<ShaderVariable>>(0);

	cbuffer_Dx12->SetGraphicDevice(vwp_graphicDevice.lock());

	cbuffer_Dx12->CreateBuffer();

	Matrix4x4 mat;
	cbuffer_Dx12->Get().World = mat;

	drawData.SetCbuffer( cbuffer_Dx12);



	auto billBoard = drawData.vlp_drawInfo->drawSettings.billboardMode;

	switch (billBoard)
	{
	case BillBoardMode::full:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_billBoard;
		break;
	case BillBoardMode::x:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_billBoardX;
		break;
	case BillBoardMode::y:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_billBoardY;
		break;
	case BillBoardMode::z:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_billBoardZ;
		break;	
	case BillBoardMode::none:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_default;
		break;
	default:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_default;
		break;
	}

	auto drawFix = drawData.vlp_drawInfo->drawSettings.drawFixParam;

	switch (drawFix)
	{
	case DrawFixParam::removeDecimalPart:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_RemoveDecimalPart;
		break;
	case DrawFixParam::removeDecimalPartXY:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_RemoveDecimalPartXY;
		break;
	case DrawFixParam::removeDecimalPartYZ:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_RemoveDecimalPartYZ;
		break;
	case DrawFixParam::removeDecimalPartXZ:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_RemoveDecimalPartXZ;
		break;
	case DrawFixParam::removeRotation:
		p_matrixUpdateFunc = &DrawObject_Dx12::MatrixUpdater_NoRotation;
		break;
	default:
		break;
	}

}

void ButiEngine::ButiRendering::DrawObject_Dx12::CreatePipeLineState(const std::uint32_t arg_exCBuffer, const std::uint32_t srvCount)
{
	if (arg_exCBuffer <= drawData.GetCBufferCount())
		return;

	drawData.SetCBufferCount(arg_exCBuffer);
	ZeroMemory(&pipeLineDesc, sizeof(pipeLineDesc));

	rootSignature =vwp_graphicDevice.lock()->CreateSrvSmpCbvMat(drawData.GetCBufferCount(), srvCount,vec_samplerBufferDescriptorHandle.size() ,rootdesc);
	
	rasterizerStateDesc =RasterizeDescHelper::GetDefaultRasterizerDesc();

	rasterizerStateDesc.CullMode = (D3D12_CULL_MODE)drawData.vlp_drawInfo->drawSettings.cullMode;
	rasterizerStateDesc.FillMode =(D3D12_FILL_MODE)drawData.vlp_drawInfo->drawSettings.isFill;
	//rasterizerStateDesc.MultisampleEnable = true;
	auto pipeLineTopology = drawData.vlp_drawInfo->drawSettings.topologyType;
	if (pipeLineTopology == TopologyType::triangleList) {
		pipeLineTopology = TopologyType::triangle;
	}else if (pipeLineTopology == TopologyType::pointList) {
		pipeLineTopology = TopologyType::point;
	}
	pipelineState = vwp_graphicDevice.lock()->GetPipelineStateManager().GetPipelineState(rootSignature, rootdesc, pipeLineDesc, rasterizerStateDesc, drawData.GetShader().lock(), drawData.vlp_drawInfo->drawSettings.blendMode,pipeLineTopology,drawData.vlp_drawInfo->isDepth );

}

void ButiEngine::ButiRendering::DrawObject_Dx12::CommandExecute()
{
	vwp_graphicDevice.lock()->GetCommandList().ExecuteBundle(commandList.Get());
}

void ButiEngine::ButiRendering::DrawObject_Dx12::BufferUpdate()
{
	(this->*p_matrixUpdateFunc)();

	drawData.GetCBuffer()->Get().View = vwp_graphicDevice.lock()->GetCameraViewMatrix();
	drawData.GetCBuffer()->Get().Projection = vwp_graphicDevice.lock()->GetProjectionMatrix();
	drawData.GetCBuffer()->Get().MVP = drawData.GetCBuffer()->Get().Projection * drawData.GetCBuffer()->Get().View * drawData.GetCBuffer()->Get().World;

	drawData.GetCBuffer()->Update();


	for (auto itr =drawData.vlp_drawInfo-> vec_exCBuffer.begin(); itr !=drawData.vlp_drawInfo-> vec_exCBuffer.end(); itr++) {
		(*itr)->Update();
	}
}

void ButiEngine::ButiRendering::DrawObject_Dx12::SetInfo()
{
	if (!IsCreated()) {
		return;
	}
	commandList = CommandListHelper::CreateBundleCommandList(vwp_graphicDevice.lock()->GetDevice(), vwp_graphicDevice.lock()->GetBundleCommandAllocator());
	CommandListHelper::Close(commandList);

	CommandListHelper::BundleReset(nullptr, commandList,vwp_graphicDevice.lock()->GetBundleCommandAllocator());
	ID3D12DescriptorHeap* ppHeaps[] = { vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetDescriptorHeap().Get() ,vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetSamplerHeap().Get() };
	auto heapCount = _countof(ppHeaps);
	commandList->SetDescriptorHeaps(heapCount, ppHeaps);

	vwp_graphicDevice.lock()->SetCommandList(commandList.Get());
	vwp_graphicDevice.lock()->SetPipeLine(pipelineState);
	CommandSet();
	vwp_graphicDevice.lock()->UnSetCommandList();

	CommandListHelper::Close(commandList);
}

void ButiEngine::ButiRendering::DrawObject_Dx12::SetInfo_WithoutCommand()
{
	if (!(drawData.vlp_drawInfo->IsContainExCBuffer("FogParameter"))) {
		drawData.vlp_drawInfo->vec_exCBuffer.push_back(drawData. vlp_renderer->GetRendererCBuffer());
	}
	Initialize(textureRegion);
	drawData.vlp_drawInfo->RemoveExCBuffer("FogParameter");
}

void ButiEngine::ButiRendering::DrawObject_Dx12::CommandSet()
{
	if (!IsCreated()) {
		return;
	}
	auto vlp_resourseContainer = drawData.vlp_renderer->GetResourceContainer();

	vwp_graphicDevice.lock()->SetPipeLine(pipelineState);

	vwp_graphicDevice.lock()->GetCommandList().SetGraphicsRootSignature(rootSignature.Get());

	std::int32_t samplerRegion = vec_samplerBufferDescriptorHandle.size();
	for (std::int32_t i = 0; i < samplerRegion; i++) {
		vwp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(textureRegion+i, vec_samplerBufferDescriptorHandle[i]);
	}

	drawData.GetCBuffer()->Attach(samplerRegion + textureRegion);

	for (auto itr = drawData.vlp_drawInfo->vec_exCBuffer.begin(); itr != drawData.vlp_drawInfo->vec_exCBuffer.end(); itr++) {
		(*itr)->Attach(samplerRegion + textureRegion);
	}
	drawData.vlp_renderer->GetRendererCBuffer()->Attach(samplerRegion + textureRegion);
	if (drawData.vlp_drawInfo->drawSettings.isShadowMap&&textureRegion) {
		vlp_resourseContainer->GetTexture( drawData.vlp_renderer->GetShadowTexture(drawData.vlp_drawInfo->layer) ).lock()->Attach( textureRegion - 1);
	}
	auto vertexType=drawData.GetShader().lock()->GetVertexShader().lock()->GetInputVertexType();
	drawData.GetMesh().lock()->Draw(vertexType);

	vwp_graphicDevice.lock()->GetCommandList().IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)drawData.vlp_drawInfo->drawSettings.topologyType);
	auto spliteCount = drawData.subset.size();

	std::uint32_t offset = 0;
	if (drawData.GetMaterial().GetSize() == drawData.subset.size()) {
		for (std::int32_t i = 0; i < drawData.subset.size(); i++) {
			drawData.GetMaterial()[i].lock()->Attach(samplerRegion + textureRegion, drawData.vlp_renderer);
			std::uint32_t count = drawData.subset.at(i);
			vwp_graphicDevice.lock()->GetCommandList().DrawIndexedInstanced(count, 1, offset, 0, 0);

			offset += count;
		}
	}
	else {
		for (std::int32_t i = 0; i < drawData.subset.size(); i++) {
			drawData.GetMaterial()[i].lock()->Attach(samplerRegion + textureRegion, drawData.vlp_renderer);
			std::uint32_t count = drawData.subset.at(i);
			vwp_graphicDevice.lock()->GetCommandList().DrawIndexedInstanced(count, 1, offset, 0, 0);

			offset += count;
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::ICBuffer> ButiEngine::ButiRendering::DrawObject_Dx12::AddICBuffer(Value_ptr<ICBuffer> arg_cbuffer)
{
	CreatePipeLineState(arg_cbuffer->GetSlot(),0);
	auto output= drawData.AddICBuffer(arg_cbuffer);
	SetInfo();
	return output;
}
bool ButiEngine::ButiRendering::DrawObject_Dx12::IsCreated()
{
	return pipelineState;
}
void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_NoRotation()
{
	drawData.GetCBuffer()->Get().World = drawData.transform.GetOnlyRotation_transpose();
}
void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_RemoveDecimalPartXY()
{
	drawData.GetCBuffer()->Get().World = drawData.transform.GetPositionXYFloor_transpose();
}
void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_RemoveDecimalPartYZ()
{
	drawData.GetCBuffer()->Get().World = drawData.transform.GetPositionYZFloor_transpose();
}
void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_RemoveDecimalPartXZ()
{
	drawData.GetCBuffer()->Get().World = drawData.transform.GetPositionXZFloor_transpose();
}
void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_default()
{
	drawData.GetCBuffer()->Get().World = drawData.transform;
}

void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_billBoard()
{
	drawData.GetCBuffer()->Get().World = drawData.transform * vwp_graphicDevice.lock()->GetViewMatrixBillBoard();

}

void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_billBoardX()
{
	drawData.GetCBuffer()->Get().World = drawData.transform * vwp_graphicDevice.lock()->GetViewMatrixBillBoardX();

}

void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_billBoardY()
{
	drawData.GetCBuffer()->Get().World = drawData.transform * vwp_graphicDevice.lock()->GetViewMatrixBillBoardY();
}

void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_billBoardZ()
{
	drawData.GetCBuffer()->Get().World = drawData.transform * vwp_graphicDevice.lock()->GetViewMatrixBillBoardZ();
}

void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_RemoveDecimalPart()
{
	drawData.GetCBuffer()->Get().World = drawData.transform.GetPositionFloor_transpose();
}