#include<iostream>
#include"../ButiRendering/Common.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"Window.h"
#include"ButiRendering_Dx12/Header/GraphicDevice.h"
#include"ButiRendering_Dx12/Header/Renderer.h"
#include"ButiRendering_Dx12/Header/ICamera.h"
#include"ButiRendering_Dx12/Header/RenderingPath.h"
#include"ButiRendering_Dx12/Header/ResourceInterface.h"
#include"ButiRendering_Dx12/Header/MeshHelper.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/CBuffer_Dx12.h"
#include"ButiRendering_Dx12/Header/DrawData/IDrawData.h"
std::int32_t main() {
	using namespace ButiEngine;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	auto vlp_window = make_value<Window>();
	HINSTANCE instance=nullptr;
	vlp_window->Initialize("RenderingTest", false, false, instance, 960, 540);
	auto vlp_graphicDevice = ButiRendering::CreateGraphicDevice(true);
	vlp_graphicDevice->SetClearColor(ButiColor::DeepPurple());
	auto winHandle = vlp_window->GetHandle();
	vlp_graphicDevice->SetWindow(*reinterpret_cast<std::int64_t*> (&(winHandle)), 960, 540);

	auto vlp_renderer = ButiRendering::CreateRenderer(vlp_graphicDevice);
	vlp_renderer->AddLayer(); 
	auto prop = ButiRendering::CameraProperty(960, 540, 0, 0);
	prop.farClip = 200.0f;
	prop.cameraName = "main";
	prop.isEditActive = false;
	prop.isInitActive = true;
	auto vlp_camera = ButiRendering::CreateCamera(prop, "main", true,vlp_renderer, vlp_graphicDevice);
	vlp_camera->Initialize();
	vlp_camera->GetCameraProperty().layer = 0;
	auto mainCameraPath = make_value< ButiRendering::ForwardCameraRenderingPath>(vlp_camera,vlp_renderer);
	mainCameraPath->SetOrder(0);
	
	Value_ptr<ButiRendering::IResource_Material> vlp_material;
	Value_ptr<ButiRendering::IResource_Shader> vlp_shader;
	Value_ptr<ButiRendering::IResource_Mesh> vlp_mesh;
	//リソース作成
	{
		vlp_material = ButiRendering::CreateMaterial(ButiRendering::MaterialValue(), List<Value_weak_ptr<ButiRendering::IResource_Texture>>(), vlp_graphicDevice);
		auto cubePrim = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>>();
		auto cubePrim_uvNormal = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal>>();
		auto cubePrim_normal=ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_Normal>> ();

		ButiRendering::MeshHelper::CreateCube(Vector3(1,1,1),std::vector<Vector4>(),*cubePrim,true);
		ButiRendering::MeshHelper::VertexConvert(*cubePrim, *cubePrim_uvNormal);
		ButiRendering::MeshHelper::VertexConvert(*cubePrim, *cubePrim_normal);
		vlp_mesh = ButiRendering::CreateMesh("Cube", { cubePrim,cubePrim_normal,cubePrim_uvNormal }, vlp_graphicDevice);

		vlp_shader = ButiRendering::CreateShader(ButiRendering::CreateVertexShader("Resource/UVNormalVS", vlp_graphicDevice), ButiRendering::CreatePixelShader("Resource/OnlyMaterialPS", vlp_graphicDevice), nullptr,"OnlyColor");
	}
	auto vlp_transform = ObjectFactory::Create<Transform>(Vector3(0,0,10),Vector3(12,15,18),Vector3(1,1,1));

	auto drawInfo = ObjectFactory::Create<ButiRendering::DrawInformation>();
	drawInfo->layer = 0;
	{
		auto info = ObjectFactory::Create<ButiRendering::CBuffer_Dx12< ButiRendering::ObjectInformation>>(3);
		info->SetExName("ObjectInformation");
		info->Get().lightDir = Vector4(Vector3(-1.0f, -1.0f, 0.0f).GetNormalize(), 1);
		info->Get().color = ButiColor::Cyan();
		drawInfo->vec_exCBuffer.push_back(info);
	}

	
	auto drawObject = ButiRendering::CreateMeshDrawObject(vlp_mesh, vlp_shader, vlp_material, vlp_renderer, vlp_graphicDevice, drawInfo,vlp_transform);
	vlp_renderer->RegistDrawObject(drawObject,false,0);
	while (vlp_window->Update()) {
		//描画開始
		vlp_renderer->BefRendering();
		mainCameraPath->BefExecute();
		vlp_graphicDevice->UploadResourceRelease();
		vlp_graphicDevice->UploadResourceBufferMerge();


		vlp_graphicDevice->Reset();
		vlp_graphicDevice->ClearWindow();
		vlp_graphicDevice->ResourceUpload();

		//描画処理
		vlp_renderer->RenderingStart();
		vlp_transform->RollWorldRotationY_Degrees(0.1);
		mainCameraPath->Execute();
		mainCameraPath->End();

		//描画終了
		vlp_graphicDevice->ResetPipeLine();
		vlp_graphicDevice->Present();


	}
	vlp_renderer->UnRegistDrawObject(drawObject,false,0);
	//解放
	vlp_material = nullptr;
	vlp_shader = nullptr;
	vlp_mesh = nullptr;
	drawObject = nullptr;
	drawInfo = nullptr;
	
	vlp_renderer->ReleaseRendererCBuffer();
	vlp_renderer->Release();
	vlp_window->Release();
	vlp_graphicDevice->Release();
	return 0;
}