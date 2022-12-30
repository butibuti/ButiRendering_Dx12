#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Resource_Mesh.h"
#include"ButiRendering_Dx12/Header/MeshPrimitive.h"

void ButiEngine::ButiRendering::Resource_Mesh::SetBackupData(Value_ptr<ButiRendering::MeshPrimitiveBase> arg_vlp_meshPrimitive)
{
	if (map_vlp_meshPrimitive.count(arg_vlp_meshPrimitive->GetVertexType())) {
		assert(0 && "“o˜^Ï‚Ý‚Ì’¸“_Œ^‚ÌƒvƒŠƒ~ƒeƒBƒu‚Å‚·");
		return;
	}
	else {
		map_vlp_meshPrimitive.emplace(arg_vlp_meshPrimitive->GetVertexType(), arg_vlp_meshPrimitive);
	}
}

std::uint32_t ButiEngine::ButiRendering::Resource_Mesh::GetVertexCount()
{
	return verticesCount;
}

std::uint32_t ButiEngine::ButiRendering::Resource_Mesh::GetIndexCount()
{
	return indexCount;
}

bool ButiEngine::ButiRendering::Resource_Mesh::GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line)
{
	return false;//map_vlp_meshPrimitive.at(Vertex::VertexFlag::UV| Vertex::VertexFlag::Normal)->IsHitRay(arg_line, arg_p_pos, arg_p_normal);
}

const ButiEngine::ButiRendering::MeshPrimitiveBase* ButiEngine::ButiRendering::Resource_Mesh::GetBackUpdata(std::uint32_t arg_vertexType) const
{
	if (map_vlp_meshPrimitive.count(arg_vertexType)) {
		return map_vlp_meshPrimitive.at(arg_vertexType).get();
	}
	return map_vlp_meshPrimitive.begin()->second.get();
}

const std::string& ButiEngine::ButiRendering::Resource_Mesh::GetTagName() const
{
	return tagName;
}

void ButiEngine::ButiRendering::Resource_Mesh::SetTagName(const std::string& arg_tagName)
{
	tagName = arg_tagName;
}
