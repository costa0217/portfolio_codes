#include "Shader_Setter.hpp"
// �������� (Constant Table)

texture				g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture		= g_NormalTexture;
	minfilter		= linear;
	magfilter		= linear;
	mipfilter		= linear;
};

// VertexShading�� �Է°����� ���� ������ ����
struct VS_IN
{
	vector		vPosition		: POSITION;
	vector		vNormal		: NORMAL;
	float2			vUV			: TEXCOORD0;
};

struct VS_OUT
{
	vector	vPosition			: POSITION;
	float2		vUV				: TEXCOORD0;
	vector	vViewPos		: TEXCOORD1;
	float3		N					: TEXCOORD2;
	vector	vProjPos			: TEXCOORD3;
	float3		T					: TEXCOORD4;
	float3		B					: TEXCOORD5;
};


// VertexShading : ������ ��ȯ (������ ��ȯ)
VS_OUT/*����*/ VS_MAIN(VS_IN In/*����*/)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : ���� * ��
	// matWVP : ���� * �� * ����
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vViewPos	= mul(In.vPosition, matWV);

	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vUV			= In.vUV;
	Out.vProjPos	= Out.vPosition;

	//Out.vWorldNormal 	= normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	float3 worldNormal = mul(In.vNormal.xyz, (float3x3)g_matWorld);
	Out.N = normalize(worldNormal);

	float3 Tangent = /*cross(float3(0.f, 1.f, 0.f), In.vNormal.xyz)*/float3(1.f, 0.f, 0.f);
	float3 worldTangent = mul(Tangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);

	float3 Binormal = /*cross(Tangent,  In.vNormal.xyz)*/ float3(0.f, 0.f, -1.f);
	float3 worldBinormal = mul(Binormal, (float3x3)g_matWorld);
	Out.B = normalize(worldBinormal);

	return Out;
}

// COLOR, TEXCOORD
struct PS_IN 
{
	float2		vUV				: TEXCOORD0;
	vector	vViewPos		: TEXCOORD1;
	float3		N					: TEXCOORD2;
	vector	vProjPos			: TEXCOORD3;
	float3		T					: TEXCOORD4;
	float3		B					: TEXCOORD5;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;	// COLOR0 : 0������Ÿ�ٿ� �׸� ����
	vector		vNormal : COLOR1;	// COLOR0 : 1������Ÿ�ٿ� �׸� ����
	vector		vDepth	: COLOR2;	// COLOR1 : 2������Ÿ�ٿ� �׸� ����
	//vector		vEffect	: COLOR3;	// COLOR1 : 2������Ÿ�ٿ� �׸� ����
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vUV);
	Out.vColor.a = 1.f;

	vector NormalTex = tex2D(NormalSampler, In.vUV);
	//Out.vColor= NormalTex;
	float3 tangentNormal = normalize(NormalTex.xyz * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);

	vector worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	//Out.vNormal	= /*vector(NormalTex.xyz, 0.f)*//*worldNormal*/;
	Out.vNormal	= /*vector(In.N.xyz, 0.f)*/worldNormal;
	Out.vDepth		= vector(In.vViewPos.z * 0.001f, In.vViewPos.z * 0.001f,  In.vViewPos.z * 0.001f, In.vProjPos.z * 0.001f);

	//Out.vEffect		= 0.f;
	return Out;
}

// technique : ��ġ�� �������ο����� �ٸ� ���̵��� �� �� �ֵ��� �Ѵ�.
technique Default
{
	// pass : ����� ĸ��ȭ
	pass Render_Terrain
	{		
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};