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
#include"ButiRendering_Dx12/Header/CBuffer.h"
#include"ButiRendering_Dx12/Header/DrawData/IDrawData.h"
#include"ButiUtil/ButiUtil/StopWatch.h"
#include<typeinfo>
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
	
	auto mainCameraPath = ButiRendering::CreateForwardRenderingPath(ButiRendering::CreateCamera(prop, "main", true, vlp_renderer, vlp_graphicDevice),vlp_renderer);
	
	Value_ptr<ButiRendering::IResource_Material> vlp_material_onlyColor, vlp_material_textureMap;
	Value_ptr<ButiRendering::IResource_Shader> vlp_shader_onlyMaterial, vlp_shader_texture;
	Value_ptr<ButiRendering::IResource_Mesh> vlp_mesh_cube, vlp_mesh_sphere;
	List<Value_ptr<ButiRendering::IResource_Texture>>list_vlp_texture;
	//ÉäÉ\Å[ÉXçÏê¨
	{
		{
			auto cubePrim = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>>();
			auto cubePrim_uvNormal = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal>>();
			auto cubePrim_normal = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_Normal>>();
			auto cubePrim_uv = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_UV>>();

			ButiRendering::BoxEightCorner boxEightCorner;
			ButiRendering::MeshHelper::CreateCube(Vector3(1, 1, 1), std::vector<Vector4>(), *cubePrim, boxEightCorner,true);
			ButiRendering::MeshHelper::VertexConvert(*cubePrim, *cubePrim_uvNormal);
			ButiRendering::MeshHelper::VertexConvert(*cubePrim, *cubePrim_normal);
			ButiRendering::MeshHelper::VertexConvert(*cubePrim, *cubePrim_uv);
			vlp_mesh_cube = ButiRendering::CreateMesh("Cube", { cubePrim,cubePrim_uv,cubePrim_normal,cubePrim_uvNormal }, vlp_graphicDevice);
		}
		{

			auto spherePrim = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>>();
			auto spherePrim_uvNormal = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal>>();
			auto spherePrim_normal = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_Normal>>();
			auto spherePrim_uv = ObjectFactory::Create<ButiRendering::MeshPrimitive<Vertex::Vertex_UV>>();

			ButiRendering::BoxEightCorner boxEightCorner;
			ButiRendering::MeshHelper::CreateSphere(Vector3(1, 1, 1),24,std::vector<Vector4>(),*spherePrim,boxEightCorner);
			ButiRendering::MeshHelper::VertexConvert(*spherePrim, *spherePrim_uvNormal);
			ButiRendering::MeshHelper::VertexConvert(*spherePrim, *spherePrim_normal);
			ButiRendering::MeshHelper::VertexConvert(*spherePrim, *spherePrim_uv);
			vlp_mesh_sphere = ButiRendering::CreateMesh("Sphere", { spherePrim,spherePrim_uv,spherePrim_normal,spherePrim_uvNormal }, vlp_graphicDevice);
		}
		vlp_shader_onlyMaterial = ButiRendering::CreateShader(ButiRendering::DefaultVertexShader::CreateNormal(vlp_graphicDevice),
			ButiRendering::DefaultPixelShader::CreateOnlyMaterial(vlp_graphicDevice), nullptr, "OnlyColor");
		vlp_shader_texture = ButiRendering::CreateShader(ButiRendering::DefaultVertexShader::CreateUV(vlp_graphicDevice),
			ButiRendering::DefaultPixelShader::CreateUV(vlp_graphicDevice), nullptr, "TextureMap");
		vlp_material_onlyColor = ButiRendering::CreateMaterial("", ButiRendering::MaterialValue(), vlp_shader_onlyMaterial, List<Value_ptr<ButiRendering::IResource_Texture>>(), ButiRendering::DrawSettings(), vlp_graphicDevice);
		list_vlp_texture.Add(ButiRendering::CreateTextureFromImageFile("Resource/ngineLogo.png", vlp_graphicDevice));
		vlp_material_textureMap = ButiRendering::CreateMaterial("", ButiRendering::MaterialValue(), vlp_shader_texture, list_vlp_texture, ButiRendering::DrawSettings(), vlp_graphicDevice);
		vlp_material_textureMap->SetIsAlpha(true);
	}

	auto drawObject = ButiRendering::CreateMeshDrawObject(vlp_mesh_sphere, vlp_material_textureMap, vlp_renderer, vlp_graphicDevice, nullptr, nullptr);
	drawObject->GetTransform()->SetLocalPositionX(2);
	drawObject->GetTransform()->SetLocalRotation(Vector3(12, 15, 18));
	vlp_renderer->RegistDrawObject(drawObject, 0);

	auto drawObject_onlyMaterial = ButiRendering::CreateMeshDrawObject(vlp_mesh_cube, vlp_material_onlyColor, vlp_renderer, vlp_graphicDevice, nullptr, nullptr);
	drawObject_onlyMaterial->GetTransform()->SetLocalPositionZ(10);
	drawObject_onlyMaterial->GetTransform()->SetLocalRotation(Vector3(12, 15, 18));
	vlp_renderer->RegistDrawObject(drawObject_onlyMaterial, 0);
	std::int32_t frame = 0;
	auto stopWatch = ButiEngine::StopWatch();

	drawObject->GetTransform()->SetBaseTransform(drawObject_onlyMaterial->GetTransform(),true);
	//drawObject->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = ButiColor::Red();
	while (vlp_window->Update()) {
		stopWatch.Start();
		//ï`âÊäJén
		vlp_renderer->RenderingStart();

		drawObject_onlyMaterial->GetTransform()->RollLocalRotationY_Degrees(1);
		//ï`âÊèàóù
		mainCameraPath->Execute();

		//ï`âÊèIóπ
		vlp_renderer->RenderingEnd();

		if (frame == 300) {
			vlp_renderer->UnRegistDrawObject(drawObject);
		}if (frame > 600) {
			vlp_renderer->RegistDrawObject(drawObject);
			frame = 0;
		}

		frame++;
		stopWatch.Stop();
		std::uint64_t millSec = stopWatch.GetMillSecond();
		constexpr float frameSec = 1.0f/60.0f*1000;
		if (millSec < frameSec) {
			Sleep(frameSec - millSec);
		}
	}
	//âï˙
	vlp_material_onlyColor = nullptr;
	vlp_material_textureMap = nullptr;
	vlp_shader_onlyMaterial = nullptr;
	vlp_shader_texture = nullptr;
	vlp_mesh_cube = nullptr;
	drawObject = nullptr; drawObject_onlyMaterial = nullptr;
	
	vlp_renderer->Release();
	vlp_window->Release();
	vlp_graphicDevice->Release();
	return 0;
}