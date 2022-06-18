#pragma once
#include"stdafx.h"
#include"Bone.h"
#include"Morph.h"
namespace ButiEngine {
namespace ButiRendering {

class Resource_Model :public IResource_Model
{
public:
	BUTIRENDERING_API Resource_Model();

	void Initialize()override {};
	void PreInitialize()override {};

	BUTIRENDERING_API void SetMesh(const Value_weak_ptr< IResource_Mesh>& arg_vwp_mesh) override;
	BUTIRENDERING_API void SetMaterial(const List<Value_weak_ptr< IResource_Material>>& arg_list_vwp_material) override;
	BUTIRENDERING_API void SetName(const std::string& arg_name) override;
	BUTIRENDERING_API void SetEngName(const std::string& arg_engName) override;
	BUTIRENDERING_API void SetModelName(const std::string& arg_modelName) override;
	BUTIRENDERING_API void SetEngModelName(const std::string& arg_engModelName) override;
	BUTIRENDERING_API void SetComment(const std::string& arg_comment) override;
	BUTIRENDERING_API void SetEngComment(const std::string& arg_engComment) override;
	BUTIRENDERING_API void AddBone(Bone& arg_bone) override;
	BUTIRENDERING_API void AddMorph(Value_ptr<Morph::Morph> arg_morph) override;
	BUTIRENDERING_API void SetVersion(const float arg_version) override;
	BUTIRENDERING_API void SetBone(const List<Bone>& arg_list_bone)override;
	BUTIRENDERING_API void SetSubset(const std::vector<std::uint32_t>& arg_subset) override;
	BUTIRENDERING_API std::vector<std::uint32_t> GetSubset() override;
	BUTIRENDERING_API const std::string& GetName() override;
	BUTIRENDERING_API const std::string& GetEngName() override;
	BUTIRENDERING_API const std::string& GetModelName() override;
	BUTIRENDERING_API const std::string& GetEngModelName() override;
	BUTIRENDERING_API const std::string& GetComment() override;
	BUTIRENDERING_API const std::string& GetEngComment() override;
	BUTIRENDERING_API List<Value_ptr<Bone>> GetBone() override;
	BUTIRENDERING_API Value_weak_ptr<IResource_Mesh> GetMesh()const override;
	BUTIRENDERING_API const List<Value_weak_ptr<IResource_Material>>& GetMaterial()const override;
	BUTIRENDERING_API List<Value_weak_ptr<IResource_Material>>& GetMaterial() override;
private:
	std::string modelName;
	std::string modelNameEng;
	std::string comment;
	std::string commentEng;
	std::string name;
	std::string nameEng;
	std::vector<Value_ptr< Morph::Morph>> vec_morphs;

	std::vector<std::uint32_t> subset;

	Value_weak_ptr<IResource_Mesh> vwp_mesh;
	List<Value_weak_ptr<IResource_Material>> list_vwp_material;

	List<Bone> list_bone;

	float version;
};
}
}