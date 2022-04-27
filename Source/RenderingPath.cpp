#include"stdafx.h"

#include "..\Header\RenderingPath.h"
#include"../Header/Camera.h"
#include"ButiRendering_Dx12/Header/Renderer.h"
ButiEngine::ButiRendering::ForwardCameraRenderingPath::ForwardCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_ptr<IScene> arg_vlp_scene)
{
	vlp_camera = arg_vlp_camera;
	vlp_scene = arg_vlp_scene;
	vlp_renderer = vlp_scene->GetRenderer();
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::RenderingPathInfo> ButiEngine::ButiRendering::ForwardCameraRenderingPath::GetRenderingPathInfo()
{
	auto output = ObjectFactory::Create<ForwardCameraRenderingPathInfo>();
	output->vlp_cameraTrnasform = vlp_camera->vlp_transform;
	output->vlp_cameraProp = make_value<CameraProjProperty>(vlp_camera->GetCameraProperty());
	return output;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::CameraRenderingPath::GetCamera()
{
	return vlp_camera;
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

	vlp_renderer->GetRendererCBuffer() ->Get().cameraPos = Vector4(vlp_camera->vlp_transform->GetWorldPosition());
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
	vlp_scene = nullptr;
}

void ButiEngine::ButiRendering::ForwardCameraRenderingPath::OnShowGUI()
{
	vlp_camera->ShowUI();
}

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

ButiEngine::ButiRendering::DeferredCameraRenderingPath::DeferredCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_ptr<IScene> arg_vlp_scene, std::string arg_forwardCamName)
{
	vlp_camera = arg_vlp_camera;
	vlp_scene = arg_vlp_scene;
	vlp_renderer = vlp_scene->GetRenderer();
	forwardCamName = arg_forwardCamName;
	auto forwardCam = vlp_scene->GetCamera(forwardCamName);
	if (forwardCam) {
		vlp_forwardCamera = forwardCam;
	}
	else {
		vlp_forwardCamera = vlp_camera;
	}
}


ButiEngine::Value_ptr<ButiEngine::ButiRendering::RenderingPathInfo> ButiEngine::ButiRendering::DeferredCameraRenderingPath::GetRenderingPathInfo()
{
	auto output = ObjectFactory::Create<DeferredCameraRenderingPathInfo>();
	output->vlp_cameraTrnasform = vlp_camera->vlp_transform;
	output->vlp_cameraProp = make_value<CameraProjProperty>(vlp_camera->GetCameraProperty());
	if (vlp_forwardCamera) {
		output->forwardCameraPathName = vlp_forwardCamera->GetName();
	}
	return output;
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
	vlp_renderer->GetRendererCBuffer()->Get().cameraPos = vlp_forwardCamera->vlp_transform->GetWorldPosition();
	vlp_renderer->GetRendererCBuffer()->Get().forwordCameraMatrix = vlp_forwardCamera->vlp_transform->ToMatrix();
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
	vlp_scene = nullptr;

}

void ButiEngine::ButiRendering::DeferredCameraRenderingPath::OnShowGUI()
{
	static char camName[128];
	GUI::BulletText("ForwardRenderingCamera");
	GUI::InputTextWithHint("##forwordCamName", forwardCamName, camName, 128);
	GUI::SameLine();
	if (GUI::Button("Change")) {
		forwardCamName = camName;
		vlp_forwardCamera = vlp_scene->GetCamera(forwardCamName);
	}

	vlp_camera->ShowUI();
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IRenderingPath> ButiEngine::ButiRendering::ForwardCameraRenderingPathInfo::CreateRenderingPath(Value_ptr<IRenderer> arg_vlp_renderer, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	auto camera= CameraCreater::CreateCamera(*vlp_cameraProp, vlp_cameraProp->cameraName, vlp_cameraProp->isInitActive, arg_vlp_renderer, arg_vlp_graphicDevice);
	camera->vlp_transform = vlp_cameraTrnasform;
	return ObjectFactory::Create< ForwardCameraRenderingPath>(camera,arg_vlp_renderer->GetScene());
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IRenderingPath> ButiEngine::ButiRendering::DeferredCameraRenderingPathInfo::CreateRenderingPath(Value_ptr<IRenderer> arg_vlp_renderer, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	auto camera = CameraCreater::CreateCamera(*vlp_cameraProp, vlp_cameraProp->cameraName, vlp_cameraProp->isInitActive, arg_vlp_renderer, arg_vlp_graphicDevice);
	camera->vlp_transform = vlp_cameraTrnasform;
	if (!camera->GetCameraProperty().projectionTexture.size()) {
		camera->vlp_transform->SetLocalScale(Vector3(2, 2, 1));
	}
	return ObjectFactory::Create< DeferredCameraRenderingPath>(camera, arg_vlp_renderer->GetScene(),forwardCameraPathName);
}
