#include "stdafx.h"
#include"ButiRendering_Dx12/Header/ShaderVariable.h"


float ButiEngine::ButiRendering::GausParameter::GaussianDistribution(const Vector2& pos, float rho)
{
	return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
}
void ButiEngine::ButiRendering::GausParameter::CalcGaus(const std::int32_t arg_width, const std::int32_t arg_height, const Vector2& arg_dir, const float arg_deviation)
{
	auto tu = 1.0f / static_cast<float>(arg_width);
	auto tv = 1.0f / static_cast<float>(arg_height);
	gausOffset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), arg_deviation);
	auto total_weight = gausOffset[0].z;
	
	gausOffset[0].x = 0.0f;
	gausOffset[0].y = 0.0f;
	
	for (auto i = 1; i < 8; ++i)
		{
		  gausOffset[i].x = arg_dir.x * i *tu;
	      gausOffset[i].y = arg_dir.y * i *tv;
	      gausOffset[i].z = GaussianDistribution(arg_dir * float(i), arg_deviation);
	      total_weight += gausOffset[i].z * 2.0f;
	  }
	
	for (auto i = 0; i < 8; ++i)
	{
	     gausOffset[i].z /= total_weight;
	}
	//gausOffset[0].z = 1.0f;
	for (auto i = 8; i < 15; ++i)
	{
	    gausOffset[i].x = -gausOffset[i - 7].x;
	    gausOffset[i].y = -gausOffset[i - 7].y;
		gausOffset[i].z = gausOffset[i - 7].z;
	}

}
