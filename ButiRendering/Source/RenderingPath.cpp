#include"stdafx.h"

#include"ButiRendering_Dx12/Header/Camera.h"
#include"ButiRendering_Dx12/Header/Renderer.h"
#include "ButiRendering_Dx12\Header\RenderingPath.h"
ButiEngine::Value_ptr<ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::CameraRenderingPath::GetCamera()
{
	return vlp_camera;
}

ButiEngine::ButiRendering::ForwardCameraRenderingPath::ForwardCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer)
{
	vlp_camera = arg_vlp_camera;
	vwp_renderer = arg_vwp_renderer;
}

void ButiEngine::ButiRendering::ForwardCameraRenderingPath::BefExecute()
{
	if (!vlp_camera->GetActive()) {
		return;
	}
	vlp_camera->BefDraw();
}

void ButiEngine::ButiRendering::ForwardCameraRenderingPath::Execute()
{
	if (!vlp_camera->GetActive()) {
		return;
	}
	vlp_camera->Start();

	vwp_renderer.lock()->GetRendererCBuffer() ->Get().cameraPos = Vector4(vlp_camera->vlp_transform->GetWorldPosition());
	vlp_camera->Draw();
	vlp_camera->Stop();
}

void ButiEngine::ButiRendering::ForwardCameraRenderingPath::End()
{
	if (!vlp_camera->GetActive()) {
		return;
	}
	vlp_camera->End();
}

void ButiEngine::ButiRendering::ForwardCameraRenderingPath::Release()
{
}

//void ButiEngine::ButiRendering::ForwardCameraRenderingPath::OnShowGUI()
//{
//	vlp_camera->ShowUI();
//}

void ButiEngine::ButiRendering::CameraRenderingPath::SetPlayActive()
{
	vlp_camera ->SetActive(vlp_camera->GetCameraProperty().isInitActive);
}

void ButiEngine::ButiRendering::CameraRenderingPath::SetEditActive()
{
	vlp_camera->SetActive(vlp_camera->GetCameraProperty().isEditActive);
}

void ButiEngine::ButiRendering::CameraRenderingPath::SetName(const std::string& arg_name)
{
	vlp_camera->SetName(arg_name);
}

const std::string& ButiEngine::ButiRendering::CameraRenderingPath::GetName() const
{
	return vlp_camera->GetName();
}

ButiEngine::ButiRendering::DeferredCameraRenderingPath::DeferredCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer, Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath)
{
	vlp_camera = arg_vlp_camera;
	vwp_renderer = arg_vwp_renderer;
}

void ButiEngine::ButiRendering::DeferredCameraRenderingPath::BefExecute()
{
	if (!vlp_camera->GetActive()) {
		return;
	}
	vlp_camera->BefDraw();
}

void ButiEngine::ButiRendering::DeferredCameraRenderingPath::Execute()
{
	if (!vlp_camera->GetActive()) {
		return;
	}
	vlp_camera->Start();
	vwp_renderer.lock()->GetRendererCBuffer()->Get().cameraPos = vlp_forwardCamera->vlp_transform->GetWorldPosition();
	vwp_renderer.lock()->GetRendererCBuffer()->Get().forwordCameraMatrix = vlp_forwardCamera->vlp_transform->ToMatrix();
	vlp_camera->Draw();
	vlp_camera->Stop();
}

void ButiEngine::ButiRendering::DeferredCameraRenderingPath::End()
{
	if (!vlp_camera->GetActive()) {
		return;
	}
	vlp_camera->End();
}

void ButiEngine::ButiRendering::DeferredCameraRenderingPath::Release()
{
}
