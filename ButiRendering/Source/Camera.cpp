#include"stdafx.h"

#include"..\..\Header\Renderer.h"
#include "..\..\Header\Camera.h"
ButiEngine::ButiRendering::Camera::Camera(const CameraProperty& arg_cameraProp, const std::string& arg_cameraName, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	cameraProp = arg_cameraProp;
	SetName(arg_cameraName);
	vlp_renderer = arg_vlp_renderer;
	vwp_graphicDevice = arg_vwp_graphicDevice;
}
const std::string& ButiEngine::ButiRendering::Camera::GetName() const
{
	return cameraName;
}

void ButiEngine::ButiRendering::Camera::Initialize()
{

	if (cameraProp.angle <= 5) {
		cameraProp.angle = 60;
	}


	cameraProp.currentWidth = cameraProp.width * cameraProp.scaleAdjusment;
	cameraProp.currentHeight = cameraProp.height * cameraProp.scaleAdjusment;

	if (cameraProp.isPararell) {

		projectionMatrix = Matrix4x4::OrthographicOffCenterLH(
			-(float)cameraProp.currentWidth / 2, (float)cameraProp.currentWidth / 2,
			-(float)cameraProp.currentHeight / 2, (float)cameraProp.currentHeight / 2,
			cameraProp.nearClip,
			cameraProp.farClip
		);
	}
	else {

		auto angle = cameraProp.angle * cameraProp.scaleAdjusment;

		projectionMatrix =
			Matrix4x4::PersepectiveFovLH(
				MathHelper::ToRadian(angle),
				static_cast<float>(cameraProp.currentWidth )/ static_cast<float>(cameraProp.currentHeight),
				cameraProp.nearClip,
				cameraProp.farClip
			);
	}

	projectionMatrix.Transpose();
}

void ButiEngine::ButiRendering::Camera::Start()
{
	cameraPos = vlp_transform->GetWorldPosition();
	viewMatrix = Matrix4x4::Scale(Vector3{ cameraProp.scaleAdjusment,cameraProp.scaleAdjusment, cameraProp.scaleAdjusment }) * vlp_transform->GetMatrix().GetInverse();
	vwp_graphicDevice.lock()->SetCameraPos(cameraPos);
	vwp_graphicDevice.lock()->SetProjectionMatrix(projectionMatrix);
	vwp_graphicDevice.lock()->SetRawViewMatrix(viewMatrix);

	auto transposed = viewMatrix.Transpose();

	vwp_graphicDevice.lock()->SetViewMatrix(transposed);

	auto billboard = transposed;
	billboard._14 = 0.0f;
	billboard._24 = 0.0f;
	billboard._34 = 0.0f;

	billboard.Inverse();

	vwp_graphicDevice.lock()->SetViewMatrix_billBoard(billboard);
	vwp_graphicDevice.lock()->SetViewMatrix_billBoardX(billboard.GetInValidYZ());
	vwp_graphicDevice.lock()->SetViewMatrix_billBoardY(billboard.GetInValidXZ());
	vwp_graphicDevice.lock()->SetViewMatrix_billBoardZ(billboard.GetInValidXY());
}

void ButiEngine::ButiRendering::Camera::SetName(const std::string& arg_name)
{
	cameraName = arg_name;
	cameraProp.cameraName = arg_name;
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


	auto projection =
		Matrix4x4::PersepectiveFovLH(
			MathHelper::ToRadian(static_cast<float>(GetCameraProperty().angle) * 2.0f),
			static_cast<float>(GetCameraProperty().currentWidth) / static_cast<float>(GetCameraProperty().currentHeight),
			static_cast<float>(GetCameraProperty().nearClip),
			static_cast<float>(GetCameraProperty().farClip)
		);
	convertedPos = (projection * ((Matrix4x4::Scale(Vector3{ 6.5, 6.5, 1 }) * vlp_transform->GetMatrix()).GetInverse()).Transpose());
	auto pos = Vector4(arg_pos, 1.0f) * convertedPos.Transpose();
	auto t = Vector3(pos.x / pos.w, pos.y / pos.w, pos.z/pos.w);
	t.x *= -1.0f * GetCameraProperty().currentWidth;
	t.y *= -1.0f * GetCameraProperty().currentHeight;
	return t;
}


ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IRenderer> ButiEngine::ButiRendering::Camera::GetRenderer() const
{
	return vlp_renderer;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::GraphicDevice> ButiEngine::ButiRendering::Camera::GetGraphicDevice() const
{
	return vwp_graphicDevice.lock();
}

ButiEngine::Value_ptr< ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::CreateCamera(const CameraProperty& arg_cameraProp, const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	Value_ptr<ICamera> out;
	out = ObjectFactory::Create<Camera>(arg_cameraProp, cameraName,arg_vlp_renderer, arg_vwp_graphicDevice);
	
	return out;
}

ButiEngine::Value_ptr< ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::CreateCamera(const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	CameraProperty cameraProp = CameraProperty(0, 0, 0, 0);
	Value_ptr<ICamera> out;
	out = ObjectFactory::Create<Camera>(cameraProp, cameraName, arg_vlp_renderer, arg_vwp_graphicDevice);
	
	return out;
}

ButiEngine::ButiRendering::CameraProperty::CameraProperty(const std::uint32_t arg_widthScale, const std::uint32_t arg_heightScale, const std::uint32_t arg_x, const std::uint32_t arg_y, const bool arg_isPararell)
	:top(arg_y), left(arg_x),isPararell(arg_isPararell)
{
	currentWidth = arg_widthScale;
	currentHeight = arg_heightScale;
	width = arg_widthScale;
	height= arg_heightScale;
}
