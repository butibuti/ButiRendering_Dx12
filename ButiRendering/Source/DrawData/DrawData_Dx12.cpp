#include "stdafx.h"

#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiRendering_Dx12/Header/DrawData/DrawData_Dx12.h"
#include"ButiRendering_Dx12/Header/Renderer.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DrawCommand_Dx12.h"
ButiEngine::ButiRendering::MeshDrawObject_Dx12::MeshDrawObject_Dx12(const Value_ptr<IResource_Mesh>& arg_vwp_mesh,  const Value_ptr<IResource_Material>& arg_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
	:MeshDrawObject_Dx12(arg_vwp_mesh,  List<Value_ptr<IResource_Material>>{arg_vwp_material}, arg_vlp_renderer, arg_vwp_graphicDevice, arg_vlp_drawInfo, arg_vlp_transform)
{
}

ButiEngine::ButiRendering::MeshDrawObject_Dx12::MeshDrawObject_Dx12(const Value_ptr<IResource_Mesh>& arg_vwp_mesh, const List<Value_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
	: DrawObject_Dx12(DrawData(arg_vlp_transform))
{
	drawData.vlp_renderer = arg_vlp_renderer;
	drawData.SetMesh(arg_vwp_mesh);
	drawData.SetMaterial( arg_list_vwp_material);
	vwp_graphicDevice = arg_vwp_graphicDevice;
	drawData.subset.push_back(arg_vwp_mesh->GetIndexCount());
	drawData.vlp_drawInfo = arg_vlp_drawInfo;
}

ButiEngine::ButiRendering::MeshDrawObject_Dx12::MeshDrawObject_Dx12(const Value_ptr<IResource_Model>& arg_vwp_model, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
	:MeshDrawObject_Dx12(arg_vwp_model->GetMesh(), arg_vwp_model->GetMaterial(), arg_vlp_renderer, arg_vwp_graphicDevice, arg_vlp_drawInfo, arg_vlp_transform)
{
	drawData.vlp_drawInfo = arg_vlp_drawInfo;
	drawData.SetModel(arg_vwp_model);
	drawData.subset = arg_vwp_model->GetSubset();
}

ButiEngine::ButiRendering::MeshDrawObject_Dx12::MeshDrawObject_Dx12(const Value_ptr<IResource_Model>& arg_vwp_model, const List<Value_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
	:MeshDrawObject_Dx12(arg_vwp_model->GetMesh(), arg_list_vwp_material, arg_vlp_renderer, arg_vwp_graphicDevice, arg_vlp_drawInfo, arg_vlp_transform)
{
	drawData.vlp_drawInfo = arg_vlp_drawInfo;
	drawData.SetModel(arg_vwp_model);
	drawData.subset = arg_vwp_model->GetSubset();
}

void ButiEngine::ButiRendering::MeshDrawObject_Dx12::DrawBefore()
{
	drawData.transform = drawData.vlp_transform->ToMatrix();
}

void ButiEngine::ButiRendering::MeshDrawObject_Dx12::Draw() {
	BufferUpdate();
	CommandSet();
}




void ButiEngine::ButiRendering::DrawObject_Dx12::Initialize()
{
	if (!drawData.vlp_drawInfo) {
		drawData.vlp_drawInfo = ObjectFactory::Create<ObjectDrawData>();
	}

	auto cbuffer_Dx12 = CreateCBuffer<Matrices>();
	cbuffer_Dx12->SetGraphicDevice(Value_weak_ptr<GraphicDevice>( vwp_graphicDevice));
	cbuffer_Dx12->CreateBuffer();
	drawData.SetCbuffer( cbuffer_Dx12);

	List<ConstantBufferReflection> list_cbRef;
	bool isUseRenderState = false;
	for (auto material : drawData.GetMaterial()) {
		list_cbRef.Add_noDuplicate(material->GetShader()->GetConstantBufferReflectionDatas());
	}
	for (auto cbRef : list_cbRef) {
		if (cbRef.m_bufferName == "RendererState") {
			isUseRenderState = true;
		}
		if (GetICBuffer(cbRef.m_bufferName)) {
			continue;
		}
		if (cbRef.m_bufferName == "ObjectInformation") {
			drawData.vlp_drawInfo->vec_exCBuffer.push_back(CreateCBuffer<ObjectInformation>());
		}
		else if (cbRef.m_bufferName == "GausParameter") {
			drawData.vlp_drawInfo->vec_exCBuffer.push_back(CreateCBuffer<GausParameter>());
		}
		else if (cbRef.m_bufferName == "ParticleParameter") {
			drawData.vlp_drawInfo->vec_exCBuffer.push_back(CreateCBuffer<ParticleParameter>());
		}
		else if (cbRef.m_bufferName == "Bone") {
			drawData.vlp_drawInfo->vec_exCBuffer.push_back(CreateCBuffer<BoneMatrix>());
		}
	}

	for (auto itr : drawData.vlp_drawInfo->vec_exCBuffer) {
		itr->SetGraphicDevice(vwp_graphicDevice.lock());
		itr->CreateBuffer();
	}
	if (isUseRenderState) {
		drawData.RemoveCBuffer<RendererState>();
		drawData.vlp_drawInfo->vec_exCBuffer.push_back(IRenderer::GetRendererStateCBuffer());
	}
	switch (drawData.vlp_drawInfo->billboardMode)
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
	switch (drawData.vlp_drawInfo->drawFixParam)
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
	std::int32_t drawOffsets = 0, index = 0;
	for (auto drawStep : drawData.subset) {
		m_list_command.Add(ObjectFactory::Create<DrawCommand_Dx12>(drawOffsets, drawStep, GetThis<IDrawObject>(), drawData.GetMesh(), drawData.GetMaterial()[index], drawData.GetCBuffer(), drawData.vlp_drawInfo->vec_exCBuffer, drawData.vlp_renderer, vwp_graphicDevice));
		drawOffsets += drawStep;
		if (drawData.subset.size() == drawData.GetMaterial().GetSize()) {
			index++;
		}
	}
}

void ButiEngine::ButiRendering::DrawObject_Dx12::CommandExecute()
{}

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
}

bool ButiEngine::ButiRendering::DrawObject_Dx12::IsCreated()
{
	assert(0);
	return true;
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
	drawData.GetCBuffer()->Get().World = drawData.transform* vwp_graphicDevice.lock()->GetViewMatrixBillBoard();

}

void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_billBoardX()
{
	drawData.GetCBuffer()->Get().World = vwp_graphicDevice.lock()->GetViewMatrixBillBoardX()* drawData.transform;

}

void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_billBoardY()
{
	drawData.GetCBuffer()->Get().World = vwp_graphicDevice.lock()->GetViewMatrixBillBoardY()* drawData.transform;
}

void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_billBoardZ()
{
	drawData.GetCBuffer()->Get().World = vwp_graphicDevice.lock()->GetViewMatrixBillBoardZ()* drawData.transform;
}

void ButiEngine::ButiRendering::DrawObject_Dx12::MatrixUpdater_RemoveDecimalPart()
{
	drawData.GetCBuffer()->Get().World = drawData.transform.GetPositionFloor_transpose();
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IDrawObject> ButiEngine::ButiRendering::CreateMeshDrawObject(const Value_ptr<IResource_Mesh>& arg_vwp_mesh,  const Value_ptr<IResource_Material>& arg_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
{
	return ObjectFactory::Create<MeshDrawObject_Dx12>(arg_vwp_mesh,arg_vwp_material, arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>(),arg_vlp_drawInfo,  arg_vlp_transform);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IDrawObject> ButiEngine::ButiRendering::CreateMeshDrawObject(const Value_ptr<IResource_Mesh>& arg_vwp_mesh,  const List<Value_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
{
	return ObjectFactory::Create<MeshDrawObject_Dx12>(arg_vwp_mesh, arg_list_vwp_material, arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>(), arg_vlp_drawInfo, arg_vlp_transform);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IDrawObject> ButiEngine::ButiRendering::CreateMeshDrawObject(const Value_ptr<IResource_Model>& arg_vwp_model,  Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
{
	return ObjectFactory::Create<MeshDrawObject_Dx12>(arg_vwp_model, arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>(), arg_vlp_drawInfo, arg_vlp_transform);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IDrawObject> ButiEngine::ButiRendering::CreateMeshDrawObject(const Value_ptr<IResource_Model>& arg_vwp_model,const List<Value_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform)
{
	return ObjectFactory::Create<MeshDrawObject_Dx12>(arg_vwp_model, arg_list_vwp_material,arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>(), arg_vlp_drawInfo, arg_vlp_transform);
}
