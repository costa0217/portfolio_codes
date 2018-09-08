#include "Shader_Setter.hpp"
// 전역변수 (Constant Table)


// VertexShading의 입력값으로 사용될 정점의 정보
struct VS_IN
{
	vector		vPosition		: POSITION;
	float3			vNormal		: NORMAL;
	float2			vUV			: TEXCOORD0;
};

struct VS_OUT
{
	vector	vPosition			: POSITION;
	float2		vUV				: TEXCOORD0;
	float3		vNormal			: TEXCOORD1;
};

VS_OUT/*정점*/ VS_MAIN(VS_IN In/*정점*/)
{
	VS_OUT		Out = (VS_OUT)0;
	matrix		matWV, matWVP;

	matWV	= mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vUV			= In.vUV;

	float3 worldNormal	= mul(In.vNormal, (float3x3)g_matWorld);
	Out.vNormal			= normalize(worldNormal);

	return Out;
}

// COLOR, TEXCOORD
struct PS_IN 
{
	float2		vUV				: TEXCOORD0;
	float3		vNormal			: TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;
	vector		vNormal	: COLOR1;	// COLOR0 : 1번렌더타겟에 그릴 색상
	vector		vDepth	: COLOR2;	// COLOR1 : 2번렌더타겟에 그릴 색상
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor		= tex2D(BaseSampler, In.vUV);
	Out.vNormal	= vector(normalize(In.vNormal), 0.f);
	Out.vDepth		= 0.f;

	return Out;
}

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Render_Logo
	{		
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}
};