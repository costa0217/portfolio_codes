#include "UnpackGBuffer.hlsli"
//#include "CalcSpotLight.hlsli"

#define MAX_SPOT_LIGHT 800

struct SpotLight {
	float3 SpotLightPos;
	float SpotLightRangeRcp;

	float3 SpotLightDir;
	float SpotCosOuterCone;

	float3 SpotLightColor;
	float SpotCosConeAttRcp;
};

cbuffer cbSpotLightDomain : register(b2) {
	SpotLight gSpotLight[MAX_SPOT_LIGHT];
}

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};


float3 CalcSpot(float3 position, Material material, uint instanceID) {

	float3 ToLight = gSpotLight[instanceID].SpotLightPos - position;
	float3 ToEye = EyePosition.xyz - position;
	float DistToLight = length(ToLight);

	//퐁 디퓨즈
	ToLight /= DistToLight; //정규화
	float NDotL = saturate(dot(ToLight, material.normal));
	float3 finalColor = gSpotLight[instanceID].SpotLightColor.rgb * NDotL;

	//블린 스펙큘러
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += gSpotLight[instanceID].SpotLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;
	//return finalColor;

	float cosAng = dot(gSpotLight[instanceID].SpotLightDir, ToLight);
	//원뿔 감쇄
	float conAtt = saturate((cosAng - gSpotLight[instanceID].SpotCosOuterCone) * gSpotLight[instanceID].SpotCosConeAttRcp);
	conAtt *= conAtt;

	//감쇄
	float DistToLightNorm = 1.0 - saturate(DistToLight * gSpotLight[instanceID].SpotLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= material.diffuseColor.xyz * Attn * conAtt;

	return finalColor;
	//return finalColor;

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
	finalColor.xyz = CalcSpot(positionW, mat, input.instanceID);

	finalColor.w = 1.0f;

	return finalColor;
}