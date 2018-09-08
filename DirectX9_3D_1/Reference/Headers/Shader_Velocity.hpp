#include "Shader_Setter.hpp"


matrix		g_matWV;
matrix		g_matPWV;

struct VS_IN
{
	float4		vPos			: POSITION;
	float3		vNormal		: NORMAL;
};
struct VS_OUT
{
	float4		vPos			: POSITION;
	float3		vNormal		: TEXCOORD0;
	float4		Tex			: TEXCOORD1;
	float4		Velocity		: TEXCOORD2;
};
VS_OUT VS_VELOCITY_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	float4		vViewPos		= mul(In.vPos, g_matWV);
	float4		vPreViewPos	= mul(In.vPos, g_matPWV);
	float4		vVelocity		= vViewPos - vPreViewPos;
	float3		vViewNormal	= mul(vector(In.vNormal.xyz, 0.f), g_matWV).xyz;

	bool		bFront	= ( 0 <= dot(vViewNormal, vVelocity.xyz));
	float4		vPresentPos	= bFront ? vViewPos : vPreViewPos;

	Out.vPos	= mul(vPresentPos, g_matProj);

	// 투영공간에서 텍스쳐 공간으로 변환
	Out.Tex.x	= Out.vPos.x + Out.vPos.w;
	Out.Tex.y	= -Out.vPos.y + Out.vPos.w;
	Out.Tex.w	= 2.f * Out.vPos.w;

	// 텍스쳐 좌표로 속도를 구한다
	float4		Speed0	= mul(vPreViewPos, g_matProj);
	Speed0 /= Speed0.w;
	float4		Speed1	= mul(vViewPos, g_matProj);
	Speed1 /= Speed1.w;

	Out.Velocity		=		Speed1 - Speed0;
	Out.Velocity.x		*=		0.5f;
	Out.Velocity.y		*=		-0.5f;

	return Out;
}

struct PS_IN
{
	float3		vNormal		: TEXCOORD0;
	float4		Tex			: TEXCOORD1;
	float4		Velocity		: TEXCOORD2;
};
struct PS_OUT
{
	/*vector	vVelocity	: COLOR0;
	vector	vTex			: COLOR1;	*/

	vector	vColor		: COLOR0;
};

PS_OUT PS_VELOCITY_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	const		int		SAMPLES = 20;
	const		float	samples	= SAMPLES;
	float		fDivice = 1 / samples;

	for(int i = 0; i < SAMPLES; ++i)
	{
		float t = (float)(i + 1)*fDivice;
		Out.vColor += tex2D(BaseSampler, In.Tex/In.Tex.w + t * In.Velocity)/* * ((i+1)*fDivide)*/ ;
	}
	Out.vColor *= fDivice;
	Out.vColor.a  = 1.f;


	return Out;
}

///////////////////////////////
///////////////////////////////

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Create_ShadowMap
	{		
		VertexShader = compile vs_3_0 VS_VELOCITY_MAIN();
		PixelShader = compile ps_3_0 PS_VELOCITY_MAIN();

		Zwriteenable = false;
		AlphaBlendenable = true;
		Srcblend = srcalpha;
		Destblend = invsrcalpha;

	}
};