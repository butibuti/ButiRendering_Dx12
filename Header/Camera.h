#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include"ButiUtil/ButiUtil/ObjectFactory.h"
#include"ButiMath/Geometry.h"
#include<string>
#include"ICamera.h"
namespace ButiEngine {
namespace ButiRendering {
class  Camera :public ICamera
{
public:
	const std::string& GetName() const override;
	void SetName(const std::string& arg_name) override;
	void SetActive(const bool arg_active) override;
	bool GetActive()const override;
	void Draw()override;
	void BefDraw()override;
	CameraProjProperty& GetCameraProperty()override;
	const CameraProjProperty& GetCameraProperty()const override;
	Matrix4x4 GetViewProjectionMatrix()override;
	Matrix4x4 GetProjectionMatrix()override;
	Matrix4x4 GetViewMatrix()override;
	Vector3 GetPosition()override;
	std::int32_t IsContaineVisibility(Value_ptr<Geometry::Box_AABB>arg_AABB)override;
	Vector3 WorldToScreen(const Vector3& arg_pos)const override;
protected:
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewMatrix;
	Vector3 cameraPos;
	CameraProjProperty cameraViewProp;
	Value_ptr<IRenderer> vlp_renderer;
	bool isActive = true;
	std::string cameraName;
private:

};
class CameraCreater {
public:
	static Value_ptr<ICamera> CreateCamera(const CameraProjProperty& arg_cameraViewProp, const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
	static Value_ptr<ICamera> CreateCamera(const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
};
}
}
#endif