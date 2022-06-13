#include"stdafx.h"

#include"ButiRendering_Dx12/Header/Resource_Texture.h"


void ButiEngine::ButiRendering::Resource_Texture::SetFilePath(const std::string & arg_textureFilePath)
{
	textureFilePath = arg_textureFilePath;
}

const std::string & ButiEngine::ButiRendering::Resource_Texture::GetTexturePath()const
{
	return textureFilePath;
}

const std::string& ButiEngine::ButiRendering::Resource_Texture::GetTagName() const
{
	return GetTexturePath();
}
