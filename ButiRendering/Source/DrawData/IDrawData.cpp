#include "stdafx.h"
#include"ButiEngineHeader/Header/Common/CollisionPrimitive.h"
#include"ButiRendering_Dx12/Header/DrawData/IDrawData.h"
#include"ButiRendering_Dx12/Header/Resource_DynamicMesh.h"

void ButiEngine::ButiRendering::DualDrawObject::Initialize()
{
    m_list_command.Add(m_vlp_befDrawObj->GetCommands());
    m_list_command.Add(m_vlp_afterDrawObj->GetCommands());
}

void ButiEngine::ButiRendering::DualDrawObject::Draw()
{
    m_vlp_befDrawObj->Draw();
    m_vlp_afterDrawObj->Draw();
}

void ButiEngine::ButiRendering::DualDrawObject::DrawBefore()
{
    m_vlp_befDrawObj->DrawBefore();
    m_vlp_afterDrawObj->DrawBefore();
}

float ButiEngine::ButiRendering::DualDrawObject::GetZ(const Matrix4x4& arg_vpMatrix)
{
    return m_vlp_befDrawObj->GetZ(arg_vpMatrix);
}

void ButiEngine::ButiRendering::DualDrawObject::SetInfo()
{
    m_vlp_befDrawObj->SetInfo();
    m_vlp_afterDrawObj->SetInfo();
}

void ButiEngine::ButiRendering::DualDrawObject::SetInfo_WithoutCommand()
{
    m_vlp_befDrawObj->SetInfo_WithoutCommand();
    m_vlp_afterDrawObj->SetInfo_WithoutCommand();
}

void ButiEngine::ButiRendering::DualDrawObject::BufferUpdate()
{
    m_vlp_befDrawObj->BufferUpdate();
    m_vlp_afterDrawObj->BufferUpdate();
}

void ButiEngine::ButiRendering::DualDrawObject::CommandSet()
{
    m_vlp_befDrawObj->CommandSet();
    m_vlp_afterDrawObj->CommandSet();
}

void ButiEngine::ButiRendering::DualDrawObject::CommandExecute()
{
    m_vlp_befDrawObj->CommandExecute();
    m_vlp_afterDrawObj->CommandExecute();
}

void ButiEngine::ButiRendering::DualDrawObject::SetTransform(Value_ptr<Transform> arg_transform)
{
    m_vlp_afterDrawObj->SetTransform(arg_transform);
    m_vlp_befDrawObj->SetTransform(arg_transform);
}



