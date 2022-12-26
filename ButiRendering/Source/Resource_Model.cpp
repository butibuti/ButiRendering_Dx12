#include "stdafx.h"
#include"ButiRendering_Dx12/Header/Resource_Model.h"
#define TINYGLTF_IMPLEMENTATION
#include "GLTF/tiny_gltf.h"
#include"../Header/Vertex.h"
#include"../Header/MeshPrimitive.h"
#include"../Header/MeshHelper.h"

#include"ButiRendering_Dx12/Header/ModelAnimation.h"
#include"ButiRendering_Dx12/Header/Resource_Model.h"

ButiEngine::ButiRendering::Resource_Model::Resource_Model()
{
}

void ButiEngine::ButiRendering::Resource_Model::SetMesh(const Value_ptr<IResource_Mesh>& arg_vlp_mesh)
{
	m_vlp_mesh = arg_vlp_mesh;
}

void ButiEngine::ButiRendering::Resource_Model::SetMaterial(const List<Value_ptr<IResource_Material>>& arg_list_vlp_material)
{
	m_list_vlp_material = arg_list_vlp_material;
}

void ButiEngine::ButiRendering::Resource_Model::SetName(const std::string& arg_name)
{
	m_name = arg_name;
}

void ButiEngine::ButiRendering::Resource_Model::SetEngName(const std::string& arg_engName)
{
	m_nameEng = arg_engName;
}

void ButiEngine::ButiRendering::Resource_Model::SetModelName(const std::string& arg_modelName)
{
	m_modelName = arg_modelName;
}

void ButiEngine::ButiRendering::Resource_Model::SetEngModelName(const std::string& arg_engModelName)
{
	m_modelNameEng = arg_engModelName;
}

void ButiEngine::ButiRendering::Resource_Model::SetComment(const std::string& arg_comment)
{
	m_comment = arg_comment;
}

void ButiEngine::ButiRendering::Resource_Model::SetEngComment(const std::string& arg_engComment)
{
	m_commentEng = arg_engComment;
}

void ButiEngine::ButiRendering::Resource_Model::AddBone(Bone & arg_bone)
{
	if (arg_bone.ownIndex == -1) {
		arg_bone.ownIndex = m_list_bone.GetSize();
		m_list_bone.Add(arg_bone);
	}
	else {
		if (m_list_bone.GetSize() <= arg_bone.ownIndex) {
			m_list_bone.Resize(arg_bone.ownIndex+1);
		}
		m_list_bone[arg_bone.ownIndex] = arg_bone;
	}
}

void ButiEngine::ButiRendering::Resource_Model::AddMorph(Value_ptr<Morph::Morph> arg_morph)
{
	m_vec_morphs.push_back(arg_morph);
}

void ButiEngine::ButiRendering::Resource_Model::SetVersion(const float arg_version)
{
	m_version = arg_version;
}

void ButiEngine::ButiRendering::Resource_Model::SetBone(const List<Bone>& arg_list_bone)
{
	m_list_bone = arg_list_bone;
}

void ButiEngine::ButiRendering::Resource_Model::SetSubset(const std::vector<std::uint32_t>& arg_subset)
{
	m_subset = arg_subset;
}


ButiEngine::List<ButiEngine::Value_ptr< ButiEngine::ButiRendering::Bone>> ButiEngine::ButiRendering::Resource_Model::GetBone()
{
	List<Value_ptr<Bone>> out;

	out.Reserve(m_list_bone.GetSize());
	for (auto bone : m_list_bone) {
		auto push = ObjectFactory::CreateCopy<Bone>(bone);
		push->transform = make_value<BoneTransform>(push->position, push->rotation, Vector3(1, 1, 1));
		out.push_back(push);
	}


	for (auto outputBone : out) {
		std::int32_t parentTransformIndex = outputBone->parentBoneIndex;
		if (parentTransformIndex >= 0) {
			outputBone->parentBone = out.at(parentTransformIndex);
			outputBone->transform->SetBaseTransform(outputBone->parentBone->transform, true);
			outputBone->transform->SetParentTransform(outputBone->parentBone->transform, true);
		}
	}
	for (auto outputBone : out) {
		if (outputBone->bindPose == Matrix4x4()) {
			outputBone->bindPose = outputBone->transform->GetBoneMatrix();
			outputBone->bindPoseInverse = outputBone->bindPose.GetInverse();
		}

		std::cout << outputBone->ownIndex << ":"+std::to_string(outputBone->bindPose.GetPosition()) << std::endl;
		//std::cout << "inverse:" << std::to_string(outputBone->bindPoseInverse) << std::endl;
	}


	for (auto itr = out.begin(); itr != out.end(); itr++) {
		(*itr)->SetOtherBoneLinks(out);
	}

	return out;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Mesh> ButiEngine::ButiRendering::Resource_Model::GetMesh() const
{
	return m_vlp_mesh;
}

const ButiEngine::List<ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Material>>& ButiEngine::ButiRendering::Resource_Model::GetMaterial() const
{
	return m_list_vlp_material;
}

ButiEngine::List<ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Material>>& ButiEngine::ButiRendering::Resource_Model::GetMaterial()
{
	return m_list_vlp_material;
}

std::vector<std::uint32_t> ButiEngine::ButiRendering::Resource_Model::GetSubset()
{
	return m_subset;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetName()
{
	return m_name;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetEngName()
{
	return m_nameEng;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetModelName()
{
	return m_modelName;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetEngModelName()
{
	return m_modelNameEng;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetComment()
{
	return m_comment;
}

const std::string & ButiEngine::ButiRendering::Resource_Model::GetEngComment()
{
	return m_commentEng;
}

void ButiEngine::ButiRendering::Resource_Model::AddMotion(Value_ptr<IResource_Motion> arg_vlp_model) {
	m_list_motion.Add(arg_vlp_model);
}
const ButiEngine::List<ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Motion>>& ButiEngine::ButiRendering::Resource_Model::GetMotion()const {
	return m_list_motion;
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Motion> ButiEngine::ButiRendering::Resource_Model::GetMotion(const std::string& arg_motionName)const {
	auto itr = m_list_motion.Find([arg_motionName](Value_ptr<IResource_Motion> arg_vlp_motion)->bool {return arg_vlp_motion->GetName() == arg_motionName; });
	if (itr!=nullptr) {
		return *itr;
	}return nullptr;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Model> ButiEngine::ButiRendering::CreateModel(const Value_ptr<IResource_Mesh>& arg_vlp_mesh, const List<Value_ptr<IResource_Material>>& arg_list_vlp_material, const List<Bone>& arg_list_bone, const std::string& arg_name)
{
	auto output = ObjectFactory::Create<Resource_Model>();
	output->SetMesh(arg_vlp_mesh);
	output->SetName(arg_name);
	output->SetMaterial(arg_list_vlp_material);
	output->SetBone(arg_list_bone);
	return output;
}

template<std::int32_t byteSize, typename VertexType>
void LoadIntData(tinygltf::Model& arg_model,
	const std::int32_t vertexCount, const std::int32_t location, ButiEngine::List<VertexType>& vertices, const unsigned char* src) {
	char dest[byteSize];
	for (std::int32_t index = 0; index < vertexCount; index++) {
		memcpy_s(dest, sizeof(dest), &src[location + index * sizeof(dest)], sizeof(dest));

		//std::cout << std::to_string(boneIndex.x) << "," << std::to_string(boneIndex.y) << "," << std::to_string(boneIndex.z) << "," << std::to_string(boneIndex.w) << std::endl;
		vertices[index].boneIndex_1 = static_cast<std::int32_t> (arg_model.skins[0].joints[dest[0 * byteSize / 4]]);
		vertices[index].boneIndex_2 = static_cast<std::int32_t> (arg_model.skins[0].joints[dest[1 * byteSize / 4]]);
		vertices[index].boneIndex_3 = static_cast<std::int32_t> (arg_model.skins[0].joints[dest[2 * byteSize / 4]]);
		vertices[index].boneIndex_4 = static_cast<std::int32_t> (arg_model.skins[0].joints[dest[3 * byteSize / 4]]);
	}
}

std::int32_t GetParentJoint(std::int32_t arg_child, const std::vector<std::int32_t>& arg_vec_joints, const std::vector<tinygltf::Node>& arg_vec_nodes) {

	for (auto joint : arg_vec_joints) {
		for (auto child : arg_vec_nodes[joint].children)
			if (child == arg_child) {
				return joint;
			}
	}
	return -1;
}

template<typename VertexType>
void LoadVertexGLTF(const std::int32_t vertexCount, tinygltf::Model& model, tinygltf::Primitive& primitive,
	ButiEngine::List<ButiEngine::Value_ptr<ButiEngine::ButiRendering::MeshPrimitiveBase>>& arg_list_primitiveBase,
	const ButiEngine::List<ButiEngine::Matrix4x4>& arg_list_bindPose) {
	using namespace ButiEngine;
	auto meshPrimitive = make_value<ButiRendering::MeshPrimitive<VertexType>>();


	meshPrimitive->vertices.Alloc(vertexCount, 0);
	for (auto& [attributeName, accessorIndex] : primitive.attributes) {
		const auto& glTFAccessor = model.accessors[accessorIndex];
		const auto& glTFBufferView = model.bufferViews[glTFAccessor.bufferView];
		const auto& glTFBuffer = model.buffers[glTFBufferView.buffer];

		auto location = glTFBufferView.byteOffset + glTFAccessor.byteOffset;

		if constexpr (std::is_base_of_v<Vertex::VertexInformation::Normal, VertexType>) {
			if (attributeName == "NORMAL") {
				Vector3 normal;
				for (std::int32_t index = 0; index < vertexCount; index++) {
					memcpy_s(&normal, sizeof(Vector3), &glTFBuffer.data.data()[location + index * sizeof(Vector3)], sizeof(Vector3));
					meshPrimitive->vertices[index].normal = normal.Normalize();
				}
			}
		}

		if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV, VertexType>) {
			if (attributeName == "TEXCOORD_0") {
				for (std::int32_t index = 0; index < vertexCount; index++) {
					memcpy_s(&meshPrimitive->vertices[index].uv, sizeof(Vector2), &glTFBuffer.data.data()[location + index * sizeof(Vector2)], sizeof(Vector2));
				}
			}
		}

		if constexpr (std::is_base_of_v<Vertex::VertexInformation::QuadBone, VertexType>) {
			if (attributeName == "JOINTS_0") {
				auto byteSize = glTFBufferView.byteLength / vertexCount;
				switch (byteSize)
				{
				case 4:
				{
					LoadIntData<4>(model, vertexCount, location, meshPrimitive->vertices, glTFBuffer.data.data());
				}
				break;
				case 8:
				{
					LoadIntData<8>(model, vertexCount, location, meshPrimitive->vertices, glTFBuffer.data.data());
				}
				break;
				case 16:
				{
					LoadIntData<16>(model, vertexCount, location, meshPrimitive->vertices, glTFBuffer.data.data());
				}
				break;
				default:
					break;
				}
			}
			if (attributeName == "WEIGHTS_0") {
				for (std::int32_t index = 0; index < vertexCount; index++) {

					memcpy_s(&meshPrimitive->vertices[index].weight_1, sizeof(Vector4), &glTFBuffer.data.data()[location + index * sizeof(Vector4)], sizeof(Vector4));

				}
			}
		}

		if constexpr (std::is_base_of_v<Vertex::VertexInformation::Vertex, VertexType>) {
			if (attributeName == "POSITION") {
				for (std::int32_t index = 0; index < vertexCount; index++) {
					memcpy_s(&meshPrimitive->vertices[index].position, sizeof(Vector3), &glTFBuffer.data.data()[location + index * sizeof(Vector3)], sizeof(Vector3));
				}
			}
		}
	}


	if constexpr (std::is_base_of_v<Vertex::VertexInformation::Normal, VertexType> && std::is_base_of_v<Vertex::VertexInformation::QuadBone, VertexType>
		&& !std::is_same_v<Vertex::ModelVertex::Vertex_Model_Normal_QuadBone, VertexType>) {
		auto vlp_primitive_normQuadBone = make_value<ButiRendering::MeshPrimitive<Vertex::ModelVertex::Vertex_Model_Normal_QuadBone>>();
		ButiRendering::MeshHelper::VertexConvert(*meshPrimitive, *vlp_primitive_normQuadBone);
		arg_list_primitiveBase.Add(vlp_primitive_normQuadBone);
	}
	if constexpr (std::is_base_of_v<Vertex::VertexInformation::Normal, VertexType> && std::is_base_of_v<Vertex::VertexInformation::UV, VertexType>
		&& !std::is_same_v<Vertex::Vertex_UV_Normal, VertexType>) {
		auto vlp_primitive_UVNormal = make_value<ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal>>();
		ButiRendering::MeshHelper::VertexConvert(*meshPrimitive, *vlp_primitive_UVNormal);
		arg_list_primitiveBase.Add(vlp_primitive_UVNormal);
	}
	if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV, VertexType> && !std::is_same_v<Vertex::Vertex_UV, VertexType>) {
		auto vlp_primitive_UV = make_value<ButiRendering::MeshPrimitive<Vertex::Vertex_UV>>();
		ButiRendering::MeshHelper::VertexConvert(*meshPrimitive, *vlp_primitive_UV);
		arg_list_primitiveBase.Add(vlp_primitive_UV);
	}
	if constexpr (std::is_base_of_v<Vertex::VertexInformation::Normal, VertexType> && !std::is_same_v<Vertex::Vertex_Normal, VertexType>) {
		auto vlp_primitive_Normal = make_value<ButiRendering::MeshPrimitive<Vertex::Vertex_Normal>>();
		ButiRendering::MeshHelper::VertexConvert(*meshPrimitive, *vlp_primitive_Normal);
		arg_list_primitiveBase.Add(vlp_primitive_Normal);
	}
	if constexpr (std::is_base_of_v<Vertex::VertexInformation::Vertex, VertexType>) {
		auto vlp_primitive = make_value<ButiRendering::MeshPrimitive<Vertex::Vertex>>();
		ButiRendering::MeshHelper::VertexConvert(*meshPrimitive, *vlp_primitive);
		arg_list_primitiveBase.Add(vlp_primitive);
	}
	arg_list_primitiveBase.Add(meshPrimitive);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Model> ButiEngine::ButiRendering::CreateModelFromGLTFBinary(ButiEngine::Value_ptr<ButiEngine::IBinaryReader> arg_reader, const std::string& arg_modelPath,
	ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::GraphicDevice> arg_vwp_graphicDevice) {
	using namespace ButiEngine;
	tinygltf::Model model;
	tinygltf::TinyGLTF gltf_ctx;
	std::string err;
	std::string warn;
	auto output = ObjectFactory::Create<ButiRendering::Resource_Model>();

	gltf_ctx.SetStoreOriginalJSONForExtrasAndExtensions(false);
	auto size = arg_reader->GetReamainSize();
	bool ret = arg_reader->GetAllData() ? gltf_ctx.LoadBinaryFromMemory(&model, &err, &warn,
		reinterpret_cast<const unsigned char*> (arg_reader->GetAllData()), size) :
		gltf_ctx.LoadBinaryFromFile(&model, &err, &warn,
			arg_reader->GetFilePath());
	List< Value_ptr< ButiRendering::MeshPrimitiveBase>> list_vlp_primtive;
	List<Value_ptr<ButiRendering::IResource_Material>> list_vlp_material;
	std::vector<std::uint32_t> list_subset;
	auto vlp_index = make_value<List<std::uint32_t>>();
	ButiRendering::BoxSurface boxSurface;

	List<Matrix4x4> list_inverse;
	std::map<std::int32_t,ButiRendering::Bone> map_bone;
	Matrix4x4 armatureMatrix;
	for (auto& skin : model.skins) {

		for (auto& scene : model.scenes) {
			for (auto node : scene.nodes) {
				if (model.nodes[node].name == skin.name) {
					if (model.nodes[node].scale.size() > 2) {
						armatureMatrix *= Matrix4x4::Scale(Vector3(model.nodes[node].scale[0],
							model.nodes[node].scale[1], model.nodes[node].scale[2]));
					}
					if (model.nodes[node].rotation.size() > 3) {
						armatureMatrix *= Quat(model.nodes[node].rotation[0], model.nodes[node].rotation[1],
							model.nodes[node].rotation[2], model.nodes[node].rotation[3]).ToMatrix();
					}
					if (model.nodes[node].translation.size() > 2) {
						armatureMatrix *= Matrix4x4::Translate(Vector3(model.nodes[node].translation[0],
							model.nodes[node].translation[1], model.nodes[node].translation[2]));
					}
				}
			}
		}
		std::int32_t index = 0;

		{
			auto& invAccess = model.accessors[skin.inverseBindMatrices];
			auto& invBufferView = model.bufferViews[invAccess.bufferView];
			auto& invBuffer = model.buffers[invBufferView.buffer];
			auto location = invBufferView.byteOffset + invAccess.byteOffset;
			list_inverse.Resize(invAccess.count);
			if (invAccess.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
				Matrix4x4 mat;
				for (std::int32_t index = 0; index < invAccess.count; index++) {
					memcpy_s(&mat, sizeof(mat), &invBuffer.data.data()[location + index * sizeof(mat)], sizeof(mat));

					list_inverse[index] = mat;
					//std::cout << m_list_bone[index].ownIndex << ":" << std::to_string(mat) << std::endl;
				}
			}
			else {
				Matrix4x4 inverseMatrix;
				double mat[16];
				list_inverse.Resize(invBufferView.byteLength / sizeof(mat));
				for (std::int32_t index = 0, size = invBufferView.byteLength / sizeof(mat); index < size; index++) {
					memcpy_s(&mat, sizeof(mat), &invBuffer.data.data()[location + index * sizeof(mat)], sizeof(mat));
				}
				inverseMatrix._11 = static_cast<float>(mat[0]); inverseMatrix._12 = static_cast<float>(mat[1]); inverseMatrix._13 = static_cast<float>(mat[2]); inverseMatrix._14 = static_cast<float>(mat[3]);
				inverseMatrix._21 = static_cast<float>(mat[4]); inverseMatrix._22 = static_cast<float>(mat[5]); inverseMatrix._23 = static_cast<float>(mat[6]); inverseMatrix._24 = static_cast<float>(mat[7]);
				inverseMatrix._31 = static_cast<float>(mat[8]); inverseMatrix._32 = static_cast<float>(mat[9]); inverseMatrix._33 = static_cast<float>(mat[10]); inverseMatrix._34 = static_cast<float>(mat[11]);
				inverseMatrix._41 = static_cast<float>(mat[12]); inverseMatrix._42 = static_cast<float>(mat[13]); inverseMatrix._43 = static_cast<float>(mat[14]); inverseMatrix._44 = static_cast<float>(mat[15]);
				list_inverse[index] = inverseMatrix.Inverse();
				std::cout << std::to_string(mat) << std::endl;
			}
			for (std::int32_t index = 0; index < list_inverse.GetSize(); index++) {
				//std::cout << index << ":" + std::to_string(list_inverse[index]) << std::endl;
			}
		}
		index = 0;

		for (auto joint : skin.joints) {
			auto& node = model.nodes[joint];
			ButiRendering::Bone bone;
			bone.boneName = node.name;
			bone.parentBoneIndex = GetParentJoint(joint, skin.joints, model.nodes);
			bone.ownIndex = joint;
			if (node.translation.size() > 2) {
				bone.position = Vector3(node.translation[0], node.translation[1], node.translation[2]);
			}
			if (node.rotation.size() > 3) {
				bone.rotation = Quat(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]).ToMatrix();
			}


			if (node.matrix.size()) {
				Matrix4x4 mat;
				mat._11 = static_cast<float>(node.matrix[0]); mat._12 = static_cast<float>(node.matrix[1]); mat._13 = static_cast<float>(node.matrix[2]); mat._14 = static_cast<float>(node.matrix[3]);
				mat._21 = static_cast<float>(node.matrix[4]); mat._22 = static_cast<float>(node.matrix[5]); mat._23 = static_cast<float>(node.matrix[6]); mat._24 = static_cast<float>(node.matrix[7]);
				mat._31 = static_cast<float>(node.matrix[8]); mat._32 = static_cast<float>(node.matrix[9]); mat._33 = static_cast<float>(node.matrix[10]); mat._34 = static_cast<float>(node.matrix[11]);
				mat._41 = static_cast<float>(node.matrix[12]); mat._42 = static_cast<float>(node.matrix[13]); mat._43 = static_cast<float>(node.matrix[14]); mat._44 = static_cast<float>(node.matrix[15]);
				bone.rotation = mat.Transpose().RemovePosition();
			}

			if (bone.parentBoneIndex == -1) {
				bone.rotation *= armatureMatrix;
				bone.position *= armatureMatrix;
			}

			if (list_inverse.GetSize()) {
				bone.bindPoseInverse = list_inverse[index];
				bone.bindPose = list_inverse[index].GetInverse();

				std::cout << index << ":" + std::to_string(list_inverse[index]) << std::endl;
			}
			map_bone.emplace (joint ,bone);
			output->AddBone(bone);
			index++;
		}


	}


	for (auto& mesh : model.meshes) {
		for (auto primitive : mesh.primitives) {

			std::int32_t vertexType = 0, vertexCount = 0;
			for (auto& attribute : primitive.attributes) {
				if (attribute.first == "NORMAL") {
					vertexType |= Vertex::VertexFlag::Normal;
				}
				else if (attribute.first == "TEXCOORD_0") {
					vertexType |= Vertex::VertexFlag::UV;
				}
				else if (attribute.first == "JOINTS_0") {
					vertexType |= Vertex::VertexFlag::QuadBone;
				}
				else if (attribute.first == "POSITION") {
					const auto& glTFAccessor = model.accessors[attribute.second];
					const auto& glTFBufferView = model.bufferViews[glTFAccessor.bufferView];
					vertexCount = glTFBufferView.byteLength / sizeof(Vector3);
				}
			}
			if (vertexType & Vertex::VertexFlag::QuadBone) {
				LoadVertexGLTF<Vertex::ModelVertex::Vertex_Model_QuadBone>(vertexCount, model, primitive, list_vlp_primtive, list_inverse);
			}
			else {
				LoadVertexGLTF<Vertex::Vertex_UV_Normal>(vertexCount, model, primitive, list_vlp_primtive, list_inverse);
			}
			// index
			{
				const auto& glTFAccessor = model.accessors[primitive.indices];
				const auto& glTFBufferView = model.bufferViews[glTFAccessor.bufferView];
				const auto& glTFBuffer = model.buffers[glTFBufferView.buffer];

				auto location = glTFBufferView.byteOffset + glTFAccessor.byteOffset;
				auto sizeInBytes = static_cast<std::uint32_t>(glTFBufferView.byteLength - glTFAccessor.byteOffset);

				auto indexCount = static_cast<uint32_t>(glTFAccessor.count);
				vlp_index->Reserve(indexCount);
				switch (glTFAccessor.componentType) {
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					for (std::int32_t index = 0; index < indexCount; index++) {
						vlp_index->Add(*reinterpret_cast<const std::uint8_t*>(&glTFBuffer.data.data()[location + index * sizeof(std::uint8_t)]));
					}
					break;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
					for (std::int32_t index = 0; index < indexCount; index++) {
						vlp_index->Add(*reinterpret_cast<const std::uint16_t*>(&glTFBuffer.data.data()[location + index * sizeof(std::uint16_t)]));
					}
					break;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					*vlp_index = List<std::uint32_t>(&glTFBuffer.data.data()[location], indexCount * sizeof(std::uint32_t));
					break;
				}
			}
			// subset
			if (primitive.material >= 0) {
				list_subset.push_back(vlp_index->GetSize());
			}
			else {
				list_subset.push_back(vlp_index->GetSize());
			}
		}
	}

	for (auto& glTFMaterial : model.materials) {

		std::string  materialName = glTFMaterial.name;
		auto filePath = arg_modelPath + "/" + glTFMaterial.name;
		MaterialValue value;
		DrawSettings drawSettings;
		if (glTFMaterial.alphaMode == "OPAQUE") {
			drawSettings.isAlpha = false;
		}
		if (glTFMaterial.alphaMode == "MASK") {

		}
		else {
			drawSettings.blendMode = ButiRendering::BlendMode::AlphaBlend;
		}

		if (glTFMaterial.doubleSided) {
			drawSettings.cullMode = ButiRendering::CullMode::none;
		}
		else {
			drawSettings.cullMode = ButiRendering::CullMode::back;
		}

		if (glTFMaterial.pbrMetallicRoughness.baseColorFactor.size()) {
			value.diffuse.x = glTFMaterial.pbrMetallicRoughness.baseColorFactor[0];
			value.diffuse.y = glTFMaterial.pbrMetallicRoughness.baseColorFactor[1];
			value.diffuse.z = glTFMaterial.pbrMetallicRoughness.baseColorFactor[2];
			value.diffuse.w = glTFMaterial.pbrMetallicRoughness.baseColorFactor[3];
		}
		if (glTFMaterial.emissiveFactor.size()) {
			value.emissive.x = glTFMaterial.emissiveFactor[0];
			value.emissive.y = glTFMaterial.emissiveFactor[1];
			value.emissive.z = glTFMaterial.emissiveFactor[2];
		}
		List<Value_ptr<ButiRendering::IResource_Texture>> list_vlp_texture;
		Value_ptr<ButiRendering::IResource_Shader> vlp_shader;
		if (model.skins.size()) {
			vlp_shader = ButiRendering::CreateShader(ButiRendering::DefaultVertexShader::CreateNormal_QuadBone(arg_vwp_graphicDevice), ButiRendering::DefaultPixelShader::CreateOnlyMaterial(arg_vwp_graphicDevice), nullptr, "OnlyMaterial_Quad");
		}
		else {
			vlp_shader = ButiRendering::CreateShader(ButiRendering::DefaultVertexShader::CreateNormal(arg_vwp_graphicDevice), ButiRendering::DefaultPixelShader::CreateOnlyMaterial(arg_vwp_graphicDevice), nullptr, "OnlyMaterial");
		}


		list_vlp_material.push_back(ButiRendering::CreateMaterial(filePath, value, vlp_shader, list_vlp_texture, drawSettings, arg_vwp_graphicDevice));

	}

	for (auto& animation : model.animations) {
		constexpr float frameSec = 60.0f;
		std::unordered_map<std::string,List<MotionKeyFrameData> >map_motionKeyFramedata;
		for (std::int32_t index = 0, size = animation.channels.size(); index < size;index++) {
			
			auto& channel = animation.channels[index];
			auto& sampler = animation.samplers[index];
			auto& targetBone = map_bone[channel.target_node];
			auto& inputAccessor = model.accessors[sampler.input];
			auto& outputAccessor = model.accessors[sampler.output];
			
			auto& inputBufferView = model.bufferViews[inputAccessor.bufferView];
			auto& inputBuffer = model.buffers[inputBufferView.buffer];
			auto& outputBufferView = model.bufferViews[outputAccessor.bufferView];
			auto& outputBuffer = model.buffers[outputBufferView.buffer];
			auto inputLocation = inputAccessor.byteOffset + inputBufferView.byteOffset;
			auto outputLocation = outputAccessor.byteOffset + outputBufferView.byteOffset;

			List<float> list_frame;
			list_frame.Reserve(inputAccessor.count);
			for (std::int32_t inputIndex = 0; inputIndex < inputAccessor.count; inputIndex++) {
				auto f =(*reinterpret_cast<const float*>(&inputBuffer.data.data()[inputLocation + inputIndex * sizeof(float)]))*frameSec;
				list_frame.Add(f);
			}
			List<MotionKeyFrameData>* p_list_motionKeyFrameData;
			if (map_motionKeyFramedata.count(targetBone.boneName)) {
				p_list_motionKeyFrameData = &map_motionKeyFramedata[targetBone.boneName];
			}
			else {
				p_list_motionKeyFrameData =&map_motionKeyFramedata.emplace(targetBone.boneName, List<MotionKeyFrameData>()).first->second;
			}

			for (std::int32_t index = 0; index < list_frame.GetSize();index++) {
				auto frame = list_frame[index];
				auto itr= p_list_motionKeyFrameData->Find(
					[frame](const MotionKeyFrameData& arg_data)->bool {return frame == arg_data.endFrame; }
				);
				if (itr == nullptr) {
					MotionKeyFrameData data;
					data.endFrame = frame;
					p_list_motionKeyFrameData->Add(data);
					itr = p_list_motionKeyFrameData->end() - 1;
				}

				if (channel.target_path == "translation") {
					itr->isTranslation = true;
					itr->pose.position =*reinterpret_cast<Vector3*> (&outputBuffer.data.data()[outputLocation + index * sizeof(Vector3)]);

				}

				if (channel.target_path == "rotation") {
					itr->isRotation = true;
					switch (outputAccessor.componentType)
					{
					case TINYGLTF_COMPONENT_TYPE_FLOAT: {

						Vector4 vec = *reinterpret_cast<Vector4*> (&outputBuffer.data.data()[outputLocation + index * sizeof(Vector4)]);
						itr->pose.rotation = Quat( vec.x, vec.y, vec.z, vec.w);
					}
						break;
					case TINYGLTF_COMPONENT_TYPE_BYTE:
					{
						std::int8_t data[4];
						memcpy_s(data, sizeof(data), &outputBuffer.data.data()[outputLocation + index * sizeof(data)],sizeof(data));
						itr->pose.rotation.x = *reinterpret_cast<float*> (&data[0]);
						itr->pose.rotation.y = *reinterpret_cast<float*> (&data[1]);
						itr->pose.rotation.z = *reinterpret_cast<float*> (&data[2]);
						itr->pose.rotation.w = *reinterpret_cast<float*> (&data[3]);
					}
						break;
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					{
						std::uint8_t data[4];
						memcpy_s(data, sizeof(data), &outputBuffer.data.data()[outputLocation + index * sizeof(data)], sizeof(data));
						itr->pose.rotation.x = *reinterpret_cast<float*> (&data[0]);
						itr->pose.rotation.y = *reinterpret_cast<float*> (&data[1]);
						itr->pose.rotation.z = *reinterpret_cast<float*> (&data[2]);
						itr->pose.rotation.w = *reinterpret_cast<float*> (&data[3]);
					}
						break;
					case TINYGLTF_COMPONENT_TYPE_SHORT:
					{
						std::int16_t data[4];
						memcpy_s(data, sizeof(data), &outputBuffer.data.data()[outputLocation + index * sizeof(data)], sizeof(data));
						itr->pose.rotation.x = *reinterpret_cast<float*> (&data[0]);
						itr->pose.rotation.y = *reinterpret_cast<float*> (&data[1]);
						itr->pose.rotation.z = *reinterpret_cast<float*> (&data[2]);
						itr->pose.rotation.w = *reinterpret_cast<float*> (&data[3]);
					}
						break;
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
					{
						std::uint16_t data[4];
						memcpy_s(data, sizeof(data), &outputBuffer.data.data()[outputLocation + index * sizeof(data)], sizeof(data));
						itr->pose.rotation.x = *reinterpret_cast<float*> (&data[0]);
						itr->pose.rotation.y = *reinterpret_cast<float*> (&data[1]);
						itr->pose.rotation.z = *reinterpret_cast<float*> (&data[2]);
						itr->pose.rotation.w = *reinterpret_cast<float*> (&data[3]);
					}
						break;
					default:
						break;
					}
					//itr->pose.rotation=itr->pose.rotation.ToMatrix().Transpose().ToQuat();
					if (targetBone.parentBoneIndex == -1) {
						itr->pose.rotation = (armatureMatrix.GetRemovePosition() * itr->pose.rotation.ToMatrix()).ToQuat();
						itr->pose.position *= armatureMatrix.GetRemoveRotation();
					}
				}
				if (channel.target_path == "scale") {
					itr->isScale = true;
					itr->pose.scale = *reinterpret_cast<Vector3*> (&outputBuffer.data.data()[outputLocation + index * sizeof(Vector3)]);
				}
			}
		}

		auto motion = CreateMotion(animation.name, map_motionKeyFramedata);
		output->AddMotion(motion);
	}

	list_vlp_primtive.ForEach([vlp_index](Value_ptr< ButiRendering::MeshPrimitiveBase> prim)->void {prim->SetIndex(vlp_index); });

	output->SetMesh(ButiRendering::CreateMesh(arg_modelPath, list_vlp_primtive, ButiRendering::BoxEightCorner(boxSurface), arg_vwp_graphicDevice));
	output->SetMaterial(list_vlp_material);
	output->SetSubset(list_subset);
	return output;
}
