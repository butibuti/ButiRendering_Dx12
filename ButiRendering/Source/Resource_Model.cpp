#include "stdafx.h"
#include"ButiRendering_Dx12/Header/Resource_Model.h"


ButiEngine::ButiRendering::Resource_Model::Resource_Model()
{
}

void ButiEngine::ButiRendering::Resource_Model::SetMesh(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh)
{
	vwp_mesh = arg_vwp_mesh;
}

void ButiEngine::ButiRendering::Resource_Model::SetMaterial(const List<Value_weak_ptr<IResource_Material>>& arg_list_vwp_material)
{
	list_vwp_material = arg_list_vwp_material;
}

void ButiEngine::ButiRendering::Resource_Model::SetName(const std::string& arg_name)
{
	name = arg_name;
}

void ButiEngine::ButiRendering::Resource_Model::SetEngName(const std::string& arg_engName)
{
	nameEng = arg_engName;
}

void ButiEngine::ButiRendering::Resource_Model::SetModelName(const std::string& arg_modelName)
{
	modelName = arg_modelName;
}

void ButiEngine::ButiRendering::Resource_Model::SetEngModelName(const std::string& arg_engModelName)
{
	modelNameEng = arg_engModelName;
}

void ButiEngine::ButiRendering::Resource_Model::SetComment(const std::string& arg_comment)
{
	comment = arg_comment;
}

void ButiEngine::ButiRendering::Resource_Model::SetEngComment(const std::string& arg_engComment)
{
	commentEng = arg_engComment;
}

void ButiEngine::ButiRendering::Resource_Model::AddBone(Bone & arg_bone)
{
	arg_bone.ownIndex = list_bone.GetSize();
	list_bone.Add(arg_bone);
}

void ButiEngine::ButiRendering::Resource_Model::AddMorph(Value_ptr<Morph::Morph> arg_morph)
{
	vec_morphs.push_back(arg_morph);
}

void ButiEngine::ButiRendering::Resource_Model::SetVersion(const float arg_version)
{
	version = arg_version;
}

void ButiEngine::ButiRendering::Resource_Model::SetBone(const List<Bone>& arg_list_bone)
{
	list_bone = arg_list_bone;
}

void ButiEngine::ButiRendering::Resource_Model::SetSubset(const std::vector<std::uint32_t>& arg_subset)
{
	subset = arg_subset;
}


ButiEngine::List<ButiEngine::Value_ptr< ButiEngine::ButiRendering::Bone>> ButiEngine::ButiRendering::Resource_Model::GetBone()
{
	List<Value_ptr<Bone>> out;

	out.Reserve(list_bone.GetSize());
	for (auto bone : list_bone) {
		auto push = ObjectFactory::CreateCopy<Bone>(bone);
		push->transform = make_value<BoneTransform>(push->position, push->rotation, Vector3(1, 1, 1));
		out.push_back(push);
	}


	for (auto itr = out.begin(); itr != out.end(); itr++) {
		std::int32_t parentTransformIndex = (*itr)->parentBoneIndex;
		if (parentTransformIndex >= 0) {
			(*itr)->parentBone = out.at(parentTransformIndex);
			(*itr)->transform->SetBaseTransform((*itr)->parentBone->transform, false);
			(*itr)->transform->SetParentTransform(dynamic_value_ptr_cast<BoneTransform>( (*itr)->parentBone->transform),true);
		}
		(*itr)->bindPose = dynamic_value_ptr_cast<BoneTransform>((*itr)->transform)->GetBoneMatrix();
		(*itr)->bindPoseInverse = (*itr)->bindPose.GetInverse();

	}


	for (auto itr = out.begin(); itr != out.end(); itr++) {
		(*itr)->SetOtherBoneLinks(out);
	}

	return out;
}

ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IResource_Mesh> ButiEngine::ButiRendering::Resource_Model::GetMesh() const
{
	return vwp_mesh;
}

const ButiEngine::List<ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IResource_Material>>& ButiEngine::ButiRendering::Resource_Model::GetMaterial() const
{
	return list_vwp_material;
}

ButiEngine::List<ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IResource_Material>>& ButiEngine::ButiRendering::Resource_Model::GetMaterial()
{
	return list_vwp_material;
}

std::vector<std::uint32_t> ButiEngine::ButiRendering::Resource_Model::GetSubset()
{
	return subset;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetName()
{
	return name;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetEngName()
{
	return nameEng;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetModelName()
{
	return modelName;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetEngModelName()
{
	return modelNameEng;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetComment()
{
	return comment;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetEngComment()
{
	return commentEng;
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Model> ButiEngine::ButiRendering::CreateModel(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const List<Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, const List<Bone>& arg_list_bone, const std::string& arg_name)
{
	auto output = ObjectFactory::Create<Resource_Model>();
	output->SetMesh(arg_vwp_mesh);
	output->SetName(arg_name);
	output->SetMaterial(arg_list_vwp_material);
	output->SetBone(arg_list_bone);
	return output;
}
