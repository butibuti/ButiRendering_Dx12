#pragma once
#include"ButiMath/ButiMath.h"
#include"ButiUtil/ButiUtil/ObjectFactory.h"
#include"ButiUtil/ButiUtil/Util.h"
#include<vector>
#include<map>
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"DrawSettings.h"
#include"ResourceInterface.h"
namespace ButiEngine {
namespace ButiRendering {
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
class IResource_Mesh;
class IResource_Material;
class IResource_Texture;
class ICBuffer;
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

	virtual void SetTopologyType(const TopologyType arg_current_topologyType)=0;
	
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

	void SetClearColor(const Vector4& arg_clearColor) { m_clearColor = arg_clearColor; }
	const Vector4& GetClearColor()const { return m_clearColor; }
	Value_ptr<IRenderTarget> GetDefaultRenderTarget() { return m_defaultRenderTarget; }
	void SetDefaultRenderTarget(Value_ptr<IRenderTarget> arg_renderTargetTexture) {
		m_defaultRenderTarget = arg_renderTargetTexture;
	}
	Value_ptr<IDepthStencil> GetDefaultDepthStencil() { return m_defaultDepthStencil; }
	void SetDefaultDepthStencil(Value_ptr<IDepthStencil> arg_depthStencil) {
		m_defaultDepthStencil=arg_depthStencil;
	}

	void SetMesh(Value_ptr<IResource_Mesh> arg_vlp_mesh,const std::int32_t arg_currentMeshVertexType);
	void SetMaterial(Value_ptr<IResource_Material> arg_vlp_material);
	void SetTexture(Value_ptr<IResource_Texture> arg_vlp_texture, const std::int32_t arg_textureRegisterIndex);
	virtual void SetSamplerState(SamplerState arg_samplerState, const std::int32_t arg_samplerStateRegisterIndex)=0;
	void SetConstantBuffer(Value_ptr<ICBuffer> arg_vlp_cBuffer,const std::int32_t arg_cBufferRegisterIndex);

	Value_ptr<IResource_Mesh>GetCurrentMesh()const { return m_vlp_currentMesh; }
	Value_ptr<IResource_Material>GetCurrentMaterial()const { return m_vlp_currentMaterial; }
	const List<Value_ptr<IResource_Texture>>& GetCurrentTexture()const { return m_list_currentTexture; }
	SamplerState GetCurrentSamplerState(const std::int32_t arg_samplerStateRegisterIndex)const { return m_map_currentSamplerState.at(arg_samplerStateRegisterIndex); }
	Value_ptr<ICBuffer>GetCurrentConstatntBuffer(const std::int32_t arg_cBufferRegisterIndex)const { return m_map_currentConstancBuffer.at(arg_cBufferRegisterIndex); }
	TopologyType GetTopologyType()const { return m_current_topologyType; }
protected:
	Value_ptr<IRenderTarget> m_defaultRenderTarget;
	Value_ptr<IDepthStencil> m_defaultDepthStencil;
	Value_ptr<IResource_Mesh> m_vlp_currentMesh;
	Value_ptr<IResource_Material> m_vlp_currentMaterial;
	std::map<std::int32_t, Value_ptr< ICBuffer>> m_map_currentConstancBuffer;
	std::map<std::int32_t, SamplerState> m_map_currentSamplerState;
	List<Value_ptr<IResource_Texture>> m_list_currentTexture;
	Matrix4x4 projectionMatrix;
	TopologyType m_current_topologyType;
	Matrix4x4 viewMatrix, viewMatrix_billBoard, viewMatrix_billBoardX, viewMatrix_billBoardY, viewMatrix_billBoardZ, rawViewMatrix;
	std::int32_t m_currentMeshVertexType=0;
	Vector4 m_clearColor = Vector4(1, 1, 1, 1);
	Vector3 cameraPos;
};
BUTIRENDERING_API Value_ptr<GraphicDevice> CreateGraphicDevice(const bool arg_isWindowApp);
}
}