#include"stdafx.h"
#include"ButiUtil/ButiUtil/ImageFileIO.h"
#include"ButiRendering_Dx12/Header/ResourceInterface.h"
namespace ButiEngine {

namespace ButiRendering {
class Resource_Font :public IResource_Font {
public:
	const std::string& GetFontName()const override {
		return name;
	}
	Value_ptr<IResource_Texture>  GetTexture()const override {
		return m_vlp_atlasTexture;
	}
	void SetTexture(Value_ptr<IResource_Texture>  arg_tag) {
		m_vlp_atlasTexture = arg_tag;
	}
	Value_ptr<IResource_Material> GetMaterial()const override {
		return m_vlp_atlasMaterial;
	}
	void SetMaterial(Value_ptr<IResource_Material>  arg_tag) {
		m_vlp_atlasMaterial = arg_tag;
	}
	void SetInfo(Value_ptr<ImageFileIO::FontResourceData> arg_vlp_info) {
		vlp_info = arg_vlp_info;
	}
	ButiFont::IFontInformation* GetFontInformation() const {
		return vlp_info->p_fontInfo;
	}
private:
	Value_ptr<IResource_Texture> m_vlp_atlasTexture;
	Value_ptr<IResource_Material> m_vlp_atlasMaterial;
	Value_ptr<ImageFileIO::FontResourceData> vlp_info;
	std::string name;
	std::int32_t size;
};
}
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Font> ButiEngine::ButiRendering::CreateFontFromFile(const std::string& arg_trueTypeFontFilePath, const std::int32_t arg_size, Value_ptr<IResource_Shader> arg_vlp_shader, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	auto fontData = ObjectFactory::Create<ImageFileIO::FontResourceData>();
	ImageFileIO::InputTTFFile(arg_trueTypeFontFilePath, arg_size, ButiFont::FontLanguage::FONTLANG_JAPANESE, *fontData);
	auto output = make_value<Resource_Font>();

	output->SetTexture(CreateTexture(fontData->vlp_texResData,arg_vlp_graphicDevice) );

	output->SetMaterial(CreateMaterial(arg_trueTypeFontFilePath + "," + std::to_string(arg_size), MaterialValue(),arg_vlp_shader,{output->GetTexture()},DrawSettings(),arg_vlp_graphicDevice));

	output->SetInfo(fontData);

	return output;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Font> ButiEngine::ButiRendering::CreateFontFromMemory(const std::string& arg_trueTypeFontFilePath, const char* arg_data, const std::int64_t arg_dataSize, const std::int32_t arg_size, Value_ptr<IResource_Shader> arg_vlp_shader, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	auto fontData = ObjectFactory::Create<ImageFileIO::FontResourceData>();
	ImageFileIO::InputTTF(arg_data, arg_dataSize, arg_trueTypeFontFilePath, arg_size, ButiFont::FontLanguage::FONTLANG_JAPANESE, *fontData);
	auto output = make_value<Resource_Font>();

	output->SetTexture(CreateTexture(fontData->vlp_texResData, arg_vlp_graphicDevice));

	output->SetMaterial(CreateMaterial(arg_trueTypeFontFilePath + "," + std::to_string(arg_size), MaterialValue(), arg_vlp_shader, { output->GetTexture() }, DrawSettings(), arg_vlp_graphicDevice));

	output->SetInfo(fontData);

	return output;
}
