#pragma once
#include"stdafx.h"
#include<algorithm>
#include"ButiRendering_Dx12/Header/Renderer.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/CBuffer_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"


namespace ButiEngine {
namespace ButiRendering {
struct RegistCommand {
	bool isAfter;
	bool isShadow;
	Value_ptr< IDrawObject> vlp_obj = nullptr;
	bool isRegist = false;
};

class Renderer :public IRenderer
{
public:
	Renderer(Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
	void Initialize()override;
	void Update()override;
	void RenderingStart()override;
	void BefRendering()override;
	void AddLayer()override;
	std::uint32_t GetLayerCount() override;
	void Rendering(const std::uint32_t arg_layer)override;
	void ShadowRendering(const std::uint32_t arg_layer)override;
	void RenderingEnd()override;
	void ClearDrawObjects()override;
	void SetShadowCamera(const std::uint32_t arg_layer, Value_ptr<ICamera> arg_vlp_camera)override;
	void SetShadowTexture(const std::uint32_t arg_layer, Value_weak_ptr<IResource_Texture> arg_shadowTex)override;
	Value_ptr<IResource_Texture> GetShadowTexture(const std::uint32_t arg_layer)override;
	Value_ptr<ICamera> GetShadowCamera(const std::uint32_t arg_layer)override;
	void RegistDrawObject(Value_ptr< IDrawObject> arg_vwp_drawObject, const bool arg_afterDraw, const std::uint32_t arg_layer = 0, const bool isShadow = false)override;
	void UnRegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_afterDraw, const std::uint32_t arg_layer = 0, const bool isShadow = false)override;
	void Release()override;
	void ReleaseRendererCBuffer()override;
	void UpdateRendererCBuffer(const RenderingSceneInfo& arg_param)override;
	Value_ptr<CBuffer<RenderingSceneInfo>> GetRendererCBuffer()override;
	Value_ptr<GraphicDevice> GetGraphicDevice() const;
	Value_ptr<IDrawLayer> GetDrawLayer(const std::int32_t arg_index)const override { return vec_drawLayers[arg_index]; }
	void RemoveDrawLayer(const std::int32_t arg_index) override { vec_drawLayers.erase(vec_drawLayers.begin() + arg_index); }
private:


	Value_weak_ptr<GraphicDevice> vwp_graphicDevice;
	std::vector<Value_ptr< IDrawLayer>> vec_drawLayers;
	Value_ptr<CBuffer<RenderingSceneInfo>> CBuffer_Renderer;
};
class DrawLayer_Shadow;
class DrawLayer :public IDrawLayer {
public:
	DrawLayer(Value_weak_ptr<IRenderer> arg_vwp_renderer);
	void Initialize()override;
	void Clear()override;
	void BefRendering()override;
	void BefUpdate();
	void Regist(Value_ptr< IDrawObject> arg_vwp_drawObject, const bool arg_isAfterRendering, const bool arg_isShadow = false)override;
	void UnRegist(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_isAfterRendering, const bool arg_isShadow = false)override;
	void DeleteDrawObj(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_isAfterRendering);
	void CommandSet()override;
	void SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera)override;
	void SetShadowTexture(Value_weak_ptr<IResource_Texture> arg_textureTag)override;

	void Rendering()override;
	void ShadowRendering() override;
	Value_ptr<ICamera> GetShadowCamera()override;
	Value_ptr<IResource_Texture> GetShadowTexture()override;

	std::vector<Value_ptr< IDrawObject>> vec_befDrawObj;
	std::vector<Value_ptr< IDrawObject>> vec_afterDrawObj;

	std::vector<RegistCommand> vec_registCommandBuff;

	std::int32_t nowFrameAdditionObjectCount = 0;
	std::int32_t nowFrameAdditionObjectCount_after = 0;
	bool changed = false, sorted = false;
	Matrix4x4 vp;
	std::vector<Value_ptr< IDrawObject>> vec_sortedAfterDrawObj;
	std::vector<std::uint32_t*> vec_befIndex;
	std::vector<std::uint32_t*> vec_afterIndex;
	Value_weak_ptr<IRenderer> vwp_renderer;

	Value_ptr<DrawLayer_Shadow> vlp_shadowLayer;

	std::unordered_map<Value_ptr<IDrawObject>, std::uint32_t*> map_drawObjIndex;

	///バンドルコマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;

	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	std::mutex mtx_commandBuffer;
};
class DrawLayer_Shadow : public DrawLayer, public IDrawLayer_Shadow {
public:
	DrawLayer_Shadow(Value_weak_ptr<IRenderer> arg_vwp_renderer) :DrawLayer(arg_vwp_renderer) {};
	void BefRendering()override;
	void Initialize()override;
	Value_ptr<ICamera> GetShadowCamera()override;
	Value_ptr<IResource_Texture> GetShadowTexture()override;
	void SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera)override;
	void SetShadowTexture(Value_weak_ptr<IResource_Texture> arg_textureTag)override;

	/// <summary>
	/// 影を書き込んだかの取得
	/// </summary>
	/// <returns></returns>
	bool IsShadowDrawed() {
		return isShadowDrawed;

	}
private:

	Value_ptr<ICamera> vlp_shadowCamera;
	Value_weak_ptr<IResource_Texture> shadowTexture;
	bool isShadowDrawed = false;
};

}
}

ButiEngine::Vector2 ComputeFogCoord(const float arg_start,const float arg_end)
{
	return ButiEngine::Vector2(
		arg_end / (arg_end - arg_start),
		-1.0f / (arg_end - arg_start)
	);
}

ButiEngine::ButiRendering::Renderer::Renderer(Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	vwp_graphicDevice = arg_vwp_graphicDevice;
}
void ButiEngine::ButiRendering::Renderer::Initialize()
{	
	CBuffer_Renderer = ObjectFactory::Create<CBuffer_Dx12<RenderingSceneInfo>>(2);

	CBuffer_Renderer->SetExName("FogParameter");
	CBuffer_Renderer->Get().fogColor = Vector4(100.0f/256.0f,149.0f/256.0f , 247.0f/256.0f, 0.0f);
	CBuffer_Renderer->Get().fogCoord = ComputeFogCoord(50,100.0f);
	CBuffer_Renderer->SetGraphicDevice(vwp_graphicDevice.lock());
	CBuffer_Renderer->CreateBuffer();

}
void ButiEngine::ButiRendering::Renderer::Update()
{
	vwp_graphicDevice.lock()->Present();
}

void ButiEngine::ButiRendering::Renderer::RenderingStart()
{
	//GetRendererCBuffer()->Get().Time +=GameDevice::WorldSpeed* 0.01f;
	for (auto itr = vec_drawLayers.begin(); itr != vec_drawLayers.end(); itr++) {
		(*itr)->CommandSet();
	}
}

void ButiEngine::ButiRendering::Renderer::Rendering(const std::uint32_t arg_layer)
{
	CBuffer_Renderer->Update();

	vec_drawLayers[arg_layer]->Rendering();
	
	
}

void ButiEngine::ButiRendering::Renderer::ShadowRendering(const std::uint32_t arg_layer)
{
	if (vec_drawLayers.at(arg_layer)->GetShadowCamera()) {
		CBuffer_Renderer->Get().shadowVP = vec_drawLayers.at(arg_layer)->GetShadowCamera()->GetViewProjectionMatrix();
		CBuffer_Renderer->Get().shadowV = vec_drawLayers.at(arg_layer)->GetShadowCamera()->GetViewMatrix();
		CBuffer_Renderer->Get().shadowCameraPos = vec_drawLayers.at(arg_layer)->GetShadowCamera()->GetPosition();
	}


	CBuffer_Renderer->Update();
	vec_drawLayers[arg_layer]->ShadowRendering();
}


void ButiEngine::ButiRendering::Renderer::RenderingEnd()
{
	vwp_graphicDevice.lock()->Present();

	vwp_graphicDevice.lock()->UploadResourceRelease();
}

void ButiEngine::ButiRendering::Renderer::BefRendering()
{
	for (auto layerItr = vec_drawLayers.begin(); layerItr != vec_drawLayers.end(); layerItr++) {
		(*layerItr)->BefRendering();

	}
}

void ButiEngine::ButiRendering::Renderer::AddLayer()
{
	auto layer = ObjectFactory::Create<DrawLayer>(GetThis<IRenderer>());
	vec_drawLayers.push_back(layer);
}

std::uint32_t ButiEngine::ButiRendering::Renderer::GetLayerCount()
{
	return vec_drawLayers.size();
}


void ButiEngine::ButiRendering::Renderer::ClearDrawObjects()
{
	for (auto layerItr = vec_drawLayers.begin(); layerItr != vec_drawLayers.end(); layerItr++) {
		(*layerItr)->Clear();
	}
	vec_drawLayers.clear();
}

void ButiEngine::ButiRendering::Renderer::RegistDrawObject(Value_ptr<IDrawObject> arg_vwp_drawObject, const bool arg_afterDraw,  const std::uint32_t arg_layer, const bool isShadow)
{
	if (arg_layer >= vec_drawLayers.size()) {
		return ;
	}

	vec_drawLayers.at(arg_layer)->Regist(arg_vwp_drawObject,arg_afterDraw,isShadow);
}

void ButiEngine::ButiRendering::Renderer::UnRegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_afterDraw,  const std::uint32_t arg_layer, const bool isShadow )
{
	if (arg_layer >= vec_drawLayers.size()) {
		return;
	}
	vec_drawLayers.at(arg_layer)->UnRegist(arg_vlp_drawObject, arg_afterDraw,isShadow);
}





void ButiEngine::ButiRendering::Renderer::SetShadowCamera(const std::uint32_t arg_layer, Value_ptr<ICamera> arg_vlp_camera)
{
	if (vec_drawLayers.size() > arg_layer) {
		vec_drawLayers.at(arg_layer)->SetShadowCamera( arg_vlp_camera);
	}
}

void ButiEngine::ButiRendering::Renderer::SetShadowTexture(const std::uint32_t arg_layer, Value_weak_ptr<IResource_Texture> arg_shadowTex)
{
	if (vec_drawLayers.size() > arg_layer) {
		vec_drawLayers.at(arg_layer)->SetShadowTexture( arg_shadowTex);
	}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::Renderer::GetShadowTexture(const std::uint32_t arg_layer)
{

	if (vec_drawLayers.size() <= arg_layer) {
		return nullptr;
	}
	return vec_drawLayers[arg_layer]->GetShadowTexture();
}
ButiEngine::Value_ptr< ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::Renderer::GetShadowCamera(const std::uint32_t arg_layer)
{
	if (vec_drawLayers.size() <= arg_layer) {
		return nullptr;
	}
	return vec_drawLayers.at(arg_layer)->GetShadowCamera();
}


void ButiEngine::ButiRendering::Renderer::Release()
{

	ClearDrawObjects();
}

void ButiEngine::ButiRendering::Renderer::ReleaseRendererCBuffer()
{
	CBuffer_Renderer = nullptr;
}

void ButiEngine::ButiRendering::Renderer::UpdateRendererCBuffer(const RenderingSceneInfo &arg_param)
{
	CBuffer_Renderer->Get().fogColor = arg_param.fogColor;
	CBuffer_Renderer->Get().fogCoord = arg_param.fogCoord;

}

ButiEngine::Value_ptr< ButiEngine::ButiRendering::CBuffer< ButiEngine::ButiRendering::RenderingSceneInfo>> ButiEngine::ButiRendering::Renderer::GetRendererCBuffer()
{
	return CBuffer_Renderer;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::GraphicDevice> ButiEngine::ButiRendering::Renderer::GetGraphicDevice() const
{
	return vwp_graphicDevice;
}

ButiEngine::ButiRendering::DrawLayer::DrawLayer(Value_weak_ptr<IRenderer> arg_vwp_renderer)
{
	vwp_renderer = arg_vwp_renderer;
}

void ButiEngine::ButiRendering::DrawLayer::Initialize()
{
	//vlp_collisionLayer = ObjectFactory::Create<Collision::CollisionLayer<IDrawObject>>(5, Vector3(-500, -500, -500), Vector3(500, 500, 500));

	vwp_graphicDevice =vwp_renderer.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>();

	commandList = CommandListHelper::CreateBundleCommandList(vwp_graphicDevice.lock()->GetDevice(),vwp_graphicDevice.lock()->GetBundleCommandAllocator());
	CommandListHelper::Close(commandList);
}

void ButiEngine::ButiRendering::DrawLayer::Clear()
{
	if (vlp_shadowLayer) {
		vlp_shadowLayer->Clear();
	}

	vec_afterDrawObj.clear();
	vec_sortedAfterDrawObj.clear();
	vec_befDrawObj.clear();

	for (auto itr = vec_befIndex.begin(); itr != vec_befIndex.end(); itr++) {
		delete* itr;
	}
	for (auto itr = vec_afterIndex.begin(); itr != vec_afterIndex.end(); itr++) {
		delete* itr;
	}
}

void ButiEngine::ButiRendering::DrawLayer::BefRendering()
{

	if (vlp_shadowLayer)
	{
		vlp_shadowLayer->BefRendering();
	}
	BefUpdate();
	//vlp_collisionLayer->Update();
}

void ButiEngine::ButiRendering::DrawLayer::BefUpdate()
{
	std::lock_guard lock(mtx_commandBuffer);
	//描画オブジェクトの登録,解除
	if (vec_registCommandBuff.size())
	{
		changed = true;
		auto endItr = vec_registCommandBuff.end();
		vec_befDrawObj.reserve(vec_befDrawObj.size() + vec_registCommandBuff.size());
		vec_afterDrawObj.reserve(vec_afterDrawObj.size() + vec_registCommandBuff.size());

		for (auto itr = vec_registCommandBuff.begin(); itr != endItr; itr++) {

			if (itr->isRegist) {
				//if (vlp_collisionLayer)
					//vlp_collisionLayer->RegistCollisionObj((itr->vlp_obj)->GetPrimitive(), itr->vlp_obj);
				std::uint32_t* index;
				if (itr->isAfter) {
					index = new std::uint32_t(vec_afterIndex.size());
					vec_afterIndex.push_back(index);
					vec_afterDrawObj.push_back(itr->vlp_obj);
					sorted = false;
				}
				else {
					index = new std::uint32_t(vec_befIndex.size());
					vec_befIndex.push_back(index);
					vec_befDrawObj.push_back(itr->vlp_obj);
				}

				map_drawObjIndex.emplace(itr->vlp_obj, index);
				itr->vlp_obj->SetInfo_WithoutCommand();
				if (itr->isAfter) {
					(itr)->vlp_obj->SetInfo();
				}
			}
			else {

				DeleteDrawObj(itr->vlp_obj, itr->isAfter);
			}

		}
		vec_registCommandBuff.clear();
		vec_befDrawObj.shrink_to_fit();
		vec_afterDrawObj.shrink_to_fit();

	}


	//描画オブジェクトの行列定数バッファ、ボーン定数バッファの更新
	{

		auto endItr = vec_befDrawObj.end();
		for (auto itr = vec_befDrawObj.begin(); itr != endItr; itr++) {
			(*itr)->DrawBefore();
		}
	}
	{

		auto endItr = vec_afterDrawObj.end();
		for (auto itr = vec_afterDrawObj.begin(); itr != endItr; itr++) {
			(*itr)->DrawBefore();
		}
	}
}

void ButiEngine::ButiRendering::DrawLayer::Regist(Value_ptr<IDrawObject> arg_vwp_drawObject, const bool arg_isAfterRendering,  const bool arg_isShadow)
{
	std::lock_guard lock(mtx_commandBuffer);
	if (arg_isShadow ) {
		if (!vlp_shadowLayer) {
			vlp_shadowLayer = ObjectFactory::Create<DrawLayer_Shadow>(vwp_renderer);
			vlp_shadowLayer->vwp_graphicDevice = vwp_graphicDevice;
		}
		


		return vlp_shadowLayer->Regist(arg_vwp_drawObject, arg_isAfterRendering, false);
		
	}


	if (arg_isAfterRendering) {

		nowFrameAdditionObjectCount_after++;
	}
	else {

		nowFrameAdditionObjectCount++;
	}

	RegistCommand cmd;

	cmd.isAfter = arg_isAfterRendering;
	cmd.vlp_obj = arg_vwp_drawObject;
	cmd.isShadow = arg_isShadow;
	cmd.isRegist = true;
	vec_registCommandBuff.push_back(cmd);

}

void ButiEngine::ButiRendering::DrawLayer::UnRegist(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_isAfterRendering, const bool arg_isShadow)
{
	std::lock_guard lock(mtx_commandBuffer);
	if (arg_isShadow&&vlp_shadowLayer) {
		vlp_shadowLayer->UnRegist(arg_vlp_drawObject, arg_isAfterRendering, false);
		return;
	}

	if (arg_isAfterRendering) {
		nowFrameAdditionObjectCount_after--;
	}
	else {
		nowFrameAdditionObjectCount--;
	}

	vec_registCommandBuff.push_back({ arg_isAfterRendering,arg_isShadow,arg_vlp_drawObject,false });
}

void ButiEngine::ButiRendering::DrawLayer::DeleteDrawObj(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_isAfterRendering)
{
	if (!map_drawObjIndex.count(arg_vlp_drawObject)) {
		return;
	}
	if (arg_isAfterRendering) {

		auto index =* map_drawObjIndex.at(arg_vlp_drawObject);
		if (index >= vec_afterDrawObj.size()) {
			return;
		}
		auto itr = vec_afterDrawObj.begin() + index;

		//if(vlp_collisionLayer)
		//vlp_collisionLayer->UnRegistCollisionObj((*itr));

		vec_afterDrawObj.erase(itr);

		delete map_drawObjIndex.at(arg_vlp_drawObject);
		map_drawObjIndex.erase(arg_vlp_drawObject);
		auto numItr = vec_afterIndex.begin() + index;
		numItr = vec_afterIndex.erase(numItr);

		for (; numItr != vec_afterIndex.end(); numItr++) {
			*(*numItr) -= 1;
		}

		sorted = false;
		return;
	}
	auto index = *map_drawObjIndex.at(arg_vlp_drawObject);
	if (index >= vec_befDrawObj.size()) {
		return;
	}
	auto itr = vec_befDrawObj.begin() + index;
	auto obj = (*itr);

	//if (vlp_collisionLayer)
	//vlp_collisionLayer->UnRegistCollisionObj(obj);

	vec_befDrawObj.erase(itr);

	delete map_drawObjIndex.at(arg_vlp_drawObject);
	map_drawObjIndex.erase(arg_vlp_drawObject);
	auto numItr = vec_befIndex.begin() + index;
	numItr = vec_befIndex.erase(numItr);

	for (; numItr != vec_befIndex.end(); numItr++) {
		*(*numItr) -= 1;
	}
}

void ButiEngine::ButiRendering::DrawLayer::CommandSet()
{
	if (vlp_shadowLayer) {
		vlp_shadowLayer->CommandSet();
	}
	if (!changed ) {
		return;
	}
	changed = false;
	CommandListHelper::BundleReset(nullptr, commandList, vwp_graphicDevice.lock()->GetBundleCommandAllocator());
	ID3D12DescriptorHeap* ppHeaps[] = { vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetDescriptorHeap().Get() ,vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetSamplerHeap().Get() };
	auto heapCount = _countof(ppHeaps);
	commandList->SetDescriptorHeaps(heapCount, ppHeaps);

	vwp_graphicDevice.lock()->SetCommandList(commandList.Get());
	auto drawObjEnd = vec_befDrawObj.end();
	for (auto itr = vec_befDrawObj.begin(); itr != drawObjEnd; itr++) {

		(*itr)->CommandSet();
	}


	vwp_graphicDevice.lock()->UnSetCommandList();

	CommandListHelper::Close(commandList);
	nowFrameAdditionObjectCount = 0;
	nowFrameAdditionObjectCount_after = 0;
}

void ButiEngine::ButiRendering::DrawLayer::SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera)
{
	if (!vlp_shadowLayer) {
		vlp_shadowLayer = ObjectFactory::Create <DrawLayer_Shadow>(vwp_renderer);
	}
	vlp_shadowLayer->SetShadowCamera(  arg_shadowCamera);
}

void ButiEngine::ButiRendering::DrawLayer::SetShadowTexture(Value_weak_ptr<IResource_Texture> arg_texture)
{
	if (!vlp_shadowLayer) {
		vlp_shadowLayer = ObjectFactory::Create <DrawLayer_Shadow>(vwp_renderer);
	}
	vlp_shadowLayer->SetShadowTexture( arg_texture);
}


void ButiEngine::ButiRendering::DrawLayer::Rendering()
{
	for (auto itr = vec_befDrawObj.begin(), endItr = vec_befDrawObj.end(); itr != endItr; itr++) {
		(*itr)->BufferUpdate();
	}
	vwp_graphicDevice.lock()->GetCommandList().ExecuteBundle(commandList.Get());
	vp = vwp_graphicDevice.lock()->GetProjectionMatrix() * vwp_graphicDevice.lock()->GetCameraViewMatrix();
	vec_sortedAfterDrawObj = vec_afterDrawObj;
	std::sort(vec_sortedAfterDrawObj.begin(), vec_sortedAfterDrawObj.end(), [this](const Value_ptr< IDrawObject> pmX, const Value_ptr<IDrawObject> pmY) {

		return pmX->GetZ(vp) > pmY->GetZ(vp);
		});


	for (auto itr = vec_sortedAfterDrawObj.begin(), endItr = vec_sortedAfterDrawObj.end(); itr != endItr; itr++) {

		(*itr)->BufferUpdate();
		(*itr)->CommandExecute();
	}
}

void ButiEngine::ButiRendering::DrawLayer::ShadowRendering()
{
	if (vlp_shadowLayer) {
		vlp_shadowLayer->Rendering();
	}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::DrawLayer::GetShadowCamera()
{
	if (vlp_shadowLayer) {
		return vlp_shadowLayer->GetShadowCamera();
	}
	return nullptr;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::DrawLayer::GetShadowTexture()
{
	if (vlp_shadowLayer) {
		return vlp_shadowLayer->GetShadowTexture();
	}

	return nullptr;
}

void ButiEngine::ButiRendering::DrawLayer_Shadow::BefRendering()
{
	isShadowDrawed = false;
	BefUpdate();
}

void ButiEngine::ButiRendering::DrawLayer_Shadow::Initialize()
{

	vwp_graphicDevice = vwp_renderer.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>();

	commandList = CommandListHelper::CreateBundleCommandList(vwp_graphicDevice.lock()->GetDevice(),vwp_graphicDevice.lock()->GetBundleCommandAllocator());
	CommandListHelper::Close(commandList);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::DrawLayer_Shadow::GetShadowCamera()
{
	return vlp_shadowCamera;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::DrawLayer_Shadow::GetShadowTexture()
{
	return shadowTexture;
}

void ButiEngine::ButiRendering::DrawLayer_Shadow::SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera)
{
	vlp_shadowCamera = arg_shadowCamera;
}

void ButiEngine::ButiRendering::DrawLayer_Shadow::SetShadowTexture(Value_weak_ptr<IResource_Texture> arg_texture)
{
	shadowTexture = arg_texture;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IRenderer> ButiEngine::ButiRendering::CreateRenderer(Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	return ObjectFactory::Create<Renderer>(arg_vwp_graphicDevice);
}