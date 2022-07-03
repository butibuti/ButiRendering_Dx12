#ifndef IDRAWOBJ_H
#define IDRAWOBJ_H
#include"../Bone.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
namespace ButiEngine {
namespace ButiRendering {
class IDrawObject :public IObject {
public:
	virtual void Draw() = 0;
	virtual void DrawBefore() {}
	virtual float GetZ(const Matrix4x4& arg_vpMatrix) = 0;
	virtual void SetInfo() {}
	virtual void SetInfo_WithoutCommand() {}
	virtual void BufferUpdate() {}
	virtual void CommandSet() {}
	virtual void CommandExecute() {}
	virtual void ShowZ() {}
	virtual List<Value_ptr<IDrawObject>>& GetChilds() { static List<Value_ptr<IDrawObject>> empty; return empty; }

	virtual void SetOctRegistPtr(std::uint32_t* arg_ptr) = 0;
	virtual std::uint32_t* GetOctRegistPtr() = 0;
};
class IBoneObject :public  IObject {
public:
	List<Value_ptr<Bone>> m_list_bone;
	List<Value_ptr<Bone>> m_list_IKBone;
	List<Value_ptr<Bone>> m_list_addBone;
	void InverseKinematic() {
		for (auto itr : m_list_IKBone) {
			(itr)->CCDInverseKinematic();
		}
	}
	inline void BonePowerAdd() {
		for (auto itr : m_list_addBone) {
			(itr)->AddBonePower();
		}
	}

	List<Value_ptr<Bone>>& GetBones() {
		return m_list_bone;
	}
	List<Value_ptr<Bone>>& GetIKBones() {
		return m_list_IKBone;
	}
	void SetIKBone() {
		m_list_IKBone.Clear();
		for (auto itr : m_list_bone) {
			if ((itr)->isIK) {
				m_list_IKBone.Add(itr);
			}
		}
	}
	void SetAddBone() {
		m_list_addBone.Clear();
		for (auto itr :m_list_bone) {
			if ((itr)->addBoneIndex >= 0) {
				m_list_addBone.push_back(itr);
			}
		}
	}
	inline Value_ptr<Bone> searchBoneByName(const std::string& arg_boneName) {
		for (auto itr:m_list_bone) {
			if ((itr)->boneName == arg_boneName) {
				return (itr);
			}
		}
		return nullptr;
	}
	inline Value_ptr<Bone> searchBoneByIndex(const std::int32_t arg_index) {
		if (arg_index < 0 || arg_index>m_list_bone.GetSize() - 1) {
			return nullptr;
		}
		else
		{
			return m_list_bone.at(arg_index);
		}
	}
};
}
}

#endif // !IDRAWOBJ_H
