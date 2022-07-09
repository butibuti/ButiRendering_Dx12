#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H
#include"ButiUtil/ButiUtil/ObjectFactory.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"ResourceInterface.h"
namespace ButiEngine {
namespace ButiRendering {
template<typename T>class CBuffer;
class IDrawObject;
class IDrawCommand :public IObject {
public:
	virtual void Execute() = 0;
	virtual void Execute_afterRendering()=0;
	virtual Value_weak_ptr<IResource_Mesh> GetMesh()const = 0;
	virtual Value_weak_ptr<IResource_Material> GetMaterial()const = 0;
	virtual	Value_ptr < CBuffer<Matrices>> GetMatrixCBuffer()const = 0;
	virtual	std::int32_t GetMeshDrawOffset() = 0;
	virtual	std::int32_t GetMeshDrawStep() = 0;
	virtual Value_weak_ptr<IDrawObject> GetDrawObject()const = 0;
	virtual bool IsAlpha()const = 0;
};
}
}

#endif // !DRAWCOMMAND_H
