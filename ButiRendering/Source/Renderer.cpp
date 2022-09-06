#pragma once
#include"stdafx.h"
#include<algorithm>
#include"ButiRendering_Dx12/Header/Renderer.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"

namespace ButiEngine {
namespace ButiRendering {
struct DrawObjectRegistCommand {
	bool isShadow;
	Value_ptr<IDrawObject> vlp_obj = nullptr;
	bool isRegist = false;
};

Value_ptr<CBuffer<RendererState>> g_CBuffer_Renderer;
class Renderer :public IRenderer
{
public:
	Renderer(Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
	~Renderer()
	{
	}
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
	void RegistDrawObject(Value_ptr< IDrawObject> arg_vwp_drawObject,  const std::uint32_t arg_layer = 0, const bool isShadow = false)override;
	void UnRegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const std::uint32_t arg_layer = 0, const bool isShadow = false)override;
	void Release()override;
	void ReleaseRendererCBuffer()override;
	void UpdateRendererCBuffer(const RendererState& arg_param)override;
	Value_ptr<CBuffer<RendererState>> GetRendererCBuffer()override;
	Value_ptr<GraphicDevice> GetGraphicDevice() const;
	Value_ptr<IDrawLayer> GetDrawLayer(const std::int32_t arg_index)const override { return m_list_drawLayers[arg_index]; }
	void RemoveDrawLayer(const std::int32_t arg_index) override { m_list_drawLayers.erase(m_list_drawLayers.begin() + arg_index); }
	void PushDrawFunction(std::function<void()> arg_drawFunction) override { m_list_drawFunctions.Add(arg_drawFunction); }
	virtual void PopDrawFunction() override { m_list_drawFunctions.RemoveLast(); }
private:


	Value_weak_ptr<GraphicDevice> m_vwp_graphicDevice;
	List<Value_ptr< IDrawLayer>> m_list_drawLayers;
	List<std::function<void()>> m_list_drawFunctions;
};
class DrawLayer_Shadow;
class DrawLayer :public IDrawLayer {
public:
	DrawLayer(Value_weak_ptr<IRenderer> arg_vwp_renderer);
	void Initialize()override;
	void Clear()override;
	void BefRendering()override;
	void BefUpdate();
	void Regist(Value_ptr< IDrawObject> arg_vwp_drawObject, const bool arg_isShadow = false)override;
	void UnRegist(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_isShadow = false)override;
	void Regist(List< Value_ptr< IDrawCommand>> arg_list_vwp_drawCommand);
	void UnRegist(List< Value_ptr<IDrawCommand>> arg_list_vwp_drawCommand);
	void DeleteDrawObj(Value_ptr< IDrawObject> arg_vlp_drawObject);
	void CreateCommandBundle()override;
	void SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera)override;
	void SetShadowTexture(Value_weak_ptr<IResource_Texture> arg_textureTag)override;

	void Rendering()override;
	void ShadowRendering() override;
	Value_ptr<ICamera> GetShadowCamera()override;
	Value_ptr<IResource_Texture> GetShadowTexture()override;
	std::int32_t GetRenderTargetSize()const override { return m_renderTargetSize; }
protected:
	void SortAfterCommands();
	bool m_changed = false, m_sorted = false;
	Matrix4x4 m_viewMatrix;
	Value_weak_ptr<IRenderer> m_vwp_renderer;
	Value_ptr<DrawLayer_Shadow> m_vlp_shadowLayer;

	List<DrawObjectRegistCommand> m_list_registCommandBuff;

	List<Value_ptr<IDrawObject>> m_list_drawObj;
	List < Value_ptr<IDrawCommand>> m_list_afterCommand, m_list_sortedAfterCommand;
	std::unordered_map<Value_ptr<IPipelineState>, List<Value_ptr<IDrawCommand>>>m_map_pipelineState_command;
	///バンドルコマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
	std::mutex m_mtx_commandBuffer;
	std::int32_t m_renderTargetSize = -1;
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
		return m_isShadowDrawed;

	}
private:

	Value_ptr<ICamera> m_vlp_shadowCamera;
	Value_weak_ptr<IResource_Texture> m_vwp_shadowTexture;
	bool m_isShadowDrawed = false;
};

Value_ptr<CBuffer<RendererState>> IRenderer::GetRendererStateCBuffer()
{
	return g_CBuffer_Renderer;
}

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
	m_vwp_graphicDevice = arg_vwp_graphicDevice;
}
void ButiEngine::ButiRendering::Renderer::Initialize()
{	
	if (!g_CBuffer_Renderer) {
		g_CBuffer_Renderer = CreateCBuffer<RendererState>();
		g_CBuffer_Renderer->Get().fogColor = Vector4(100.0f / 256.0f, 149.0f / 256.0f, 247.0f / 256.0f, 0.0f);
		g_CBuffer_Renderer->Get().fogCoord = ComputeFogCoord(50, 100.0f);
		g_CBuffer_Renderer->SetGraphicDevice(m_vwp_graphicDevice);
		g_CBuffer_Renderer->CreateBuffer();
	}

}
void ButiEngine::ButiRendering::Renderer::Update()
{
	m_vwp_graphicDevice.lock()->Present();
}

void ButiEngine::ButiRendering::Renderer::RenderingStart()
{
	BefRendering();
	m_vwp_graphicDevice.lock()->UploadResourceBufferMerge();
	m_vwp_graphicDevice.lock()->Reset();
	m_vwp_graphicDevice.lock()->ClearWindow();
	m_vwp_graphicDevice.lock()->ResourceUpload();
	//GetRendererCBuffer()->Get().Time +=GameDevice::WorldSpeed* 0.01f;
	for (auto itr: m_list_drawLayers) {
		itr->CreateCommandBundle();
	}
}

void ButiEngine::ButiRendering::Renderer::Rendering(const std::uint32_t arg_layer)
{
#ifdef DEBUG
	if (m_list_drawLayers.GetSize() <= arg_layer) {
		return;
	}
#endif // DEBUG

	g_CBuffer_Renderer->Update();

	m_list_drawLayers[arg_layer]->Rendering();
	
	
}

void ButiEngine::ButiRendering::Renderer::ShadowRendering(const std::uint32_t arg_layer)
{
	if (m_list_drawLayers.at(arg_layer)->GetShadowCamera()) {
		if (!g_CBuffer_Renderer) {
			g_CBuffer_Renderer = CreateCBuffer<RendererState>();
			g_CBuffer_Renderer->Get().fogColor = Vector4(100.0f / 256.0f, 149.0f / 256.0f, 247.0f / 256.0f, 0.0f);
			g_CBuffer_Renderer->Get().fogCoord = ComputeFogCoord(50, 100.0f);
			g_CBuffer_Renderer->SetGraphicDevice(m_vwp_graphicDevice.lock());
			g_CBuffer_Renderer->CreateBuffer();
		}
		g_CBuffer_Renderer->Get().shadowVP = m_list_drawLayers.at(arg_layer)->GetShadowCamera()->GetViewProjectionMatrix();
		g_CBuffer_Renderer->Get().shadowV = m_list_drawLayers.at(arg_layer)->GetShadowCamera()->GetViewMatrix();
		g_CBuffer_Renderer->Get().shadowCameraPos = m_list_drawLayers.at(arg_layer)->GetShadowCamera()->GetPosition();
	}


	g_CBuffer_Renderer->Update();
	m_list_drawLayers[arg_layer]->ShadowRendering();
}


void ButiEngine::ButiRendering::Renderer::RenderingEnd()
{
	for (auto func : m_list_drawFunctions) {
		func();
	}
	m_vwp_graphicDevice.lock()->ResetPipeLine();
	m_vwp_graphicDevice.lock()->Present();
	m_vwp_graphicDevice.lock()->UploadResourceRelease();
	m_vwp_graphicDevice.lock()->SetMesh(nullptr,0);
	m_vwp_graphicDevice.lock()->SetMaterial(nullptr);
}

void ButiEngine::ButiRendering::Renderer::BefRendering()
{
	for (auto layerItr = m_list_drawLayers.begin(); layerItr != m_list_drawLayers.end(); layerItr++) {
		(*layerItr)->BefRendering();

	}
}

void ButiEngine::ButiRendering::Renderer::AddLayer()
{
	auto layer = ObjectFactory::Create<DrawLayer>(GetThis<IRenderer>());
	m_list_drawLayers.push_back(layer);
}

std::uint32_t ButiEngine::ButiRendering::Renderer::GetLayerCount()
{
	return m_list_drawLayers.GetSize();
}


void ButiEngine::ButiRendering::Renderer::ClearDrawObjects()
{
	for (auto layerItr :m_list_drawLayers) {
		(layerItr)->Clear();
	}
	m_list_drawLayers.Clear();
}

void ButiEngine::ButiRendering::Renderer::RegistDrawObject(Value_ptr<IDrawObject> arg_vwp_drawObject,   const std::uint32_t arg_layer, const bool isShadow)
{
	if (arg_layer >= m_list_drawLayers.GetSize()) {
		return ;
	}

	m_list_drawLayers.at(arg_layer)->Regist(arg_vwp_drawObject,isShadow);
}

void ButiEngine::ButiRendering::Renderer::UnRegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const std::uint32_t arg_layer, const bool isShadow )
{
	if (arg_layer >= m_list_drawLayers.GetSize()) {
		return;
	}
	m_list_drawLayers.at(arg_layer)->UnRegist(arg_vlp_drawObject, isShadow);
}





void ButiEngine::ButiRendering::Renderer::SetShadowCamera(const std::uint32_t arg_layer, Value_ptr<ICamera> arg_vlp_camera)
{
	if (m_list_drawLayers.GetSize() > arg_layer) {
		m_list_drawLayers.at(arg_layer)->SetShadowCamera( arg_vlp_camera);
	}
}

void ButiEngine::ButiRendering::Renderer::SetShadowTexture(const std::uint32_t arg_layer, Value_weak_ptr<IResource_Texture> arg_shadowTex)
{
	if (m_list_drawLayers.GetSize() > arg_layer) {
		m_list_drawLayers.at(arg_layer)->SetShadowTexture( arg_shadowTex);
	}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::Renderer::GetShadowTexture(const std::uint32_t arg_layer)
{

	if (m_list_drawLayers.GetSize() <= arg_layer) {
		return nullptr;
	}
	return m_list_drawLayers[arg_layer]->GetShadowTexture();
}
ButiEngine::Value_ptr< ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::Renderer::GetShadowCamera(const std::uint32_t arg_layer)
{
	if (m_list_drawLayers.GetSize() <= arg_layer) {
		return nullptr;
	}
	return m_list_drawLayers.at(arg_layer)->GetShadowCamera();
}
void ButiEngine::ButiRendering::Renderer::Release()
{
	ReleaseRendererCBuffer();
	ClearDrawObjects();
}

void ButiEngine::ButiRendering::Renderer::ReleaseRendererCBuffer()
{
	g_CBuffer_Renderer = nullptr;
}

void ButiEngine::ButiRendering::Renderer::UpdateRendererCBuffer(const RendererState &arg_param)
{
	if (!g_CBuffer_Renderer) {
		g_CBuffer_Renderer = CreateCBuffer<RendererState>();
		g_CBuffer_Renderer->Get().fogColor = Vector4(100.0f / 256.0f, 149.0f / 256.0f, 247.0f / 256.0f, 0.0f);
		g_CBuffer_Renderer->Get().fogCoord = ComputeFogCoord(50, 100.0f);
		g_CBuffer_Renderer->SetGraphicDevice(m_vwp_graphicDevice.lock());
		g_CBuffer_Renderer->CreateBuffer();
	}
	g_CBuffer_Renderer->Get().fogColor = arg_param.fogColor;
	g_CBuffer_Renderer->Get().fogCoord = arg_param.fogCoord;

}

ButiEngine::Value_ptr< ButiEngine::ButiRendering::CBuffer< ButiEngine::ButiRendering::RendererState>> ButiEngine::ButiRendering::Renderer::GetRendererCBuffer()
{
	if (!g_CBuffer_Renderer) {
		g_CBuffer_Renderer = CreateCBuffer<RendererState>();
		g_CBuffer_Renderer->Get().fogColor = Vector4(100.0f / 256.0f, 149.0f / 256.0f, 247.0f / 256.0f, 0.0f);
		g_CBuffer_Renderer->Get().fogCoord = ComputeFogCoord(50, 100.0f);
		g_CBuffer_Renderer->SetGraphicDevice(m_vwp_graphicDevice.lock());
		g_CBuffer_Renderer->CreateBuffer();
	}
	return g_CBuffer_Renderer;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::GraphicDevice> ButiEngine::ButiRendering::Renderer::GetGraphicDevice() const
{
	return m_vwp_graphicDevice;
}

ButiEngine::ButiRendering::DrawLayer::DrawLayer(Value_weak_ptr<IRenderer> arg_vwp_renderer)
{
	m_vwp_renderer = arg_vwp_renderer;
}

void ButiEngine::ButiRendering::DrawLayer::Initialize()
{
	//vlp_collisionLayer = ObjectFactory::Create<Collision::CollisionLayer<IDrawObject>>(5, Vector3(-500, -500, -500), Vector3(500, 500, 500));

	m_vwp_graphicDevice =m_vwp_renderer.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>();

	m_commandList = CommandListHelper::CreateBundleCommandList(m_vwp_graphicDevice.lock()->GetDevice(),m_vwp_graphicDevice.lock()->GetBundleCommandAllocator());
	CommandListHelper::Close(m_commandList);
}

void ButiEngine::ButiRendering::DrawLayer::Clear()
{
	if (m_vlp_shadowLayer) {
		m_vlp_shadowLayer->Clear();
	}
	m_list_drawObj.Clear();
	m_list_afterCommand.Clear();
	m_map_pipelineState_command.clear();
}

void ButiEngine::ButiRendering::DrawLayer::BefRendering()
{
	if (m_vlp_shadowLayer)
	{
		m_vlp_shadowLayer->BefRendering();
	}
	BefUpdate();
	//vlp_collisionLayer->Update();
}

void ButiEngine::ButiRendering::DrawLayer::BefUpdate()
{
	std::lock_guard lock(m_mtx_commandBuffer);
	//描画オブジェクトの登録,解除
	if (m_list_registCommandBuff.GetSize())
	{
		m_changed = true;
		m_list_drawObj.Reserve(m_list_drawObj.GetSize() + m_list_registCommandBuff.GetSize());
		for (auto itr : m_list_registCommandBuff) {
			if (itr.isRegist) {
				Regist(itr.vlp_obj->GetCommands());
				m_list_drawObj.Add(itr.vlp_obj);
			}
			else {
				UnRegist(itr.vlp_obj->GetCommands());
				DeleteDrawObj(itr.vlp_obj);
			}
		}
		m_list_registCommandBuff.Clear();

	}


	//描画オブジェクトの行列定数バッファ、ボーン定数バッファの更新
	{
		for (auto itr : m_list_drawObj) {
			itr->DrawBefore();
		}
	}
}

void ButiEngine::ButiRendering::DrawLayer::Regist(Value_ptr<IDrawObject> arg_vwp_drawObject, const bool arg_isShadow)
{
	std::lock_guard lock(m_mtx_commandBuffer);
	if (arg_isShadow ) {
		if (!m_vlp_shadowLayer) {
			m_vlp_shadowLayer = ObjectFactory::Create<DrawLayer_Shadow>(m_vwp_renderer);
			m_vlp_shadowLayer->m_vwp_graphicDevice = m_vwp_graphicDevice;
		}
		return m_vlp_shadowLayer->Regist(arg_vwp_drawObject, false);		
	}
	DrawObjectRegistCommand cmd;
	cmd.vlp_obj = arg_vwp_drawObject;
	cmd.isShadow = arg_isShadow;
	cmd.isRegist = true;
	m_list_registCommandBuff.push_back(cmd);

}

void ButiEngine::ButiRendering::DrawLayer::UnRegist(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_isShadow)
{
	std::lock_guard lock(m_mtx_commandBuffer);
	if (arg_isShadow&&m_vlp_shadowLayer) {
		m_vlp_shadowLayer->UnRegist(arg_vlp_drawObject, false);
		return;
	}

	m_list_registCommandBuff.push_back({ arg_isShadow,arg_vlp_drawObject,false });
}

void ButiEngine::ButiRendering::DrawLayer::Regist(List<Value_ptr<IDrawCommand>> arg_list_vwp_drawCommand)
{
	for (auto command:arg_list_vwp_drawCommand) {
		if (command->IsAlpha()) {
			m_list_afterCommand.Add(command);
			continue;
		}
		if (!m_map_pipelineState_command.count(command->GetMaterial().lock()->GetPipelineState())) {
			m_map_pipelineState_command.emplace(command->GetMaterial().lock()->GetPipelineState(), List<Value_ptr<IDrawCommand>>());
		}
		m_map_pipelineState_command.at(command->GetMaterial().lock()->GetPipelineState()).Add(command);
	}
}

void ButiEngine::ButiRendering::DrawLayer::UnRegist(List<Value_ptr<IDrawCommand>> arg_list_vwp_drawCommand)
{
	for (auto command : arg_list_vwp_drawCommand) {
		if (command->IsAlpha()) {
			m_list_afterCommand.Remove(command);
			continue;
		}
		m_map_pipelineState_command.at(command->GetMaterial().lock()->GetPipelineState()).Remove(command);
	}
}

void ButiEngine::ButiRendering::DrawLayer::DeleteDrawObj(Value_ptr< IDrawObject> arg_vlp_drawObject)
{
	m_list_drawObj.Remove(arg_vlp_drawObject);
}

void ButiEngine::ButiRendering::DrawLayer::CreateCommandBundle()
{
	if (m_vlp_shadowLayer) {
		m_vlp_shadowLayer->CreateCommandBundle();
	}
	if (!m_changed ) {
		return;
	}
	m_changed = false;
	m_renderTargetSize = -1;
	CommandListHelper::BundleReset(nullptr, m_commandList, m_vwp_graphicDevice.lock()->GetBundleCommandAllocator());
	ID3D12DescriptorHeap* ppHeaps[] = { m_vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetDescriptorHeap().Get() ,m_vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetSamplerHeap().Get() };
	auto heapCount = _countof(ppHeaps);
	m_commandList->SetDescriptorHeaps(heapCount, ppHeaps);

	m_vwp_graphicDevice.lock()->SetCommandList(m_commandList.Get());

	for (auto& pipelineAndList : m_map_pipelineState_command) {
		if (!pipelineAndList.second.GetSize()) {
			continue;
		}
		m_renderTargetSize =m_renderTargetSize==-1? pipelineAndList.first->GetShader()->GetOutputLayout().m_list_element.GetSize():m_renderTargetSize;
		if (pipelineAndList.first->GetShader()->GetOutputLayout().m_list_element.GetSize()!=m_renderTargetSize) {
			continue;
		}
		pipelineAndList.first->Attach();
		pipelineAndList.first->GetRootSignature()->Attach();
		for (auto command : pipelineAndList.second) {
			command->Execute();
		}
	}

	m_vwp_graphicDevice.lock()->UnSetCommandList();
	CommandListHelper::Close(m_commandList);
}

void ButiEngine::ButiRendering::DrawLayer::SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera)
{
	if (!m_vlp_shadowLayer) {
		m_vlp_shadowLayer = ObjectFactory::Create <DrawLayer_Shadow>(m_vwp_renderer);
	}
	m_vlp_shadowLayer->SetShadowCamera(  arg_shadowCamera);
}

void ButiEngine::ButiRendering::DrawLayer::SetShadowTexture(Value_weak_ptr<IResource_Texture> arg_texture)
{
	if (!m_vlp_shadowLayer) {
		m_vlp_shadowLayer = ObjectFactory::Create <DrawLayer_Shadow>(m_vwp_renderer);
	}
	m_vlp_shadowLayer->SetShadowTexture( arg_texture);
}


void ButiEngine::ButiRendering::DrawLayer::Rendering()
{
	for (auto itr : m_list_drawObj) {
		itr->BufferUpdate();
	}
	m_vwp_graphicDevice.lock()->GetCommandList().ExecuteBundle(m_commandList.Get());
	m_viewMatrix = m_vwp_graphicDevice.lock()->GetProjectionMatrix() * m_vwp_graphicDevice.lock()->GetCameraViewMatrix();
	SortAfterCommands();
	for (auto itr : m_list_sortedAfterCommand) {
		itr->Execute_afterRendering();
	}
}

void ButiEngine::ButiRendering::DrawLayer::ShadowRendering()
{
	if (m_vlp_shadowLayer) {
		m_vlp_shadowLayer->Rendering();
	}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::DrawLayer::GetShadowCamera()
{
	if (m_vlp_shadowLayer) {
		return m_vlp_shadowLayer->GetShadowCamera();
	}
	return nullptr;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::DrawLayer::GetShadowTexture()
{
	if (m_vlp_shadowLayer) {
		return m_vlp_shadowLayer->GetShadowTexture();
	}

	return nullptr;
}

void ButiEngine::ButiRendering::DrawLayer::SortAfterCommands()
{
	/*if (m_sorted) {
		return;
	}
	m_sorted = true;*/
	m_list_sortedAfterCommand = m_list_afterCommand;
	std::sort(m_list_sortedAfterCommand.begin(), m_list_sortedAfterCommand.end(), [this](const Value_ptr< IDrawCommand> pmX, const Value_ptr<IDrawCommand> pmY)
		{
			auto x = pmX->GetDrawObject().lock()->GetZ(m_viewMatrix), y = pmY->GetDrawObject().lock()->GetZ(m_viewMatrix);
			return x> y; 
		});
}

void ButiEngine::ButiRendering::DrawLayer_Shadow::BefRendering()
{
	m_isShadowDrawed = false;
	BefUpdate();
}

void ButiEngine::ButiRendering::DrawLayer_Shadow::Initialize()
{

	m_vwp_graphicDevice = m_vwp_renderer.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>();

	m_commandList = CommandListHelper::CreateBundleCommandList(m_vwp_graphicDevice.lock()->GetDevice(),m_vwp_graphicDevice.lock()->GetBundleCommandAllocator());
	CommandListHelper::Close(m_commandList);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::ICamera> ButiEngine::ButiRendering::DrawLayer_Shadow::GetShadowCamera()
{
	return m_vlp_shadowCamera;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::DrawLayer_Shadow::GetShadowTexture()
{
	return m_vwp_shadowTexture;
}

void ButiEngine::ButiRendering::DrawLayer_Shadow::SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera)
{
	m_vlp_shadowCamera = arg_shadowCamera;
}

void ButiEngine::ButiRendering::DrawLayer_Shadow::SetShadowTexture(Value_weak_ptr<IResource_Texture> arg_texture)
{
	m_vwp_shadowTexture = arg_texture;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IRenderer> ButiEngine::ButiRendering::CreateRenderer(Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	return ObjectFactory::Create<Renderer>(arg_vwp_graphicDevice);
}