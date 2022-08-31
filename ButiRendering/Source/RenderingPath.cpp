#include"stdafx.h"

#include"ButiRendering_Dx12/Header/Camera.h"
#include"ButiRendering_Dx12/Header/Renderer.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include "ButiRendering_Dx12\Header\RenderingPath.h"
namespace ButiEngine {
namespace ButiRendering {
class ForwardCameraRenderingPath_Dx12 :public ForwardCameraRenderingPath{
public:
	ForwardCameraRenderingPath_Dx12(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer) :ForwardCameraRenderingPath(arg_vlp_camera, arg_vwp_renderer){
		vwp_graphicDevice = arg_vwp_renderer.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>();
		viewport.TopLeftX =static_cast<float>(arg_vlp_camera->GetCameraProperty().left);
		viewport.TopLeftY = static_cast<float>(arg_vlp_camera->GetCameraProperty().top);
		viewport.Width = static_cast<float>(arg_vlp_camera->GetCameraProperty().currentWidth);
		viewport.Height = static_cast<float>(arg_vlp_camera->GetCameraProperty().currentHeight);
		viewport.MinDepth = static_cast<float>(arg_vlp_camera->GetCameraProperty().front);
		viewport.MaxDepth = 1.0f;

		scissorRect.left = 0;
		scissorRect.right = static_cast<std::int64_t>(arg_vlp_camera->GetCameraProperty().currentWidth);
		scissorRect.top = 0;
		scissorRect.bottom = static_cast<std::int64_t>(arg_vlp_camera->GetCameraProperty().currentHeight);
	}

	BUTIRENDERING_API void Execute() override;
private:

	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> drawCommandList;
	D3D12_RECT     scissorRect;
	D3D12_VIEWPORT    viewport;
};

class DeferredCameraRenderingPath_Dx12 :public DeferredCameraRenderingPath {
public:
	DeferredCameraRenderingPath_Dx12(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer, Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath) :DeferredCameraRenderingPath(arg_vlp_camera, arg_vwp_renderer,arg_vwp_forwardPath) {
		vwp_graphicDevice = arg_vwp_renderer.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>();
		viewport.TopLeftX = static_cast<float>(arg_vlp_camera->GetCameraProperty().left);
		viewport.TopLeftY = static_cast<float>(arg_vlp_camera->GetCameraProperty().top);
		viewport.Width = static_cast<float>(arg_vlp_camera->GetCameraProperty().currentWidth);
		viewport.Height = static_cast<float>(arg_vlp_camera->GetCameraProperty().currentHeight);
		viewport.MinDepth = static_cast<float>(arg_vlp_camera->GetCameraProperty().front);
		viewport.MaxDepth = 1.0f;

		scissorRect.left = 0;
		scissorRect.right = static_cast<std::int64_t>(arg_vlp_camera->GetCameraProperty().currentWidth);
		scissorRect.top = 0;
		scissorRect.bottom = static_cast<std::int64_t>(arg_vlp_camera->GetCameraProperty().currentHeight);
	}

	BUTIRENDERING_API void Execute() override;
private:

	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> drawCommandList;
	D3D12_RECT     scissorRect;
	D3D12_VIEWPORT    viewport;
};
}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::CameraRenderingPath::GetCamera()
{
	return vlp_camera;
}

ButiEngine::ButiRendering::ForwardCameraRenderingPath::ForwardCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer)
{
	vlp_camera = arg_vlp_camera;
	vwp_renderer = arg_vwp_renderer;
}


void ButiEngine::ButiRendering::ForwardCameraRenderingPath_Dx12::Execute()
{
	if (!m_isCurrentActive) {
		return;
	}

	vlp_camera->Start();

	if (!drawCommandList) {
		drawCommandList = CommandListHelper::CreateDefault(nullptr, vwp_graphicDevice.lock()->GetDevice(), vwp_graphicDevice.lock()->GetCommandAllocator());
		CommandListHelper::Close(drawCommandList);
	}
	CommandListHelper::Reset(nullptr, drawCommandList, vwp_graphicDevice.lock()->GetCommandAllocator());
	vwp_graphicDevice.lock()->SetCommandList(drawCommandList.Get());
	vwp_graphicDevice.lock()->DrawStart();

	if (m_depthStencilTexture) {
		vwp_graphicDevice.lock()->GetCommandList().RSSetScissorRects(1, &scissorRect);
		m_depthStencilTexture->SetDepthStencilView();
	}
	else {
		vwp_graphicDevice.lock()->CommandList_SetScissorRect();
		vwp_graphicDevice.lock()->ClearDepthStancil(1.0f);
		vwp_graphicDevice.lock()->SetDefaultDepthStencil();
	}

	if (m_list_vlp_renderTarget.GetSize()) {
		vwp_renderer.lock()->GetRendererCBuffer()->Get().pixelScale = Vector2(1.0 / scissorRect.right, 1.0 / scissorRect.bottom);
		for (auto renderTarget : m_list_vlp_renderTarget) {
			renderTarget->SetRenderTarget();
		}
	}
	else {
		vwp_renderer.lock()->GetRendererCBuffer()->Get().pixelScale = Vector2(1.0 / vlp_camera->GetCameraProperty().currentWidth, 1.0 / vlp_camera->GetCameraProperty().currentHeight);
		vwp_graphicDevice.lock()->SetDefaultRenderTarget();
	}
	vwp_graphicDevice.lock()->CommandList_SetRenderTargetView();
	vwp_graphicDevice.lock()->GetCommandList().RSSetViewports(1, &viewport);

	vwp_renderer.lock()->GetRendererCBuffer() ->Get().cameraPos = Vector4(vlp_camera->vlp_transform->GetWorldPosition());

	if (m_isShadowRendering) {
		vwp_renderer.lock()->ShadowRendering(m_layer);
	}
	else {
		vwp_renderer.lock()->Rendering(m_layer);
	}


	for (auto renderTarget : m_list_vlp_renderTarget) {
		renderTarget->DisSetRenderTarget();
	}
	if (m_depthStencilTexture) {
		m_depthStencilTexture->DisSetDepthStencil();
	}

	CommandListHelper::Close(drawCommandList);
	vwp_graphicDevice.lock()->DrawEnd();
}


void ButiEngine::ButiRendering::DeferredCameraRenderingPath_Dx12::Execute()
{

	if (!m_isCurrentActive) {
		return;
	}
	vwp_renderer.lock()->GetRendererCBuffer()->Get().cameraPos = vlp_forwardCamera->vlp_transform->GetWorldPosition();
	vwp_renderer.lock()->GetRendererCBuffer()->Get().forwordCameraMatrix = vlp_forwardCamera->vlp_transform->ToMatrix();
	vwp_renderer.lock()->GetRendererCBuffer()->Update();

	vlp_camera->Start();
	if (!drawCommandList) {
		drawCommandList = CommandListHelper::CreateDefault(nullptr, vwp_graphicDevice.lock()->GetDevice(), vwp_graphicDevice.lock()->GetCommandAllocator());
		CommandListHelper::Close(drawCommandList);
	}
	CommandListHelper::Reset(nullptr, drawCommandList, vwp_graphicDevice.lock()->GetCommandAllocator());
	vwp_graphicDevice.lock()->SetCommandList(drawCommandList.Get());
	vwp_graphicDevice.lock()->DrawStart();

	if (m_depthStencilTexture) {
		vwp_graphicDevice.lock()->GetCommandList().RSSetScissorRects(1, &scissorRect);
		m_depthStencilTexture->SetDepthStencilView();
	}
	else {
		vwp_graphicDevice.lock()->CommandList_SetScissorRect();
		vwp_graphicDevice.lock()->ClearDepthStancil(1.0f);
		vwp_graphicDevice.lock()->SetDefaultDepthStencil();
	}

	if (m_list_vlp_renderTarget.GetSize()) {
		vwp_renderer.lock()->GetRendererCBuffer()->Get().pixelScale = Vector2(1.0 / scissorRect.right, 1.0 / scissorRect.bottom);
		for (auto renderTarget : m_list_vlp_renderTarget) {
			renderTarget->SetRenderTarget();
		}
	}
	else {
		vwp_renderer.lock()->GetRendererCBuffer()->Get().pixelScale = Vector2(1.0 / vlp_camera->GetCameraProperty().currentWidth, 1.0 / vlp_camera->GetCameraProperty().currentHeight);
		vwp_graphicDevice.lock()->SetDefaultRenderTarget();
	}
	vwp_graphicDevice.lock()->CommandList_SetRenderTargetView();
	vwp_graphicDevice.lock()->GetCommandList().RSSetViewports(1, &viewport);

	vwp_renderer.lock()->Rendering(m_layer);

	for (auto renderTarget : m_list_vlp_renderTarget) {
		renderTarget->DisSetRenderTarget();
	}
	if (m_depthStencilTexture) {
		m_depthStencilTexture->DisSetDepthStencil();
	}

	CommandListHelper::Close(drawCommandList);
	vwp_graphicDevice.lock()->DrawEnd();
}

void ButiEngine::ButiRendering::CameraRenderingPath::ActivatePlayScene()
{
	m_isCurrentActive= m_isPlayActive;
}

void ButiEngine::ButiRendering::CameraRenderingPath::ActivateEditScene()
{
	m_isCurrentActive = m_isEditActive;
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
	vlp_forwardCamera = arg_vwp_forwardPath.lock()->GetCamera();
	vwp_forwardPath = arg_vwp_forwardPath;
}



ButiEngine::Value_ptr<ButiEngine::ButiRendering::ForwardCameraRenderingPath> ButiEngine::ButiRendering::CreateForwardRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer)
{
	return ObjectFactory::Create<ForwardCameraRenderingPath_Dx12>(arg_vlp_camera,arg_vwp_renderer);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::DeferredCameraRenderingPath> ButiEngine::ButiRendering::CreateDeferredRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer, Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath)
{
	return ObjectFactory::Create<DeferredCameraRenderingPath_Dx12>(arg_vlp_camera,arg_vwp_renderer,arg_vwp_forwardPath);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::ForwardCameraRenderingPath> ButiEngine::ButiRendering::CreateForwardRenderingPath(Value_ptr<DeferredCameraRenderingPath> arg_path)
{
	auto output = ObjectFactory::Create<ForwardCameraRenderingPath_Dx12>(arg_path->GetCamera(), arg_path->GetRenderer());
	output->SetLayer(arg_path->GetLayer());
	output->SetIsEditActive(arg_path->IsEditActive());
	output->SetIsPlayActive(arg_path->IsPlayActive());
	for (auto rt : arg_path->GetRenderTargets()) {
		output->PushRenderTarget(rt);
	}

	output->SetDepthStencil(arg_path->GetDepthStencil());
	return output;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::DeferredCameraRenderingPath> ButiEngine::ButiRendering::CreateDeferredRenderingPath(Value_ptr<ForwardCameraRenderingPath> arg_path, Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath)
{
	auto output= ObjectFactory::Create<DeferredCameraRenderingPath_Dx12>(arg_path->GetCamera(), arg_path->GetRenderer(), arg_vwp_forwardPath);
	output->SetLayer(arg_path->GetLayer());
	output->SetIsEditActive(arg_path->IsEditActive());
	output->SetIsPlayActive(arg_path->IsPlayActive());
	for (auto rt : arg_path->GetRenderTargets()) {
		output->PushRenderTarget(rt);
	}

	output->SetDepthStencil(arg_path->GetDepthStencil());
	return output;
}
