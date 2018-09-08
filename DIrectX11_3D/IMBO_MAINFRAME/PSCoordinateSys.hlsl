#include "PackGbuffer.hlsli"

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float gSpecIntensity : packoffset(c1.y);
}

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
};

// (������) �� �����Ϳ� ���� ��� �Լ��Դϴ�.
float4 main(PixelShaderInput input) : SV_TARGET
{
	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;

	float4 cColor = cColor = gMaterialColor;
	return cColor;
	//float Depth = input.position.z / input.position.w;

	//return (PackGBuffer(cColor.xyz, float3(0.f, 1.f, 0.f), gSpecIntensity, gSpecExp));

}
//float4 main(PixelShaderInput input) : SV_TARGET
//{
//
//return gMaterialColor;
//
//}
