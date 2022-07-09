#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/ShaderHelper_Dx12.h"

#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Shader_Dx12.h"
#include"ButiRendering_Dx12/Header/Vertex.h"
#include<direct.h>

namespace VS {
namespace UV {
#include"../../CompiledShader/VS_UV.fxc"
}
namespace UVAttribute {
#include"../../CompiledShader/VS_UVAttribute.fxc"
}
namespace UV_Shadow {
#include"../../CompiledShader/VS_UV_Shadow.fxc"
}
namespace UVFog {
#include"../../CompiledShader/VS_UVFog.fxc"
}
namespace UVNormal {
#include"../../CompiledShader/VS_UVNormal.fxc"
}
namespace UVColor {
#include"../../CompiledShader/VS_UVColor.fxc"
}
namespace UVNormal_Shadow {
#include"../../CompiledShader/VS_UVNormal_Shadow.fxc"
}
namespace UVNormalAttribute {
#include"../../CompiledShader/VS_UVNormalAttribute.fxc"
}
namespace UVNormalFog {
#include"../../CompiledShader/VS_UVNormalFog.fxc"
}
namespace UVNormalFog_Shadow {
#include"../../CompiledShader/VS_UVNormalFog_Shadow.fxc"
}
namespace UVNormalPhong_Shadow {
#include"../../CompiledShader/VS_UVNormalPhong_Shadow.fxc"
}
namespace UVNormalPosition {
#include"../../CompiledShader/VS_UVNormalPosition.fxc"
}
namespace UVNormalTangent_Shadow {
#include"../../CompiledShader/VS_UVNormalTangent_Shadow.fxc"
}
namespace UVPosition {
#include"../../CompiledShader/VS_UVPosition.fxc"
}
namespace ImmediateParticle {
#include"../../CompiledShader/VS_ImmediateParticle.fxc"
}
namespace UVNormalPhong {
#include"../../CompiledShader/VS_UVNormalPhong.fxc"
}
namespace UVFog_Shadow {
#include"../../CompiledShader/VS_UVFog_Shadow.fxc"
}
namespace ShadowMap {
#include"../../CompiledShader/VS_ShadowMap.fxc"
}
namespace ShadowMap_UV {
#include"../../CompiledShader/VS_ShadowMap_UV.fxc"
}
namespace UVNormalTangent {
#include"../../CompiledShader/VS_UVNormalTangent.fxc"
}
namespace UVNormalColor {
#include"../../CompiledShader/VS_UVNormalColor.fxc"
}
namespace UVNormalColorFog {
#include"../../CompiledShader/VS_UVNormalColorFog.fxc"
}
namespace UVNormalColorFog_Shadow {
#include"../../CompiledShader/VS_UVNormalColorFog_Shadow.fxc"
}
}
namespace PS {
namespace Grid {
#include"../../CompiledShader/PS_Grid.fxc"
}
namespace Grid_Shadow {
#include"../../CompiledShader/PS_Grid_Shadow.fxc"
}
namespace Grid_worldColor {
#include"../../CompiledShader/PS_Grid_worldColor.fxc"
}
namespace GridFog {
#include"../../CompiledShader/PS_GridFog.fxc"
}
namespace OnlyMaterial {
#include"../../CompiledShader/PS_OnlyMaterial.fxc"
}
namespace OnlyMaterial_BloomSource {
#include"../../CompiledShader/PS_OnlyMaterial_BloomSource.fxc"
}
namespace PostEffect_GausBlur {
#include"../../CompiledShader/PS_PostEffect_GausBlur.fxc"
}
namespace PostEffect_GausBlurFloat2 {
#include"../../CompiledShader/PS_PostEffect_GausBlurFloat2.fxc"
}
namespace ShadowMap {
#include"../../CompiledShader/PS_ShadowMap.fxc"
}
namespace ShadowMap_UV {
#include"../../CompiledShader/PS_ShadowMap_UV.fxc"
}
namespace SpriteAnimation {
#include"../../CompiledShader/PS_SpriteAnimation.fxc"
}
namespace UVColor {
#include"../../CompiledShader/PS_UVColor.fxc"
}
namespace UVColor_Shadow {
#include"../../CompiledShader/PS_UVColor_Shadow.fxc"
}
namespace UVColorFog {
#include"../../CompiledShader/PS_UVColorFog.fxc"
}
namespace UVColorFog_Shadow {
#include"../../CompiledShader/PS_UVColorFog_Shadow.fxc"
}
namespace UVDepth {
#include"../../CompiledShader/PS_UVDepth.fxc"
}
namespace UV {
#include"../../CompiledShader/PS_UV.fxc"
}
namespace UVFog {
#include"../../CompiledShader/PS_UVFog.fxc"
}
namespace UVFog_Shadow {
#include"../../CompiledShader/PS_UVFog_Shadow.fxc"
}
namespace UVNormal {
#include"../../CompiledShader/PS_UVNormal.fxc"
}
namespace UVNormal_Shadow {
#include"../../CompiledShader/PS_UVNormal_Shadow.fxc"
}
namespace UVNormalColor {
#include"../../CompiledShader/PS_UVNormalColor.fxc"
}
namespace UVNormalColor_Shadow {
#include"../../CompiledShader/PS_UVNormalColor_Shadow.fxc"
}
namespace UVNormalColorFog {
#include"../../CompiledShader/PS_UVNormalColorFog.fxc"
}
namespace UVNormalColorFog_Shadow {
#include"../../CompiledShader/PS_UVNormalColorFog_Shadow.fxc"
}
namespace UVNormalFog {
#include"../../CompiledShader/PS_UVNormalFog.fxc"
}
namespace UVNormalFog_Shadow {
#include"../../CompiledShader/PS_UVNormalFog_Shadow.fxc"
}
}
namespace GS {
namespace OutLine {
#include"../../CompiledShader/GS_OutLine.fxc"
}
namespace PointToCube {
#include"../../CompiledShader/GS_PointToCube.fxc"
}
namespace PointToCube_WithoutUV {
#include"../../CompiledShader/GS_PointToCube_WithoutUV.fxc"
}
namespace PointToPlane {
#include"../../CompiledShader/GS_PointToPlane.fxc"
}
namespace PointToTriPolygon_WithoutUV {
#include"../../CompiledShader/GS_PointToTriPolygon_WithoutUV.fxc"
}
}
ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Resource_PixelShader_Dx12(const std::string& arg_shaderName,const std::string& arg_filePath, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	m_shaderName = arg_shaderName;
	auto file = BinaryReader(arg_filePath);
	auto size = file.GetReamainSize();
	auto source = file.ReadCharactor();
	CreateShaderData(reinterpret_cast<unsigned char*> (source), size,false);
	delete source;
}

ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Resource_PixelShader_Dx12(const std::string& arg_shaderName, const std::stringstream& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	m_shaderName = arg_shaderName;

	ShaderCompile_Dx12::Compile(arg_source.str(), nullptr, m_pixelShaderBlob, "PSMain", "ps_5_1");
}

ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Resource_PixelShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName,  Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	m_shaderName = arg_shaderName;
	CreateShaderData(arg_compiledSource, arg_size,true);

}


ButiEngine::ButiRendering::Resource_PixelShader_Dx12::~Resource_PixelShader_Dx12()
{

}

void ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Attach() const
{

}

void ButiEngine::ButiRendering::Resource_PixelShader_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

Microsoft::WRL::ComPtr<ID3D10Blob>& ButiEngine::ButiRendering::Resource_PixelShader_Dx12::GetPixelShaderBlob()
{
	return m_pixelShaderBlob;
}

void ButiEngine::ButiRendering::Resource_PixelShader_Dx12::CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const bool arg_isfxc)
{
	auto hr = D3DCreateBlob(arg_size, m_pixelShaderBlob.GetAddressOf());
	memcpy(m_pixelShaderBlob->GetBufferPointer(), arg_compiledSource, arg_size);
	if (arg_isfxc) {
		ShaderHelper::FXCShaderReflection(arg_compiledSource, arg_size, m_list_constBufferReflection, m_list_textureReflection,m_list_samplerReflection,m_inputLayoutReflection, m_outputLayoutReflection);
	}
	else {
		ShaderHelper::DXCShaderReflection(arg_compiledSource,arg_size,m_list_constBufferReflection, m_list_textureReflection, m_list_samplerReflection, m_inputLayoutReflection, m_outputLayoutReflection);
	}

}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Resource_VertexShader_Dx12(const std::string& arg_shaderName, const std::string& arg_filePath, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	m_shaderName = arg_shaderName;
	auto file = BinaryReader(arg_filePath);
	auto size = file.GetReamainSize();
	auto source = file.ReadCharactor();
	CreateShaderData(reinterpret_cast<unsigned char*> (source), size,false);
	delete source;
}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Resource_VertexShader_Dx12(const std::string& arg_shaderName, const std::stringstream& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	m_shaderName = arg_shaderName;

	ShaderCompile_Dx12::Compile(arg_source.str(), nullptr,m_vertexShaderBlob ,"VSMain", "vs_5_1");
	std::vector<ShaderElement> vec_elem;

	for (auto inputLayout : m_list_inputElementDesc) {
		auto sementicStr = std::string(inputLayout.SemanticName);
		if (sementicStr == "NORMAL") {
			m_inputVertexType |= Vertex::VertexFlag::Normal;
		}
		else if (sementicStr == "TEXCOORD") {
			m_inputVertexType |= Vertex::VertexFlag::UV;
		}
		else if (sementicStr == "TANGENT") {
			m_inputVertexType |= Vertex::VertexFlag::Tangent;
		}
		else if (sementicStr == "COLOR") {
			m_inputVertexType |= Vertex::VertexFlag::Color;
		}
		else if (sementicStr == "BONE") {
			m_inputVertexType |= Vertex::VertexFlag::SingleBone;
		}
		else if (sementicStr == "BONEINDEXTWO") {
			m_inputVertexType |= Vertex::VertexFlag::DouleBone;
		}
		else if (sementicStr == "BONEINDEXFOUR") {
			m_inputVertexType ^= Vertex::VertexFlag::DouleBone;
			m_inputVertexType |= Vertex::VertexFlag::QuadBone;
		}
		else if (sementicStr == "SDEFC") {
			m_inputVertexType ^= Vertex::VertexFlag::QuadBone;
			m_inputVertexType |= Vertex::VertexFlag::PMX;
		}
	}
}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Resource_VertexShader_Dx12(const unsigned char* arg_compiledSource,  const std::int32_t arg_size, const std::string& arg_shaderName, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	m_shaderName = arg_shaderName;
	CreateShaderData(arg_compiledSource, arg_size,true);
}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::~Resource_VertexShader_Dx12()
{
	for (std::int32_t i = 0; i < m_list_inputElementDesc.GetSize(); i++) {
		ButiMemorySystem::Allocator::deallocate(m_list_inputElementDesc.at(i).SemanticName);
	}
}

void ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Attach() const
{
}

void ButiEngine::ButiRendering::Resource_VertexShader_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

Microsoft::WRL::ComPtr<ID3DBlob>& ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetVertexShaderBlob()
{
	return m_vertexShaderBlob;
}

const ButiEngine::List<D3D12_INPUT_ELEMENT_DESC>& ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetInputLayoutList()const
{
	return m_list_inputElementDesc;
}
std::uint32_t ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetInputVertexType() const
{
	return m_inputVertexType;
}

void ButiEngine::ButiRendering::Resource_VertexShader_Dx12::CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const bool arg_isfxc)
{
	auto hr = D3DCreateBlob(arg_size, m_vertexShaderBlob.GetAddressOf());
	memcpy(m_vertexShaderBlob->GetBufferPointer(), arg_compiledSource, arg_size);
	if (arg_isfxc) {
		ShaderHelper::FXCShaderReflection(arg_compiledSource, arg_size, m_list_constBufferReflection, m_list_textureReflection, m_list_samplerReflection, m_inputLayoutReflection, m_outputLayoutReflection);
	}
	else {
		ShaderHelper::DXCShaderReflection(arg_compiledSource, arg_size, m_list_constBufferReflection, m_list_textureReflection, m_list_samplerReflection, m_inputLayoutReflection, m_outputLayoutReflection);
	}
	for (auto elem : m_inputLayoutReflection.m_list_element) {
		auto d3dElem = D3D12_INPUT_ELEMENT_DESC();
		d3dElem.Format = static_cast<DXGI_FORMAT>(elem.format);
		d3dElem.InputSlot = elem.inputSlot;
		d3dElem.SemanticIndex = elem.semanticIndex;
		d3dElem.AlignedByteOffset = elem.alignedByteOffset;
		d3dElem.InstanceDataStepRate = elem.instanceDataStepRate;
		d3dElem.InputSlotClass = static_cast<D3D12_INPUT_CLASSIFICATION>(elem.inputSlotClass);
		char* semanticP = reinterpret_cast<char*>(ButiMemorySystem::Allocator::allocate(elem.semanticName.size() + 1));
		memcpy(semanticP, elem.semanticName.c_str(), elem.semanticName.size() + 1);
		d3dElem.SemanticName = semanticP;
		m_list_inputElementDesc.Add(d3dElem);
		if (elem.semanticName == "NORMAL") {
			m_inputVertexType |= Vertex::VertexFlag::Normal;
		}
		else if (elem.semanticName == "TEXCOORD") {
			m_inputVertexType |= Vertex::VertexFlag::UV;
		}
		else if (elem.semanticName == "TANGENT") {
			m_inputVertexType |= Vertex::VertexFlag::Tangent;
		}
		else if (elem.semanticName == "COLOR") {
			m_inputVertexType |= Vertex::VertexFlag::Color;
		}
		else if (elem.semanticName == "BONE") {
			m_inputVertexType |= Vertex::VertexFlag::SingleBone;
		}
		else if (elem.semanticName == "BONEINDEXTWO") {
			m_inputVertexType |= Vertex::VertexFlag::DouleBone;
		}
		else if (elem.semanticName == "BONEINDEXFOUR") {
			m_inputVertexType ^= Vertex::VertexFlag::DouleBone;
			m_inputVertexType |= Vertex::VertexFlag::QuadBone;
		}
		else if (elem.semanticName == "SDEFC") {
			m_inputVertexType ^= Vertex::VertexFlag::QuadBone;
			m_inputVertexType |= Vertex::VertexFlag::PMX;
		}
	}
}


void ButiEngine::ButiRendering::ShaderCompile_Dx12::Compile(const std::string& arg_source, const char* arg_filePath, Microsoft::WRL::ComPtr<ID3DBlob>& arg_out, const char* arg_entryPoint, const char* arg_shaderModel)
{
	Microsoft::WRL::ComPtr<ID3DBlob> error = nullptr;
	auto hr = D3DCompile(arg_source.c_str(), arg_source.length(),arg_filePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, arg_entryPoint, arg_shaderModel, D3DCOMPILE_ENABLE_STRICTNESS, 0, arg_out.GetAddressOf(), error.GetAddressOf());

	if (error != nullptr) {
		MessageBoxA(nullptr, (char*)error->GetBufferPointer(), "Shader error", MB_ICONERROR | MB_OK);
	}
}

ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Resource_GeometryShader_Dx12(const std::string& arg_shaderName, const std::string& arg_shaderPath, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	m_shaderName = StringHelper::GetFileName(arg_shaderPath, false);
	auto file = BinaryReader(arg_shaderPath);
	auto size = file.GetReamainSize();
	auto source = file.ReadCharactor();
	CreateShaderData(reinterpret_cast<unsigned char*> (source), size,false);
	delete source;
}
ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Resource_GeometryShader_Dx12(const std::string& arg_shaderName, const std::stringstream& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	m_shaderName = arg_shaderName;
	ShaderCompile_Dx12::Compile(arg_source.str(),arg_shaderName.c_str(), m_geometryShaderBlob,"GSMain","gs_5_1");
}

ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Resource_GeometryShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	m_shaderName = arg_shaderName;
	CreateShaderData(arg_compiledSource, arg_size,true);
}

ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::~Resource_GeometryShader_Dx12()
{
}

void ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Attach() const
{
}

void ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	m_vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

Microsoft::WRL::ComPtr<ID3DBlob>& ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::GetGeometryShaderBlob()
{
	return m_geometryShaderBlob;
}


void ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const bool arg_isfxc)
{
	auto hr = D3DCreateBlob(arg_size, m_geometryShaderBlob.GetAddressOf());
	memcpy(m_geometryShaderBlob->GetBufferPointer(), arg_compiledSource, arg_size); if (arg_isfxc) {
		ShaderHelper::FXCShaderReflection(arg_compiledSource, arg_size, m_list_constBufferReflection, m_list_textureReflection, m_list_samplerReflection, m_inputLayoutReflection, m_outputLayoutReflection);
	}
	else {
		ShaderHelper::DXCShaderReflection(arg_compiledSource, arg_size, m_list_constBufferReflection, m_list_textureReflection, m_list_samplerReflection, m_inputLayoutReflection, m_outputLayoutReflection);
	}
}



ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::CreateVertexShader(const std::string& arg_shaderName, const std::string& arg_filePath, const std::string& arg_inputLayoutfilePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(arg_shaderName, arg_filePath, arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::CreatePixelShader(const std::string& arg_shaderName, const std::string& arg_filePath, const std::string& arg_outputFormatFilePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(arg_shaderName, arg_filePath,arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::CreateGeometryShader(const std::string& arg_shaderName, const std::string& arg_filePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_GeometryShader_Dx12>(arg_shaderName,arg_filePath, arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::CreateVertexShaderFromSource(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(arg_shaderName, arg_source, arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::CreatePixelShaderFromSource(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(arg_shaderName, arg_source, arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::CreateGeometryShaderFromSource(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_GeometryShader_Dx12>(arg_shaderName, arg_source, arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Shader> ButiEngine::ButiRendering::CreateShader(Value_ptr<IResource_VertexShader> arg_vlp_vertexShader, Value_ptr<IResource_PixelShader> arg_vlp_pixelShader, Value_ptr<IResource_GeometryShader> arg_vlp_geometryShader, const std::string& arg_shaderName)
{
	return (arg_vlp_geometryShader?arg_vlp_vertexShader->GetOutputLayout()==arg_vlp_geometryShader->GetInputLayout()&& arg_vlp_geometryShader->GetOutputLayout() == arg_vlp_pixelShader->GetInputLayout():
		arg_vlp_vertexShader->GetOutputLayout() == arg_vlp_pixelShader->GetInputLayout())?		
		ObjectFactory::Create<Resource_Shader>(arg_vlp_vertexShader, arg_vlp_pixelShader, arg_vlp_geometryShader, arg_shaderName):nullptr;
}
void ButiEngine::ButiRendering::ShaderCompile(const std::string& arg_sourceFilePath, const std::string& arg_outputFilePath)
{
	return ShaderHelper::Compile(arg_sourceFilePath, arg_outputFilePath, ShaderHelper::CompileType::Dx12);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormal(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormal::g_VSMain, sizeof(VS::UVNormal::g_VSMain), "UVNormal",  arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormal_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader:shader=ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormal_Shadow::g_VSMain, sizeof(VS::UVNormal_Shadow::g_VSMain), "UVNormal_Shadow",  arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalAttribute(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalAttribute::g_VSMain, sizeof(VS::UVNormalAttribute::g_VSMain), "UVNormalAttribute", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalColor::g_VSMain, sizeof(VS::UVNormalColor::g_VSMain), "UVNormalColor", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalColorFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalColorFog::g_VSMain, sizeof(VS::UVNormalColorFog::g_VSMain), "UVNormalColorFog", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalColorFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalColorFog_Shadow::g_VSMain, sizeof(VS::UVNormalColorFog_Shadow::g_VSMain), "UVNormalColorFog_Shadow", arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalFog::g_VSMain, sizeof(VS::UVNormalFog::g_VSMain), "UVNormalFog", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalFog_Shadow::g_VSMain, sizeof(VS::UVNormalFog_Shadow::g_VSMain), "UVNormalFog_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalPhong(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalPhong::g_VSMain, sizeof(VS::UVNormalPhong::g_VSMain), "UVNormalPhong", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalPhong_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalPhong_Shadow::g_VSMain, sizeof(VS::UVNormalPhong_Shadow::g_VSMain), "UVNormalPhong_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalPosition(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalPosition::g_VSMain, sizeof(VS::UVNormalPosition::g_VSMain), "UVNormalPosition", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalTangent(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalTangent::g_VSMain, sizeof(VS::UVNormalTangent::g_VSMain), "UVNormalTangent", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalTangent_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalTangent_Shadow::g_VSMain, sizeof(VS::UVNormalTangent_Shadow::g_VSMain), "UVNormalTangent_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVPosition(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVPosition::g_VSMain, sizeof(VS::UVPosition::g_VSMain), "UVPosition", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateShadowMap(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::ShadowMap::g_VSMain, sizeof(VS::ShadowMap::g_VSMain), "ShadowMap", arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateShadowMap_UV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::ShadowMap_UV::g_VSMain, sizeof(VS::ShadowMap_UV::g_VSMain), "ShadowMap_UV", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader?shader:shader= ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UV::g_VSMain, sizeof(VS::UV::g_VSMain), "UV", arg_vlp_graphicDevice);;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUV_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UV_Shadow::g_VSMain, sizeof(VS::UV_Shadow::g_VSMain), "UV_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVAttribute(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVAttribute::g_VSMain, sizeof(VS::UVAttribute::g_VSMain), "UVAttribute", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVColor::g_VSMain, sizeof(VS::UVColor::g_VSMain), "UVColor", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVFog::g_VSMain, sizeof(VS::UVFog::g_VSMain), "UVFog", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVFog_Shadow::g_VSMain, sizeof(VS::UVFog_Shadow::g_VSMain), "UVFog_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateGrid(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::Grid::g_PSMain, sizeof(PS::Grid::g_PSMain), "Grid", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateGrid_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::Grid_Shadow::g_PSMain, sizeof(PS::Grid_Shadow::g_PSMain), "Grid_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateGrid_worldColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::Grid_worldColor::g_PSMain, sizeof(PS::Grid_worldColor::g_PSMain), "Grid_worldColor", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateGridFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::GridFog::g_PSMain, sizeof(PS::GridFog::g_PSMain), "GridFog", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateOnlyMaterial(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::OnlyMaterial::g_PSMain, sizeof(PS::OnlyMaterial::g_PSMain), "OnlyMaterial", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateOnlyMaterial_BloomSource(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::OnlyMaterial_BloomSource::g_PSMain, sizeof(PS::OnlyMaterial_BloomSource::g_PSMain), "OnlyMaterial_BloomSource", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreatePostEffect_GausBlur(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::PostEffect_GausBlur::g_PSMain, sizeof(PS::PostEffect_GausBlur::g_PSMain), "PostEffect_GausBlur", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreatePostEffect_GausBlurFloat2(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::PostEffect_GausBlurFloat2::g_PSMain, sizeof(PS::PostEffect_GausBlurFloat2::g_PSMain), "PostEffect_GausBlurFloat2", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateShadowMap(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::ShadowMap::g_PSMain, sizeof(PS::ShadowMap::g_PSMain), "ShadowMap", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateShadowMap_UV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::ShadowMap_UV::g_PSMain, sizeof(PS::ShadowMap_UV::g_PSMain), "ShadowMap_UV", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateSpriteAnimation(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::SpriteAnimation::g_PSMain, sizeof(PS::SpriteAnimation::g_PSMain), "SpriteAnimation", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVColor::g_PSMain, sizeof(PS::UVColor::g_PSMain), "UVColor", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVColor_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVColor_Shadow::g_PSMain, sizeof(PS::UVColor_Shadow::g_PSMain), "UVColor_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVColorFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVColorFog::g_PSMain, sizeof(PS::UVColorFog::g_PSMain), "UVColorFog", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVColorFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVColorFog_Shadow::g_PSMain, sizeof(PS::UVColorFog_Shadow::g_PSMain), "UVColorFog_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVDepth(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVDepth::g_PSMain, sizeof(PS::UVDepth::g_PSMain), "UVDepth", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UV::g_PSMain, sizeof(PS::UV::g_PSMain), "UV", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVFog::g_PSMain, sizeof(PS::UVFog::g_PSMain), "UVFog", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVFog_Shadow::g_PSMain, sizeof(PS::UVFog_Shadow::g_PSMain), "UVFog_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormal(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormal::g_PSMain, sizeof(PS::UVNormal::g_PSMain), "UVNormal", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormal_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormal_Shadow::g_PSMain, sizeof(PS::UVNormal_Shadow::g_PSMain), "UVNormal_Shadow", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalColor::g_PSMain, sizeof(PS::UVNormalColor::g_PSMain), "UVNormalColor", arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalColor_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalColor_Shadow::g_PSMain, sizeof(PS::UVNormalColor_Shadow::g_PSMain), "UVNormalColor_Shadow", arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalColorFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	 return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalColorFog::g_PSMain, sizeof(PS::UVNormalColorFog::g_PSMain), "UVNormalColorFog", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalColorFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	 return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalColorFog_Shadow::g_PSMain, sizeof(PS::UVNormalColorFog_Shadow::g_PSMain), "UVNormalColorFog_Shadow", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	 return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalFog::g_PSMain, sizeof(PS::UVNormalFog::g_PSMain), "UVNormalFog", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 static Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> shader = nullptr;
	 return shader ? shader : shader = ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalFog_Shadow::g_PSMain, sizeof(PS::UVNormalFog_Shadow::g_PSMain), "UVNormalFog_Shadow", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreateOutLine(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 static Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> shader = nullptr;
	 return shader ? shader : shader = ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::OutLine::g_GSMain, sizeof(GS::OutLine::g_GSMain), "OutLine", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreatePointToCube(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 static Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> shader = nullptr;
	 return shader ? shader : shader = ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::PointToCube::g_GSMain, sizeof(GS::PointToCube::g_GSMain), "PointToCube", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreatePointToCube_WithoutUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 static Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> shader = nullptr;
	 return shader ? shader : shader = ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::PointToCube_WithoutUV::g_GSMain, sizeof(GS::PointToCube_WithoutUV::g_GSMain), "PointToCube_WithoutUV", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreatePointToPlane(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 static Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> shader = nullptr;
	 return shader ? shader : shader = ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::PointToPlane::g_GSMain, sizeof(GS::PointToPlane::g_GSMain), "PointToPlane", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreatePointToTriPolygon_WithoutUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 static Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> shader = nullptr;
	 return shader ? shader : shader = ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::PointToTriPolygon_WithoutUV::g_GSMain, sizeof(GS::PointToTriPolygon_WithoutUV::g_GSMain), "PointToTriPolygon_WithoutUV", arg_vlp_graphicDevice);
 }
