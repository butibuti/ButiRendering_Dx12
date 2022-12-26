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
	BUTIRENDERING_API MotionKeyFrameData(const float arg_endFrame, const Vector3& arg_position, const Vector3& arg_rotation, const Vector3& arg_scale = Vector3(1, 1, 1));
	BUTIRENDERING_API MotionKeyFrameData(const float arg_endFrame, const Pose& arg_pose);

	float endFrame = 0;
	Pose pose;
	LarpData larp;
	bool isTranslation = false, isRotation = false,isScale=false;
};


}
}

#endif // !MODELANIMATION_H