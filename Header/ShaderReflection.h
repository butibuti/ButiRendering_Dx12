#ifndef SHADERREFLECTION_H
#define SHADERREFLECTION_H
#include"RenderingStatus.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
namespace ButiEngine {
namespace ButiRendering {
struct ShaderStructReflection {
	List<ShaderElement> m_list_element;
	inline bool operator==(const ShaderStructReflection& arg_other)const {
		return  m_list_element == arg_other.m_list_element;
	}
}; 
struct ConstantBufferReflection {
	std::int32_t m_slot = -1;
	std::int32_t m_descCount = 1;
	std::string m_bufferName;
	List<Format> m_list_format;
	inline bool operator==(const ConstantBufferReflection& arg_other)const {
		return m_bufferName == arg_other.m_bufferName && m_list_format == arg_other.m_list_format&&m_slot==arg_other.m_slot&&m_descCount==arg_other.m_descCount;
	}
	inline bool IsNotUse()const {
		return m_list_format.IsEmpty();
	}
};
struct TextureReflection {
	std::string m_name;
	Format m_format;
	inline bool operator==(const TextureReflection& arg_other)const {
		return  m_name == arg_other.m_name&& m_format== arg_other.m_format;
	}
};
struct SamplerReflection {
	std::string m_name;
	inline bool operator==(const SamplerReflection& arg_other)const {
		return  m_name== arg_other.m_name;
	}
};
}
}

#endif // !SHADERREFLECTION_H
