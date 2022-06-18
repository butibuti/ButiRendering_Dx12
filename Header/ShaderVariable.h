#pragma once
#include"ButiMath/ButiMath.h"
namespace ButiEngine {

namespace ButiRendering {
struct Matrices
{
	Matrix4x4 World;
	Matrix4x4 View;
	Matrix4x4 Projection;
	Matrix4x4 MVP;
	Matrices() {
	};
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(World);
		archive(View);
		archive(Projection);
		archive(MVP);
	}
};


struct RenderingSceneInfo {

	Vector4 fogColor;
	Vector4 cameraPos;
	Vector2 fogCoord;
	Vector2 pixelScale;
	Matrix4x4 shadowVP;
	Matrix4x4 shadowV;
	Matrix4x4 forwordCameraMatrix;
	Vector3 shadowCameraPos;
	float Time = 0.0;
	RenderingSceneInfo() {
	}


	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(fogColor);
		archive(cameraPos);
		archive(fogCoord);
		archive(shadowVP);
		archive(shadowV);
		archive(forwordCameraMatrix);
		archive(shadowCameraPos);
		archive(Time);
	}
};

struct ObjectInformation {
	Vector4 lightDir;
	Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector2 Tiling = Vector2(1.0f, 1.0f);
	Vector2 OffSet;
	Vector4 ExInfo;
	ObjectInformation() {
	};
	~ObjectInformation() {

	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(lightDir);
		archive(color);
		archive(Tiling);
		archive(OffSet);
		archive(ExInfo);
	}
};
constexpr std::int32_t gausOffsetLength=0x10;
struct GausVariable {

	GausVariable() {
	}

	Vector4 gausOffset[gausOffsetLength];
	BUTIRENDERING_API float GaussianDistribution(const Vector2& pos, float rho);

	BUTIRENDERING_API void CalcGaus(const std::int32_t width, const std::int32_t height, const Vector2& dir, const float deviation);
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(gausOffset);
	}
};

struct MaterialValue {
	Vector4 emissive = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//roughness は現在emissiveのAを使用
	float materialID = 2.1;
	float roughness;
	MaterialValue() {
	};
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(emissive);
		archive(diffuse);
		archive(ambient);
		archive(specular);
	}
};

struct MaterialValue_Deferred {
	Vector4 emissive = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	MaterialValue_Deferred() {

	};
	MaterialValue_Deferred(const MaterialValue& arg_v) :emissive(arg_v.emissive),diffuse(arg_v.diffuse),ambient(arg_v.ambient),specular(arg_v.specular){};
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(emissive);
		archive(diffuse);
		archive(ambient);
		archive(specular);
	}
};

struct ParticleParameter {
	Vector4 startColor = Vector4(0, 0, 0, 1);
	Vector4 endColor = Vector4(1, 1, 1, 1);
	float time = 0.0f;
	float power = 0.0f;
	std::int32_t noise = 0;
	float outer = 20.0f;
	float center = 1.0f;
	float size = 1.0f;
	float minSize = 0.1f;
	float rotationPase = 5.0f;

	ParticleParameter() {
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(startColor);
		archive(endColor);
		archive(time);
		archive(power);
		archive(noise);
		archive(outer);
		archive(center);
		archive(size);
		archive(minSize);
		archive(rotationPase);
	}

};
}
}
