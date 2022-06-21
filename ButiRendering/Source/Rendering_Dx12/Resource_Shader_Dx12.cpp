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
ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Resource_PixelShader_Dx12(const std::string& arg_shaderName, const std::string& arg_filePath, const std::string& arg_outputFormatSource ,Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = arg_shaderName;
	auto file = BinaryReader(arg_filePath);
	auto size = file.GetReamainSize();
	auto source = file.ReadCharactor();
	ShaderHelper::InputCereal(vec_formats, arg_outputFormatSource);
	CreateShaderData(reinterpret_cast<unsigned char*> (source), size, vec_formats);
	delete source;
}

ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Resource_PixelShader_Dx12(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = StringHelper::GetFileName(arg_shaderName, false);

	ShaderCompile_Dx12::Compile(arg_source, nullptr, pixelShaderBlob, "PSMain", "ps_5_1");
	ShaderHelper::CreateDx12PSOutputVector(nullptr, arg_source, vec_formats);
}

ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Resource_PixelShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName, const std::vector<std::int32_t>& arg_vec_formats, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = StringHelper::GetFileName(arg_shaderName, false);
	CreateShaderData(arg_compiledSource, arg_size, arg_vec_formats);
}


ButiEngine::ButiRendering::Resource_PixelShader_Dx12::~Resource_PixelShader_Dx12()
{

}

void ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Attach() const
{

}

void ButiEngine::ButiRendering::Resource_PixelShader_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{

	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

std::vector<std::int32_t>& ButiEngine::ButiRendering::Resource_PixelShader_Dx12::GetFormats()
{
	return vec_formats;
}


Microsoft::WRL::ComPtr<ID3D10Blob>& ButiEngine::ButiRendering::Resource_PixelShader_Dx12::GetPixelShaderBlob()
{
	return pixelShaderBlob;
}

std::string ButiEngine::ButiRendering::Resource_PixelShader_Dx12::GetName() const
{
	return shaderName;
}

std::vector<std::uint32_t> ButiEngine::ButiRendering::Resource_PixelShader_Dx12::GetOutputPixelFormat() const
{
	assert(0 && "ToDo");
	return std::vector<std::uint32_t>();
}

void ButiEngine::ButiRendering::Resource_PixelShader_Dx12::CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::vector<std::int32_t>& arg_vec_format)
{
	auto hr = D3DCreateBlob(arg_size, pixelShaderBlob.GetAddressOf());
	memcpy(pixelShaderBlob->GetBufferPointer(), arg_compiledSource, arg_size);
	vec_formats = arg_vec_format;
}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Resource_VertexShader_Dx12(const std::string& arg_shaderName, const std::string& arg_filePath, const std::string& arg_inputLayoutPath, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = arg_shaderName;
	auto file = BinaryReader(arg_filePath);
	auto size = file.GetReamainSize();
	auto source = file.ReadCharactor();
	std::vector<InputElement> vec_elem;
	ShaderHelper::InputCereal(vec_elem, arg_inputLayoutPath);
	CreateShaderData(reinterpret_cast<unsigned char*> (source), size,vec_elem);
	delete source;
}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Resource_VertexShader_Dx12(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = StringHelper::GetFileName(arg_shaderName, false);

	ShaderCompile_Dx12::Compile(arg_source, nullptr,vertexShaderBlob ,"VSMain", "vs_5_1");
	std::vector<InputElement> vec_elem;
	ShaderHelper::CreateInputElementVector(nullptr,arg_source, vec_elem);
	for (auto inputLayout : vec_inputElementDesc) {
		auto sementicStr = std::string(inputLayout.SemanticName);
		if (sementicStr == "NORMAL") {
			inputVertexType |= Vertex::VertexFlag::Normal;
		}
		else if (sementicStr == "TEXCOORD") {
			inputVertexType |= Vertex::VertexFlag::UV;
		}
		else if (sementicStr == "TANGENT") {
			inputVertexType |= Vertex::VertexFlag::Tangent;
		}
		else if (sementicStr == "COLOR") {
			inputVertexType |= Vertex::VertexFlag::Color;
		}
		else if (sementicStr == "BONE") {
			inputVertexType |= Vertex::VertexFlag::SingleBone;
		}
		else if (sementicStr == "BONEINDEXTWO") {
			inputVertexType |= Vertex::VertexFlag::DouleBone;
		}
		else if (sementicStr == "BONEINDEXFOUR") {
			inputVertexType ^= Vertex::VertexFlag::DouleBone;
			inputVertexType |= Vertex::VertexFlag::QuadBone;
		}
		else if (sementicStr == "SDEFC") {
			inputVertexType ^= Vertex::VertexFlag::QuadBone;
			inputVertexType |= Vertex::VertexFlag::PMX;
		}
	}
}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Resource_VertexShader_Dx12(const unsigned char* arg_compiledSource,  const std::int32_t arg_size, const std::string& arg_shaderName,const std::vector<InputElement>& arg_vec_inputElemnt, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = arg_shaderName;
	CreateShaderData(arg_compiledSource, arg_size, arg_vec_inputElemnt);
}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::~Resource_VertexShader_Dx12()
{
	for (std::int32_t i = 0; i < vec_inputElementDesc.size(); i++) {
		ButiMemorySystem::Allocator::deallocate(vec_inputElementDesc.at(i).SemanticName);
	}
}

void ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Attach() const
{
}

void ButiEngine::ButiRendering::Resource_VertexShader_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

Microsoft::WRL::ComPtr<ID3DBlob>& ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetVertexShaderBlob()
{
	return vertexShaderBlob;
}

std::vector<D3D12_INPUT_ELEMENT_DESC>& ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetInputLayoutVector()
{
	return vec_inputElementDesc;
}

std::string ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetName() const
{
	return shaderName;
}

std::uint32_t ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetInputVertexType() const
{
	return inputVertexType;
}

void ButiEngine::ButiRendering::Resource_VertexShader_Dx12::CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size,const std::vector<InputElement>& arg_vec_inputElemnt)
{
	auto hr = D3DCreateBlob(arg_size, vertexShaderBlob.GetAddressOf());
	memcpy(vertexShaderBlob->GetBufferPointer(), arg_compiledSource, arg_size);
	for (auto elem : arg_vec_inputElemnt) {
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
		vec_inputElementDesc.push_back(d3dElem);
		if (elem.semanticName == "NORMAL") {
			inputVertexType |= Vertex::VertexFlag::Normal;
		}
		else if (elem.semanticName == "TEXCOORD") {
			inputVertexType |= Vertex::VertexFlag::UV;
		}
		else if (elem.semanticName == "TANGENT") {
			inputVertexType |= Vertex::VertexFlag::Tangent;
		}
		else if (elem.semanticName == "COLOR") {
			inputVertexType |= Vertex::VertexFlag::Color;
		}
		else if (elem.semanticName == "BONE") {
			inputVertexType |= Vertex::VertexFlag::SingleBone;
		}
		else if (elem.semanticName == "BONEINDEXTWO") {
			inputVertexType |= Vertex::VertexFlag::DouleBone;
		}
		else if (elem.semanticName == "BONEINDEXFOUR") {
			inputVertexType ^= Vertex::VertexFlag::DouleBone;
			inputVertexType |= Vertex::VertexFlag::QuadBone;
		}
		else if (elem.semanticName == "SDEFC") {
			inputVertexType ^= Vertex::VertexFlag::QuadBone;
			inputVertexType |= Vertex::VertexFlag::PMX;
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

ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Resource_GeometryShader_Dx12(const std::string& arg_shaderPath, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = StringHelper::GetFileName(arg_shaderPath, false);
	auto file = BinaryReader(arg_shaderPath);
	auto size = file.GetReamainSize();
	auto source = file.ReadCharactor();
	CreateShaderData(reinterpret_cast<unsigned char*> (source), size);
	delete source;
}
ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Resource_GeometryShader_Dx12(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = arg_shaderName;
	ShaderCompile_Dx12::Compile(arg_source,arg_shaderName.c_str(), geometryShaderBlob,"GSMain","gs_5_1");
}

ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Resource_GeometryShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = arg_shaderName;
	CreateShaderData(arg_compiledSource, arg_size);
}

ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::~Resource_GeometryShader_Dx12()
{
}

void ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Attach() const
{
}

void ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

Microsoft::WRL::ComPtr<ID3DBlob>& ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::GetGeometryShaderBlob()
{
	return geometryShaderBlob;
}

std::string ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::GetName() const
{
	return shaderName;
}

void ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size)
{
	auto hr = D3DCreateBlob(arg_size, geometryShaderBlob.GetAddressOf());
	memcpy(geometryShaderBlob->GetBufferPointer(), arg_compiledSource, arg_size);
}



ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::CreateVertexShader(const std::string& arg_shaderName, const std::string& arg_filePath, const std::string& arg_inputLayoutfilePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(arg_shaderName, arg_filePath, arg_inputLayoutfilePath,arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::CreatePixelShader(const std::string& arg_shaderName, const std::string& arg_filePath, const std::string& arg_outputFormatFilePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(arg_shaderName, arg_filePath,arg_outputFormatFilePath ,arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::CreateGeometryShader(const std::string& arg_shaderName, const std::string& arg_filePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_GeometryShader_Dx12>(arg_filePath, arg_vlp_graphicDevice);
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
	return ObjectFactory::Create<Resource_Shader>(arg_vlp_vertexShader, arg_vlp_pixelShader, arg_vlp_geometryShader, arg_shaderName);
}
void ButiEngine::ButiRendering::ShaderCompile(const std::string& arg_sourceFilePath, const std::string& arg_outputFilePath)
{
	return ShaderHelper::Compile(arg_sourceFilePath, arg_outputFilePath, ShaderHelper::CompileType::Dx12);
}

std::vector<ButiEngine::ButiRendering::InputElement>& GetPositionOnlyElement() {
	static auto output = std::vector<ButiEngine::ButiRendering::InputElement>();
	if (!output.size()) {
		ButiEngine::ButiRendering::InputElement elem;
		elem.semanticName = "POSITION";
		elem.format = ButiEngine::ButiRendering::Format::R32G32B32_FLOAT;
		elem.alignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		elem.inputSlot = 0;
		elem.semanticIndex = 0;
		elem.instanceDataStepRate = 0;
		elem.inputSlotClass = ButiEngine::ButiRendering::InputClassfication::PER_VERTEX_DATA;
		output.push_back(elem);
	}
	return output;
}
std::vector<ButiEngine::ButiRendering::InputElement>& GetUVElement() {
	static auto output = std::vector<ButiEngine::ButiRendering::InputElement>();
	if (!output.size()) {
		ButiEngine::ButiRendering::InputElement elem;
		elem.semanticName = "POSITION";
		elem.format = ButiEngine::ButiRendering::Format::R32G32B32_FLOAT;
		elem.alignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		elem.inputSlot = 0;
		elem.semanticIndex = 0;
		elem.instanceDataStepRate = 0;
		elem.inputSlotClass = ButiEngine::ButiRendering::InputClassfication::PER_VERTEX_DATA;
		output.push_back(elem);
		elem.semanticName = "TEXCOORD";
		elem.format = ButiEngine::ButiRendering::Format::R32G32_FLOAT;
		output.push_back(elem);
	}
	return output;
}
std::vector<ButiEngine::ButiRendering::InputElement>& GetUVNormalElement() {
	static auto output = std::vector<ButiEngine::ButiRendering::InputElement>();
	if (!output.size()) {
		ButiEngine::ButiRendering::InputElement elem;
		elem.semanticName = "POSITION";
		elem.format = ButiEngine::ButiRendering::Format::R32G32B32_FLOAT;
		elem.alignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		elem.inputSlot = 0;
		elem.semanticIndex = 0;
		elem.instanceDataStepRate = 0;
		elem.inputSlotClass = ButiEngine::ButiRendering::InputClassfication::PER_VERTEX_DATA;
		output.push_back(elem);
		elem.semanticName = "TEXCOORD";
		elem.format = ButiEngine::ButiRendering::Format::R32G32_FLOAT;
		output.push_back(elem);
		elem.semanticName = "NORMAL";
		elem.format = ButiEngine::ButiRendering::Format::R32G32B32_FLOAT;
		output.push_back(elem);
	}
	return output;
}
std::vector<ButiEngine::ButiRendering::InputElement>& GetUVNormalColorElement() {
	static auto output = std::vector<ButiEngine::ButiRendering::InputElement>();
	if (!output.size()) {
		ButiEngine::ButiRendering::InputElement elem;
		elem.semanticName = "POSITION";
		elem.format = ButiEngine::ButiRendering::Format::R32G32B32_FLOAT;
		elem.alignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		elem.inputSlot = 0;
		elem.semanticIndex = 0;
		elem.instanceDataStepRate = 0;
		elem.inputSlotClass = ButiEngine::ButiRendering::InputClassfication::PER_VERTEX_DATA;
		output.push_back(elem);
		elem.semanticName = "TEXCOORD";
		elem.format = ButiEngine::ButiRendering::Format::R32G32_FLOAT;
		output.push_back(elem);
		elem.semanticName = "NORMAL";
		elem.format = ButiEngine::ButiRendering::Format::R32G32B32_FLOAT;
		output.push_back(elem);
		elem.semanticName = "COLOR";
		elem.format = ButiEngine::ButiRendering::Format::R32G32B32A32_FLOAT;
		output.push_back(elem);
	}
	return output;
}
std::vector<ButiEngine::ButiRendering::InputElement>& GetUVColorElement() {
	static auto output = std::vector<ButiEngine::ButiRendering::InputElement>();
	if (!output.size()) {
		ButiEngine::ButiRendering::InputElement elem;
		elem.semanticName = "POSITION";
		elem.format = ButiEngine::ButiRendering::Format::R32G32B32_FLOAT;
		elem.alignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		elem.inputSlot = 0;
		elem.semanticIndex = 0;
		elem.instanceDataStepRate = 0;
		elem.inputSlotClass = ButiEngine::ButiRendering::InputClassfication::PER_VERTEX_DATA;
		output.push_back(elem);
		elem.semanticName = "TEXCOORD";
		elem.format = ButiEngine::ButiRendering::Format::R32G32_FLOAT;
		output.push_back(elem);
		elem.semanticName = "COLOR";
		elem.format = ButiEngine::ButiRendering::Format::R32G32B32A32_FLOAT;
		output.push_back(elem);
	}
	return output;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormal(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormal::g_VSMain, sizeof(VS::UVNormal::g_VSMain), "UVNormal", GetUVNormalElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormal_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormal_Shadow::g_VSMain, sizeof(VS::UVNormal_Shadow::g_VSMain), "UVNormal_Shadow", GetUVNormalElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalAttribute(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalAttribute::g_VSMain, sizeof(VS::UVNormalAttribute::g_VSMain), "UVNormalAttribute", GetUVNormalElement() , arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalColor::g_VSMain, sizeof(VS::UVNormalColor::g_VSMain), "UVNormalColor", GetUVNormalColorElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalColorFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalColorFog::g_VSMain, sizeof(VS::UVNormalColorFog::g_VSMain), "UVNormalColorFog", GetUVNormalColorElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalColorFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalColorFog_Shadow::g_VSMain, sizeof(VS::UVNormalColorFog_Shadow::g_VSMain), "UVNormalColorFog_Shadow", GetUVNormalColorElement(), arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalFog::g_VSMain, sizeof(VS::UVNormalFog::g_VSMain), "UVNormalFog", GetUVNormalElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalFog_Shadow::g_VSMain, sizeof(VS::UVNormalFog_Shadow::g_VSMain), "UVNormalFog_Shadow", GetUVNormalElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalPhong(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalPhong::g_VSMain, sizeof(VS::UVNormalPhong::g_VSMain), "UVNormalPhong", GetUVNormalElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalPhong_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalPhong_Shadow::g_VSMain, sizeof(VS::UVNormalPhong_Shadow::g_VSMain), "UVNormalPhong_Shadow", GetUVNormalElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalPosition(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalPosition::g_VSMain, sizeof(VS::UVNormalPosition::g_VSMain), "UVNormalPosition", GetUVNormalElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalTangent(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalTangent::g_VSMain, sizeof(VS::UVNormalTangent::g_VSMain), "UVNormalTangent", GetUVNormalElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVNormalTangent_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVNormalTangent_Shadow::g_VSMain, sizeof(VS::UVNormalTangent_Shadow::g_VSMain), "UVNormalTangent_Shadow", GetUVNormalElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVPosition(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVPosition::g_VSMain, sizeof(VS::UVPosition::g_VSMain), "UVPosition", GetUVElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateShadowMap(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::ShadowMap::g_VSMain, sizeof(VS::ShadowMap::g_VSMain), "ShadowMap", GetPositionOnlyElement(), arg_vlp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateShadowMap_UV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::ShadowMap_UV::g_VSMain, sizeof(VS::ShadowMap_UV::g_VSMain), "ShadowMap_UV", GetUVElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UV::g_VSMain, sizeof(VS::UV::g_VSMain), "UV", GetUVElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUV_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UV_Shadow::g_VSMain, sizeof(VS::UV_Shadow::g_VSMain), "UV_Shadow", GetUVElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVAttribute(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVAttribute::g_VSMain, sizeof(VS::UVAttribute::g_VSMain), "UVAttribute", GetUVElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVColor::g_VSMain, sizeof(VS::UVColor::g_VSMain), "UVColor", GetUVColorElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVFog::g_VSMain, sizeof(VS::UVFog::g_VSMain), "UVFog", GetUVElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::DefaultVertexShader::CreateUVFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_VertexShader_Dx12>(VS::UVFog_Shadow::g_VSMain, sizeof(VS::UVFog_Shadow::g_VSMain), "UVFog_Shadow", GetUVElement(), arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateGrid(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{	
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::Grid::g_PSMain, sizeof(PS::Grid::g_PSMain), "Grid",std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateGrid_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::Grid_Shadow::g_PSMain, sizeof(PS::Grid_Shadow::g_PSMain), "Grid_Shadow", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateGrid_worldColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::Grid_worldColor::g_PSMain, sizeof(PS::Grid_worldColor::g_PSMain), "Grid_worldColor", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateGridFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::GridFog::g_PSMain, sizeof(PS::GridFog::g_PSMain), "GridFog", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateOnlyMaterial(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::OnlyMaterial::g_PSMain, sizeof(PS::OnlyMaterial::g_PSMain), "OnlyMaterial", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateOnlyMaterial_BloomSource(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::OnlyMaterial_BloomSource::g_PSMain, sizeof(PS::OnlyMaterial_BloomSource::g_PSMain), "OnlyMaterial_BloomSource", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreatePostEffect_GausBlur(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::PostEffect_GausBlur::g_PSMain, sizeof(PS::PostEffect_GausBlur::g_PSMain), "PostEffect_GausBlur", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreatePostEffect_GausBlurFloat2(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::PostEffect_GausBlurFloat2::g_PSMain, sizeof(PS::PostEffect_GausBlurFloat2::g_PSMain), "PostEffect_GausBlurFloat2", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateShadowMap(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::ShadowMap::g_PSMain, sizeof(PS::ShadowMap::g_PSMain), "ShadowMap", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateShadowMap_UV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::ShadowMap_UV::g_PSMain, sizeof(PS::ShadowMap_UV::g_PSMain), "ShadowMap_UV", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateSpriteAnimation(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::SpriteAnimation::g_PSMain, sizeof(PS::SpriteAnimation::g_PSMain), "SpriteAnimation", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVColor::g_PSMain, sizeof(PS::UVColor::g_PSMain), "UVColor", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVColor_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVColor_Shadow::g_PSMain, sizeof(PS::UVColor_Shadow::g_PSMain), "UVColor_Shadow", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVColorFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVColorFog::g_PSMain, sizeof(PS::UVColorFog::g_PSMain), "UVColorFog", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVColorFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVColorFog_Shadow::g_PSMain, sizeof(PS::UVColorFog_Shadow::g_PSMain), "UVColorFog_Shadow", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVDepth(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVDepth::g_PSMain, sizeof(PS::UVDepth::g_PSMain), "UVDepth", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UV::g_PSMain, sizeof(PS::UV::g_PSMain), "UV", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVFog::g_PSMain, sizeof(PS::UVFog::g_PSMain), "UVFog", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVFog_Shadow::g_PSMain, sizeof(PS::UVFog_Shadow::g_PSMain), "UVFog_Shadow", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormal(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	 return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormal::g_PSMain, sizeof(PS::UVNormal::g_PSMain), "UVNormal", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormal_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	 return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormal_Shadow::g_PSMain, sizeof(PS::UVNormal_Shadow::g_PSMain), "UVNormal_Shadow", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalColor(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	 return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalColor::g_PSMain, sizeof(PS::UVNormalColor::g_PSMain), "UVNormalColor", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalColor_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	 return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalColor_Shadow::g_PSMain, sizeof(PS::UVNormalColor_Shadow::g_PSMain), "UVNormalColor_Shadow", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalColorFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	 return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalColorFog::g_PSMain, sizeof(PS::UVNormalColorFog::g_PSMain), "UVNormalColorFog", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalColorFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	 return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalColorFog_Shadow::g_PSMain, sizeof(PS::UVNormalColorFog_Shadow::g_PSMain), "UVNormalColorFog_Shadow", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalFog(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	 return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalFog::g_PSMain, sizeof(PS::UVNormalFog::g_PSMain), "UVNormalFog", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::DefaultPixelShader::CreateUVNormalFog_Shadow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	 return ObjectFactory::Create<Resource_PixelShader_Dx12>(PS::UVNormalFog_Shadow::g_PSMain, sizeof(PS::UVNormalFog_Shadow::g_PSMain), "UVNormalFog_Shadow", std::vector<int32_t> { static_cast<std::int32_t> (Format::R8G8B8A8_UNORM) }, arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreateOutLine(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	 return ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::OutLine::g_GSMain, sizeof(GS::OutLine::g_GSMain), "OutLine", arg_vlp_graphicDevice);
}

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreatePointToCube(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 return ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::PointToCube::g_GSMain, sizeof(GS::PointToCube::g_GSMain), "PointToCube", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreatePointToCube_WithoutUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 return ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::PointToCube_WithoutUV::g_GSMain, sizeof(GS::PointToCube_WithoutUV::g_GSMain), "PointToCube_WithoutUV", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreatePointToPlane(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 return ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::PointToPlane::g_GSMain, sizeof(GS::PointToPlane::g_GSMain), "PointToPlane", arg_vlp_graphicDevice);
 }

 ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::DefaultGeometryShader::CreatePointToTriPolygon_WithoutUV(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
 {
	 return ObjectFactory::Create<Resource_GeometryShader_Dx12>(GS::PointToTriPolygon_WithoutUV::g_GSMain, sizeof(GS::PointToTriPolygon_WithoutUV::g_GSMain), "PointToTriPolygon_WithoutUV", arg_vlp_graphicDevice);
 }
