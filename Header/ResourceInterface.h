#ifndef BUTIRENDERING_RESOURCEEINTERFACE_H
#define BUTIRENDERING_RESOURCEEINTERFACE_H
#include<string>
#include<vector>
#include"ButiMath/ButiMath.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"ButiUtil/ButiUtil/ObjectFactory.h"
#include"ShaderVariable.h"
namespace ButiEngine {

namespace ImageFileIO {
struct TextureResourceData;
}
namespace Vertex {
enum class VertexType :std::uint8_t;
}

namespace ButiRendering {


class RenderingPathData;
class IRenderTarget;
class IDepthStencil;
/// <summary>
/// 描画パスのインターフェース
/// </summary>
class IRenderingPath :public IObject {
public:
	void Initialize()override {}
	void PreInitialize()override {}
	/// <summary>
	/// 実行
	/// </summary>
	virtual void Execute() = 0;
	/// <summary>
	/// ゲームシーン実行中に有効な描画パスなら起動、違ったら停止
	/// </summary>
	virtual void ActivatePlayScene() = 0;
	/// <summary>
	/// ゲームシーン編集中に有効な描画パスなら起動、違ったら停止
	/// </summary>
	virtual void ActivateEditScene() = 0;
	/// <summary>
	/// 名前の設定
	/// </summary>
	/// <param name="arg_name">名前</param>
	virtual void SetName(const std::string& arg_name) = 0;
	/// <summary>
	/// 名前の取得
	/// </summary>
	/// <returns>名前</returns>
	virtual const std::string& GetName()const = 0;
	/// <summary>
	/// 描画結果を出力するレンダーターゲットテクスチャの追加
	/// </summary>
	/// <param name="arg_renderTarget">描画結果を出力するレンダーターゲットテクスチャ</param>
	virtual void PushRenderTarget(Value_ptr<IRenderTarget> arg_renderTarget) = 0;
	/// <summary>
	/// 描画結果を出力するレンダーターゲットテクスチャの削除
	/// </summary>
	/// <param name="arg_renderTarget">描画結果を出力するレンダーターゲットテクスチャ</param>
	virtual void RemoveRenderTarget(Value_ptr<IRenderTarget> arg_renderTarget) = 0;
	/// <summary>
	/// 参照、出力する深度テクスチャの設定
	/// </summary>
	/// <param name="arg_depthStencil">参照、出力する深度テクスチャ</param>
	virtual void SetDepthStencil(Value_ptr<IDepthStencil> arg_depthStencil) = 0;
	virtual void SetOrder(const std::int32_t arg_order) = 0;
	virtual std::int32_t GetOrder()const = 0;
	virtual void SetLayer(const std::int32_t arg_layerIndex) = 0;
	virtual std::int32_t GetLayer()const = 0;
	virtual void SetIsShadowRendering(const bool arg_isShadpwRendering) = 0;
	virtual bool IsShadowRendering()const = 0;
	virtual void SetIsPlayActive(const bool arg_isActive) = 0;
	virtual bool IsPlayActive()const = 0;
	virtual void SetIsEditActive(const bool arg_isActive) = 0;
	virtual bool IsEditActive()const = 0;
	virtual void SetIsCurrentActive(const bool arg_isActive) = 0;
	virtual bool IsCurrentActive()const = 0;

};
class ModelAnimation;
struct MotionKeyFrameData;
struct Bone;
namespace Morph {
class Morph;
}
class IRenderer;
class MeshPrimitiveBase;
class GraphicDevice;
class IRenderTarget {
public:
	virtual void SetRenderTarget(Vector4& arg_clearColor) = 0;
	virtual void DisSetRenderTarget() = 0;
	virtual void SetIsCleared(bool arg_isClear) = 0;
	virtual Vector2 GetSize() = 0;
	virtual std::string GetName()const = 0;
};
class IDepthStencil {
public:
	virtual void SetDepthStencilView() = 0;
	virtual void DisSetDepthStencil() = 0;
	virtual void SetIsCleared(bool arg_isClear) = 0;
	virtual Vector2 GetSize() = 0;
	virtual std::string GetName()const = 0;
};
/// <summary>
/// DescriptorHeapの延長時にリアクションを行うオブジェクトのインターフェース
/// </summary>
class IDescriptorHeapUpdateListner {
public:
	virtual void OnDescriptorHeapUpdate() = 0;
};
class IResource_Motion :public IObject {
public:
	virtual Value_ptr<ModelAnimation> GetAnimation() = 0;
	virtual void AddKeyFrame(const std::string& arg_boneName, const MotionKeyFrameData& arg_data) = 0;
	virtual void AddKeyFrameLane(const std::string& arg_boneName, const std::vector<MotionKeyFrameData>& arg_datas) = 0;
};
class IResource_VertexShader :public IObject {
public:
	virtual void Attach()const = 0;
	virtual std::string GetName()const = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	virtual std::uint32_t GetInputVertexType()const = 0;
};
class IResource_PixelShader :public IObject {
public:
	virtual void Attach()const = 0;
	virtual std::string GetName()const = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	virtual std::vector<std::uint32_t> GetOutputPixelFormat()const = 0;
};
class IResource_GeometryShader :public IObject {
public:
	virtual void Attach()const = 0;
	virtual std::string GetName()const = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
};

class IResource_Shader :public IObject {
public:
	virtual void Attach()const = 0;

	virtual bool GetIsContainGeometryShader()const = 0;

	virtual std::string GetShaderName()const = 0;

	virtual Value_ptr<IResource_VertexShader> GetVertexShader() = 0;
	virtual Value_ptr<IResource_PixelShader> GetPixelShader() = 0;
	virtual Value_ptr<IResource_GeometryShader> GetGeometryShader() = 0;
};

class IResource_Texture :public IObject {
public:
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	virtual void Attach(std::int32_t slot) = 0;
	virtual UInt2 GetSize() const = 0;
	virtual std::int32_t GetDataSize() const = 0;
	virtual std::int32_t GetStride() const = 0;
	virtual std::int32_t GetFormat()const = 0;
	virtual void SetFilePath(const std::string& arg_textureFilePath) = 0;
	virtual void ToPNG(const std::string& arg_textureFilePath) {}
	virtual void* GetResourcePtr() { return nullptr; }
	virtual const std::string& GetTexturePath() const = 0;
	virtual const unsigned char* GetRawData() = 0;
	virtual const std::string& GetTagName()const = 0;
	virtual void SetEditorViewed(const bool arg_isViewed) {}
	virtual bool GetEditorViewed()const { return false; }
	virtual void Wait() {}
};
class IResource_Material :public IObject {
public:
	virtual void Attach(const std::uint32_t arg_slotOffset, Value_ptr<IRenderer> arg_vlp_renderer) = 0;
	virtual const std::string& GetMaterialName() = 0;
	virtual const std::string& GetMaterialNameEng() = 0;
	virtual const std::string& GetMaterialComment() = 0;
	virtual void SetMaterialName(const std::string& arg_name) = 0;
	virtual void SetMaterialNameEng(const std::string& arg_name) = 0;
	virtual void SetComment(const std::string& arg_comment) = 0;
	virtual const MaterialValue& GetMaterialVariable()const = 0;
	virtual MaterialValue& GetMaterialVariable()= 0;
	virtual Value_weak_ptr<IResource_Texture> GetTexture(const std::int32_t arg_index = 0)const = 0;
	virtual Value_weak_ptr<IResource_Texture> GetSphereTexture()const = 0;
	virtual void Update() = 0;
	virtual std::uint32_t GetTextureCount()const = 0;
	virtual void SetMaterialIndex(const std::int32_t arg_index) = 0;
	virtual void SetTexture(const std::uint32_t arg_index, Value_weak_ptr<IResource_Texture>  arg_vwp_texture) = 0;
	virtual void SetTexture(const List<Value_weak_ptr<IResource_Texture>>& arg_list_vwp_textures) = 0;
	virtual void SetSphereTexture(Value_weak_ptr<IResource_Texture>  arg_vwp_texture) = 0;
	virtual void SetMaterialVariable(const MaterialValue& arg_var) = 0;
	virtual MaterialValue_Deferred GetMaterialDeferredValue()const = 0;
	virtual const List<Value_weak_ptr<IResource_Texture>>& GetTextures()const = 0;
	virtual const std::string& GetTagName()const = 0;
	virtual void SetMaterialList(const List<Value_ptr<IResource_Material>>& arg_list_material){}
};
class IResource_Mesh :public IObject {
public:
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	virtual void SetBackupData(Value_ptr<MeshPrimitiveBase> arg_vlp_meshPrimitive) = 0;
	virtual void Draw(const std::uint32_t arg_vertexType) = 0;
	virtual std::uint32_t GetVertexCount() = 0;
	virtual std::uint32_t GetIndexCount() = 0;
	virtual bool GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line) = 0;
	virtual const MeshPrimitiveBase* GetBackUpdata(std::uint32_t arg_vertexType)const = 0;
	virtual const std::string& GetTagName()const = 0;
};
class IResource_Model :public IObject {
public:
	virtual void SetMesh(const Value_weak_ptr< IResource_Mesh>& arg_vwp_mesh) = 0;
	virtual void SetMaterial(const List<Value_weak_ptr< IResource_Material>>& arg_list_vwp_material) = 0;
	virtual void SetName(const std::string& arg_name) = 0;
	virtual void SetEngName(const std::string& arg_engName) = 0;
	virtual void SetModelName(const std::string& arg_modelName) = 0;
	virtual void SetEngModelName(const std::string& arg_engModelName) = 0;
	virtual void SetComment(const std::string& arg_comment) = 0;
	virtual void SetEngComment(const std::string& arg_engComment) = 0;
	virtual void AddBone(Bone& arg_bone) = 0;
	virtual void AddMorph(Value_ptr<Morph::Morph> arg_morph) = 0;
	virtual void SetVersion(const float arg_version) = 0;
	virtual void SetSubset(const std::vector<std::uint32_t>& arg_subset) = 0;
	virtual void SetBone(const List<Bone>& arg_list_bone) = 0;
	virtual std::vector<std::uint32_t> GetSubset() = 0;
	virtual const std::string& GetName() = 0;
	virtual const std::string& GetEngName() = 0;
	virtual const std::string& GetModelName() = 0;
	virtual const std::string& GetEngModelName() = 0;
	virtual const std::string& GetComment() = 0;
	virtual const std::string& GetEngComment() = 0;
	virtual List<Value_ptr<Bone>> GetBone() = 0;
	virtual Value_weak_ptr<IResource_Mesh> GetMesh()const = 0;
	virtual const List<Value_weak_ptr<IResource_Material>>& GetMaterial()const = 0;
	virtual List<Value_weak_ptr<IResource_Material>>& GetMaterial() = 0;
};
BUTIRENDERING_API void ShaderCompile(const std::string& arg_sourceFilePath, const std::string& arg_outputFilePath);
BUTIRENDERING_API Value_ptr<IResource_Model> CreateModel(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const List<Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, const List<Bone>& arg_list_bone,const std::string& arg_name);
BUTIRENDERING_API Value_ptr<IResource_Mesh>CreateMesh(const std::string& arg_meshName, const List< ButiEngine::Value_ptr< ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_Mesh>CreateDynamicMesh(const std::string& arg_meshName, const List< ButiEngine::Value_ptr< ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_Material> CreateMaterial(const MaterialValue& arg_var, Value_weak_ptr<IResource_Texture> arg_texture, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_Material> CreateMaterial(const MaterialValue& arg_var, const List< Value_weak_ptr<IResource_Texture>>& arg_list_texture, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_Texture> CreateTexture(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_Texture> CreateRenderTarget(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, const std::int32_t arg_format, Value_ptr<GraphicDevice> arg_vwp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_Texture> CreateDepthStencil(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData,  Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateVertexShader(const std::string& arg_shaderName, const std::string& arg_filePath, const std::string& arg_inputLayoutfilePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreatePixelShader(const std::string& arg_shaderName, const std::string& arg_filePath, const std::string& arg_outputFormatFilePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_GeometryShader> CreateGeometryShader(const std::string& arg_shaderName, const std::string& arg_filePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
namespace DefaultVertexShader {
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateShadowMap(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateShadowMap_UV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUV_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVAttribute(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormal(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormal_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalAttribute(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalColorFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalColorFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalPhong(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalPhong_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalPosition(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalTangent(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVNormalTangent_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateUVPosition(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
}
namespace DefaultPixelShader {
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateGrid(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateGrid_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateGrid_worldColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateGridFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateOnlyMaterial(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateOnlyMaterial_BloomSource(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreatePostEffect_GausBlur(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreatePostEffect_GausBlurFloat2(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateShadowMap(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateShadowMap_UV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateSpriteAnimation(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVColor_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVColorFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVColorFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVDepth(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVNormal(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVNormal_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVNormalColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVNormalColor_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVNormalColorFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVNormalColorFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVNormalFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreateUVNormalFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
}
namespace DefaultGeometryShader {
BUTIRENDERING_API Value_ptr<IResource_GeometryShader> CreateOutLine(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_GeometryShader> CreatePointToCube(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_GeometryShader> CreatePointToCube_WithoutUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_GeometryShader> CreatePointToPlane(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_GeometryShader> CreatePointToTriPolygon_WithoutUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
}
BUTIRENDERING_API Value_ptr<IResource_VertexShader> CreateVertexShaderFromSource(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_PixelShader> CreatePixelShaderFromSource(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_GeometryShader> CreateGeometryShaderFromSource(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_vlp_graphicDevice);
BUTIRENDERING_API Value_ptr<IResource_Shader> CreateShader(Value_ptr<IResource_VertexShader> arg_vlp_vertexShader, Value_ptr<IResource_PixelShader> arg_vlp_pixelShader, Value_ptr<IResource_GeometryShader> arg_vlp_geometryShader,const std::string& arg_shaderName);
}
}

#endif