#include "stdafx.h"
#include"Header/Common/CollisionPrimitive.h"
#include "../../Header/DrawData/IDrawData.h"
#include"../../Header/Resource_RealTimeMesh.h"
void ButiEngine::ButiRendering::DrawData::SetBlendMode(const BlendMode& arg_BlendMode)
{
}

void ButiEngine::ButiRendering::DrawData::MatrixUpdate()
{
}

ButiEngine::Value_ptr<ButiEngine::Collision::CollisionPrimitive_Box_AABB> ButiEngine::ButiRendering::DrawObject::GetMeshAABB()
{
    const ButiRendering::BoxEightCorner& box =drawData.GetMesh().lock()->GetBackUpdata(Vertex::VertexFlag::Normal| Vertex::VertexFlag::UV)->GetBoxEightCorner();
    Vector3 length = box.up_right_front - box.down_left_back;
    auto output = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(length, drawData.vlp_transform);

    return output;
}

ButiEngine::Value_ptr<ButiEngine::Collision::CollisionPrimitive_Box_OBB> ButiEngine::ButiRendering::DrawObject::GetMeshOBB()
{
    if (drawData.GetMesh().lock()->IsThis<Resource_RealTimeMesh>())
    {
        return ObjectFactory::Create<Collision::CollisionPrimitive_Box_OBB>(Vector3Const::Zero, drawData.vlp_transform);
    }
    const ButiRendering::BoxEightCorner& box = drawData.GetMesh().lock()->GetBackUpdata(Vertex::VertexFlag::Normal| Vertex::VertexFlag::UV)->GetBoxEightCorner();
    Vector3 length = box.up_right_front - box.down_left_back;
    auto output = ObjectFactory::Create<Collision::CollisionPrimitive_Box_OBB>(length, drawData.vlp_transform);

    return output;
}

void ButiEngine::ButiRendering::DrawObject::SetPrimitive(ButiEngine::Value_ptr<Collision::CollisionPrimitive> arg_prim)
{
    drawData.SetPrimitive( arg_prim);
}

void ButiEngine::ButiRendering::DrawObject::SetOctRegistPtr(std::uint32_t* arg_ptr)
{
    drawData.SetCollisionIndex(arg_ptr);
}

ButiEngine::Value_ptr<ButiEngine::Collision::CollisionPrimitive> ButiEngine::ButiRendering::DrawObject::GetPrimitive()
{
    return drawData.GetPrimitive();
}

std::uint32_t* ButiEngine::ButiRendering::DrawObject::GetOctRegistPtr()
{
    return drawData.GetCollisionIndex();
}


void ButiEngine::ButiRendering::DualDrawObject::Initialize()
{
}

void ButiEngine::ButiRendering::DualDrawObject::PreInitialize()
{
}

void ButiEngine::ButiRendering::DualDrawObject::Draw()
{
    vlp_befDrawObj->Draw();
    vlp_afterDrawObj->Draw();
}

void ButiEngine::ButiRendering::DualDrawObject::DrawBefore()
{
    vlp_befDrawObj->DrawBefore();
    vlp_afterDrawObj->DrawBefore();
}

float ButiEngine::ButiRendering::DualDrawObject::GetZ(const Matrix4x4& arg_vpMatrix)
{
    return vlp_befDrawObj->GetZ(arg_vpMatrix);
}

void ButiEngine::ButiRendering::DualDrawObject::SetInfo()
{
    vlp_befDrawObj->SetInfo();
    vlp_afterDrawObj->SetInfo();
}

void ButiEngine::ButiRendering::DualDrawObject::SetInfo_WithoutCommand()
{
    vlp_befDrawObj->SetInfo_WithoutCommand();
    vlp_afterDrawObj->SetInfo_WithoutCommand();
}

void ButiEngine::ButiRendering::DualDrawObject::BufferUpdate()
{
    vlp_befDrawObj->BufferUpdate();
    vlp_afterDrawObj->BufferUpdate();
}

void ButiEngine::ButiRendering::DualDrawObject::CommandSet()
{
    vlp_befDrawObj->CommandSet();
    vlp_afterDrawObj->CommandSet();
}

void ButiEngine::ButiRendering::DualDrawObject::CommandExecute()
{
    vlp_befDrawObj->CommandExecute();
    vlp_afterDrawObj->CommandExecute();
}

void ButiEngine::ButiRendering::DualDrawObject::SetTransform(const Value_ptr<Transform>& arg_transform)
{
    vlp_afterDrawObj->SetTransform(arg_transform);
    vlp_befDrawObj->SetTransform(arg_transform);
}

ButiEngine::Value_ptr<ButiEngine::Collision::CollisionPrimitive_Box_AABB> ButiEngine::ButiRendering::DualDrawObject::GetMeshAABB()
{
    return vlp_befDrawObj->GetMeshAABB();
}

ButiEngine::Value_ptr<ButiEngine::Collision::CollisionPrimitive_Box_OBB> ButiEngine::ButiRendering::DualDrawObject::GetMeshOBB()
{
    return vlp_befDrawObj->GetMeshOBB();
}

void ButiEngine::ButiRendering::DualDrawObject::SetPrimitive(Value_ptr<Collision::CollisionPrimitive> arg_prim)
{
    vlp_befDrawObj->SetPrimitive(arg_prim);
}

void ButiEngine::ButiRendering::DualDrawObject::SetOctRegistPtr(std::uint32_t* arg_ptr)
{
    vlp_befDrawObj->SetOctRegistPtr(arg_ptr);
}

ButiEngine::Value_ptr<ButiEngine::Collision::CollisionPrimitive> ButiEngine::ButiRendering::DualDrawObject::GetPrimitive()
{
    return vlp_befDrawObj->GetPrimitive();
}

std::uint32_t* ButiEngine::ButiRendering::DualDrawObject::GetOctRegistPtr()
{
    return vlp_befDrawObj->GetOctRegistPtr();
}

