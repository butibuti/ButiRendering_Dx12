#include "stdafx.h"
#include"ButiEngineHeader/Header/Common/CollisionPrimitive.h"
#include"ButiRendering_Dx12/Header/DrawData/IDrawData.h"
#include"ButiRendering_Dx12/Header/Resource_RealTimeMesh.h"
void ButiEngine::ButiRendering::DrawData::SetBlendMode(const BlendMode& arg_BlendMode)
{
}

void ButiEngine::ButiRendering::DrawData::MatrixUpdate()
{
}

void ButiEngine::ButiRendering::DrawObject::SetOctRegistPtr(std::uint32_t* arg_ptr)
{
    drawData.SetCollisionIndex(arg_ptr);
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


void ButiEngine::ButiRendering::DualDrawObject::SetOctRegistPtr(std::uint32_t* arg_ptr)
{
    vlp_befDrawObj->SetOctRegistPtr(arg_ptr);
}


std::uint32_t* ButiEngine::ButiRendering::DualDrawObject::GetOctRegistPtr()
{
    return vlp_befDrawObj->GetOctRegistPtr();
}

