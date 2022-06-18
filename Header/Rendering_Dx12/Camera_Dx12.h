#ifndef _CAMERA_DX12_H
#define _CAMERA_DX12_H

#include"stdafx.h"
#include"CBuffer_Dx12.h"
#include"../Camera.h"
namespace ButiEngine {

namespace ButiRendering {
class Camera_Dx12 :public Camera
{
public:
	Camera_Dx12(const CameraProperty& arg_cameraProp, const std::string& cameraName, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	~Camera_Dx12();
	void PreInitialize()override {}
	void Stop() const override;
	void End()override;
private:

};
}
}

#endif // !_CAMERA_DX12_H