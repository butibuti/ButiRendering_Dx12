#include"stdafx.h"

#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"..\../Header/Renderer.h"
#include "..\..\Header\Rendering_Dx12\Camera_Dx12.h"

ButiEngine::ButiRendering::Camera_Dx12::Camera_Dx12(const CameraProperty& arg_cameraProp, const std::string& arg_cameraName, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
{
	cameraProp = arg_cameraProp;
	SetName(arg_cameraName);
	vlp_renderer = arg_vlp_renderer;
	vwp_graphicDevice = arg_vwp_graphicDevice;
}

ButiEngine::ButiRendering::Camera_Dx12::~Camera_Dx12()
{
}

void ButiEngine::ButiRendering::Camera_Dx12::Initialize()
{
	if (cameraProp.angle <= 5) {
		cameraProp.angle = 60;
	}

	if (cameraProp.m_list_vlp_renderTarget.GetSize()&& cameraProp.m_list_vlp_renderTarget.At(0) == GetGraphicDevice()->GetDefaultRenderTarget()) {
		cameraProp.currentWidth = cameraProp.width / 2;
		cameraProp.currentHeight = cameraProp.height / 2;
	}
	else
	{
		cameraProp.currentWidth = cameraProp.width;
		cameraProp.currentHeight = cameraProp.height;
	}

	if (cameraProp.isPararell) {
		
		projectionMatrix =Matrix4x4::OrthographicOffCenterLH(
			-(float)cameraProp.currentWidth / 2, (float)cameraProp.currentWidth / 2,
			-(float)cameraProp.currentHeight / 2, (float)cameraProp.currentHeight / 2,
			cameraProp.nearClip,
			cameraProp.farClip
		);
	}
	else {

		auto angle = cameraProp.angle;
		
		if (cameraProp.m_list_vlp_renderTarget.GetSize()&&cameraProp.m_list_vlp_renderTarget.At(0) == GetGraphicDevice()->GetDefaultRenderTarget()) {
			angle *= 0.5f;
		}
		projectionMatrix =
			Matrix4x4::PersepectiveFovLH(
				MathHelper::ToRadian(angle),
				(float)cameraProp.currentWidth / (float)cameraProp.currentHeight,
				cameraProp.nearClip,
				cameraProp.farClip
			);
	}

	projectionMatrix.Transpose();

	viewport.TopLeftX = (float)cameraProp.left;
	viewport.TopLeftY = (float)cameraProp.top;
	viewport.Width = static_cast<FLOAT>(cameraProp.currentWidth);
	viewport.Height = static_cast<FLOAT>(cameraProp.currentHeight);
	viewport.MinDepth = cameraProp.front;
	viewport.MaxDepth = 1.0f;

	scissorRect.left = 0;
	scissorRect.right = cameraProp.currentWidth;
	scissorRect.top = 0;
	scissorRect.bottom = cameraProp.currentHeight;





}

void ButiEngine::ButiRendering::Camera_Dx12::Start()
{
	if (!drawCommandList) {
		drawCommandList = CommandListHelper::CreateDefault(nullptr, vwp_graphicDevice.lock()->GetDevice(), vwp_graphicDevice.lock()->GetCommandAllocator());
		CommandListHelper::Close(drawCommandList);
	}
	CommandListHelper::Reset(nullptr, drawCommandList, vwp_graphicDevice.lock()->GetCommandAllocator());
	vwp_graphicDevice.lock()->SetCommandList(drawCommandList.Get());
	vwp_graphicDevice.lock()->DrawStart();

	if (cameraProp.m_depthStencilTexture.lock()) {
		vwp_graphicDevice.lock()->GetCommandList().RSSetScissorRects(1, &scissorRect);
		cameraProp.m_depthStencilTexture.lock()->SetDepthStencilView();
	}
	else {
		vwp_graphicDevice.lock()->CommandList_SetScissorRect();
		vwp_graphicDevice.lock()->ClearDepthStancil(1.0f);
		vwp_graphicDevice.lock()->SetDefaultDepthStencil();
	}

	if (cameraProp.m_list_vlp_renderTarget.GetSize()) {
		vlp_renderer->GetRendererCBuffer()->Get().pixelScale = Vector2(1.0 / scissorRect.right, 1.0 / scissorRect.bottom);
		for (auto renderTarget : cameraProp.m_list_vlp_renderTarget) {
			renderTarget.lock()->SetRenderTarget(cameraProp.clearColor);
		}
	}
	else {
		vlp_renderer->GetRendererCBuffer()->Get().pixelScale = Vector2(1.0 / cameraProp.currentWidth , 1.0 / cameraProp.currentHeight);
		vwp_graphicDevice.lock()->SetDefaultRenderTarget();
	}
	vwp_graphicDevice.lock()->CommandList_SetRenderTargetView();
	vwp_graphicDevice.lock()->SetCameraPos(cameraPos);
	vwp_graphicDevice.lock()->SetProjectionMatrix(projectionMatrix);
	vwp_graphicDevice.lock()->SetRawViewMatrix(viewMatrix);

	auto transposed = viewMatrix.Transpose();

	vwp_graphicDevice.lock()->SetViewMatrix(transposed);

	auto billboard= transposed;
	billboard._14 = 0.0f;
	billboard._24 = 0.0f;
	billboard._34 = 0.0f;

	billboard.Inverse();

	vwp_graphicDevice.lock()->SetViewMatrix_billBoard(billboard);
	vwp_graphicDevice.lock()->SetViewMatrix_billBoardX(billboard.GetInValidYZ());
	vwp_graphicDevice.lock()->SetViewMatrix_billBoardY(billboard.GetInValidXZ());
	vwp_graphicDevice.lock()->SetViewMatrix_billBoardZ(billboard.GetInValidXY());

	vwp_graphicDevice.lock()->GetCommandList().RSSetViewports(1, &viewport);

}

void ButiEngine::ButiRendering::Camera_Dx12::Stop() const
{
	for (auto renderTarget : cameraProp.m_list_vlp_renderTarget) {
		renderTarget.lock()->DisSetRenderTarget();
	}
	if (cameraProp.m_depthStencilTexture.lock()) {
		cameraProp.m_depthStencilTexture.lock()->DisSetDepthStencil();
	}
	CommandListHelper::Close(drawCommandList);
	vwp_graphicDevice.lock()->DrawEnd();

}

void ButiEngine::ButiRendering::Camera_Dx12::End()
{

	for (auto renderTarget : cameraProp.m_list_vlp_renderTarget) {
		renderTarget.lock()->SetIsCleared(false);
	}
	if (cameraProp.m_depthStencilTexture.lock()) {
		cameraProp.m_depthStencilTexture.lock()->SetIsCleared(false);
	}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::GraphicDevice> ButiEngine::ButiRendering::Camera_Dx12::GetGraphicDevice() const
{
	return vwp_graphicDevice.lock();
}
