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
		ARCHIVE_BUTI(World);
		ARCHIVE_BUTI(View);
		ARCHIVE_BUTI(Projection);
		ARCHIVE_BUTI(MVP);
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
		ARCHIVE_BUTI(fogColor);
		ARCHIVE_BUTI(cameraPos);
		ARCHIVE_BUTI(fogCoord);
		ARCHIVE_BUTI(shadowVP);
		ARCHIVE_BUTI(shadowV);
		ARCHIVE_BUTI(forwordCameraMatrix);
		ARCHIVE_BUTI(shadowCameraPos);
		ARCHIVE_BUTI(Time);
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
	void SetLightDir(const Vector4& arg_lightDir) { lightDir = arg_lightDir; }
	void SetColor(const Vector4& arg_color) { color = arg_color; }
	void SetExInfo(const Vector4& arg_exInfo) { ExInfo = arg_exInfo; }
	void SetTiling(const Vector2& arg_tiling) { Tiling = arg_tiling; }
	void SetOffSet(const Vector2& arg_offset) { OffSet= arg_offset; }
	Vector4 GetLightDir()const { return lightDir; }
	Vector4 GetColor()const { return color; }
	Vector4 GetExInfo()const { return ExInfo; }
	Vector2 GetTiling()const { return Tiling; }
	Vector2 GetOffSet()const { return OffSet; }
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(lightDir);
		ARCHIVE_BUTI(color);
		ARCHIVE_BUTI(Tiling);
		ARCHIVE_BUTI(OffSet);
		ARCHIVE_BUTI(ExInfo);
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
		ARCHIVE_BUTI(gausOffset);
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
		ARCHIVE_BUTI(emissive);
		ARCHIVE_BUTI(diffuse);
		ARCHIVE_BUTI(ambient);
		ARCHIVE_BUTI(specular);
		ARCHIVE_BUTI(roughness);
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
		ARCHIVE_BUTI(datas);
	}
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "MaterialList";
		return bufferName;
	}
};
struct BoneMatrix {
	Matrix4x4 datas[64];
	BoneMatrix() { std::memset(datas, 0, sizeof(datas)); }
	template<class Archive>
	void serialize(Archive& archive)
	{
		//ARCHIVE_BUTI(datas);
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
		ARCHIVE_BUTI(startColor);
		ARCHIVE_BUTI(endColor);
		ARCHIVE_BUTI(time);
		ARCHIVE_BUTI(power);
		ARCHIVE_BUTI(noise);
		ARCHIVE_BUTI(outer);
		ARCHIVE_BUTI(center);
		ARCHIVE_BUTI(size);
		ARCHIVE_BUTI(minSize);
		ARCHIVE_BUTI(rotationPase);
	}
	static const std::string& GetConstantBufferName() {
		static const std::string bufferName = "ParticleParameter";
		return bufferName;
	}

};
}
}
