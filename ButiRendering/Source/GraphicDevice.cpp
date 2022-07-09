#pragma once
#include"stdafx.h"
#include"ButiRendering_Dx12/Header/GraphicDevice.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"

ButiEngine::Matrix4x4 ButiEngine::ButiRendering::GraphicDevice::GetProjectionMatrix()
{
	return projectionMatrix;
}


ButiEngine::Matrix4x4 ButiEngine::ButiRendering::GraphicDevice::GetCameraViewMatrix()
{
	return viewMatrix;
}

const ButiEngine::Vector3& ButiEngine::ButiRendering::GraphicDevice::GetCameraPos()
{
	return cameraPos;
}

void ButiEngine::ButiRendering::GraphicDevice::SetCameraPos(const Vector3& arg_pos)
{
	cameraPos = arg_pos;
}

ButiEngine::Matrix4x4 ButiEngine::ButiRendering::GraphicDevice::GetRawViewMatrix()
{
	return rawViewMatrix;
}


void ButiEngine::ButiRendering::GraphicDevice::SetProjectionMatrix(const Matrix4x4 & arg_projectionMatrix)
{
	projectionMatrix = arg_projectionMatrix;
}


void ButiEngine::ButiRendering::GraphicDevice::SetViewMatrix(const Matrix4x4 & arg_viewMatrix)
{
	viewMatrix = arg_viewMatrix;
}

void ButiEngine::ButiRendering::GraphicDevice::SetRawViewMatrix(const Matrix4x4& arg_viewMatrix)
{
	rawViewMatrix = arg_viewMatrix;
}

void ButiEngine::ButiRendering::GraphicDevice::SetViewMatrix_billBoard(const Matrix4x4& arg_viewMatrix)
{
	viewMatrix_billBoard = arg_viewMatrix;
}

void ButiEngine::ButiRendering::GraphicDevice::SetViewMatrix_billBoardX(const Matrix4x4& arg_viewMatrix)
{
	viewMatrix_billBoardX = arg_viewMatrix;
}

void ButiEngine::ButiRendering::GraphicDevice::SetViewMatrix_billBoardY(const Matrix4x4& arg_viewMatrix)
{
	viewMatrix_billBoardY = arg_viewMatrix;
}

void ButiEngine::ButiRendering::GraphicDevice::SetViewMatrix_billBoardZ(const Matrix4x4& arg_viewMatrix)
{
	viewMatrix_billBoardZ = arg_viewMatrix;
}

void ButiEngine::ButiRendering::GraphicDevice::SetMesh(Value_ptr<IResource_Mesh> arg_vlp_mesh, std::int32_t arg_currentMeshVertexType)
{
	if (m_vlp_currentMesh == arg_vlp_mesh&&m_currentMeshVertexType==arg_currentMeshVertexType) {
		return;
	}
	m_vlp_currentMesh = arg_vlp_mesh;
	m_currentMeshVertexType = arg_currentMeshVertexType;
	if (!m_vlp_currentMesh) {
		return;
	}
	m_vlp_currentMesh->Draw(m_currentMeshVertexType);
}

void ButiEngine::ButiRendering::GraphicDevice::SetMaterial(Value_ptr<IResource_Material> arg_vlp_material)
{
	if (m_vlp_currentMaterial == arg_vlp_material) {
		return;
	}
	m_vlp_currentMaterial = arg_vlp_material;

	if (!m_vlp_currentMaterial) {
		return;
	}
	m_vlp_currentMaterial->Attach();
}

void ButiEngine::ButiRendering::GraphicDevice::SetTexture(Value_ptr<IResource_Texture> arg_vlp_texture, const std::int32_t arg_textureRegisterIndex)
{
	if (m_list_currentTexture.GetSize() <= arg_textureRegisterIndex) {
		for (std::int32_t additionIndex = 0; m_list_currentTexture.GetSize() <= arg_textureRegisterIndex; additionIndex++) {
			
			if (m_list_currentTexture.GetSize() == arg_textureRegisterIndex) {
				m_list_currentTexture.Add(arg_vlp_texture);
			}
			else {
				m_list_currentTexture.Add(nullptr);
			}
		}
	}
	else {
		if (m_list_currentTexture[arg_textureRegisterIndex] == arg_vlp_texture) {
			return;
		}
	}
	arg_vlp_texture->Attach(arg_textureRegisterIndex);
}

void ButiEngine::ButiRendering::GraphicDevice::SetConstantBuffer(Value_ptr<ICBuffer> arg_vlp_cBuffer, const std::int32_t arg_cBufferRegisterIndex)
{
	if (m_map_currentConstancBuffer.count(arg_cBufferRegisterIndex)) {
		if (m_map_currentConstancBuffer.at(arg_cBufferRegisterIndex) == arg_vlp_cBuffer) { return; }

		m_map_currentConstancBuffer.at(arg_cBufferRegisterIndex) = arg_vlp_cBuffer;
	}
	else {
		m_map_currentConstancBuffer.emplace(arg_cBufferRegisterIndex, arg_vlp_cBuffer);
	}
	
	arg_vlp_cBuffer->Attach(arg_cBufferRegisterIndex);

}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::GraphicDevice> ButiEngine::ButiRendering::CreateGraphicDevice(const bool arg_isWindowApp)
{
	return CreateGraphicDeviceDx12(arg_isWindowApp);
}