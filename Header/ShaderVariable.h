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
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "ObjectMatrix";
		return bufferName;
	}
};


struct RendererState {

	Vector4 fogColor;
	Vector4 cameraPos;
	Vector2 fogCoord;
	Vector2 pixelScale;
	Matrix4x4 shadowVP;
	Matrix4x4 shadowV;
	Matrix4x4 forwordCameraMatrix;
	Vector3 shadowCameraPos;
	float Time = 0.0;
	RendererState() {
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
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "RendererState";
		return bufferName;
	}
};

struct ObjectInformation {
	Vector4 lightDir = Vector4(Vector3(-1.0f, -1.0f, -1.0f).GetNormalize(), 1);
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
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "ObjectInformation";
		return bufferName;
	}
};
constexpr std::int32_t gausOffsetLength=0x10;
struct GausParameter {

	GausParameter() {
	}

	Vector4 gausOffset[gausOffsetLength];
	BUTIRENDERING_API float GaussianDistribution(const Vector2& pos, float rho);

	BUTIRENDERING_API void CalcGaus(const std::int32_t width, const std::int32_t height, const Vector2& dir, const float deviation);
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(gausOffset);
	}
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "GausParameter";
		return bufferName;
	}
};

struct MaterialValue {
	Vector4 emissive = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 specular = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	float materialID = 2;
	float roughness=1.0f;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(emissive);
		archive(diffuse);
		archive(ambient);
		archive(specular);
		archive(roughness);
	}
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "Material";
		return bufferName;
	}
};

struct MaterialValue_Deferred {
	MaterialValue datas [256];
	MaterialValue_Deferred(){memset(datas, 0, sizeof(datas)); };
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(datas);
	}
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "MaterialList";
		return bufferName;
	}
};
struct BoneMatrix {
	Matrix4x4 datas[256];
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(datas);
	}
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "Bone";
		return bufferName;
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
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "ParticleParameter";
		return bufferName;
	}

};
}
}
