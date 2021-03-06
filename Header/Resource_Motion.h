#include"stdafx.h"
#include"ModelAnimation.h"
#include"ResourceInterface.h"
namespace ButiEngine {

namespace ButiRendering {
class Resource_Motion :public IResource_Motion {
public:
	void Initialize() {}
	void PreInitialize() {}
	BUTIRENDERING_API Value_ptr<ModelAnimation> GetAnimation()override;
	BUTIRENDERING_API void AddKeyFrame(const std::string& arg_boneName, const MotionKeyFrameData& arg_data)override;
	BUTIRENDERING_API void AddKeyFrameLane(const std::string& arg_boneName, const std::vector<MotionKeyFrameData>& arg_datas)override;
private:
	std::unordered_map<std::string, std::vector<MotionKeyFrameData>> map_boneKeyFrames;
};
}
}