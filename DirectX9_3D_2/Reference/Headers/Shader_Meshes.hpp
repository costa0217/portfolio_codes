#include "Shader_Base.hpp"

vector				g_vLightDir; // 방향성광원의 방향벡터를 얻어오겠다.

vector				g_vLightDiffuse; // 광원의 난반사 색상을 얻어오자.
vector				g_vLightAmbient;
vector				g_vLightSpecular;

vector				g_vMeshDiffuse; // 지형객체의 난반사 색상을 얻어온다.
vector				g_vMeshAmbient;
vector				g_vMeshSpecular;

vector				g_vWorldCamPos;

texture				g_NormalTexture;
texture				g_SpecularTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};
sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
};


struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float3		vViewDir : TEXCOORD1;
	float3			T	: TEXCOORD2;
	float3			B	: TEXCOORD3;
	float3			N	: TEXCOORD4;
};

// VertexShading : 정점의 변환 (영역의 변환)
VS_OUT/*정점*/ VS_MAIN(vector vPosition : POSITION
					 , float3 vNormal : NORMAL0
					 , float3 vTangent : TANGENT
					 , float3 vBinormal : BINORMAL
					 , float2 vTexUV : TEXCOORD0)
{
	VS_OUT		Out = (VS_OUT)0;

	Out.vPosition = mul( float4(vPosition.xyz, 1.f), g_matWorld );
	Out.vPosition = mul( Out.vPosition, g_matView );
	Out.vPosition = mul( Out.vPosition, g_matProj );

	Out.vTexUV = vTexUV;	

	float4 vWorldPos = mul(float4(vPosition.xyz, 1.f), g_matWorld);
	Out.vViewDir = normalize(vWorldPos.xyz - g_vWorldCamPos.xyz);

	float3 worldNormal = mul(vNormal, (float3x3)g_matWorld);
	Out.N = normalize(worldNormal);


	float3 Tangent = cross(float3(0.f, 1.f, 0.f), vNormal);
	float3 worldTangent = mul(Tangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);


	float3 Binormal = cross(Tangent, vNormal);
	float3 worldBinormal = mul(Binormal, (float3x3)g_matWorld);
	Out.B = normalize(worldBinormal);



	return Out;
}

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
	float3		vViewDir : TEXCOORD1;
	float3			T	: TEXCOORD2;
	float3			B	: TEXCOORD3;
	float3			N	: TEXCOORD4;
};
struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float3 tangentNormal = tex2D(NormalSampler, In.vTexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);

	vector worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	vector lightDir = normalize(g_vLightDir);
	vector diffuse = saturate(dot(worldNormal, -lightDir));

	vector vResultColor = tex2D(BaseSampler, In.vTexUV);

	diffuse = g_vLightDiffuse * g_vMeshDiffuse * vResultColor * diffuse;

	vector specular = 0;
	if(diffuse.x > 0.f)
	{
		vector reflection = reflect(lightDir, worldNormal);
		vector viewDir = normalize(vector(In.vViewDir, 0.f));

		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.f);

		float4 specularIntesity = tex2D(SpecularSampler, In.vTexUV);
		specular *= vector(specularIntesity.rgb, 1.f);

		specular = g_vLightSpecular * g_vMeshSpecular * specular;
	}


	vector ambient = vector(0.2f, 0.2f, 0.2f, 1.f) * vResultColor;

	Out.vColor = ambient + diffuse + (specular);
	Out.vColor.a = vResultColor.a;

	return Out;
}

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Default_Mesh
	{		
		AlphaBlendenable = true;
		Srcblend = srcalpha;
		Destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};