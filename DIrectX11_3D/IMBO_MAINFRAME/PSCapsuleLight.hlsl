#include "UnpackGBuffer.hlsli"
//#include "CalcCapsuleLight.hlsli"
#define MAX_CAPSULE_LIGHT 800

struct CapsuleLight {
	float3 CapsuleLightPos;
	float CapsuleLightRangeRcp;

	float3 CapsuleLightDir;
	float CapsuleLightLen;

	float3 CapsuleLightColor;
	float CapsuleLightIntensity;
};

cbuffer cbCapsuleLightDomain : register(b2) {
	CapsuleLight gCapsuleLight[MAX_CAPSULE_LIGHT];
}

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

float3 CalcCapsule(float3 position, Material material, uint instanceID) {
	float3 ToEye = EyePosition.xyz - position;

	//�ȼ��� ĸ�� ���� ������ �ּҰŸ� ���ϱ�
	float3 ToCapsuleStart = position - gCapsuleLight[instanceID].CapsuleLightPos;
	float DistOnLine = dot(ToCapsuleStart, gCapsuleLight[instanceID].CapsuleLightDir) / gCapsuleLight[instanceID].CapsuleLightLen;
	DistOnLine = saturate(DistOnLine) * gCapsuleLight[instanceID].CapsuleLightLen;
	float3 PointOnLine = gCapsuleLight[instanceID].CapsuleLightPos
		+ gCapsuleLight[instanceID].CapsuleLightDir * DistOnLine;
	float3 ToLight = PointOnLine - position;
	float DistToLight = length(ToLight);

	//�� ��ǻ��
	ToLight /= DistToLight; //Normalize
	float NDotL = saturate(dot(ToLight, material.normal));
	float3 finalColor = material.diffuseColor.xyz * NDotL;

	//�� ����ŧ��
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + ToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += pow(NDotH, material.specExp) * material.specIntensity;

	//����
	float DistToLightNorm = 1.0 - saturate(DistToLight * gCapsuleLight[instanceID].CapsuleLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;

	finalColor *= gCapsuleLight[instanceID].CapsuleLightColor.rgb * Attn;

	return finalColor;

}

struct DS_OUTPUT
{
	float4 Position	: SV_POSITION;
	float2 cpPos	: TEXCOORD0;
	uint instanceID : TEXCOORD1;
	// TODO: �ٸ� ������ ����/�߰��մϴ�.
};

float4 main(DS_OUTPUT input) : SV_TARGET
{
	//GBuffer ����ŷ
	float2 uv = input.Position.xy;
	SURFACE_DATA gbd = UnpackGBuffer(uv);
	//���� ����ü�� ������ ��ȯ
	Material mat;
	mat.normal = gbd.Normal;
	mat.diffuseColor.xyz = gbd.Color;
	mat.diffuseColor.w = 1.0f;
	mat.specExp = g_SpecPowerRange.x + g_SpecPowerRange.y + gbd.SpecPow;
	mat.specIntensity = gbd.SpecInt;

	//���� ��ġ ����
	float3 positionW = CalcWorldPos(input.cpPos, gbd.LinearDepth);

	//���� ���� ���
	float4 finalColor;
	finalColor.xyz = CalcCapsule(positionW, mat, input.instanceID);
	finalColor.w = 1.0f;

	return finalColor;
}