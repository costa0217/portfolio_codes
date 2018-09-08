#include "PackGbuffer.hlsli"


Texture2D	gtxtUI		: register(t0);

sampler textureSampler = sampler_state
{
	addressU = Clamp;
	addressV = Clamp;
	mipfilter = NONE;
	minfilter = LINEAR;
	magfilter = LINEAR;
};

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

struct PS_OUT {
	float4 Color : SV_TARGET0;
	//float4 Normal : SV_TARGET1;
};

PS_OUT main(VS_OUTPUT input) : SV_TARGET
{
	PS_OUT output = (PS_OUT)0;
	output.Color = gtxtUI.Sample(textureSampler, input.texCoord);
	////cBaseTexColor.a = 1.f;
	//output.Normal = float4(0.f, 0.f, 1.f, 1.f);
	return output;

	//return (PackGBuffer(cColor.xyz, normalize(normalW), cSpec.xyz, gSpecExp));

	//PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;

	//float4 cColor = gtxtUI.Sample(textureSampler, input.texCoord);

	//float3 normal = float3(0.f, 1.f, 0.f);
	//normal = 2.0f * normal - 1.0f;

	//return (PackGBuffer(cColor.xyz, normalize(normal), float3(1.f, 1.f,1.f), 1.f));
}