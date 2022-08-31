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
	BUTIRENDERING_API void ActivatePlayScene()override;
	BUTIRENDERING_API void ActivateEditScene()override;
	void SetOrder(const std::int32_t arg_order)override { m_order = arg_order; }
	std::int32_t GetOrder()const override { return m_order; }
	void SetLayer(const std::int32_t arg_layerIndex)override { m_layer = arg_layerIndex; }
	std::int32_t GetLayer()const override { return m_layer; }
	void SetIsShadowRendering(const bool arg_isShadpwRendering) override { m_isShadowRendering = arg_isShadpwRendering; }
	bool IsShadowRendering()const override { return m_isShadowRendering; }
	void SetIsPlayActive(const bool arg_isActive) override { m_isPlayActive = arg_isActive; }
	bool IsPlayActive()const override { return m_isPlayActive; }
	void SetIsEditActive(const bool arg_isActive) override { m_isEditActive = arg_isActive; }
	bool IsEditActive()const override { return m_isEditActive; }
	void SetIsCurrentActive(const bool arg_isActive) override { m_isCurrentActive= arg_isActive; }
	bool IsCurrentActive()const override { return m_isCurrentActive; }

	void PushRenderTarget(Value_ptr<IRenderTarget> arg_renderTarget) override{ m_list_vlp_renderTarget.Add(arg_renderTarget); }
	void RemoveRenderTarget(Value_ptr<IRenderTarget> arg_renderTarget)override { m_list_vlp_renderTarget.Remove(arg_renderTarget); }
	void SetDepthStencil(Value_ptr<IDepthStencil> arg_depthStencil) override{ m_depthStencilTexture = arg_depthStencil; }

	Value_ptr<IDepthStencil> GetDepthStencil()const { return m_depthStencilTexture; }
	List<Value_ptr<IRenderTarget>>&GetRenderTargets() { return m_list_vlp_renderTarget; }
	const List<Value_ptr<IRenderTarget>>& GetRenderTargets()const override { return m_list_vlp_renderTarget; }
protected:
	Value_ptr<ICamera> vlp_camera;
	std::int32_t m_order=0,m_layer=0;
	bool m_isPlayActive = true, m_isEditActive = false, m_isShadowRendering = false,m_isCurrentActive=true;
	List<Value_ptr<IRenderTarget>> m_list_vlp_renderTarget;
	Value_ptr<IDepthStencil> m_depthStencilTexture;
	List<Value_ptr<IResource_Texture>> m_list_vlp_shadowTexture;
	List<Value_ptr<IResource_Texture>> m_list_vlp_staticShadowTexture;
};

class ForwardCameraRenderingPath :public CameraRenderingPath {
public:
	BUTIRENDERING_API ForwardCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera,Value_weak_ptr<IRenderer> arg_vwp_renderer);
	Value_weak_ptr<IRenderer> GetRenderer()const { return vwp_renderer; }
protected:
	Value_weak_ptr<IRenderer> vwp_renderer;
};

class DeferredCameraRenderingPath :public CameraRenderingPath {
public:
	BUTIRENDERING_API DeferredCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer, Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath);
	void SetForwardPath(Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath) {
		vwp_forwardPath = arg_vwp_forwardPath;
	}
	Value_weak_ptr<ForwardCameraRenderingPath> GetForwardPath() { return vwp_forwardPath; }
	Value_weak_ptr<IRenderer> GetRenderer()const { return vwp_renderer; }
protected:
	Value_ptr<ICamera> vlp_forwardCamera;
	Value_weak_ptr<IRenderer> vwp_renderer;
	Value_weak_ptr<ForwardCameraRenderingPath> vwp_forwardPath;
};
BUTIRENDERING_API Value_ptr<ForwardCameraRenderingPath> CreateForwardRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer);
BUTIRENDERING_API Value_ptr<DeferredCameraRenderingPath> CreateDeferredRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_weak_ptr<IRenderer> arg_vwp_renderer, Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath);
BUTIRENDERING_API Value_ptr<ForwardCameraRenderingPath> CreateForwardRenderingPath(Value_ptr<DeferredCameraRenderingPath> arg_path);
BUTIRENDERING_API Value_ptr<DeferredCameraRenderingPath> CreateDeferredRenderingPath(Value_ptr<ForwardCameraRenderingPath> arg_path, Value_weak_ptr<ForwardCameraRenderingPath> arg_vwp_forwardPath);
}
}

#endif // !define RENDERINGPATH_H

