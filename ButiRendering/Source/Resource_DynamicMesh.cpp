#include"stdafx.h"
#include"ButiRendering_Dx12/Header/MeshPrimitive.h"
#include"ButiRendering_Dx12/Header/Resource_DynamicMesh.h"


void ButiEngine::ButiRendering::Resource_DynamicMesh::SetBackupData(Value_ptr<ButiRendering::MeshPrimitiveBase> arg_vlp_meshPrimitive)
{
    vlp_meshPrimitive = arg_vlp_meshPrimitive;
    vertexType = vlp_meshPrimitive->GetVertexType();
}

std::uint32_t ButiEngine::ButiRendering::Resource_DynamicMesh::GetVertexCount()
{
    return verticesCount;
}

std::uint32_t ButiEngine::ButiRendering::Resource_DynamicMesh::GetIndexCount()
{
    return indexCount;
}


const ButiEngine::ButiRendering::MeshPrimitiveBase* ButiEngine::ButiRendering::Resource_DynamicMesh::GetBackUpdata(std::uint32_t arg_vertexType) const
{
    if (vertexType != arg_vertexType) {
        assert(0&&"àŸÇ»ÇÈí∏ì_å^ÇÃóvãÅÇ≈Ç∑");
        return nullptr;
    }
    return vlp_meshPrimitive.get();
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::MeshPrimitiveBase> ButiEngine::ButiRendering::Resource_DynamicMesh::GetBackupData_Row()
{
    return vlp_meshPrimitive;
}

bool ButiEngine::ButiRendering::Resource_DynamicMesh::GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line)
{
	return false;// vlp_meshPrimitive->IsHitRay(arg_line, arg_p_pos, arg_p_normal);
}

const std::string& ButiEngine::ButiRendering::Resource_DynamicMesh::GetTagName() const
{
    return tagName;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh::SetTagName(const std::string& arg_tagName)
{
    tagName = arg_tagName;
}

