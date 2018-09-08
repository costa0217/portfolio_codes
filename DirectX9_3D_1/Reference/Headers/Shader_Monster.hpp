#include "Shader_Setter.hpp"

// 전역변수 (Constant Table)
vector			g_WorldCamPos;
vector			g_LightDir;

texture			g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture		= g_NormalTexture;
};

texture			g_EnvironmentTexture;
sampler EnvironmentSampler = sampler_state
{
	texture		= g_EnvironmentTexture;
};




// VertexShading의 입력값으로 사용될 정점의 정보
struct VS_IN
{
	vector		vPosition		: POSITION;
	float3			vNormal		: NORMAL;
	float3			vTangent	: TANGENT;
	float3			vBinormal	: BINORMAL;
	float2			vUV			: TEXCOORD0;
};

struct VS_OUT
{
	vector	vPosition			: POSITION;
	float2		vUV				: TEXCOORD0;
	vector	vViewPos		: TEXCOORD1;
	//vector	vWorldNormal	: TEXCOORD2;
	float3		T					: TEXCOORD2;
	float3		B					: TEXCOORD3;
	float3		N					: TEXCOORD4;
	float3		vLook				: TEXCOORD5;
	vector	vProjPos			: TEXCOORD6;
};


////// Pass 1 ////////////////////////////////////////////////////////////////////////////////////////////////////////
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);
	matWVP			= mul(matWV, g_matProj);	

	Out.vViewPos	= mul(In.vPosition, matWV);

	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vUV			= In.vUV;
	Out.vProjPos	= Out.vPosition;

	Out.vLook		= normalize(g_WorldCamPos - mul(In.vPosition, g_matWorld)).xyz;

	//Out.vWorldNormal 	= normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.N = normalize(worldNormal);

	float3 Tangent = cross(float3(0.f, 1.f, 0.f), In.vNormal);
	float3 worldTangent = mul(Tangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);

	float3 Binormal = cross(Tangent,  In.vNormal);
	float3 worldBinormal = mul(Binormal, (float3x3)g_matWorld);
	Out.B = normalize(worldBinormal);
	return Out;
}

// COLOR, TEXCOORD
struct PS_IN 
{
	float2		vUV				: TEXCOORD0;
	vector	vViewPos		: TEXCOORD1;
	//vector	vWorldNormal	: TEXCOORD2;
	float3         T				: TEXCOORD2;
	float3         B				: TEXCOORD3;
	float3         N				: TEXCOORD4;
	float3		vLook				: TEXCOORD5;
	vector	vProjPos			: TEXCOORD6;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;	// COLOR0 : 0번렌더타겟에 그릴 색상
	vector		vNormal : COLOR1;	// COLOR0 : 1번렌더타겟에 그릴 색상
	vector		vDepth	: COLOR2;	// COLOR1 : 2번렌더타겟에 그릴 색상
	//vector		vEffect	: COLOR3;	// COLOR1 : 2번렌더타겟에 그릴 색상
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vUV);

	clip( 0.1 >= vColor.a ? -1 : 1);

	vector NormalTex = tex2D(NormalSampler, In.vUV);
	float3 tangentNormal = normalize(NormalTex.xyz * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);

	vector worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	float4		vLook		= vector(normalize(In.vLook).xyz, 0.f);
	float		fRimLight = smoothstep(1.f - 0.2f, 1.f, 1 -max(0, dot(worldNormal, vLook)));

	Out.vColor		= vColor + fRimLight;
	Out.vNormal	= worldNormal;
	Out.vDepth		= vector(In.vViewPos.z * 0.001f, In.vViewPos.z * 0.001f,  In.vViewPos.z * 0.001f, In.vProjPos.z * 0.001f);

	//Out.vEffect		= 0.f;
	return Out;
}
////// Pass 2 ////////////////////////////////////////////////////////////////////////////////////////////////////////

texture			g_NoiseTexture;
sampler NoiseSampler = sampler_state
{
	texture		= g_NoiseTexture;
};
float	a = 0.9;
float	g_fTime;


PS_OUT PS_DIEMONSTER(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vUV);

	clip( 0.1 >= vColor.a ? -1 : 1);

	vector NormalTex = tex2D(NormalSampler, In.vUV);
	float3 tangentNormal = normalize(NormalTex.xyz * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);

	vector worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	float4		vLook		= vector(normalize(In.vLook).xyz, 0.f);
	float		fRimLight = smoothstep(1.f - 0.2f, 1.f, 1 -max(0, dot(worldNormal, vLook)));

	Out.vColor		= vColor + fRimLight;
	Out.vNormal	= worldNormal;
	Out.vDepth		= vector(In.vViewPos.z * 0.001f, In.vViewPos.z * 0.001f,  In.vViewPos.z * 0.001f, In.vProjPos.z * 0.001f);

	return Out;
}


// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Render_StaticMesh
	{		
		/*Alphablendenable	= true;
		srcblend				= srcalpha;
		destblend			= invsrcalpha;*/

		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}

	pass Render_DieMonster
	{	
		CullMode				= None;
		AlphaBlendEnable	= true;
		DestBlend			= One;
		SrcBlend				= SrcAlpha;
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_DIEMONSTER();
	}
};