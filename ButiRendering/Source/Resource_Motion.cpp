#pragma once
#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Resource_Motion.h"
#include"ButiRendering_Dx12/Header/ModelAnimation.h"



ButiEngine::Value_ptr<ButiEngine::ButiRendering::ModelAnimation> ButiEngine::ButiRendering::Resource_Motion::GetAnimation()
{
	auto output = ObjectFactory::Create<ModelAnimation>();

	for (auto itr = map_boneKeyFrames.begin(); itr != map_boneKeyFrames.end(); itr++) {
		auto motionLane = ObjectFactory::Create<  BoneMotionTimeLine>();
		motionLane->SetMotionData(itr->second);
		motionLane->SetBoneName(itr->first);
		output->AddMotionTimeLine( motionLane);
	}
	return output;
}

void ButiEngine::ButiRendering::Resource_Motion::AddKeyFrame(const std::string& arg_boneName, const MotionKeyFrameData& arg_data)
{
	if (!map_boneKeyFrames.count(arg_boneName)) {
		auto vec_boneName = std::vector<MotionKeyFrameData>();
		
		map_boneKeyFrames.emplace(arg_boneName, vec_boneName);
		map_boneKeyFrames.at(arg_boneName).push_back(arg_data);
		return;
	}

	map_boneKeyFrames.at(arg_boneName).push_back(arg_data);
}

void ButiEngine::ButiRendering::Resource_Motion::AddKeyFrameLane(const std::string& arg_boneName, const std::vector<MotionKeyFrameData>& arg_datas)
{

	if (map_boneKeyFrames.count(arg_boneName)) {
		return;
	}

	map_boneKeyFrames.emplace(arg_boneName,arg_datas);
}
