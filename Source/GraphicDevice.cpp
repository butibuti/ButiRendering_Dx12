#pragma once
#include"stdafx.h"
#include"ButiRendering_Dx12/Header/GraphicDevice.h"


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

ButiEngine::Value_weak_ptr<ButiEngine::IApplication> ButiEngine::ButiRendering::GraphicDevice::GetApplication()
{
	return vwp_application;
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

void ButiEngine::ButiRendering::GraphicDevice::SetClearColor(const Vector4& arg_clearColor)
{
	color = arg_clearColor;
	clearColor[0] = color.x;
	clearColor[1] = color.y;
	clearColor[2] = color.z;
	clearColor[3] = color.w;
}

ButiEngine::Vector4 ButiEngine::ButiRendering::GraphicDevice::GetClearColor()
{
	return color;
}
