#include "Shader_Base.hpp"

vector				g_vLightDir; // 방향성광원의 방향벡터를 얻어오겠다.

vector				g_vLightDiffuse; // 광원의 난반사 색상을 얻어오자.
vector				g_vLightAmbient;
vector				g_vLightSpecular;

vector				g_vTerrainDiffuse; // 지형객체의 난반사 색상을 얻어온다.
vector				g_vTerrainAmbient;
vector				g_vTerrainSpecular;

vector				g_vWorldCamPos;

texture				g_NormalTexture1;
texture				g_NormalTexture2;
texture				g_NormalTexture3;
texture				g_NormalTexture4;

texture				g_TileTexture1;
texture				g_TileTexture2;
texture				g_TileTexture3;

texture				g_ColorHeightTexture;

sampler NormalSampler1 = sampler_state
{
	texture = g_NormalTexture1;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
sampler NormalSampler2 = sampler_state
{
	texture = g_NormalTexture2;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
sampler NormalSampler3 = sampler_state
{
	texture = g_NormalTexture3;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
sampler NormalSampler4 = sampler_state
{
	texture = g_NormalTexture4;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
sampler Tile1Sampler = sampler_state
{
	texture = g_TileTexture1;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
sampler Tile2Sampler = sampler_state
{
	texture = g_TileTexture2;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
sampler Tile3Sampler = sampler_state
{
	texture = g_TileTexture3;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
sampler ColorHeightSampler = sampler_state
{
	texture = g_ColorHeightTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct VS_OUT
{
	vector			vPosition : POSITION;
	float2			vTexUV : TEXCOORD0;
	float3			vViewDir : TEXCOORD1;
	float3			T	: TEXCOORD2;
	float3			B	: TEXCOORD3;
	float3			N	: TEXCOORD4;
	float			fWorldCamDist : TEXCOORD5;
	
};

VS_OUT VS_MAIN(vector vPosition : POSITION
			   , float3 vNormal : NORMAL
			   , float3 vTangent : TANGENT
			   , float3 vBinormal : BINORMAL
			   , float2 vTexUV : TEXCOORD0)
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul( float4(vPosition.xyz, 1.f), g_matWorld );
	Out.vPosition = mul( Out.vPosition, g_matView );
	Out.vPosition = mul( Out.vPosition, g_matProj );

	Out.vTexUV = vTexUV;	

	float4 vWorldPos = mul(float4(vPosition.xyz, 1.f), g_matWorld);
	Out.vViewDir = normalize(vWorldPos.xyz - g_vWorldCamPos.xyz);
	Out.fWorldCamDist = distance(vWorldPos, g_vWorldCamPos);
	
	//Out.vViewDir = normalize(g_vLightDir.xyz);

	float3 worldNormal = mul(vNormal, (float3x3)g_matWorld);
	Out.N = normalize(worldNormal);

	
	float3 Tangent = cross(float3(0.f, 1.f, 0.f), vNormal);
	float3 worldTangent = mul(Tangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);


	float3 Binormal = cross(Tangent, vNormal);
	float3 worldBinormal = mul(vBinormal, (float3x3)g_matWorld);
	Out.B = normalize(worldBinormal);


	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
	float3			vViewDir : TEXCOORD1;
	float3			T	: TEXCOORD2;
	float3			B	: TEXCOORD3;
	float3			N	: TEXCOORD4;
	float			fWorldCamDist : TEXCOORD5;

};
struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In) : COLOR
{
	PS_OUT			Out = (PS_OUT)0;

	vector vSplatingColor = tex2D(ColorHeightSampler, In.vTexUV);

	float3 tangentNormal;
	vector vResultColor;

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);

	if(vSplatingColor.x < 0.5f
		&& vSplatingColor.y < 0.5f
		&& vSplatingColor.z < 0.5f)
	{
		tangentNormal = tex2D(NormalSampler4, In.vTexUV * 10.f).xyz;
		vResultColor = tex2D(Tile3Sampler, In.vTexUV * 10.f);	
	}
	else
	{
		if(vSplatingColor.x >= 0.5f)
		{
			tangentNormal = tex2D(NormalSampler1, In.vTexUV * 20.f).xyz;
			vResultColor = tex2D(BaseSampler, In.vTexUV * 20.f);
		}
		else if(vSplatingColor.y >= 0.5f)
		{
			tangentNormal = tex2D(NormalSampler2, In.vTexUV * 20.f).xyz;
			vResultColor = tex2D(Tile1Sampler, In.vTexUV * 20.f);
		}
		else if(vSplatingColor.z >= 0.5f)
		{
			tangentNormal = tex2D(NormalSampler3, In.vTexUV * 20.f).xyz;
			vResultColor = tex2D(Tile2Sampler, In.vTexUV * 20.f);
		}
	}

	
	//else/*(vSplatingColor.w >= 0.5f)*/
	//{
	//	tangentNormal = tex2D(NormalSampler4, In.vTexUV * 10.f).xyz;
	//	vResultColor = tex2D(Tile3Sampler, In.vTexUV * 10.f);		
	//}
	tangentNormal = normalize(tangentNormal * 2 - 1);

	vector worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	vector lightDir = normalize(g_vLightDir);
	vector diffuse = max(dot(worldNormal, -lightDir), 0.f);
	
	diffuse = g_vLightDiffuse * g_vTerrainDiffuse * vResultColor * diffuse;
	vector specular = 0;

	if(diffuse.x > 0.f)
	{
		vector reflection = reflect(lightDir, worldNormal);
		vector viewDir = normalize(vector(In.vViewDir, 0.f));

		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.0f);

		specular = g_vLightSpecular * g_vTerrainSpecular * specular;
	}

	vector ambient = vector(0.1f, 0.1f, 0.1f, 1.f);

	Out.vColor = ambient + diffuse + (specular * 0.5f);

	//if(In.fWorldCamDist > 15)
	//	Out.vColor.xyz -= 0.1f;

	return Out;
}

technique Default
{
	pass Default_Terrain
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};