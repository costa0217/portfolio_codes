#include "PackGbuffer.hlsli"

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float3 specIntensity;
};
cbuffer gBigWaterInfo : register(b0) {
	uint flag : packoffset(c0);
	float height : packoffset(c0.y);
}
cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float gSpecIntensity : packoffset(c1.y);
}

cbuffer cbPsCameraDynamic : register(b12) {
	float4x4 ViewInv : packoffset(c0);
	float4 EyePosition : packoffset(c4);
}
cbuffer gRimInfo : register(b6) {
	float4 gRimColor : packoffset(c0);
}

//texture
Texture2D    gtxtDefault : register(t0);
Texture2D    gtxtSpec : register(t1);
Texture2D	gtxtCP : register(t2);
Texture2D	gtxtNormal : register(t3);

SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);
//texture

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangentW : TANGENT;
	float3 binormalW : BINORMAL;
};

// (������) �� �����Ϳ� ���� ��� �Լ��Դϴ�.
PS_GBUFFER_OUT main(PixelShaderInput input)
{
	if (flag == 1) {
		if (height >= input.positionW.y) discard;
	}
	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;
	float4 cCPColor = gtxtCP.Sample(gssWRAP_LINEAR, input.uv);
	clip(cCPColor.g < 0.05f ? -1 : 1);

	float4	vLook = float4(normalize(EyePosition.xyz - input.positionW.xyz), 1.f);
	float4	fRimLight = smoothstep(0.94f - (1.f - gRimColor.y) * 0.03f , 1.f, 1 - max(0, dot(input.normalW, vLook)));
	//fRimLight.z = 0.f;

	fRimLight *= gRimColor;
	float4 cColor = gtxtDefault.Sample(gssWRAP_LINEAR, input.uv) * gMaterialColor + (fRimLight * 0.5f);
	float4 cSpec = gtxtSpec.Sample(gssWRAP_LINEAR, input.uv);

	float3 T = normalize(input.tangentW);
	float3 B = normalize(input.binormalW);
	float3 N = normalize(input.normalW);

	float3x3 TBN = float3x3(T, B, N);
	float3 normal = gtxtNormal.Sample(gssCLAMP_POINT, input.uv).xyz;
	normal = 2.0f * normal - 1.0f;
	float3 normalW = mul(normal, TBN);

	return (PackGBuffer(cColor.xyz, normalize(normalW), cSpec.xyz, gSpecExp));

}
//float4 main(PixelShaderInput input) : SV_TARGET
//{
//
//return gMaterialColor;
//
//}
