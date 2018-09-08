
//#include "Directional_AmbientLight.hlsli"
#include "UnpackGBuffer.hlsli"
//#include "CalcDirectionalLight.hlsli"
#include "CalcDirectionalShadow.hlsli"

Texture2D<float> AOTex : register(t4);

SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

//directional
cbuffer Dir_AmbLightConstants : register(b2) {
	float4 DirToLight:packoffset(c0);
	float4 DirLightColor:packoffset(c1);
	float4 DirLightPower:packoffset(c2);

	float4 AmbientDown : packoffset(c3);
	float4 AmbientRange : packoffset(c4);
	float4 AmbientUp : packoffset(c5);
	float4 AmbientColor : packoffset(c6);
}
cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float3 gSpecIntensity : packoffset(c1.y);
}

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float3 specIntensity;
};

//directional
float3 CalcDirectional(float3 position, Material material) {
	//퐁 디퓨즈
	float4 dirToLight = normalize(DirToLight);
	float NDotL = dot(dirToLight.xyz, material.normal);
	float3 finalColor = DirLightColor.rgb * saturate(NDotL);

	//블린 스펙큘러
	float3 ToEye = EyePosition.xyz - position;
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + DirToLight.xyz);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += DirLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;

	return finalColor * material.diffuseColor.rgb;
}
//directional
//ambient
float3 CalcAmbient(float3 normal, float3 color) {
	//[-1, 1]에서 [0, 1]으로 변환
	float up = normal.y * 0.5 + 0.5;

	
	//ambient값 계산
	float3 Ambient = AmbientDown.xyz + up * AmbientUp.xyz;

	//색상 값에 엠비언트 값 적용
	return Ambient * color;
}
//ambient


float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{

	
	SURFACE_DATA gbd = UnpackGBuffer_Tex(input.uv);

	if (gbd.depth > 0.99999 /*|| false == all(gbd.Normal)*/) {
		return float4(gbd.Color.xyz, 1.0);
	}

	//데이터를 재질 구조체로 변환
	Material mat;
	mat.normal = gbd.Normal.xyz;
	mat.diffuseColor.xyz = gbd.Color;
	mat.diffuseColor.w = 1.0f;//완전 불투명

	mat.specExp = g_SpecPowerRange.x + g_SpecPowerRange.y * gbd.SpecPow;
	mat.specIntensity = gbd.SpecInt;

	//월드 위치 복원
	float2 cpPos = input.uv * float2(2.0, -2.0) - float2(1.0, -1.0);
	float3 positionW = CalcWorldPos(cpPos, gbd.LinearDepth);
	float ao = AOTex.Sample(gssCLAMP_LINEAR, input.uv);

	//엠비언트 및 디렉셔널 라이트 비중 계산
	float4 finalColor;
	finalColor.xyz = CalcAmbient(mat.normal, mat.diffuseColor.xyz) * ao;
	finalColor.xyz += CalcDirectional(positionW, mat);
	//finalColor.xyz = mat.normal;

	float3 dir = float3(0, 0, 0) - DirToLight;
	float shadowvalue = staticShaderPCF(positionW, dot(mat.normal, dir));
	if(shadowvalue == 1)	shadowvalue = ShaderPCF(positionW, dot(mat.normal, dir));
	finalColor.rgb *= shadowvalue;
		
	finalColor.a = 1.0f;
	return finalColor;
}