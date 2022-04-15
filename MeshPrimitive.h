#ifndef MESHPRIMITIVE_H
#define MESHPRIMITIVE_H
#include"../../ButiMath/ButiMath.h"
#include"Vertex.h"
#include"RenderingStatus.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include<utility>
namespace ButiEngine {

namespace ButiRendering {


class VertexLayout
{
public:
	/** VertexLayout ���쐬���܂��B */
	static Value_ptr<VertexLayout> create();

	/** ���C�A�E�g�ɐV�����v�f��ǉ����܂��B */
	void addElement(std::int32_t streamIndex, VertexElementType type, VertexElementUsage usage, std::int32_t usageIndex, VertexInputRate rate = VertexInputRate::Vertex);

	void addElement(const VertexElement& element);

	//std::int32_t elementCount() const { return m_vertexElements.size(); }

	//const VertexElement& element() const { return m_vertexElements[i]; }

	// TODO: internal
	const List<VertexElement>& vertexElements() const { return m_vertexElements; }

protected:
	//void onDispose(bool explicitDisposing) ;
	//void onManagerFinalizing()  { dispose(); }
	//void onChangeDevice(detail::IGraphicsDevice* device) ;


	VertexLayout();
	virtual ~VertexLayout();

	/** @copydoc create() */
	void init();

	/** @copydoc create(const VertexElement*, std::int32_t) */
	void init(const VertexElement* elements, std::int32_t count);

private:
	//detail::IVertexDeclaration* resolveRHIObject(GraphicsContext* context, bool* outModified);

	//detail::GraphicsManager* m_manager;
	//Ref<detail::IVertexDeclaration> m_deviceObj;
	List<VertexElement> m_vertexElements;
	bool m_modified;

	//friend class detail::GraphicsResourceInternal;
};


class IndexBuffer
{
public:
	//static Value_ptr<IndexBuffer> create(std::int32_t indexCount, IndexBufferFormat format, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

	//static Value_ptr<IndexBuffer> create(std::int32_t indexCount, IndexBufferFormat format, const void* initialData, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

	/** �C���f�b�N�X�̐����擾���܂��B */
	std::int32_t size() const;

	/** �C���f�b�N�X�o�b�t�@�̃o�C�g�T�C�Y���擾���܂��B */
	std::int32_t bytesSize() const;

	/** �C���f�b�N�X�o�b�t�@�̗e�ʂ��m�ۂ��܂��B */
	void reserve(std::int32_t indexCount);

	/** �C���f�b�N�X�o�b�t�@�̃T�C�Y��ύX���܂��B */
	void resize(std::int32_t indexCount);

	/** �C���f�b�N�X�o�b�t�@���ێ�����f�[�^�ɃA�N�Z�X���܂��B���̃o�b�t�@�����̕`��Ɏg�p�����Ƃ��A�����I�� unmap ����܂��B */
	void* map(MapMode mode);

	void unmap() {}	// TODO: ���ړ]��

	/** �C���f�b�N�X�o�b�t�@���N���A���܂��B */
	void clear();

	/** �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g��ύX���܂��B */
	void setFormat(IndexBufferFormat format);

	/** �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g���擾���܂��B */
	IndexBufferFormat format() const { return m_format; }

	/** �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g���擾���܂��B */
	std::int32_t stride() const;

	/** �C���f�b�N�X�̒l��ݒ肵�܂��B */
	void setIndex(std::int32_t index, std::int32_t vertexIndex);

	/** �C���f�b�N�X�̒l���擾���܂��B */
	std::int32_t index(std::int32_t index);

	/** ���\�[�X�̎g�p���@��ύX���܂��B(default: Static) */
	//void setResourceUsage(GraphicsResourceUsage usage);

	/** ���\�[�X�̊Ǘ����@��ύX���܂��B(default: Managed) */
	//void setResourcePool(GraphicsResourcePool pool);

protected:
	void onDispose(bool explicitDisposing);
	void onManagerFinalizing() {
		//dispose(); 
	}
	//void onChangeDevice(detail::IGraphicsDevice* device) ;

	IndexBuffer();
	virtual ~IndexBuffer();

	/** @copydoc create(std::int32_t, IndexBufferFormat, GraphicsResourceUsage) */
	//void init(std::int32_t indexCount, IndexBufferFormat format, GraphicsResourceUsage usage);

	/** @copydoc create(std::int32_t, IndexBufferFormat, const void*, GraphicsResourceUsage) */
	//void init(std::int32_t indexCount, IndexBufferFormat format, const void* initialData, GraphicsResourceUsage usage);

	//void init(std::int32_t indexCount, GraphicsResourceUsage usage);

private:
	std::int32_t getIndexStride() const;
	//detail::RHIResource* resolveRHIObject(GraphicsContext* context, bool* outModified);
	//bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

	static std::int32_t getIndexStride(IndexBufferFormat format) { return (format == IndexBufferFormat::UInt16) ? 2 : 4; }
	static std::int32_t getIndexBufferSize(IndexBufferFormat format, std::int32_t indexCount) { return getIndexStride(format) * indexCount; }
	//static bool shortLifeBuffer(GraphicsResourceUsage usage, GraphicsResourcePool pool) { return usage == GraphicsResourceUsage::Static && pool == GraphicsResourcePool::None; }

	//detail::GraphicsManager* m_manager;
	//Ref<detail::RHIResource> m_rhiObject;
	IndexBufferFormat m_format;
	//GraphicsResourceUsage m_usage;
	//GraphicsResourcePool m_pool;
	std::int32_t m_primaryIndexCount;
	//std::vector<byte_t> m_buffer;
	//void* m_rhiMappedBuffer;
	void* m_mappedBuffer;
	bool m_initialUpdate;
	bool m_modified;

	//friend class detail::GraphicsResourceInternal;
};
class VertexBuffer
{
public:
	/**
	 * ���_�o�b�t�@���쐬���܂��B
	 * @param[in]   bufferSize  : ���_�o�b�t�@�̃T�C�Y (�o�C�g�P��)
	 * @param[in]   usage       : ���\�[�X�̎g�p���@
	 */
	 //static Value_ptr<VertexBuffer> create(size_t bufferSize, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

	 /**
	  * ���_�o�b�t�@���쐬���܂��B
	  * @param[in]   bufferSize  : ���_�o�b�t�@�̃T�C�Y (�o�C�g�P��)
	  * @param[in]   initialData : �����l�Ƃ��ď������ޒ��_�f�[�^
	  * @param[in]   usage       : ���\�[�X�̎g�p���@
	  */
	  //static Value_ptr<VertexBuffer> create(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

	  /** ���_�o�b�t�@�̃o�C�g�T�C�Y���擾���܂��B */
	std::int32_t size() const;

	/** ���_�o�b�t�@�̗e�ʂ��m�ۂ��܂��B */
	void reserve(std::int32_t size);

	/** ���_�o�b�t�@�̃T�C�Y��ύX���܂��B */
	void resize(std::int32_t size);

	const void* data() const;

	//   /** ���_�o�b�t�@���ێ�����f�[�^�ɃA�N�Z�X���܂��B���̃o�b�t�@�����̕`��Ɏg�p�����Ƃ��A�����I�� unmap ����܂��B  */
	//   void* map(MapMode mode);

	   //void unmap() {}	// TODO: ���ړ]��

	void* writableData(uint64_t lockOffset = 0, uint64_t lockSize = 0);

	/** ���_�o�b�t�@���N���A���܂��B */
	void clear();

	/** ���\�[�X�̎g�p���@��ύX���܂��B(default: Static) */
	//void setResourceUsage(GraphicsResourceUsage usage);

	/** ���\�[�X�̊Ǘ����@��ύX���܂��B(default: Managed) */
	//void setResourcePool(GraphicsResourcePool pool);

	//detail::DescriptorResourceType descriptorResourceType() const override { return detail::DescriptorResourceType_Buffer; }

	//Ref<detail::RHIResource> m_rhiObject;
protected:
	void onDispose(bool explicitDisposing);
	void onManagerFinalizing() { /*dispose(); */ }
	//void onChangeDevice(detail::IGraphicsDevice* device) ;


	VertexBuffer();
	virtual ~VertexBuffer();

	/** @copydoc create(size_t, GraphicsResourceUsage) */
	//void init(size_t bufferSize, GraphicsResourceUsage usage);

	/** @copydoc create(size_t, const void*, GraphicsResourceUsage) */
	//void init(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage);

private:
	//detail::RHIResource* resolveRHIObject(GraphicsContext* context, bool* outModified);
	//bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }

	//detail::GraphicsManager* m_manager;
	//GraphicsResourceUsage m_usage;
	//GraphicsResourcePool m_pool;
	size_t m_primarySize;

	//std::vector<byte_t> m_buffer;
	//void* m_rhiMappedBuffer;
	void* m_mappedBuffer;
	bool m_initialUpdate;
	bool m_modified;

	uint64_t m_dirtyOffset;
	uint64_t m_dirtySize;

	//friend class detail::GraphicsResourceInternal;
};


namespace MeshDetail {
class MeshManager;
class MeshImporter;
class MorphInstance;
}

enum class InterleavedVertexGroup
{
	Main,
	Skinning,
	AdditionalUV,
	Undefined,
};


struct BoxSurface {
	float up, down, left, right, front, back;
};
struct BoxEightCorner {
	BoxEightCorner() {}
	Vector3 up_left_front;
	Vector3 up_right_front;
	Vector3 up_left_back;
	Vector3 up_right_back;
	Vector3 down_left_front;
	Vector3 down_right_front;
	Vector3 down_left_back;
	Vector3 down_right_back;
};
class MeshPrimitiveBase:public enable_value_from_this<MeshPrimitiveBase> {
public:
	BoxEightCorner eightCorner = BoxEightCorner();
	MeshPrimitiveBase() {}
	virtual ~MeshPrimitiveBase() {
	}
	virtual bool IsHitRay(const Line& arg_line, Vector3* arg_p_pos, Vector3* arg_p_normal) = 0;
	virtual std::uint32_t GetVertexSize()const = 0;
	virtual std::uint32_t GetVertexCount()const = 0;
	virtual std::uint32_t GetIndexCount()const=0;
	virtual const void* GetVertexData()const = 0;
	virtual const std::uint32_t* GetIndexData()const = 0;
	virtual std::uint32_t GetVertexType()const = 0;
	virtual void SetIndex(const std::vector<std::uint32_t>& arg_vec_index) = 0;
	virtual std::vector<std::uint32_t>& GetIndex() = 0;
	inline void SetBoxSurfaces(const BoxSurface& arg_boxSurface) {
		eightCorner.up_left_front = Vector3(arg_boxSurface.left, arg_boxSurface.up, arg_boxSurface.front);
		eightCorner.up_right_front = Vector3(arg_boxSurface.right, arg_boxSurface.up, arg_boxSurface.front);
		eightCorner.up_left_back = Vector3(arg_boxSurface.left, arg_boxSurface.up, arg_boxSurface.back);
		eightCorner.up_right_back = Vector3(arg_boxSurface.right, arg_boxSurface.up, arg_boxSurface.back);
		eightCorner.down_left_front = Vector3(arg_boxSurface.left, arg_boxSurface.down, arg_boxSurface.front);
		eightCorner.down_right_front = Vector3(arg_boxSurface.right, arg_boxSurface.down, arg_boxSurface.front);
		eightCorner.down_left_back = Vector3(arg_boxSurface.left, arg_boxSurface.down, arg_boxSurface.back);
		eightCorner.down_right_back = Vector3(arg_boxSurface.right, arg_boxSurface.down, arg_boxSurface.back);
	}
	template<typename T>
	inline Value_ptr<T> GetThis() {
		auto Ptr = dynamic_value_ptr_cast<T>(value_from_this ());
		if (Ptr) {
			return Ptr;
		}
		else {
			assert(0 && "�s�\�ȃL���X�g�ł�");
		}
		return nullptr;

	}
	inline const BoxEightCorner& GetBoxEightCorner()const {
		return eightCorner;
	}
};
template<typename T>
class MeshPrimitive : public MeshPrimitiveBase {
public:
	void Clear() {
		vertices.clear();
		indices.clear();
	}
	std::vector<T> vertices;
	std::vector<std::uint32_t> indices;
	std::uint32_t GetVertexSize()const override {
		return sizeof(T);
	}
	const void* GetVertexData()const override {
		return vertices.data();
	}
	const std::uint32_t* GetIndexData()const override{
		return indices.data();
	}
	std::uint32_t GetVertexCount()const override {
		return vertices.size();
	}
	std::uint32_t GetIndexCount()const override {
		return indices.size();
	}
	std::uint32_t GetVertexType()const override {
		std::uint32_t output=0;
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV, T>)
		{
			output |= Vertex::VertexFlag::UV;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::Normal, T>)
		{
			output |= Vertex::VertexFlag::Normal;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::Tangent, T>)
		{
			output |= Vertex::VertexFlag::Tangent;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::Color, T>)
		{
			output |= Vertex::VertexFlag::Color;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::SingleBone, T>)
		{
			output |= Vertex::VertexFlag::SingleBone;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::DoubleBone, T>)
		{
			output |= Vertex::VertexFlag::DouleBone;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::QuadBone, T>)
		{
			output |= Vertex::VertexFlag::QuadBone;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::PMX, T>)
		{
			output |= Vertex::VertexFlag::PMX;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV_ex1, T>)
		{
			output |= Vertex::VertexFlag::UV_ex1;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV_ex2, T>)
		{
			output |= Vertex::VertexFlag::UV_ex2;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV_ex3, T>)
		{
			output |= Vertex::VertexFlag::UV_ex3;
		}
		if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV_ex4, T>)
		{
			output |= Vertex::VertexFlag::UV_ex4;
		}
		return output;
	}
	void SetIndex(const std::vector<std::uint32_t>& arg_vec_index)override {
		indices = arg_vec_index;
	}
	std::vector<std::uint32_t>& GetIndex()override {
		return indices;
	}
	bool IsHitRay(const Line& arg_line, Vector3* arg_p_pos, Vector3* arg_p_normal) override {
		assert(0 && "���ݎg�p��~��");
		for (auto itr = indices.begin(),endItr= indices.end(); itr != endItr; itr = itr + 3) {

			/*if (Geometry::SurfaceHit::IsHitLinePolygon(arg_line, *arg_p_pos, vertices.at(*itr).position, vertices.at(*(itr + 1)).position, vertices.at(*(itr + 2)).position)) {

				*arg_p_normal = (vertices.at(*(itr + 1)).position - vertices.at(*itr).position).Cross(vertices.at(*(itr + 2)).position - vertices.at(*itr).position).GetNormalize();

				return true;
			}*/
		}
		return false;
	}
};


}
}
#endif