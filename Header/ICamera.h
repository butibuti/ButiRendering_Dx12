#ifndef ICAMERA_H
#define ICAMERA_H
#include"ButiMath/ButiMath.h"
#include"ButiMath/Transform.h"
#include <vector>
#include"ResourceInterface.h"
#include"RenderingStatus.h"
namespace ButiEngine {
namespace Geometry {
class Box_AABB;
}
namespace ButiRendering {
class IRenderer;
class GraphicDevice;
struct CameraProperty {
	CameraProperty() {};
	BUTIRENDERING_API CameraProperty(const std::uint32_t arg_widthScale, const std::uint32_t arg_heightScale, const std::uint32_t arg_x, const std::uint32_t arg_y, const bool arg_isPararell = false, std::uint32_t arg_layer = 0);

	std::int32_t currentWidth = 0, currentHeight = 0, width=0, height=0, left = 0, top = 0;
	float front = 0.0f, angle = 60.0f, farClip = 50.0f, nearClip = 0.1f;
	bool isPararell = false;
	std::uint32_t layer = 0;
	std::int32_t renderingInfo = 0;
	std::string cameraName;
	Vector4 clearColor;
	bool isInitActive = true, isEditActive = false, isShadow = false;

	List<Value_weak_ptr<IRenderTarget>> m_list_vlp_renderTarget;
	Value_weak_ptr<IDepthStencil> m_depthStencilTexture;
	List<Value_weak_ptr<IResource_Texture>> m_list_vlp_shadowTexture;
	List<Value_weak_ptr<IResource_Texture>> m_list_vlp_staticShadowTexture;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(width);
		archive(height);
		archive(left);
		archive(top);
		archive(front);
		archive(angle);
		archive(farClip);
		archive(nearClip);
		archive(isPararell);
		archive(layer);
		archive(front);
		archive(renderingInfo);
		archive(cameraName);
		archive(clearColor);
		archive(isInitActive);
		archive(isEditActive);
		archive(isShadow);
	}

};


/// <summary>
/// カメラのインターフェース
/// </summary>
class ICamera :public IObject {
public:

	Value_ptr<Transform> vlp_transform = ObjectFactory::Create<Transform>(Vector3(0, 0, 0));

	/// <summary>
	/// 描画開始
	/// </summary>
	virtual void Start() = 0;
	/// <summary>
	/// 描画停止
	/// </summary>
	virtual void Stop()const = 0;

	virtual void Initialize()override {}
	virtual void PreInitialize()override {}
	/// <summary>
	/// 名前の取得
	/// </summary>
	/// <returns>名前</returns>
	virtual const std::string& GetName() const = 0;
	/// <summary>
	/// 名前の設定
	/// </summary>
	/// <param name="arg_name">名前</param>
	virtual void SetName(const std::string& arg_name) = 0;
	/// <summary>
	/// 実行するかの設定
	/// </summary>
	/// <param name="arg_active"></param>
	virtual void SetActive(const bool arg_active) = 0;
	/// <summary>
	/// 実行するかの取得
	/// </summary>
	/// <returns></returns>
	virtual bool GetActive()const = 0;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// カメラ設定の取得
	/// </summary>
	/// <returns></returns>
	virtual CameraProperty& GetCameraProperty() = 0;
	/// <summary>
	/// カメラ設定の取得
	/// </summary>
	/// <returns></returns>
	virtual const CameraProperty& GetCameraProperty() const= 0;
	/// <summary>
	/// カメラに形状が写っているかの取得
	/// </summary>
	/// <param name="arg_AABB">形状</param>
	/// <returns></returns>
	virtual std::int32_t IsContaineVisibility(Value_ptr<Geometry::Box_AABB>arg_AABB) = 0;
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End() = 0;
	/// <summary>
	/// 描画結果を出力するレンダーターゲットテクスチャの設定
	/// </summary>
	/// <param name="arg_renderTarget">描画結果を出力するレンダーターゲットテクスチャ</param>
	virtual void SetRenderTarget(Value_ptr<IRenderTarget> arg_renderTarget) = 0;
	/// <summary>
	/// 参照、出力する深度テクスチャの設定
	/// </summary>
	/// <param name="arg_depthStencil">参照、出力する深度テクスチャ</param>
	virtual void SetDepthStencil(Value_ptr<IDepthStencil> arg_depthStencil) = 0;
	/// <summary>
	/// 描画前処理
	/// </summary>
	virtual void BefDraw() = 0;
	/// <summary>
	/// VP行列の取得
	/// </summary>
	/// <returns>VP行列</returns>
	virtual Matrix4x4 GetViewProjectionMatrix() = 0;
	/// <summary>
	/// V行列の取得
	/// </summary>
	/// <returns>V行列</returns>
	virtual Matrix4x4 GetViewMatrix() = 0;
	/// <summary>
	/// P行列の取得
	/// </summary>
	/// <returns>P行列</returns>
	virtual Matrix4x4 GetProjectionMatrix() = 0;
	/// <summary>
	/// カメラ位置の取得
	/// </summary>
	/// <returns>カメラ位置</returns>
	virtual Vector3 GetPosition() = 0;
	virtual Vector3 WorldToScreen(const Vector3& arg_pos)const = 0;
	virtual Value_weak_ptr<IRenderer> GetRenderer()const = 0;
	virtual Value_ptr<GraphicDevice> GetGraphicDevice()const = 0;
protected:
};
BUTIRENDERING_API Value_ptr<ICamera> CreateCamera(const CameraProperty& arg_cameraProp, const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
BUTIRENDERING_API Value_ptr<ICamera> CreateCamera(const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
}
}

#endif // !ICAMERA_H
