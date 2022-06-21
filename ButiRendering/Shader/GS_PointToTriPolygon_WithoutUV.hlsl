#include"DefaultShader.hlsli"


static const float3 array_Offset[3] = {
    float3(0.0f,0.5f,0.0f),
    float3(0.866025448f,-0.49999994f,0.0f),
    float3(-0.866025448f,-0.49999994f,0.0f)
};



[maxvertexcount(6)]
void GSMain(
    point  Vertex_UV_Normal_Color  input[1],
    inout TriangleStream<Pixel_Normal_Color> stream)
{

    float4 position = input[0].position;

    float4x4 vp = mul(viewMatrix, projectionMatrix);


    Pixel_Normal_Color pixel;

    pixel.normal = input[0].normal;
    pixel.color = float4(input[0].normal, 1);
    float3x3 rotation = QuaternionToMatrix(input[0].color);
    float scale = 0.1f * input[0].uv.x;



    pixel.position = position + float4(mul(array_Offset[0] * scale, rotation), 0);
    pixel.position = mul(pixel.position, vp);
    pixel.normal = mul(float3(0.0,0.0,-1.0), rotation);
    stream.Append(pixel);
    pixel.position = position + float4(mul(array_Offset[1] * scale, rotation), 0);
    pixel.position = mul(pixel.position, vp);
    stream.Append(pixel);
    pixel.position = position + float4(mul(array_Offset[2] * scale, rotation), 0);
    pixel.position = mul(pixel.position, vp);
    stream.Append(pixel);

    stream.RestartStrip();


    pixel.position = position + float4(mul(array_Offset[2] * scale, rotation), 0);
    pixel.position = mul(pixel.position, vp);
    pixel.normal = mul(float3(0.0, 0.0, 1.0), rotation);
    stream.Append(pixel);
    pixel.position = position + float4(mul(array_Offset[1] * scale, rotation), 0);
    pixel.position = mul(pixel.position, vp);
    stream.Append(pixel);
    pixel.position = position + float4(mul(array_Offset[0] * scale, rotation), 0);
    pixel.position = mul(pixel.position, vp);
    stream.Append(pixel);

    stream.RestartStrip();
}