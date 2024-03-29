#pragma once
#ifndef RENDERER_H
#define RENDERER_H
//#include"stdafx.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"DrawData/IDrawObject.h"
#include<functional>
namespace ButiEngine {
namespace ButiRendering {
class IDrawLayer;
class ICamera;
class RendererState;
template <typename T>class CBuffer;
/// <summary>
/// シーンが所持する描画用クラス
/// </summary>
class IRenderer :public IObject {
public:
	void PreInitialize()override {}
	virtual void Update() = 0;

	/// <summary>
	/// 全体のレンダリング開始
	/// </summary>
	virtual void RenderingStart() = 0;
	/// <summary>
	/// レンダリング開始前の処理
	/// </summary>
	virtual void BefRendering() = 0;
	/// <summary>
	/// 描画レイヤの追加
	/// </summary>
	virtual void AddLayer() = 0;
	/// <summary>
	/// レイヤーの数の取得
	/// </summary>
	/// <returns>レイヤー数</returns>
	virtual std::uint32_t GetLayerCount() = 0;
	virtual Value_ptr<IDrawLayer> GetDrawLayer(const std::int32_t arg_index)const=0;
	virtual void RemoveDrawLayer(const std::int32_t arg_index)=0;
	/// <summary>
	/// 描画レイヤーのレンダリング
	/// </summary>
	/// <param name="arg_layerIndex">描画するレイヤーの番号</param>
	virtual void Rendering(const std::uint32_t arg_layerIndex) = 0;
	/// <summary>
	/// 描画レイヤーの影のレンダリング
	/// </summary>
	/// <param name="arg_layerIndex">描画するレイヤーの番号</param>
	virtual void ShadowRendering(const std::uint32_t arg_layerIndex) = 0;
	/// <summary>
	/// 全体のレンダリング終了
	/// </summary>
	virtual void RenderingEnd() = 0;
	/// <summary>
	/// 描画オブジェクトの解放
	/// </summary>
	virtual void ClearDrawObjects() = 0;
	/// <summary>
	/// 描画レイヤーに対するシャドウ用テクスチャの設定
	/// </summary>
	/// <param name="arg_layerIndex">設定するレイヤーの番号</param>
	/// <param name="arg_shadowTex">設定するシャドウ用テクスチャ</param>
	virtual void SetShadowTexture(const std::uint32_t arg_layerIndex, Value_weak_ptr<IResource_Texture> arg_shadowTex) = 0;
	/// <summary>
	/// 描画レイヤーが使用するシャドウ用テクスチャの取得
	/// </summary>
	/// <param name="arg_layerIndex">レイヤーの番号</param>
	/// <returns>使用しているシャドウ用テクスチャ</returns>
	virtual Value_ptr<IResource_Texture> GetShadowTexture(const std::uint32_t arg_layerIndex) = 0;
	/// <summary>
	/// 描画オブジェクトの登録
	/// </summary>
	/// <param name="arg_vlp_drawObject">描画オブジェクト</param>
	/// <param name="arg_layerIndex">登録する描画レイヤーの番号</param>
	/// <param name="isShadow">影を落とすか</param>
	virtual void RegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const std::uint32_t arg_layerIndex = 0, const bool isShadow = false) = 0;
	/// <summary>
	/// 描画オブジェクトの登録解除
	/// </summary>
	/// <param name="arg_vlp_drawObject">描画オブジェクト</param>
	/// <param name="arg_layerIndex">登録する描画レイヤーの番号</param>
	/// <param name="isShadow">影を落とすか</param>
	virtual void UnRegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const std::uint32_t arg_layerIndex = 0, const bool isShadow = false) = 0;
	/// <summary>
	/// 解放処理
	/// </summary>
	virtual void Release() = 0;
	/// <summary>
	/// Renderer制御の定数バッファ更新
	/// </summary>
	/// <param name="arg_param">更新する値</param>
	virtual void UpdateRendererCBuffer(const RendererState& arg_param) = 0;
	/// <summary>
	/// Renderer制御の定数バッファ解放
	/// </summary>
	virtual void ReleaseRendererCBuffer() = 0;
	/// <summary>
	/// Renderer制御の定数バッファ取得
	/// </summary>
	/// <returns>Renderer制御の定数バッファ</returns>
	virtual Value_ptr<CBuffer<RendererState>> GetRendererCBuffer() = 0;
	/// <summary>
	/// シャドウ用カメラの設定
	/// </summary>
	/// <param name="arg_layer">設定するレイヤーの番号</param>
	/// <param name="arg_vlp_camera">設定するカメラ</param>
	virtual void SetShadowCamera(const std::uint32_t arg_layer, Value_ptr<ICamera> arg_vlp_camera) = 0;
	/// <summary>
	/// シャドウ用カメラの取得
	/// </summary>
	/// <param name="arg_layer">取得するレイヤーの番号</param>
	/// <returns>シャドウ用カメラ</returns>
	virtual Value_ptr<ICamera> GetShadowCamera(const std::uint32_t arg_layer) = 0;
	virtual Value_ptr<GraphicDevice> GetGraphicDevice() const= 0;
	virtual void PushDrawFunction(std::function<void()> arg_drawFunction) = 0;
	virtual void PopDrawFunction() = 0;
	static Value_ptr<CBuffer<RendererState>> GetRendererStateCBuffer();
};

/// <summary>
/// 描画オブジェクトレイヤーのインターフェース
/// </summary>
class IDrawLayer :public IObject {
public:
	/// <summary>
	/// 中身の削除
	/// </summary>
	virtual void Clear() = 0;
	/// <summary>
	/// レンダリング処理の前に行う処理を走らせる
	/// </summary>
	virtual void BefRendering() = 0;
	/// <summary>
	/// 描画オブジェクトを登録
	/// </summary>
	/// <param name="arg_vwp_drawObject">描画オブジェクト</param>
	/// <param name="arg_ret_pim">描画オブジェクトの形状</param>
	/// <param name="arg_isShadow">影を生成するオブジェクトか</param>
	virtual void Regist(Value_ptr< IDrawObject> arg_vwp_drawObject, const bool arg_isShadow = false) = 0;
	/// <summary>
	/// 描画オブジェクトの登録解除
	/// </summary>
	/// <param name="arg_vlp_drawObject">描画オブジェクト</param>
	/// <param name="arg_isShadow">影を生成するオブジェクトか</param>
	virtual void UnRegist(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_isShadow = false) = 0;
	/// <summary>
	/// シャドウ撮影用カメラの登録
	/// </summary>
	/// <param name="arg_shadowCamera"></param>
	virtual void SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera) = 0;
	/// <summary>
	/// シャドウ読み込みテクスチャの登録
	/// </summary>
	/// <param name="arg_texture"></param>
	virtual void SetShadowTexture(Value_weak_ptr<IResource_Texture> arg_texture) = 0;
	/// <summary>
	/// レンダリングの実行
	/// </summary>
	virtual void Rendering() = 0;
	/// <summary>
	/// 描画コマンドの構築
	/// </summary>
	virtual void CreateCommandBundle() = 0;
	/// <summary>
	/// 影のレンダリング
	/// </summary>
	virtual void ShadowRendering() = 0;
	/// <summary>
	/// シャドウ用カメラの取得
	/// </summary>
	/// <returns></returns>
	virtual Value_ptr<ICamera> GetShadowCamera() = 0;
	/// <summary>
	/// シャドウ用テクスチャの取得
	/// </summary>
	/// <returns></returns>
	virtual Value_ptr<IResource_Texture> GetShadowTexture() = 0;

	virtual std::int32_t GetRenderTargetSize()const = 0;
};
/// <summary>
/// 影用の描画レイヤー
/// </summary>
class IDrawLayer_Shadow {
public:
	/// <summary>
	/// 影を書き込んだかの取得
	/// </summary>
	/// <returns></returns>
	virtual bool IsShadowDrawed() = 0;
};

BUTIRENDERING_API Value_ptr<IRenderer> CreateRenderer(Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
}
}

#endif // !RENDERER_H
