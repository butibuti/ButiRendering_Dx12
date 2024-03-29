#ifndef BONE_H
#define BONE_H
#pragma once
#include"ButiMath/Transform.h"
#include<string>
namespace ButiEngine {
namespace ButiRendering {
struct IModelObject;
struct Bone;
struct IKLink {
	~IKLink() {
		if (axis) {
			delete axis;
		}
	}
	std::int32_t linkBone;
	Value_ptr<Bone> vlp_linkBone;
	bool radianLimit = false;
	float bonesLength = 0;
	Quat befBoneInitRotation;
	Vector3 upLimit;
	Vector3 downLimit;
	Vector3* axis = nullptr;

	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(radianLimit);
		ARCHIVE_BUTI(upLimit);
		ARCHIVE_BUTI(downLimit);
		ARCHIVE_BUTI(bonesLength);
		ARCHIVE_BUTI(linkBone);
	}
};
struct IKData {
	BUTIRENDERING_API ~IKData();
	std::int32_t targetBoneIndex;
	std::int32_t extremityBoneIndex;
	Value_ptr<Bone> vlp_targetBone;
	std::int32_t loopCount;
	float maxRadian;
	float sum_bonesLength = 0;
	std::vector<IKLink> links;
	//Vector3* p_jointPoints=nullptr;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(targetBoneIndex);
		ARCHIVE_BUTI(extremityBoneIndex);
		ARCHIVE_BUTI(loopCount);
		ARCHIVE_BUTI(maxRadian);
		ARCHIVE_BUTI(sum_bonesLength);
		ARCHIVE_BUTI(links);
	}
};
struct Bone
{
	std::int32_t ownIndex=-1;
	std::string boneName;
	std::string boneNameEng;

	Vector3 position;
	Matrix4x4 rotation;
	std::int32_t parentBoneIndex = -1;
	std::int32_t transformFloor = 0;

	Value_ptr<Bone> parentBone;

	bool isIK = false;

	Matrix4x4 bindPose;
	Matrix4x4 bindPoseInverse;

	BUTIRENDERING_API Matrix4x4 GetPoseMatrix();

	Value_ptr<BoneTransform> transform;

	bool canRotate;
	bool canMove;
	bool canControll;

	bool isFreezeAxis = false;
	Vector3 freezeAxis;

	bool addRotate = false;
	bool addMove = false;
	std::int32_t addBoneIndex = -1;
	float addPercentage;

	Value_ptr<Bone> addBone;

	std::int32_t outParentTransform = -1;
	bool isPhisicsAfterTransform;

	Vector3 localXaxis;
	Vector3 localZaxis;

	std::int32_t jointBone = -1;
	Vector3 jointPos;
	std::vector<IKData> ikDatas;

	bool addLocal = false;

	BUTIRENDERING_API void SetOtherBoneLinks(const List<Value_ptr< Bone>>& arg_vec_bones);

	BUTIRENDERING_API void AddBonePower();

	BUTIRENDERING_API void InverseKinematic();
	BUTIRENDERING_API void CCDInverseKinematic();
	BUTIRENDERING_API Matrix4x4 GetOriginalBoneMatrix(const List<Bone>& arg_bones)const ;
};
}
}
#endif // !BONE_H