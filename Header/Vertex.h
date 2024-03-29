#ifndef VERTEX_H
#define VERTEX_H
#include"ButiMath/ButiMath.h"
namespace ButiEngine {
namespace Vertex {
enum VertexFlag {
	Position=0b00, UV=0b01,Normal=0b10,Tangent=0b100,Color=0b1000, SingleBone = 0b10000, DouleBone = 0b100000, QuadBone=0b1000000,PMX=0b10000000,
	UV_ex1 = 0b100000000, UV_ex2 = 0b1000000000, UV_ex3 = 0b1000000000, UV_ex4 = 0b1000000000,

};
enum class VertexType:std::uint8_t {
	Vertex_UV ,
	Vertex_Normal ,
	Vertex_UV_Normal,
	Vertex_UV_Normal_Tangent,
	Vertex_UV_Normal_Tangent_Color ,
	Vertex_UV_Normal_Color ,
	Vertex_Model_SingleBone,
	Vertex_Model_SingleBone_UVEx01,
	Vertex_Model_SingleBone_UVEx02,
	Vertex_Model_SingleBone_UVEx03,
	Vertex_Model_SingleBone_UVEx04,
	Vertex_Model_SingleBone_Tangent,
	Vertex_Model_DoubleBone,
	Vertex_Model_DoubleBone_UVEx01,
	Vertex_Model_DoubleBone_UVEx02,
	Vertex_Model_DoubleBone_UVEx03,
	Vertex_Model_DoubleBone_UVEx04,
	Vertex_Model_Normal_QuadBone,
	Vertex_Model_QuadBone,
	Vertex_Model_QuadBone_UVEx01,
	Vertex_Model_QuadBone_UVEx02,
	Vertex_Model_QuadBone_UVEx03,
	Vertex_Model_QuadBone_UVEx04,
	Vertex_Model_QuadBone_Tangent ,
	Vertex_Model_SDEFBone,
	Vertex_Model_SDEFBone_UVEX01,
	Vertex_Model_SDEFBone_UVEX02,
	Vertex_Model_SDEFBone_UVEX03,
	Vertex_Model_SDEFBone_UVEX04 ,
	Vertex_Model_Mix ,
};
using namespace ButiEngine;
namespace VertexInformation {
struct Vertex {
	Vector3 position;
	Vertex(const Vector3& arg_position) :position(arg_position) {}
	Vertex() {}
	Vector3 GetPosition()const {
		return position;
	}

};
struct UV {
	Vector2 uv;
	UV(const Vector2& arg_uv) :uv(arg_uv) {};
	UV() {}
	inline void SetUV(const Vector2& arg_uv) { uv = arg_uv; }
	inline Vector2 GetUV()const { return uv; }
};
struct UV_ex1 {
	Vector4 exuv1;
	UV_ex1(const Vector4& arg_uv) :exuv1(arg_uv) {};
	UV_ex1() {}
	inline void SetUV_ex1(const Vector4& arg_uv) { exuv1 = arg_uv; }
	inline const Vector4& GetUV_ex1()const { return exuv1; }
};
struct UV_ex2 {
	Vector4 exuv2;
	UV_ex2(const Vector4& arg_exuv2) :exuv2(arg_exuv2) {};
	UV_ex2() {}
	inline void SetUV_ex2(const Vector4& arg_exuv2) { exuv2 = arg_exuv2; }
	inline const Vector4& GetUV_ex2()const { return exuv2; }
};
struct UV_ex3 {
	Vector4 exuv3;
	UV_ex3(const Vector4& arg_exuv3) :exuv3(arg_exuv3) {};
	UV_ex3() {}
	inline void SetUV_ex3(const Vector4& arg_exuv3) { exuv3 = arg_exuv3; }
	inline const Vector4& GetUV_ex3()const { return exuv3; }
};
struct UV_ex4 {
	Vector4 exuv4;
	UV_ex4(const Vector4& arg_exuv4) :exuv4(arg_exuv4) {};
	UV_ex4() {}
	inline void SetUV_ex4(const Vector4& arg_exuv4) { exuv4 = arg_exuv4; }
	inline const Vector4& GetUV_ex4()const { return exuv4; }
};
struct Normal {
	Vector3 normal;
	Normal(const Vector3& arg_normal) :normal(arg_normal) {}
	Normal() {}
	inline void SetNormal(const Vector3& arg_normal) { normal = arg_normal; }
	inline const Vector3& GetNormal()const { return normal; }
};
struct Tangent {
	Vector3 tangent;
	Tangent(const Vector3& arg_tangent):tangent(arg_tangent){}
	Tangent(){}
	inline void SetTangent(const Vector3& arg_tangent) { tangent = arg_tangent; }
	inline const Vector3& GetTangent() { return tangent; }
};
struct Color {
	Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Color(const Vector4& arg_color) :color(arg_color) {}
	Color() {}
	inline void SetColor(const Vector4& arg_color) { color = arg_color; }
	inline const Vector4& GetColor()const { return color; }
};
struct SingleBone {
	std::int16_t boneIndex = -1;
};
struct DoubleBone {
	std::int32_t boneIndex_1 = -1;
	std::int32_t boneIndex_2 = -1;
	float weight = 0.0f;
};
struct QuadBone {
	std::int32_t boneIndex_1 = -1;
	std::int32_t boneIndex_2 = -1;
	std::int32_t boneIndex_3 = -1;
	std::int32_t boneIndex_4 = -1;
	float weight_1 = 0.0f;
	float weight_2 = 0.0f;
	float weight_3 = 0.0f;
	float weight_4 = 0.0f;
};
struct SDEFBone {
	std::int32_t boneIndex_1 = -1;
	std::int32_t boneIndex_2 = -1;
	float weight = 0.0f;
	Vector3 SDEF_C;
	Vector3 SDEF_R0;
	Vector3 SDEF_R1;
};
struct PMX {
	std::int32_t boneIndex_1 = -1;
	std::int32_t boneIndex_2 = -1;
	std::int32_t boneIndex_3 = -1;
	std::int32_t boneIndex_4 = -1;
	float weight_1 = 0.0f;
	float weight_2 = 0.0f;
	float weight_3 = 0.0f;
	float weight_4 = 0.0f;
	Vector3 SDEF_C;
	Vector3 SDEF_R0;
	Vector3 SDEF_R1;
};
}
struct Vertex :public VertexInformation::Vertex
{
	Vertex(const Vector3& arg_position) :VertexInformation::Vertex(arg_position){}
	Vertex() {}
};
struct Vertex_UV :public VertexInformation::Vertex, public VertexInformation::UV
{

	Vertex_UV(const Vector3& arg_position, const Vector2& arg_uv) :Vertex(arg_position), UV(arg_uv) {}
	Vertex_UV() {}
};
struct Vertex_Normal :public VertexInformation::Vertex, public VertexInformation::Normal
{
	
	Vertex_Normal(const Vector3& arg_position, const Vector3& arg_normal) :Vertex(arg_position), Normal(arg_normal)
	{
	}
	Vertex_Normal() {

	}
};
struct Vertex_Color :public VertexInformation::Vertex, public VertexInformation::Color
{
	

	Vertex_Color(const Vector3& arg_position, const Vector4& arg_color) :Vertex(arg_position), Color(arg_color)
	{}
	Vertex_Color() {}
};
struct Vertex_UV_Color :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Color
{
	
};
struct Vertex_UV_Normal :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal
{
	
	static VertexType GetType() {
		return VertexType::Vertex_UV_Normal;
	}
};
struct Vertex_UV_Normal_UVex01 :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::UV_ex1
{
	

};
struct Vertex_UV_Normal_UVex02 :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::UV_ex1, public VertexInformation::UV_ex2
{
	

};
struct Vertex_UV_Normal_UVex03 :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::UV_ex1, public VertexInformation::UV_ex2, public VertexInformation::UV_ex3
{
	

};
struct Vertex_UV_Normal_UVex04 :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::UV_ex1, public VertexInformation::UV_ex2, public VertexInformation::UV_ex3, public VertexInformation::UV_ex4
{
	

};

namespace ModelVertex {

struct Vertex_Model_SingleBone :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::SingleBone
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SingleBone;
	}
};
struct Vertex_Model_SingleBone_Tangent :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal,public VertexInformation::Tangent ,public VertexInformation::SingleBone
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SingleBone_Tangent;
	}
};
struct Vertex_Model_SingleBone_UVex01 :public Vertex_Model_SingleBone, public VertexInformation::UV_ex1
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SingleBone_UVEx01;
	}
};
struct Vertex_Model_SingleBone_UVex02 :public Vertex_Model_SingleBone_UVex01, public VertexInformation::UV_ex2
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SingleBone_UVEx02;
	}
};
struct Vertex_Model_SingleBone_UVex03 :public Vertex_Model_SingleBone_UVex02, public VertexInformation::UV_ex3
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SingleBone_UVEx03;
	}
};
struct Vertex_Model_SingleBone_UVex04 :public Vertex_Model_SingleBone_UVex03, public VertexInformation::UV_ex4
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SingleBone_UVEx04;
	}
};


struct Vertex_Model_DoubleBone :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::DoubleBone
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_DoubleBone;
	}
};

struct Vertex_Model_DoubleBone_UVex01 :public Vertex_Model_DoubleBone, public VertexInformation::UV_ex1
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_DoubleBone_UVEx01;
	}
};
struct Vertex_Model_DoubleBone_UVex02 :public Vertex_Model_DoubleBone_UVex01, public VertexInformation::UV_ex2
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_DoubleBone_UVEx02;
	}
};
struct Vertex_Model_DoubleBone_UVex03 :public Vertex_Model_DoubleBone_UVex02, public VertexInformation::UV_ex3
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_DoubleBone_UVEx03;
	}
};
struct Vertex_Model_DoubleBone_UVex04 :public Vertex_Model_DoubleBone_UVex03, public VertexInformation::UV_ex4
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_DoubleBone_UVEx04;
	}
};

struct Vertex_Model_QuadBone :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::QuadBone
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_QuadBone;
	}
};
struct Vertex_Model_Normal_QuadBone :public VertexInformation::Vertex, public VertexInformation::Normal, public VertexInformation::QuadBone
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_Normal_QuadBone;
	}
};
struct Vertex_Model_QuadBone_Tangent :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal,public VertexInformation::Tangent, public VertexInformation::QuadBone
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_QuadBone_Tangent;
	}
};

struct Vertex_Model_QuadBone_UVex01 :public Vertex_Model_QuadBone, public VertexInformation::UV_ex1
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_QuadBone_UVEx01;
	}
};
struct Vertex_Model_QuadBone_UVex02 :public Vertex_Model_QuadBone_UVex01, public VertexInformation::UV_ex2
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_QuadBone_UVEx02;
	}
};
struct Vertex_Model_QuadBone_UVex03 :public Vertex_Model_QuadBone_UVex02, public VertexInformation::UV_ex3
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_QuadBone_UVEx03;
	}
};
struct Vertex_Model_QuadBone_UVex04 :public Vertex_Model_QuadBone_UVex03, public VertexInformation::UV_ex4
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_QuadBone_UVEx04;
	}
};

struct Vertex_Model_SdefBone :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::SDEFBone
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SDEFBone;
	}
};


struct Vertex_Model_SdefBone_UVex01 :public Vertex_Model_SdefBone, public VertexInformation::UV_ex1
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SDEFBone_UVEX01;
	}
};
struct Vertex_Model_SdefBone_UVex02 :public Vertex_Model_SdefBone_UVex01, public VertexInformation::UV_ex2
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SDEFBone_UVEX01;
	}
};
struct Vertex_Model_SdefBone_UVex03 :public Vertex_Model_SdefBone_UVex02, public VertexInformation::UV_ex3
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SDEFBone_UVEX02;
	}
};
struct Vertex_Model_SdefBone_UVex04 :public Vertex_Model_SdefBone_UVex03, public VertexInformation::UV_ex4
{
	static VertexType GetType() {
		return VertexType::Vertex_Model_SDEFBone_UVEX03;
	}
};

struct Vertex_Model_PMX :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::PMX
{
	using type_quad = Vertex_Model_QuadBone;
	using type_single = Vertex_Model_SingleBone;
	using type_double = Vertex_Model_DoubleBone;
	using type_sdef = Vertex_Model_SdefBone;
	using this_type = Vertex_Model_PMX;
	Vertex_Model_PMX(){}
	this_type& operator=(const type_single arg_other) {
		position = arg_other.position;
		uv = arg_other.uv;
		normal = arg_other.normal;
		boneIndex_1 = arg_other.boneIndex;
		weight_1 = 1.0f;
		weight_2 = -1;
		weight_3 = -1;
		weight_4 = -1;
		return *this;
	}
	this_type& operator=(const type_double arg_other) {
		position = arg_other.position;
		uv = arg_other.uv;
		normal = arg_other.normal;
		boneIndex_1 = arg_other.boneIndex_1;
		boneIndex_2 = arg_other.boneIndex_2;
		weight_1 = arg_other.weight;
		weight_2 = 1.0f-weight_1;
		weight_3 = -1;
		weight_4 = -1;
		return *this;
	}
	this_type& operator=(const type_quad arg_other) {

		position = arg_other.position;
		uv = arg_other.uv;
		normal = arg_other.normal;
		boneIndex_1 = arg_other.boneIndex_1;
		boneIndex_2 = arg_other.boneIndex_2;
		boneIndex_3 = arg_other.boneIndex_3;
		boneIndex_4 = arg_other.boneIndex_4;
		weight_1 = arg_other.weight_1;
		weight_2 = arg_other.weight_2;
		weight_3 = arg_other.weight_3;
		weight_4 = arg_other.weight_4;
		return *this;
	}
	this_type& operator=(const type_sdef arg_other) {
		position = arg_other.position;
		uv = arg_other.uv;
		normal = arg_other.normal;
		boneIndex_1 = arg_other.boneIndex_1;
		boneIndex_2 = arg_other.boneIndex_2;
		weight_1 = arg_other.weight;
		weight_2 = 1.0f - weight_1;
		weight_3 = -1;
		weight_4 = -1;
		SDEF_C = arg_other.SDEF_C;
		SDEF_R0 = arg_other.SDEF_R0;
		SDEF_R1 = arg_other.SDEF_R1;
		return *this;
	}
};

struct Vertex_Model_PMX_UVex01 :public Vertex_Model_PMX, public VertexInformation::UV_ex1
{
	using type_quad = Vertex_Model_QuadBone_UVex01;
	using type_single = Vertex_Model_SingleBone_UVex01;
	using type_double = Vertex_Model_DoubleBone_UVex01;
	using type_sdef = Vertex_Model_SdefBone_UVex01;
	using this_type = Vertex_Model_PMX_UVex01;
	this_type& operator=(const type_single arg_other) {
		static_cast<Vertex_Model_PMX&>(*this) = arg_other;
		exuv1 = arg_other.exuv1;
		return *this;
	}
	this_type& operator=(const type_double arg_other) {
		static_cast<Vertex_Model_PMX&>(*this) = arg_other;
		exuv1 = arg_other.exuv1;
		return *this;
	}
	this_type& operator=(const type_quad arg_other) {
		static_cast<Vertex_Model_PMX&>(*this) = arg_other;
		exuv1 = arg_other.exuv1;
		return *this;
	}
	this_type& operator=(const type_sdef arg_other) {
		static_cast<Vertex_Model_PMX&>(*this) = arg_other;
		exuv1 = arg_other.exuv1;
		return *this;
	}
};
struct Vertex_Model_PMX_UVex02 :public Vertex_Model_PMX_UVex01, public VertexInformation::UV_ex2
{
	using type_quad = Vertex_Model_QuadBone_UVex02;
	using type_single = Vertex_Model_SingleBone_UVex02;
	using type_double = Vertex_Model_DoubleBone_UVex02;
	using type_sdef = Vertex_Model_SdefBone_UVex02;
	using this_type = Vertex_Model_PMX_UVex02;
	this_type& operator=(const type_single arg_other) {
		static_cast<Vertex_Model_PMX_UVex01&>(*this) = arg_other;
		exuv2 = arg_other.exuv2;
		return *this;
	}
	this_type& operator=(const type_double arg_other) {
		static_cast<Vertex_Model_PMX_UVex01&>(*this) = arg_other;
		exuv2 = arg_other.exuv2;
		return *this;
	}
	this_type& operator=(const type_quad arg_other) {
		static_cast<Vertex_Model_PMX_UVex01&>(*this) = arg_other;
		exuv2 = arg_other.exuv2;
		return *this;
	}
	this_type& operator=(const type_sdef arg_other) {
		static_cast<Vertex_Model_PMX_UVex01&>(*this) = arg_other;
		exuv2 = arg_other.exuv2;
		return *this;
	}
};
struct Vertex_Model_PMX_UVex03 :public Vertex_Model_PMX_UVex02, public VertexInformation::UV_ex3
{
	using type_quad = Vertex_Model_QuadBone_UVex03;
	using type_single = Vertex_Model_SingleBone_UVex03;
	using type_double = Vertex_Model_DoubleBone_UVex03;
	using type_sdef = Vertex_Model_SdefBone_UVex03;
	using this_type = Vertex_Model_PMX_UVex03;
	this_type& operator=(const type_single arg_other) {
		static_cast<Vertex_Model_PMX_UVex02&>(*this) = arg_other;
		exuv3 = arg_other.exuv3;
		return *this;
	}
	this_type& operator=(const type_double arg_other) {
		static_cast<Vertex_Model_PMX_UVex02&>(*this) = arg_other;
		exuv3 = arg_other.exuv3;
		return *this;
	}
	this_type& operator=(const type_quad arg_other) {
		static_cast<Vertex_Model_PMX_UVex02&>(*this) = arg_other;
		exuv3 = arg_other.exuv3;
		return *this;
	}
	this_type& operator=(const type_sdef arg_other) {
		static_cast<Vertex_Model_PMX_UVex02&>(*this) = arg_other;
		exuv3 = arg_other.exuv3;
		return *this;
	}
};
struct Vertex_Model_PMX_UVex04 :public Vertex_Model_PMX_UVex03, public VertexInformation::UV_ex4
{
	using type_quad = Vertex_Model_QuadBone_UVex04;
	using type_single = Vertex_Model_SingleBone_UVex04;
	using type_double = Vertex_Model_DoubleBone_UVex04;
	using type_sdef = Vertex_Model_SdefBone_UVex04;
	using this_type = Vertex_Model_PMX_UVex04;
	this_type& operator=(const type_single arg_other) {
		static_cast<Vertex_Model_PMX_UVex03&>(*this) = arg_other;
		exuv4 = arg_other.exuv4;
		return *this;
	}
	this_type& operator=(const type_double arg_other) {
		static_cast<Vertex_Model_PMX_UVex03&>(*this) = arg_other;
		exuv4 = arg_other.exuv4;
		return *this;
	}
	this_type& operator=(const type_quad arg_other) {
		static_cast<Vertex_Model_PMX_UVex03&>(*this) = arg_other;
		exuv4 = arg_other.exuv4;
		return *this;
	}
	this_type& operator=(const type_sdef arg_other) {
		static_cast<Vertex_Model_PMX_UVex03&>(*this) = arg_other;
		exuv4 = arg_other.exuv4;
		return *this;
	}
};
}
struct Vertex_Normal_Color :public VertexInformation::Vertex, public VertexInformation::Normal, public VertexInformation::Color
{
};

struct Vertex_UV_Normal_Color :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Color
{
	Vertex_UV_Normal_Color() {}
	Vertex_UV_Normal_Color(const Vector3& arg_position, const Vector2& arg_uv) :Vertex(arg_position), UV(arg_uv) {}
	Vertex_UV_Normal_Color(const Vector3& arg_position, const Vector2& arg_uv, const Vector3& arg_normal) :Vertex(arg_position), UV(arg_uv), Normal(arg_normal) {}

};


struct Vertex_UV_Normal_Tangent :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Tangent
{
	static constexpr VertexType GetType() {
		return VertexType::Vertex_UV_Normal_Tangent;
	}
};
struct Vertex_UV_Normal_Tangent_Color :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Tangent, public VertexInformation::Color
{
	Vertex_UV_Normal_Tangent_Color(){}
	Vertex_UV_Normal_Tangent_Color(const Vector3& arg_position,const Vector2& arg_uv,const Vector3& arg_normal,const Vector3& arg_tangent)
	:Vertex(arg_position),UV(arg_uv),Normal(arg_normal),Tangent(arg_tangent)
	{}
	static constexpr VertexType GetType() {
		return VertexType::Vertex_UV_Normal_Tangent_Color;
	}
};

}
}

#endif // !VERTEX_H
