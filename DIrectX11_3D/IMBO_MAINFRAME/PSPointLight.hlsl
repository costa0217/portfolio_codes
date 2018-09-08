#include "UnpackGBuffer.hlsli"
//#include "CalcPointLight.hlsli"
#define MAX_POINT_LIGHT 800

struct PointLight {
	float3 PointLightPos;
	float PointLightRange;
	float3 PointLightColor;
	float PointLightIntensity;
};

cbuffer cbPointLight : register(b2) {
	PointLight gPointLight[MAX_POINT_LIGHT];
}


struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

float3 CalcPoint(float3 position, Material material, uint instanceID) {
	float3 ToLight = gPointLight[instanceID].PointLightPos - position;
	float3 ToEye = EyePosition.xyz - position;
	float DistToLight = length(ToLight);

	ToLight /= DistToLight; //Normalize
	float NDotL = saturate(dot(ToLight, material.normal));
	float3 finalColor = gPointLight[instanceID].PointLightColor.rgb * NDotL;


	//블린 스펙큘러
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += gPointLight[instanceID].PointLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;

	//감쇄
	float DistToLightNorm = 1.0 - saturate(DistToLight * gPointLight[instanceID].PointLightRange);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= material.diffuseColor.xyz * Attn;

	return finalColor;
}


struct DS_OUTPUT
{
	float4 Position	: SV_POSITION;
	float2 cpPos	: TEXCOORD0;
	uint instanceID : TEXCOORD1;
	// TODO: 다른 내용을 변경/추가합니다.
};


float4 main(DS_OUTPUT input) : SV_TARGET
{
	//GBuffer 언패킹
	float2 uv = input.Position.xy;
	SURFACE_DATA gbd = UnpackGBuffer(uv);

	//재질 구조체로 데이터 변환
	Material mat;
	mat.normal = gbd.Normal;
	mat.diffuseColor.xyz = gbd.Color;
	mat.diffuseColor.w = 1.0f;
	mat.specExp = g_SpecPowerRange.x + g_SpecPowerRange.y + gbd.SpecPow;
	mat.specIntensity = gbd.SpecInt;

	//월드 위치 복원
	float3 positionW = CalcWorldPos(input.cpPos, gbd.LinearDepth);

	//조명 분포 계산
	float4 finalColor;
	finalColor.xyz = CalcPoint(positionW, mat, input.instanceID);
	finalColor.w = 1.0f;

	return finalColor;
}