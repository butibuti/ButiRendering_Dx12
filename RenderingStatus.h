#ifndef RENDERINGSTATUS_H
#define RENDERINGSTATUS_H
#include<cstdint>
namespace ButiEngine {
namespace ButiRendering {

enum class PrimitiveTopology : std::uint8_t
{
	TriangleList,
	TriangleStrip,
	TriangleFan,
	LineList,
	LineStrip,
	PointList,
};

enum class VertexElementType : std::uint8_t
{
	Unknown,
	Float1,
	Float2,
	Float3,
	Float4,
	Ubyte4,
	Color4,
	Short2,
	Short4,
};

enum class VertexElementUsage
{
	Unknown,
	Position,
	Normal,
	Color,
	TexCoord,
	PointSize,
	Tangent,
	Binormal,
	BlendIndices,
	BlendWeight,
};

enum class VertexInputRate
{
	Vertex,
	Instance,
};

enum class MapMode
{
	Read, Write,
};

struct VertexElement
{
	uint32_t StreamIndex;
	VertexElementType Type;
	VertexElementUsage Usage;
	uint32_t UsageIndex;
	VertexInputRate rate = VertexInputRate::Vertex;
};
enum class IndexBufferFormat
{
	UInt16, UInt32,
};

struct MeshSection
{
	std::int32_t startIndex;
	std::int32_t	primitiveCount;
	std::int32_t	materialIndex;
};
struct MeshSection2
{
	std::int32_t startIndex;
	std::int32_t primitiveCount;
	std::int32_t materialIndex;

	PrimitiveTopology topology;
};

}
}
#endif // !RENDERINGSTATUS_H
