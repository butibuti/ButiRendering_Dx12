#pragma once
#include"ButiMath/ButiMath.h"
#include"ButiUtil/ButiUtil/ObjectFactory.h"
#include"ButiUtil/ButiUtil/Util.h"
#include<vector>
#include<map>
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"DrawSettings.h"
namespace ButiEngine {
namespace ButiRendering {
class IResource_Texture;
class IDepthStencil;
class IRenderTarget;
struct GPUResource {
	virtual void ResourceUpdate() {}
	virtual void UpdateResourceRelease() {}
	virtual void CopyForOutput() {}
	virtual void DataLock() {}
	virtual void DataUnlock() {}
	virtual void OutputToMemory() {}
	virtual void Swap() {}
};
}
}

namespace ButiEngine {
namespace ButiRendering {
class GraphicDevice :public IObject
{
public:
	virtual void Present() = 0;
	virtual void Release() = 0;
	virtual void ResourceUpload() = 0;
	virtual void UploadResourceRelease() = 0;
	virtual void UploadResourceBufferMerge() = 0;
	virtual void DrawStart() {}
	virtual void SetDefaultRenderTarget() = 0;
	virtual void Reset()=0;
	virtual void ClearWindow() = 0;
	virtual void ClearDepthStancil(const float arg_depth) = 0;
	
	BUTIRENDERING_API Matrix4x4 GetProjectionMatrix();
	BUTIRENDERING_API Matrix4x4 GetCameraViewMatrix();
	BUTIRENDERING_API const Vector3& GetCameraPos();
	BUTIRENDERING_API void SetCameraPos(const Vector3& arg_pos);
	BUTIRENDERING_API Matrix4x4 GetRawViewMatrix();
	virtual void SetWindow(std::int64_t arg_handle, std::int32_t arg_width, std::int32_t arg_height)=0;
	BUTIRENDERING_API void SetProjectionMatrix(const Matrix4x4& arg_projectionMatrix);
	BUTIRENDERING_API void SetViewMatrix(const Matrix4x4& arg_viewMatrix);
	BUTIRENDERING_API void SetRawViewMatrix(const Matrix4x4& arg_viewMatrix);
	BUTIRENDERING_API void SetViewMatrix_billBoard(const Matrix4x4& arg_viewMatrix);
	BUTIRENDERING_API void SetViewMatrix_billBoardX(const Matrix4x4& arg_viewMatrix);
	BUTIRENDERING_API void SetViewMatrix_billBoardY(const Matrix4x4& arg_viewMatrix);
	BUTIRENDERING_API void SetViewMatrix_billBoardZ(const Matrix4x4& arg_viewMatrix);
	virtual void ResetPipeLine() = 0;
	virtual void PipelineClear() = 0;

	const Matrix4x4& GetViewMatrixBillBoard() {
		return viewMatrix_billBoard;
	}
	const Matrix4x4& GetViewMatrixBillBoardX() {
		return viewMatrix_billBoardX;
	}
	const Matrix4x4& GetViewMatrixBillBoardY() {
		return viewMatrix_billBoardY;
	}
	const Matrix4x4& GetViewMatrixBillBoardZ() {
		return viewMatrix_billBoardZ;
	}

	BUTIRENDERING_API void SetClearColor(const Vector4& arg_clearColor);
	BUTIRENDERING_API Vector4 GetClearColor();
	Value_ptr<IRenderTarget> GetDefaultRenderTarget() { return m_defaultRenderTarget; }
	void SetDefaultRenderTarget(Value_ptr<IRenderTarget> arg_renderTargetTexture) {
		m_defaultRenderTarget = arg_renderTargetTexture;
	}
	Value_ptr<IDepthStencil> GetDefaultDepthStencil() { return m_defaultDepthStencil; }
	void SetDefaultDepthStencil(Value_ptr<IDepthStencil> arg_depthStencil) {
		m_defaultDepthStencil=arg_depthStencil;
	}
protected:
	Value_ptr<IRenderTarget> m_defaultRenderTarget;
	Value_ptr<IDepthStencil> m_defaultDepthStencil;
	Matrix4x4 projectionMatrix;

	Matrix4x4 viewMatrix;
	Matrix4x4 viewMatrix_billBoard;
	Matrix4x4 viewMatrix_billBoardX;
	Matrix4x4 viewMatrix_billBoardY;
	Matrix4x4 viewMatrix_billBoardZ;
	Matrix4x4 rawViewMatrix;

	Vector4 clearColor = Vector4(1, 1, 1, 1);
	Vector3 cameraPos;
};
BUTIRENDERING_API Value_ptr<GraphicDevice> CreateGraphicDevice(const bool arg_isWindowApp);
}
}