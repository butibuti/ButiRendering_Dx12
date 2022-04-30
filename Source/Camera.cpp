#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Camera_Dx12.h"
#include"ButiRendering_Dx12/Header/Camera.h"
#include"Header/Common/CollisionPrimitive.h"
#include"../Header/Renderer.h"
const std::string& ButiEngine::ButiRendering::Camera::GetName() const
{
	return cameraName;
}

void ButiEngine::ButiRendering::Camera::SetName(const std::string& arg_name)
{
	cameraName = arg_name;
	cameraViewProp.cameraName = arg_name;
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
	if (cameraViewProp.isShadow) {
		vlp_renderer->ShadowRendering(cameraViewProp.layer);
	}
	else {
		vlp_renderer->Rendering(cameraViewProp.layer);
	}
}

void ButiEngine::ButiRendering::Camera::BefDraw()
{
	cameraPos= vlp_transform->GetWorldPosition();

#ifdef _EDITORBUILD
	viewMatrix = (Matrix4x4::Scale(Vector3{ 2, 2, 1 }) * vlp_transform->GetMatrix()).GetInverse();
#else
	viewMatrix = (vlp_transform->GetMatrix()).GetInverse();
#endif
}

ButiEngine::ButiRendering::CameraProjProperty& ButiEngine::ButiRendering::Camera::GetCameraProperty()
{
	return cameraViewProp;
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
	if (Geometry::SurfaceHit::IsHitAABBCameraFrustum(*arg_AABB, vlp_transform, projectionMatrix, cameraViewProp.nearClip, cameraViewProp.farClip)) {


		auto ray = ObjectFactory::Create<Collision::CollisionPrimitive_Segment>(vlp_transform, arg_AABB->position);
		//ray->Update();
		auto objects = vlp_renderer->GetHitDrawObjects(ray, cameraViewProp.layer);

		return objects.size();
	}
	return -1;
}

ButiEngine::Value_ptr< ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::CameraCreater::CreateCamera(const CameraProjProperty& arg_cameraViewProp, const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	Value_ptr<ICamera> out;
	out = ObjectFactory::Create<Camera_Dx12>(arg_cameraViewProp, cameraName,arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
	
	out->SetActive(initActive);
	return out;
}

ButiEngine::Value_ptr< ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::CameraCreater::CreateCamera(const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	CameraProjProperty cameraViewProp = CameraProjProperty(0, 0, 0, 0);
	Value_ptr<ICamera> out;
	out = ObjectFactory::Create<Camera_Dx12>(cameraViewProp, cameraName, arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
	
	out->SetActive(initActive);
	return out;
}

ButiEngine::ButiRendering::CameraProjProperty::CameraProjProperty(const std::uint32_t arg_widthScale, const std::uint32_t arg_heightScale, const std::uint32_t arg_x, const std::uint32_t arg_y, const bool arg_isPararell, std::uint32_t arg_layer)
	:top(arg_y), left(arg_x),isPararell(arg_isPararell),layer(arg_layer)
{
	currentWidth = arg_widthScale;
	currentHeight = arg_heightScale;
	width = arg_widthScale;
	height= arg_heightScale;
}
