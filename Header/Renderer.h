#pragma once
#include"stdafx.h"
#include"DrawData/IDrawObject.h"
namespace ButiEngine {
namespace ButiRendering {

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
	//所属しているシーンの取得
	virtual Value_ptr<IScene> GetScene() = 0;
	/// <summary>
	/// 描画レイヤーに対するシャドウ用テクスチャの設定
	/// </summary>
	/// <param name="arg_layerIndex">設定するレイヤーの番号</param>
	/// <param name="arg_shadowTex">設定するシャドウ用テクスチャ</param>
	virtual void SetShadowTexture(const std::uint32_t arg_layerIndex, TextureTag arg_shadowTex) = 0;
	/// <summary>
	/// 描画レイヤーが使用するシャドウ用テクスチャの取得
	/// </summary>
	/// <param name="arg_layerIndex">レイヤーの番号</param>
	/// <returns>使用しているシャドウ用テクスチャ</returns>
	virtual TextureTag GetShadowTexture(const std::uint32_t arg_layerIndex) = 0;
	/// <summary>
	/// 描画オブジェクトの登録
	/// </summary>
	/// <param name="arg_vlp_drawObject">描画オブジェクト</param>
	/// <param name="arg_afterDraw">遅らせて描画するか(半透明オブジェクト等)</param>
	/// <param name="arg_layerIndex">登録する描画レイヤーの番号</param>
	/// <param name="isShadow">影を落とすか</param>
	virtual void RegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_afterDraw, const std::uint32_t arg_layerIndex = 0, const bool isShadow = false) = 0;
	/// <summary>
	/// 描画オブジェクトの登録解除
	/// </summary>
	/// <param name="arg_vlp_drawObject">描画オブジェクト</param>
	/// <param name="arg_afterDraw">遅らせて描画するか(半透明オブジェクト等)</param>
	/// <param name="arg_layerIndex">登録する描画レイヤーの番号</param>
	/// <param name="isShadow">影を落とすか</param>
	virtual void UnRegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_afterDraw, const std::uint32_t arg_layerIndex = 0, const bool isShadow = false) = 0;
	/// <summary>
	/// GUIの呼び出し
	/// </summary>
	virtual void ShowUI() = 0;
	/// <summary>
	/// 解放処理
	/// </summary>
	virtual void Release() = 0;
	/// <summary>
	/// Renderer制御の定数バッファ更新
	/// </summary>
	/// <param name="arg_param">更新する値</param>
	virtual void UpdateRendererCBuffer(const RenderingSceneInfo& arg_param) = 0;
	/// <summary>
	/// Renderer制御の定数バッファ解放
	/// </summary>
	virtual void ReleaseRendererCBuffer() = 0;
	/// <summary>
	/// Renderer制御の定数バッファ取得
	/// </summary>
	/// <returns>Renderer制御の定数バッファ</returns>
	virtual Value_ptr<CBuffer<RenderingSceneInfo>> GetRendererCBuffer() = 0;
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
	/// <summary>
	/// 描画オブジェクトとの衝突判定（現在使用不可）
	/// </summary>
	/// <param name="arg_prim">衝突判定に使う形状</param>
	/// <param name="arg_layer">判定するレイヤーの番号</param>
	/// <returns>衝突している描画オブジェクトのvector</returns>
	virtual std::vector< Value_ptr<IDrawObject>> GetHitDrawObjects(Value_ptr<Collision::CollisionPrimitive> arg_prim, const std::int32_t arg_layer) = 0;
	/// <summary>
	/// 使用しているResourceContainerの取得
	/// </summary>
	/// <returns>使用しているResourceContainer</returns>
	virtual Value_ptr<IResourceContainer> GetResourceContainer() = 0;
};

/// <summary>
/// 描画オブジェクトレイヤーのインターフェース
/// </summary>
class IDrawLayer :public IObject {
public:

	void Initialize()override {};
	void PreInitialize()override {}

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
	/// <param name="arg_isAfterRendering">遅らせて描画するか(半透明オブジェクト等)</param>
	/// <param name="arg_ret_pim">描画オブジェクトの形状</param>
	/// <param name="arg_isShadow">影を生成するオブジェクトか</param>
	virtual void Regist(Value_ptr< IDrawObject> arg_vwp_drawObject, const bool arg_isAfterRendering, Value_ptr<Collision::CollisionPrimitive_Box_OBB> arg_ret_pim = nullptr, const bool arg_isShadow = false) = 0;
	/// <summary>
	/// 描画オブジェクトの登録解除
	/// </summary>
	/// <param name="arg_vlp_drawObject">描画オブジェクト</param>
	/// <param name="arg_isAfterRendering">描画を遅らせるか</param>
	/// <param name="arg_isShadow">影を生成するオブジェクトか</param>
	virtual void UnRegist(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_isAfterRendering, const bool arg_isShadow = false) = 0;
	/// <summary>
	/// シャドウ撮影用カメラの登録
	/// </summary>
	/// <param name="arg_shadowCamera"></param>
	virtual void SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera) = 0;
	/// <summary>
	/// シャドウ読み込みテクスチャの登録
	/// </summary>
	/// <param name="arg_textureTag"></param>
	virtual void SetShadowTexture(TextureTag arg_textureTag) = 0;
	/// <summary>
	/// レンダリングの実行
	/// </summary>
	virtual void Rendering() = 0;
	/// <summary>
	/// 描画コマンドの構築
	/// </summary>
	virtual void CommandSet() = 0;
	/// <summary>
	/// 影のレンダリング
	/// </summary>
	virtual void ShadowRendering() = 0;
	/// <summary>
	/// 衝突レイヤーの取得(現在使用不可)
	/// </summary>
	/// <returns></returns>
	virtual Value_ptr<Collision::CollisionLayer<IDrawObject>> GetCollisionLayer() = 0;
	/// <summary>
	/// シャドウ用カメラの取得
	/// </summary>
	/// <returns></returns>
	virtual Value_ptr<ICamera> GetShadowCamera() = 0;
	/// <summary>
	/// シャドウ用テクスチャの取得
	/// </summary>
	/// <returns></returns>
	virtual TextureTag GetShadowTexture() = 0;
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

Value_ptr<IRenderer> CreateRenderer(Value_weak_ptr<IScene> arg_vwp_iscene);
}
}