#include"DefaultShader.hlsli"
static const float wDiv = 1.0 / 16;
float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
    float4 output = 0;

    for (float i = 0.0f; i <= 15.0f;i+=1.0f) {
        output += gausOffset[i].z * mainTexture.Sample(mainSampler, pixel.uv + gausOffset[i].xy  );
    }
    //output.w *= wDiv;
    return output;
}