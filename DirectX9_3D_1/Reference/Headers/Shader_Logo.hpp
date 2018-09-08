#include "Shader_Setter.hpp"
// �������� (Constant Table)


// VertexShading�� �Է°����� ���� ������ ����
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

VS_OUT/*����*/ VS_MAIN(VS_IN In/*����*/)
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
	vector		vNormal	: COLOR1;	// COLOR0 : 1������Ÿ�ٿ� �׸� ����
	vector		vDepth	: COLOR2;	// COLOR1 : 2������Ÿ�ٿ� �׸� ����
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor		= tex2D(BaseSampler, In.vUV);
	Out.vNormal	= vector(normalize(In.vNormal), 0.f);
	Out.vDepth		= 0.f;

	return Out;
}

// technique : ��ġ�� �������ο����� �ٸ� ���̵��� �� �� �ֵ��� �Ѵ�.
technique Default
{
	// pass : ����� ĸ��ȭ
	pass Render_Logo
	{		
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}
};