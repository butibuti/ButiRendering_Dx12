#include "stdafx.h"
#include"ButiRendering_Dx12/Header/ModelAnimation.h"
#include"ButiRendering_Dx12/Header/DrawData/IDrawData.h"
#include"../Header/Bone.h"

ButiEngine::ButiRendering::BoneMotionTimeLine::BoneMotionTimeLine()
{
	auto itr = vec_data.begin();
}

ButiEngine::ButiRendering::BoneMotionTimeLine::~BoneMotionTimeLine()
{
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::SetBone(Value_ptr<Bone> arg_vlp_bone)
{
	bone = arg_vlp_bone;
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::PreStart()
{
	currentMotionItr = vec_data.begin();
	befMotionItr = vec_data.begin();
	isActive = true;
	Start();
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::Reset()
{
	PreStart();
}


void ButiEngine::ButiRendering::BoneMotionTimeLine::SetMotionData(const MotionKeyFrameData& arg_motionData)
{
	vec_data.push_back(arg_motionData);
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::SetMotionData(const std::vector<MotionKeyFrameData>& arg_motionDatas)
{
	vec_data = arg_motionDatas;
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::Start()
{
	if (!bone) {
		isActive = false;
		return;
	}
	initPosition= bone->transform->GetLocalPosition();

	initRotate = befMotionItr->pose.rotation;
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::FrameSet(const float frame)
{
	if (!isActive) {
		return;
	}

	if (frame >= (float)currentMotionItr-> endFrame) {

		befMotionItr = currentMotionItr;
		currentMotionItr++;
		if (currentMotionItr == vec_data.end()) {
			isActive = false;
			return;
		}
		else {
			Start();

		}
	}
	auto relativeFrame = frame - (float)befMotionItr-> endFrame;
	auto frameRange = currentMotionItr->endFrame - befMotionItr->endFrame;
	if (frameRange) {
		auto t = (float)relativeFrame / (float)frameRange;

		auto rotateT = currentMotionItr->larp.rotationBezier.GetYFromNuton(t);
		
		//std::cout << t << std::endl;

		Quat currentRotation = MathHelper::LearpQuat(initRotate, currentMotionItr->pose.rotation, rotateT);


		bone->transform->SetLocalRotation(Matrix4x4((currentRotation)));
		

		auto xt = currentMotionItr->larp.translationXBezier.GetYFromNuton(t);
		auto yt = currentMotionItr->larp.translationYBezier.GetYFromNuton(t);
		auto zt = currentMotionItr->larp.translationZBezier.GetYFromNuton(t);

		bone->transform->SetLocalPosition(MathHelper::LerpPosition(initPosition, currentMotionItr->pose.position,xt,yt,zt));
	}
}

std::uint32_t ButiEngine::ButiRendering::BoneMotionTimeLine::GetEndFrame()
{
	return vec_data.rbegin()->endFrame;
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::LocalPoseSet(Value_ptr<Transform> arg_parentBone)
{
	for (auto itr = vec_data.begin(); itr != vec_data.end(); itr++) {
		itr->pose.position += bone->transform->GetLocalPosition();
	}
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::SetBoneName(const std::string& arg_name)
{
	boneName = arg_name;
}

std::string ButiEngine::ButiRendering::BoneMotionTimeLine::GetContentsName()
{
	return boneName;
}

void ButiEngine::ButiRendering::ModelAnimation::Update(const float arg_frame)
{
	if (frame >= endFrame) {
		if (isRoop) {
			frame = 0;
			Reset();
		}
		else {
			return;
		}
	}
	auto end = vec_timeLines.end();
	frame += arg_frame;
	frame = max(frame, 0);
	for (auto itr = vec_timeLines.begin(); itr !=end; itr++) {
		(*itr)->FrameSet(frame);
	}
	//vlp_boneDrawObj->InverseKinematic();
	//vlp_boneDrawObj->BonePowerAdd();
}

void ButiEngine::ButiRendering::ModelAnimation::IKTest()
{
	vlp_boneDrawObj->InverseKinematic();
	vlp_boneDrawObj->BonePowerAdd();
}


void ButiEngine::ButiRendering::ModelAnimation::Reset()
{
	for (auto itr = vec_timeLines.begin(); itr != vec_timeLines.end(); itr++) {
		(*itr)->Reset();
	}
}

void ButiEngine::ButiRendering::ModelAnimation::PreMotionStart(Value_ptr<Transform> arg_parentBoneTransform)
{
	for (auto itr = vec_timeLines.begin(); itr != vec_timeLines.end();itr++ ) {
		auto bone = vlp_boneDrawObj->searchBoneByName((*itr)->GetContentsName());
		if (!bone) {
			continue;
		}
		(*itr)->SetBone(bone);
		(*itr)->LocalPoseSet(arg_parentBoneTransform);
		(*itr)->PreStart();
		//(itr->second)->Start();
		endFrame = max((*itr)->GetEndFrame(), endFrame);
	}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IMotionTimeLine> ButiEngine::ButiRendering::ModelAnimation::AddMotionTimeLine( Value_ptr<IMotionTimeLine> arg_motion)
{

	vec_timeLines.push_back(arg_motion);

	return arg_motion;
}

bool ButiEngine::ButiRendering::ModelAnimation::SetLoop(const bool arg_isloop)
{
	isRoop = arg_isloop;
	return isRoop;
}

void ButiEngine::ButiRendering::ModelAnimation::SetBoneDrawObj(Value_ptr<IBoneObject> arg_vlp_boneDrawObj)
{
	vlp_boneDrawObj = arg_vlp_boneDrawObj;
}

float ButiEngine::ButiRendering::ModelAnimation::GetFrame() const
{
	return frame;
}

ButiEngine::ButiRendering::Pose::Pose(const Vector3& arg_position, const Vector3& arg_rotate, const Vector3& arg_scale):position(arg_position),scale(arg_scale)
{
	Matrix4x4 rotationMatrix = Matrix4x4::RollX(
		MathHelper::ToRadian (arg_rotate.x)
	) *
		Matrix4x4::RollY(
			MathHelper::ToRadian(arg_rotate.y)
		) *
		Matrix4x4::RollZ(
			MathHelper::ToRadian(arg_rotate.z)
		);

	rotation = rotationMatrix.ToQuat();
}

ButiEngine::ButiRendering::MotionKeyFrameData::MotionKeyFrameData(const std::uint32_t arg_endFrame, const Vector3& arg_position, const Vector3& arg_rotation,const Vector3& arg_scale)
	:MotionKeyFrameData(arg_endFrame,Pose(arg_position,arg_rotation,arg_scale))
{
}

ButiEngine::ButiRendering::MotionKeyFrameData::MotionKeyFrameData(const std::uint32_t arg_endFrame, const Pose& arg_pose)
{
	endFrame = arg_endFrame;
	pose = arg_pose;
}

ButiEngine::ButiRendering::LarpData::LarpData(BYTE* arg_data)
	: rotationBezier(Vector2(0,0), Vector2(arg_data[3]/127, arg_data[7]/127), Vector2(arg_data[11]/127, arg_data[15]/127), Vector2(1,1))
	, translationXBezier(Vector2(0, 0), Vector2(arg_data[0]/127, arg_data[4]/127), Vector2(arg_data[8]/127, arg_data[12]/127), Vector2(1, 1))
	, translationYBezier(Vector2(0, 0), Vector2(arg_data[1]/127, arg_data[5]/127), Vector2(arg_data[9]/127, arg_data[13]/127), Vector2(1, 1))
	, translationZBezier(Vector2(0, 0), Vector2(arg_data[2]/127, arg_data[6]/127), Vector2(arg_data[10]/127, arg_data[14]/127), Vector2(1, 1))
{
}

ButiEngine::ButiRendering::LarpData::LarpData()
	:rotationBezier(Vector2(0,0), Vector2(0,0), Vector2(1,1), Vector2(1,1))
	,translationXBezier(Vector2(0,0), Vector2(0,0), Vector2(1,1), Vector2(1,1))
	,translationYBezier(Vector2(0,0), Vector2(0,0), Vector2(1,1), Vector2(1,1))
	,translationZBezier(Vector2(0,0), Vector2(0,0), Vector2(1,1), Vector2(1,1))
{
	
}
