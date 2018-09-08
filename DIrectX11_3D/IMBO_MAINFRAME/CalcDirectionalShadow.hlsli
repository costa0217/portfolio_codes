//SamplerComparisonState gssSHADOW : register(s4);
SamplerState gssSHADOW : register(s4);
Texture2D<float> ShadowMap : register(t5);
Texture2D<float> StaticShadowMap : register(t6);
Texture2DArray<float> StaticShadowMapArray : register(t9);

cbuffer CalcDirShadowBuffer : register(b4) {
	float bias : packoffset(c0.x);
	float bias_offset : packoffset(c0.y);
	float kernelhalf : packoffset(c0.z);
	float dist_sum : packoffset(c0.w);
	matrix gmtxLightVP : packoffset(c1);
};
cbuffer CalcDirStaticShadowBuffer : register(b5) {
	matrix gmtxLightVPStatic;
};



//shadow calc
float2 texOffset(int u, int v){
	return float2(u * 1.0f / 4096, v * 1.0f / 4096);
}
float ShaderPCF(float3 position, float cosTheta) {
	float4 posShadowMap = mul(float4(position, 1.f), gmtxLightVP);
	float3 UVD = posShadowMap.xyz / posShadowMap.w;

	UVD.xy = 0.5 * UVD.xy + 0.5;
	UVD.y = 1 - UVD.y;

	float LightDepth = ShadowMap.Sample(gssSHADOW, UVD.xy).x;
	//float LightDepth = StaticShadowMapArray.Sample(gssSHADOW, float3(UVD.xy, 0)).x;
	float fCamposDepth = (UVD.z);

	float value = bias;//d
	float offset = bias_offset;//d
	float bias = value*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
	bias = clamp(bias, value - offset, value + offset);
	
	bool bStatic = false;
	float sum = 0.f;
	float fxlsize = 1 / 4096.f;
	float x, y;
	if (LightDepth == 1){
		return 1;
	}


	if (LightDepth < fCamposDepth - bias) {
		[unroll(9)]
		for (y = -kernelhalf; y <= kernelhalf; y += 1.f){
			[unroll(9)]
			for (x = -kernelhalf; x <= kernelhalf; x += 1.f){
				float2 pcfUV = UVD + float2(x * fxlsize, y * fxlsize);
				//LightDepth = StaticShadowMapArray.Sample(gssSHADOW, float3(pcfUV, 0)).x;
				float LightDepth = ShadowMap.Sample(gssSHADOW, pcfUV).x;

				if (LightDepth < fCamposDepth - bias){
					sum += dist_sum;
				}
			}
		}
	}
	return saturate(1 - sum);
}


float staticShaderPCF(float3 position, float cosTheta) {
	float4 posShadowMap = mul(float4(position, 1.f), gmtxLightVPStatic);
	float3 UVD = posShadowMap.xyz / posShadowMap.w;

	UVD.xy = 0.5 * UVD.xy + 0.5;
	UVD.y = 1 - UVD.y;

	float LightDepth = StaticShadowMap.Sample(gssSHADOW, UVD.xy).x;
	float fCamposDepth = (UVD.z);

	float value = bias;//d
	float offset = bias_offset;//d
	float bias = value*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
	bias = clamp(bias, value - offset, value + offset);

	bool bStatic = false;
	float sum = 0.f;
	float fxlsize = 1 / 4096.f;
	float x, y;
	if (LightDepth == 1) {
		return 1;
	}


	if (LightDepth < fCamposDepth - bias) {
		[unroll(9)]
		for (y = -kernelhalf; y <= kernelhalf; y += 1.f) {
			[unroll(9)]
			for (x = -kernelhalf; x <= kernelhalf; x += 1.f) {
				float2 pcfUV = UVD + float2(x * fxlsize, y * fxlsize);
				float LightDepth = bStatic == StaticShadowMap.Sample(gssSHADOW, pcfUV).x;

				if (LightDepth < fCamposDepth - bias) {
					sum += dist_sum;
				}
			}
		}
	}
	return saturate(1 - sum);
}
//shadow calc