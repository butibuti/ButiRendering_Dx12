#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Camera_Dx12.h"
#include"..\..\Header\Renderer.h"
#include "..\..\Header\Camera.h"
const std::string& ButiEngine::ButiRendering::Camera::GetName() const
{
	return cameraName;
}

void ButiEngine::ButiRendering::Camera::SetName(const std::string& arg_name)
{
	cameraName = arg_name;
	cameraProp.cameraName = arg_name;
}


void ButiEngine::ButiRendering::Camera::SetActive(const bool arg_active)
{
	isActive = arg_active;
}

bool ButiEngine::ButiRendering::Camera::GetActive() const
{
	return isActive;
}

void ButiEngine::ButiRendering::Camera::Draw()
{
	if (cameraProp.isShadow) {
		vlp_renderer->ShadowRendering(cameraProp.layer);
	}
	else {
		vlp_renderer->Rendering(cameraProp.layer);
	}
}

void ButiEngine::ButiRendering::Camera::BefDraw()
{
	cameraPos= vlp_transform->GetWorldPosition();

	if (cameraProp.m_list_vlp_renderTarget.GetSize()==1&& cameraProp.m_list_vlp_renderTarget.at(0)==GetGraphicDevice()->GetDefaultRenderTarget()) {
		viewMatrix = (Matrix4x4::Scale(Vector3{ 2, 2, 1 }) * vlp_transform->GetMatrix()).GetInverse();
	}
	else {
		viewMatrix = vlp_transform->GetMatrix().GetInverse();
	}
}

ButiEngine::ButiRendering::CameraProperty& ButiEngine::ButiRendering::Camera::GetCameraProperty()
{
	return cameraProp;
}

const ButiEngine::ButiRendering::CameraProperty& ButiEngine::ButiRendering::Camera::GetCameraProperty() const
{
	return cameraProp;
}

ButiEngine::Matrix4x4 ButiEngine::ButiRendering::Camera::GetViewProjectionMatrix()
{
	return (projectionMatrix*viewMatrix);
}

ButiEngine::Matrix4x4 ButiEngine::ButiRendering::Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

ButiEngine::Matrix4x4 ButiEngine::ButiRendering::Camera::GetViewMatrix()
{
	return viewMatrix;
}

ButiEngine::Vector3 ButiEngine::ButiRendering::Camera::GetPosition()
{
	return vlp_transform->GetWorldPosition();
}

std::int32_t ButiEngine::ButiRendering::Camera::IsContaineVisibility(Value_ptr<Geometry::Box_AABB> arg_AABB)
{
	return -1;
}

ButiEngine::Vector3 ButiEngine::ButiRendering::Camera::WorldToScreen(const Vector3& arg_pos) const
{
	Matrix4x4 convertedPos;

	if (cameraProp.m_list_vlp_renderTarget.GetSize() == 1 && cameraProp.m_list_vlp_renderTarget.at(0) == GetGraphicDevice()->GetDefaultRenderTarget()) {
		auto projection =
			Matrix4x4::PersepectiveFovLH(
				MathHelper::ToRadian(static_cast<float>(GetCameraProperty().angle * 0.5f)),
				static_cast<float>(GetCameraProperty().currentWidth) / static_cast<float>(GetCameraProperty().currentHeight),
				static_cast<float>(GetCameraProperty().nearClip),
				static_cast<float>(GetCameraProperty().farClip)
			);
		convertedPos = (projection * ((Matrix4x4::Scale(Vector3{ 40.0, 40.0, 1 }) * vlp_transform->GetMatrix()).GetInverse()).Transpose());
	}
	else {
		auto projection =
			Matrix4x4::PersepectiveFovLH(
				MathHelper::ToRadian(static_cast<float>(GetCameraProperty().angle) * 2.0f),
				static_cast<float>(GetCameraProperty().currentWidth) / static_cast<float>(GetCameraProperty().currentHeight),
				static_cast<float>(GetCameraProperty().nearClip),
				static_cast<float>(GetCameraProperty().farClip)
			);
		convertedPos = (projection * ((Matrix4x4::Scale(Vector3{ 6.5, 6.5, 1 }) * vlp_transform->GetMatrix()).GetInverse()).Transpose());
	}
	auto pos = Vector4(arg_pos, 1.0f) * convertedPos.Transpose();
	auto t = Vector3(pos.x / pos.w, pos.y / pos.w, pos.z/pos.w);
	t.x *= -1.0f * GetCameraProperty().currentWidth;
	t.y *= -1.0f * GetCameraProperty().currentHeight;
	return t;
}

void ButiEngine::ButiRendering::Camera::SetRenderTarget(Value_ptr<IRenderTarget> arg_renderTarget)
{
	cameraProp.m_list_vlp_renderTarget.Add(arg_renderTarget);
}

void ButiEngine::ButiRendering::Camera::SetDepthStencil(Value_ptr<IDepthStencil> arg_depthStencil)
{
	cameraProp.m_depthStencilTexture = arg_depthStencil;
}

ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IRenderer> ButiEngine::ButiRendering::Camera::GetRenderer() const
{
	return vlp_renderer;
}

ButiEngine::Value_ptr< ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::CreateCamera(const CameraProperty& arg_cameraProp, const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	Value_ptr<ICamera> out;
	out = ObjectFactory::Create<Camera_Dx12>(arg_cameraProp, cameraName,arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
	
	out->SetActive(initActive);
	return out;
}

ButiEngine::Value_ptr< ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::CreateCamera(const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	CameraProperty cameraProp = CameraProperty(0, 0, 0, 0);
	Value_ptr<ICamera> out;
	out = ObjectFactory::Create<Camera_Dx12>(cameraProp, cameraName, arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
	
	out->SetActive(initActive);
	return out;
}

ButiEngine::ButiRendering::CameraProperty::CameraProperty(const std::uint32_t arg_widthScale, const std::uint32_t arg_heightScale, const std::uint32_t arg_x, const std::uint32_t arg_y, const bool arg_isPararell, std::uint32_t arg_layer)
	:top(arg_y), left(arg_x),isPararell(arg_isPararell),layer(arg_layer)
{
	currentWidth = arg_widthScale;
	currentHeight = arg_heightScale;
	width = arg_widthScale;
	height= arg_heightScale;
}
