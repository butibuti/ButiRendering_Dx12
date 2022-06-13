#ifndef RENDERINGPATH_H
#define RENDERINGPATH_H
#include"ResourceInterface.h"
#include"ICamera.h"
#include"ButiMath/Transform.h"
namespace ButiEngine {
namespace ButiRendering {


class CameraRenderingPath :public IRenderingPath {
public:
	BUTIRENDERING_API Value_ptr<ICamera> GetCamera();
	BUTIRENDERING_API void SetName(const std::string& arg_name);
	BUTIRENDERING_API const std::string& GetName()const;
	BUTIRENDERING_API void SetPlayActive()override;
	BUTIRENDERING_API void SetEditActive()override;
	void SetOrder(const std::int32_t arg_order)override { m_order = arg_order; }
	std::int32_t GetOrder()const override { return m_order; }
protected:
	Value_ptr<ICamera> vlp_camera;
	std::int32_t m_order=0;
};

class ForwardCameraRenderingPath :public CameraRenderingPath {
public:
	BUTIRENDERING_API ForwardCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera,Value_weak_ptr<IRenderer> arg_vwp_renderer);
	BUTIRENDERING_API void BefExecute()override;
	BUTIRENDERING_API void Execute() override;
	BUTIRENDERING_API void End() override;
	BUTIRENDERING_API void Release() override;
private:
	Value_weak_ptr<IRenderer> vwp_renderer;
};

class DeferredCameraRenderingPath :public CameraRenderingPath {
public:
	BUTIRENDERING_API DeferredCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer, Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath);
	BUTIRENDERING_API void BefExecute()override;
	BUTIRENDERING_API void Execute() override;
	BUTIRENDERING_API void End() override;
	BUTIRENDERING_API void Release() override;
	void SetForwardPath(Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath) {
		vwp_forwardPath = arg_vwp_forwardPath;
	}
	Value_weak_ptr<ForwardCameraRenderingPath> GetForwardPath() { return vwp_forwardPath; }
private:
	Value_ptr<ICamera> vlp_forwardCamera;
	Value_weak_ptr<IRenderer> vwp_renderer;
	Value_weak_ptr<ForwardCameraRenderingPath> vwp_forwardPath;
};

}
}

#endif // !define RENDERINGPATH_H

