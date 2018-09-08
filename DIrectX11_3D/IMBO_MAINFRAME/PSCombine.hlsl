
cbuffer Dynamic : register(b0)
{
	float2 SunPos : packoffset(c0);
	uint2 pad1 : packoffset(c0.z);
	float3 RayColor : packoffset(c1.x);
}
cbuffer Static : register(b9)
{
	float InitDecay : packoffset(c0.x);
	float DistDecay : packoffset(c0.y);
	float MaxDeltaLen : packoffset(c0.z);
	uint pad2 : packoffset(c0.w);
}

Texture2D<float> IntensityTex : register(t0);
SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);


struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 UV : TEXCOORD0;
};

float4 main(VS_OUTPUT In) : SV_TARGET
{
	float rayIntensity = IntensityTex.Sample(gssWRAP_LINEAR, In.UV);

	return float4(RayColor * rayIntensity, 1.0f);
}