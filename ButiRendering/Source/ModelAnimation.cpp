#include "stdafx.h"
#include"ButiRendering_Dx12/Header/ModelAnimation.h"
#include"ButiRendering_Dx12/Header/DrawData/IDrawData.h"
#include"ButiRendering_Dx12/Header/Bone.h"

namespace ButiEngine {
namespace ButiRendering {

class BoneMotionTimeLine :public IMotionTimeLine {
public:
	BoneMotionTimeLine();
	~BoneMotionTimeLine();
	void SetBone(Value_ptr<Bone> arg_vlp_bone)override;
	void PreStart()override;
	void Reset()override;
	void SetMotionData(const MotionKeyFrameData& arg_motionData) override;
	void SetMotionData(const List<MotionKeyFrameData>& arg_motionDatas) override;
	void Start() override;
	void FrameSet(const float frame)override;
	float GetEndFrame() override;
	void SetBoneName(const std::string& arg_name)override;
	std::string GetContentsName()override;
	const List< MotionKeyFrameData >& GetKeyFrameDatas()const override { return m_list_keyFrameData; }
private:
	bool isActive = false;
	Quat initRotate;
	Vector3 initPosition;
	List< MotionKeyFrameData > m_list_keyFrameData;
	List< MotionKeyFrameData >::iterator_type currentMotionItr;
	List< MotionKeyFrameData >::iterator_type befMotionItr;
	Value_ptr<Bone> bone;
	std::string boneName;
};
class ModelAnimation :public IObject, public IModelAnimation
{
public:
	void Update(const float arg_frame)override;
	void SetProgress(const float arg_progress)override;
	void IKTest()override;
	void Reset()override;
	void BoneSet()override;
	Value_ptr<IMotionTimeLine> AddMotionTimeLine(Value_ptr<IMotionTimeLine> arg_motion)override;
	bool SetIsLoop(const bool arg_isLoop)override;
	const List<Value_ptr<IMotionTimeLine>>& GetMotionTimeLine()const override{ return m_list_timeLines; }
	void SetBoneDrawObj(Value_ptr<IBoneObject> arg_vlp_boneDrawObj)override;
	float GetFrame()const override;
	bool IsEnd()const override { return frame >= endFrame; }
	Value_ptr<IResource_Motion> GetResource()const override{ return m_vlp_resource; }
	void SetResource(Value_ptr<IResource_Motion> arg_vlp_resource) { m_vlp_resource = arg_vlp_resource; }
private:
	bool isActive = true, isLoop = false;
	float frame = 0.0f, endFrame = 0;


	List<Value_ptr<IMotionTimeLine>> m_list_timeLines;
	Value_ptr<IBoneObject> m_vlp_boneDrawObj;
	Value_ptr<IResource_Motion> m_vlp_resource;
};

class AnimationController:public IAnimationController {
public:
	void SetProgress(const float arg_progress)override {
		if (m_vlp_transitionAnim) {
			m_vlp_transitionAnim->SetProgress(arg_progress);
			if (arg_progress>=1.0f) {
				m_vlp_transitionAnim = nullptr;
			}
		}
		else if (m_vlp_anim) {
			m_vlp_anim->SetProgress(arg_progress);
		}
	}
	void Update(const float arg_frame) override {
		m_frame += arg_frame;
		if (m_vlp_transitionAnim) {
			m_vlp_transitionAnim->Update(arg_frame);
			if (m_vlp_transitionAnim->IsEnd()) {
				m_vlp_transitionAnim = nullptr;
			}
		}
		else if(m_vlp_anim) {
			m_vlp_anim->Update(arg_frame);
		}
	}
	void ChangeAnimation(const float arg_frame, Value_ptr<IModelAnimation> arg_anim) override {
		if (!arg_anim) { return; }
		if (m_vlp_anim && m_vlp_anim->GetResource() == arg_anim->GetResource()) { return; }
		m_vlp_anim = arg_anim;
		m_vlp_anim->SetBoneDrawObj(m_vlp_boneDrawObj);
		if (arg_frame <= 0.0f) { return; }

		auto newTransitionAnim = ObjectFactory::Create<ModelAnimation>();
		newTransitionAnim->SetBoneDrawObj(m_vlp_boneDrawObj);
		
		for (auto line:arg_anim->GetMotionTimeLine()) {
			auto transLine = ObjectFactory::Create<BoneMotionTimeLine>();
			auto lineBoneName = line->GetContentsName();
			auto boneItr = m_vlp_boneDrawObj->GetBones().Find([lineBoneName](Value_ptr<Bone> arg_bone)->bool {return lineBoneName == arg_bone->boneName; });
			if (boneItr == nullptr) { continue; }
			MotionKeyFrameData start, end;
			end.endFrame = arg_frame;
			

			start.pose.position = (*boneItr)->transform->GetLocalPosition();
			start.pose.rotation = (*boneItr)->transform->GetLocalRotation().ToQuat();
			start.pose.scale = (*boneItr)->transform->GetLocalScale();
			start.isTranslation = true;
			start.isRotation = true;
			start.isScale = true;
			end.isTranslation = true;
			end.isRotation = true;
			end.isScale = true;

			end.pose = line->GetKeyFrameDatas().GetFront().pose;

			transLine->SetMotionData(start);
			transLine->SetMotionData(end);
			transLine->SetBoneName(lineBoneName);
			newTransitionAnim->AddMotionTimeLine(transLine);
		}
		m_vlp_transitionAnim = newTransitionAnim;
		m_vlp_transitionAnim->BoneSet();
	}
	Value_ptr<IModelAnimation> GetCurrentModelAnimation() override { return m_vlp_anim; }

	void SetBoneDrawObj(Value_ptr<IBoneObject> arg_vlp_boneDrawObj) {
		m_vlp_boneDrawObj = arg_vlp_boneDrawObj;
	}
private:
	float m_frame,m_transitionFrame;
	Value_ptr<IModelAnimation> m_vlp_anim, m_vlp_transitionAnim;
	Value_ptr<IBoneObject> m_vlp_boneDrawObj;
};
}
}

ButiEngine::ButiRendering::BoneMotionTimeLine::BoneMotionTimeLine()
{
	auto itr = m_list_keyFrameData.begin();
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
	currentMotionItr = m_list_keyFrameData.begin();
	befMotionItr = m_list_keyFrameData.begin();
	isActive = true;
	Start();
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::Reset()
{
	PreStart();
}


void ButiEngine::ButiRendering::BoneMotionTimeLine::SetMotionData(const MotionKeyFrameData& arg_motionData)
{
	m_list_keyFrameData.push_back(arg_motionData);
}

void ButiEngine::ButiRendering::BoneMotionTimeLine::SetMotionData(const List<MotionKeyFrameData>& arg_motionDatas)
{
	m_list_keyFrameData = arg_motionDatas;
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

	if (frame >= currentMotionItr-> endFrame) {

		befMotionItr = currentMotionItr;
		currentMotionItr++;
		if (currentMotionItr == m_list_keyFrameData.end()) {
			isActive = false;
			return;
		}
		else {
			Start();

		}
	}
	auto relativeFrame = frame - befMotionItr-> endFrame;
	auto frameRange = currentMotionItr->endFrame - befMotionItr->endFrame;
	if (frameRange) {
		auto t = relativeFrame / frameRange;
		if (currentMotionItr->isRotation) {
			auto rotateT = currentMotionItr->larp.rotationBezier.GetYFromNuton(t);
			Quat currentRotation = MathHelper::LearpQuat(initRotate, currentMotionItr->pose.rotation, t);
			//Quat currentRotation = currentMotionItr->pose.rotation;
			bone->transform->SetLocalRotation(Matrix4x4((currentRotation)));
		}
		
		if (currentMotionItr->isTranslation) {
			bone->transform->SetLocalPosition(MathHelper::LerpPosition(initPosition, currentMotionItr->pose.position, t, t, t));
			//bone->transform->SetLocalPosition(currentMotionItr->pose.position);
		}
	}
}

float ButiEngine::ButiRendering::BoneMotionTimeLine::GetEndFrame()
{
	return m_list_keyFrameData.GetLast().endFrame;
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
		if (isLoop) {
			frame = 0;
			Reset();
		}
		else {
			return;
		}
	}
	frame += arg_frame;
	frame = max(frame, 0);
	for (auto line:m_list_timeLines) {
		(line)->FrameSet(frame);
	}
	//vlp_boneDrawObj->InverseKinematic();
	//vlp_boneDrawObj->BonePowerAdd();
}

void ButiEngine::ButiRendering::ModelAnimation::SetProgress(const float arg_progress)
{
	frame = endFrame *max( min(arg_progress,1.0f),0.0f);
	for (auto line : m_list_timeLines) {
		(line)->FrameSet(frame);
	}
}

void ButiEngine::ButiRendering::ModelAnimation::IKTest()
{
	m_vlp_boneDrawObj->InverseKinematic();
	m_vlp_boneDrawObj->BonePowerAdd();
}


void ButiEngine::ButiRendering::ModelAnimation::Reset()
{
	for (auto line:m_list_timeLines) {
		line->Reset();
	}
}

void ButiEngine::ButiRendering::ModelAnimation::BoneSet()
{
	for (auto line:m_list_timeLines) {
		auto bone = m_vlp_boneDrawObj->searchBoneByName(line->GetContentsName());
		if (!bone) {
			continue;
		}
		line->SetBone(bone);
		line->PreStart();
		//(itr->second)->Start();
		endFrame = max(line->GetEndFrame(), endFrame);
	}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IMotionTimeLine> ButiEngine::ButiRendering::ModelAnimation::AddMotionTimeLine( Value_ptr<IMotionTimeLine> arg_motion)
{

	m_list_timeLines.push_back(arg_motion);

	return arg_motion;
}

bool ButiEngine::ButiRendering::ModelAnimation::SetIsLoop(const bool arg_isloop)
{
	isLoop = arg_isloop;
	return isLoop;
}

void ButiEngine::ButiRendering::ModelAnimation::SetBoneDrawObj(Value_ptr<IBoneObject> arg_vlp_boneDrawObj)
{
	m_vlp_boneDrawObj = arg_vlp_boneDrawObj;
	BoneSet();
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

ButiEngine::ButiRendering::MotionKeyFrameData::MotionKeyFrameData(const float arg_endFrame, const Vector3& arg_position, const Vector3& arg_rotation,const Vector3& arg_scale)
	:MotionKeyFrameData(arg_endFrame,Pose(arg_position,arg_rotation,arg_scale))
{
}

ButiEngine::ButiRendering::MotionKeyFrameData::MotionKeyFrameData(const float arg_endFrame, const Pose& arg_pose)
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



ButiEngine::Value_ptr<ButiEngine::ButiRendering::IMotionTimeLine> ButiEngine::ButiRendering::CreateMotionTimeLine()
{
	return ObjectFactory::Create<BoneMotionTimeLine>();
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IModelAnimation> ButiEngine::ButiRendering::CreateModelAnimation(Value_ptr<IResource_Motion> arg_vlp_resource)
{
	auto output= ObjectFactory::Create<ModelAnimation>();
	output->SetResource(arg_vlp_resource);
	return output;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IAnimationController> ButiEngine::ButiRendering::CreateAnimationController(Value_ptr<IBoneObject> arg_vlp_boneDrawObj)
{
	auto output = ObjectFactory::Create<AnimationController>();
	output->SetBoneDrawObj(arg_vlp_boneDrawObj);
	return output;
}
