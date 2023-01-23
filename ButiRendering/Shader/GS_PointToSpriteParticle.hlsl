#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Use_ObjectInformation(b1)
static const float3 array_Offset[4]={
    float3(-1.0f,1.0f,0.0),
    float3(1.0f,1.0f,0.0),
    float3(-1.0f,-1.0f,0.0),
    float3(1.0f,-1.0f,0.0),
};
static const float2 aspect = float2 (0.9f, 1.6f);

[maxvertexcount(6)]
void GSMain(
    point  Vertex_UV_Normal_Color  input[1],
    inout TriangleStream<Pixel_UV_Color> stream)
{

    float4 position = mul(input[0].position, mul(viewMatrix, projectionMatrix));
    Pixel_UV_Color pixel;
    pixel.color = float4(input[0].normal, 1);
    float3x3 rotation = RollZ(input[0].color.w);
    float scale = 0.1f*input[0].color.z;


    pixel.position = position + float4(mul(array_Offset[0] * scale, rotation).xy*aspect,0, 0);
    pixel.uv.x = input[0].uv.x;
    pixel.uv.y = input[0].uv.y;
    stream.Append(pixel);
    pixel.position = position + float4(mul(array_Offset[1] * scale, rotation).xy * aspect, 0, 0);
    pixel.uv.x = input[0].uv.x+ lightDir.z;
    pixel.uv.y = input[0].uv.y;
    stream.Append(pixel);
    pixel.position = position + float4(mul(array_Offset[2] * scale, rotation).xy * aspect, 0, 0);
    pixel.uv.x = input[0].uv.x ;
    pixel.uv.y = input[0].uv.y + lightDir.w;
    stream.Append(pixel);
    stream.RestartStrip();

    pixel.position = position + float4(mul(array_Offset[1] * scale, rotation).xy * aspect, 0, 0);
    pixel.uv.x = input[0].uv.x + lightDir.z;
    pixel.uv.y = input[0].uv.y;
    stream.Append(pixel);
    pixel.position = position + float4(mul(array_Offset[3] * scale, rotation).xy * aspect, 0, 0);
    pixel.uv.x = input[0].uv.x + lightDir.z;
    pixel.uv.y = input[0].uv.y+lightDir.w;
    stream.Append(pixel);
    pixel.position = position + float4(mul(array_Offset[2] * scale, rotation).xy * aspect, 0, 0);
    pixel.uv.x = input[0].uv.x;
    pixel.uv.y = input[0].uv.y + lightDir.w;
    stream.Append(pixel);
    stream.RestartStrip();
}