#ifndef DRAWSETTINGS_H
#define DRAWSETTINGS_H

#include"RenderingStatus.h"
namespace ButiEngine {
namespace ButiRendering {

struct DrawSettings
{
	DrawSettings() {}
	DrawSettings(CullMode arg_cullMode) :cullMode(arg_cullMode) {}
	DrawSettings(CullMode arg_cullMode, bool arg_isFill) :cullMode(arg_cullMode) {
		if (!arg_isFill) {
			isFill = FillMode::wire;
		}
	}
	DrawSettings(BlendMode arg_blendMode) :blendMode(arg_blendMode) {}
	DrawSettings(bool arg_isFill) {
		if (!arg_isFill) {
			isFill = FillMode::wire;
		}
	}
	DrawSettings(BlendMode arg_blendMode, CullMode arg_cullMode) :cullMode(arg_cullMode), blendMode(arg_blendMode) {}
	DrawSettings(BlendMode arg_blendMode, CullMode arg_cullMode, bool arg_isFill) :cullMode(arg_cullMode), blendMode(arg_blendMode) {
		if (!arg_isFill) {
			isFill = FillMode::wire;
		}
	}
	CullMode cullMode = CullMode::back;
	FillMode isFill = FillMode::solid;
	BlendMode blendMode = BlendMode::AlphaBlend;
	std::vector<SamplerState >vec_samplerMode = { SamplerState::LinearClamp };
	TopologyType topologyType = TopologyType::triangleList;
	bool isShadowMap = false;
	bool isAlpha = false;
	bool isDepth = true;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cullMode);
		archive(isFill);
		archive(blendMode);
		archive(vec_samplerMode);
		archive(topologyType);
		archive(isShadowMap);
		archive(isAlpha);
		archive(isDepth);
	}

	inline bool operator==(const DrawSettings& other)const {

		if (cullMode == other.cullMode && isFill == other.isFill) {
			return true;
		}

		return false;
	}
};
}
}

namespace std {

template<>
struct hash<ButiEngine::ButiRendering::DrawSettings> {
public:
	size_t operator()(const ButiEngine::ButiRendering::DrawSettings& data)const {

		std::size_t seed = 0;
		ButiEngine::hash_combine(seed, data.isFill);
		ButiEngine::hash_combine(seed, (std::int32_t)data.cullMode);


		return seed;

	}
};
}
#endif // !DRAWSETTINGS_H
