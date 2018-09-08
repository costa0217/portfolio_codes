#include "Shader_Setter.hpp"
// 전역변수 (Constant Table)
float			g_fAlpha;

texture			g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture		= g_NormalTexture;
	//minfilter		= linear;
	//magfilter		= linear;
	//mipfilter		= linear;
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
	vector	vProjPos			: TEXCOORD5;
};


// VertexShading : 정점의 변환 (영역의 변환)
VS_OUT/*정점*/ VS_MAIN(VS_IN In/*정점*/)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : 월드 * 뷰
	// matWVP : 월드 * 뷰 * 투영
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vViewPos	= mul(In.vPosition, matWV);

	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos	= Out.vPosition;
	Out.vUV			= In.vUV;

	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.N = normalize(worldNormal);

	float3 worldTangent = mul(In.vTangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);

	float3 worldBinormal = mul(In.vBinormal, (float3x3)g_matWorld);
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
	vector	vProjPos			: TEXCOORD5;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;
	vector		vNormal	: COLOR1;	// COLOR0 : 1번렌더타겟에 그릴 색상
	vector		vDepth	: COLOR2;	// COLOR1 : 2번렌더타겟에 그릴 색상
	//vector		vEffect	: COLOR3;	// COLOR1 : 2번렌더타겟에 그릴 색상
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vUV);
	//Out.vColor -= vector(0.2f, 0.2f, 0.2f, 0.f);
	float3 tangentNormal = tex2D(NormalSampler, In.vUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);

	vector worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	Out.vNormal	= worldNormal/*vector(tangentNormal, 0.f)*/;
	Out.vDepth		= vector(In.vViewPos.z * 0.001f, In.vViewPos.z * 0.001f,  In.vViewPos.z * 0.001f, In.vProjPos.z * 0.001f);
	//Out.vEffect		= 0.f;

	return Out;
}
PS_OUT PS_MAIN_AlphaBlend(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vUV);
	Out.vColor.a = g_fAlpha;
	float3 tangentNormal = tex2D(NormalSampler, In.vUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);

	vector worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	Out.vNormal	= worldNormal/*vector(tangentNormal, 0.f)*/;
	Out.vDepth		= vector(In.vViewPos.z * 0.001f, In.vViewPos.z * 0.001f,  In.vViewPos.z * 0.001f, In.vProjPos.z * 0.001f);
	//Out.vEffect		= tex2D(BaseSampler, In.vUV);
	//Out.vEffect		= 0.f;

	return Out;
}

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Render_StaticMesh
	{		
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass Render_StaticMesh_AlphaTest
	{		
		AlphaTestEnable = true;
		AlphaRef = 0x88;
		AlphaFunc = Greater;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass Render_StaticMesh_AlphaBlend
	{		
		Alphablendenable	= true;
		srcblend			= srcalpha;
		destblend			= invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_AlphaBlend();
	}
};