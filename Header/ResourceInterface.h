#ifndef BUTIRENDERING_RESOURCEEINTERFACE_H
#define BUTIRENDERING_RESOURCEEINTERFACE_H
#include<string>
#include<vector>
#include"ButiMath/ButiMath.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"ButiUtil/ButiUtil/ObjectFactory.h"
#include"ButiUtil/ButiUtil/ID.h"
#include"ShaderVariable.h"
namespace ButiEngine {

namespace Vertex {
enum class VertexType :std::uint8_t;
}

namespace ButiRendering {
class IResource_Mesh;
class IResource_Motion;
class IResource_VertexShader;
class IResource_PixelShader;
class IResource_GeometryShader;
class IResource_Shader;
class IResource_Texture;
class IResource_Model;
class IResource_Material;
}

using MeshTag = ID<ButiRendering::IResource_Mesh>;
using MotionTag = ID<ButiRendering::IResource_Motion>;
using VertexShaderTag = ID < ButiRendering::IResource_VertexShader > ;
using PixelShaderTag = ID<ButiRendering::IResource_PixelShader>;
using GeometryShaderTag = ID<ButiRendering::IResource_GeometryShader>;
using ShaderTag = ID<ButiRendering::IResource_Shader>;
using TextureTag = ID<ButiRendering::IResource_Texture>;
using ModelTag = ID<ButiRendering::IResource_Model>;
using MaterialTag = ID<ButiRendering::IResource_Material>;
namespace ButiRendering {


class RenderingPathInfo;
/// <summary>
/// 描画パスのインターフェース
/// </summary>
class IRenderingPath :public IObject {
public:
	void Initialize()override {}
	void PreInitialize()override {}
	/// <summary>
	/// フレームの開始時の処理
	/// </summary>
	virtual void BefExecute() = 0;
	/// <summary>
	/// 実行
	/// </summary>
	virtual void Execute() = 0;
	/// <summary>
	/// フレームの終了時の処理
	/// </summary>
	virtual void End() = 0;
	/// <summary>
	/// 解放処理
	/// </summary>
	virtual void Release() = 0;
	/// <summary>
	/// GUI呼び出し
	/// </summary>
	virtual void OnShowGUI() = 0;
	/// <summary>
	/// ゲームシーン実行中に有効な描画パスなら起動、違ったら停止
	/// </summary>
	virtual void SetPlayActive() = 0;
	/// <summary>
	/// ゲームシーン編集中に有効な描画パスなら起動、違ったら停止
	/// </summary>
	virtual void SetEditActive() = 0;
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
	/// 描画パスの保存データの取得
	/// </summary>
	/// <returns>描画パスの保存データ</returns>
	virtual Value_ptr<RenderingPathInfo> GetRenderingPathInfo() = 0;
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
};
class IDepthStencil {
public:
	virtual void SetDepthStencilView() = 0;
	virtual void DisSetDepthStencil() = 0;
	virtual void SetIsCleared(bool arg_isClear) = 0;
	virtual Vector2 GetSize() = 0;
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

	virtual Value_weak_ptr<IResource_VertexShader> GetVertexShader() = 0;
	virtual Value_weak_ptr<IResource_PixelShader> GetPixelShader() = 0;
	virtual Value_weak_ptr<IResource_GeometryShader> GetGeometryShader() = 0;
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
	virtual GUIWindowReaction ShowTextureWindow(const std::string& arg_windowName = "", const std::int32_t winFrag = 0) { return GUIWindowReaction(); }
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
	virtual void SetSphereTexture(Value_weak_ptr<IResource_Texture>  arg_vwp_texture) = 0;
	virtual void SetMaterialVariable(const MaterialValue& arg_var) = 0;
	virtual MaterialValue_Deferred GetMaterialDeferredValue()const = 0;
	virtual const List<Value_weak_ptr<IResource_Texture>>& GetTextures()const = 0;
	virtual const std::string& GetTagName()const = 0;
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

	virtual std::vector<std::uint32_t> GetSubset() = 0;
	virtual const std::string& GetName() = 0;
	virtual const std::string& GetEngName() = 0;
	virtual const std::string& GetModelName() = 0;
	virtual const std::string& GetEngModelName() = 0;
	virtual const std::string& GetComment() = 0;
	virtual const std::string& GetEngComment() = 0;
	virtual std::vector<Value_ptr<Bone>> GetBone() = 0;
	virtual Value_weak_ptr<IResource_Mesh> GetMesh()const = 0;
	virtual const List<Value_weak_ptr<IResource_Material>>& GetMaterial()const = 0;
	virtual List<Value_weak_ptr<IResource_Material>>& GetMaterial() = 0;
};
}
}

#endif