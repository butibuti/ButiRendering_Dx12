#ifndef RENDERINGSTATUS_H
#define RENDERINGSTATUS_H
#include<cstdint>
namespace ButiEngine {
namespace ButiRendering {

enum class CullMode {
	none = 1, front = 2, back = 3
};
enum class BlendMode {
	AlphaBlend = 0, Addition = 1, Subtruction = 2, Reverse = 3, NoBlend = 4
};
enum class BillBoardMode {
	none = 0, full = 1, x = 2, y = 3, z = 4,
};
enum class DrawFixParam {
	none, removeRotation, removeDecimalPart, removeDecimalPartXY, removeDecimalPartYZ, removeDecimalPartXZ
};
enum class TopologyType {
	triangleList = 4, point = 1, line = 2, triangle = 3, pointList = 1,
};
enum class FillMode {
	solid = 3, wire = 2
};

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
