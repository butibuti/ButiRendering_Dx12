#pragma once
#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Resource_Motion.h"
#include"ButiRendering_Dx12/Header/ModelAnimation.h"

namespace ButiEngine {
namespace ButiRendering {

class Resource_Motion :public IResource_Motion {
public:
	const std::string& GetName()const override { return m_name; }
	void SetName(const std::string& arg_name)override { m_name = arg_name; }
	BUTIRENDERING_API Value_ptr<IModelAnimation> GetAnimation()override;
	BUTIRENDERING_API void AddKeyFrame(const std::string& arg_boneName, const MotionKeyFrameData& arg_data)override;
	BUTIRENDERING_API void AddKeyFrameLane(const std::string& arg_boneName, const List<MotionKeyFrameData>& arg_datas)override;

	std::unordered_map<std::string, List<MotionKeyFrameData>> m_map_boneKeyFrames;
	std::string m_name;
};
}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IModelAnimation> ButiEngine::ButiRendering::Resource_Motion::GetAnimation()
{
	auto output = CreateModelAnimation(GetThis<IResource_Motion>());

	for (auto itr = m_map_boneKeyFrames.begin(); itr != m_map_boneKeyFrames.end(); itr++) {
		auto motionLane = CreateMotionTimeLine();
		motionLane->SetMotionData(itr->second);
		motionLane->SetBoneName(itr->first);
		output->AddMotionTimeLine( motionLane);
	}
	return output;
}

void ButiEngine::ButiRendering::Resource_Motion::AddKeyFrame(const std::string& arg_boneName, const MotionKeyFrameData& arg_data)
{
	if (!m_map_boneKeyFrames.count(arg_boneName)) {
		auto vec_boneName = List<MotionKeyFrameData>();
		
		m_map_boneKeyFrames.emplace(arg_boneName, vec_boneName);
		m_map_boneKeyFrames.at(arg_boneName).push_back(arg_data);
		return;
	}

	m_map_boneKeyFrames.at(arg_boneName).push_back(arg_data);
}

void ButiEngine::ButiRendering::Resource_Motion::AddKeyFrameLane(const std::string& arg_boneName, const List<MotionKeyFrameData>& arg_datas)
{

	if (m_map_boneKeyFrames.count(arg_boneName)) {
		return;
	}

	m_map_boneKeyFrames.emplace(arg_boneName,arg_datas);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Motion> ButiEngine::ButiRendering::CreateMotion(const std::string& arg_name, const std::unordered_map<std::string, List<MotionKeyFrameData> >& arg_keyFrameData) {
	auto output = ObjectFactory::Create<Resource_Motion>();
	output->SetName(arg_name);
	output->m_map_boneKeyFrames = arg_keyFrameData;
	return output;
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Motion> ButiEngine::ButiRendering::CreateMotion() {
	auto output = ObjectFactory::Create<Resource_Motion>();
	return output;
}

