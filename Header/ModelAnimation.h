#ifndef MODELANIMATION_H
#define MODELANIMATION_H
#pragma once
#include<map>
#include"Bone.h"
namespace ButiEngine {
namespace ButiRendering {
class IBoneObject;
struct LarpData {
	BUTIRENDERING_API LarpData(BYTE* arg_data);
	BUTIRENDERING_API LarpData();

	CubicBezierCurve2D rotationBezier;
	CubicBezierCurve2D translationXBezier;
	CubicBezierCurve2D translationYBezier;
	CubicBezierCurve2D translationZBezier;
};
struct Pose {
	Pose() {}
	BUTIRENDERING_API Pose(const Vector3& arg_position, const Vector3& arg_rotation, const Vector3& arg_scale);
	Vector3 position;
	Quat rotation;
	Vector3 scale;
};

struct IKActive {
	std::wstring boneName;
	bool isActive;
};

struct MotionKeyFrameData {
	MotionKeyFrameData() {}
	BUTIRENDERING_API MotionKeyFrameData(const std::uint32_t arg_endFrame, const Vector3& arg_position, const Vector3& arg_rotation, const Vector3& arg_scale = Vector3(1, 1, 1));
	BUTIRENDERING_API MotionKeyFrameData(const std::uint32_t arg_endFrame, const Pose& arg_pose);

	std::uint32_t endFrame = 0;
	Pose pose;
	LarpData larp;
};


class IMotionTimeLine :public IObject {
public:
	void Initialize()override {}
	void PreInitialize()override {}
	virtual void Start() = 0;
	virtual void FrameSet(const float) = 0;
	virtual void Reset() = 0;
	virtual std::uint32_t GetEndFrame() = 0;
	virtual void PreStart() = 0;
	virtual void SetBone(Value_ptr<Bone> arg_vlp_bone) = 0;
	virtual void SetMotionData(const MotionKeyFrameData& arg_motionData) = 0;
	virtual void SetMotionData(const std::vector<MotionKeyFrameData>& arg_motionDatas) = 0;
	virtual void LocalPoseSet(Value_ptr<Transform> arg_parentBone) = 0;
	virtual std::string GetContentsName() = 0;
};
class BoneMotionTimeLine :public IMotionTimeLine {
public:
	BUTIRENDERING_API BoneMotionTimeLine();
	BUTIRENDERING_API ~BoneMotionTimeLine();
	BUTIRENDERING_API void SetBone(Value_ptr<Bone> arg_vlp_bone)override;
	BUTIRENDERING_API void PreStart()override;
	BUTIRENDERING_API void Reset()override;
	BUTIRENDERING_API void SetMotionData(const MotionKeyFrameData& arg_motionData) override;
	BUTIRENDERING_API void SetMotionData(const std::vector<MotionKeyFrameData>& arg_motionDatas) override;
	BUTIRENDERING_API void Start() override;
	BUTIRENDERING_API void FrameSet(const float frame)override;
	BUTIRENDERING_API std::uint32_t GetEndFrame() override;
	BUTIRENDERING_API void LocalPoseSet(Value_ptr<Transform> arg_parentBone)override;
	BUTIRENDERING_API void SetBoneName(const std::string& arg_name);
	BUTIRENDERING_API std::string GetContentsName()override;
private:
	bool isActive = false;
	Quat initRotate;
	Vector3 initPosition;
	std::vector< MotionKeyFrameData > vec_data;
	std::vector< MotionKeyFrameData >::iterator currentMotionItr;
	std::vector< MotionKeyFrameData >::iterator befMotionItr;
	Value_ptr<Bone> bone;
	std::string boneName;
};
class ModelAnimation :public IObject
{
public:
	void Initialize() override {}
	void PreInitialize() override {}
	BUTIRENDERING_API void Update(const float arg_frame);
	BUTIRENDERING_API void IKTest();
	BUTIRENDERING_API void Reset();
	BUTIRENDERING_API void PreMotionStart(Value_ptr<Transform> arg_parentBoneTransform);
	BUTIRENDERING_API Value_ptr<IMotionTimeLine> AddMotionTimeLine(Value_ptr<IMotionTimeLine> arg_motion);
	BUTIRENDERING_API bool SetLoop(const bool arg_isLoop);
	BUTIRENDERING_API void SetBoneDrawObj(Value_ptr<IBoneObject> arg_vlp_boneDrawObj);
	BUTIRENDERING_API float GetFrame()const;
private:
	bool isActive = true;
	bool isRoop = false;
	float frame = 0.0f;
	std::uint32_t endFrame = 0;


	std::vector<Value_ptr<IMotionTimeLine>> vec_timeLines;
	Value_ptr<IBoneObject> vlp_boneDrawObj;
};
}
}

#endif // !MODELANIMATION_H