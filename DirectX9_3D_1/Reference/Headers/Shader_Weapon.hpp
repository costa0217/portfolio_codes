#include "Shader_Setter.hpp"
// �������� (Constant Table)
vector			g_Color;
vector			g_ColorG;
vector			g_ColorB;

texture			g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture		= g_NormalTexture;
	minfilter		= linear;
	magfilter		= linear;
	mipfilter		= linear;
};

vector			g_WorldCamPos;

// VertexShading�� �Է°����� ���� ������ ����
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

	Out.vLook		= normalize(g_WorldCamPos - mul(In.vPosition, g_matWorld)).xyz;

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
	float3		vLook				: TEXCOORD5;
	vector	vProjPos			: TEXCOORD6;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;
	vector		vNormal	: COLOR1;	// COLOR0 : 1������Ÿ�ٿ� �׸� ����
	vector		vDepth	: COLOR2;	// COLOR1 : 2������Ÿ�ٿ� �׸� ����
	//vector		vEffect	: COLOR3;	// COLOR1 : 2������Ÿ�ٿ� �׸� ����
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vUV);
	//Out.vColor -= vector(0.2f, 0.2f, 0.2f, 0.f);
	
	float vRed;		// R ���� ��
	float vGreen;	// G ���� ��
	float vBlue;		// B ���� ��

	vRed		= ( Out.vColor.r * g_Color.r ) + ( Out.vColor.g * g_ColorG.r ) + ( Out.vColor.b * g_ColorB.r );
	vGreen	= ( Out.vColor.r * g_Color.g ) + ( Out.vColor.g * g_ColorG.g ) + ( Out.vColor.b * g_ColorB.g );
	vBlue		= ( Out.vColor.r * g_Color.b ) + ( Out.vColor.g * g_ColorG.b ) + ( Out.vColor.b * g_ColorB.b );

	Out.vColor.r = vRed;
	Out.vColor.g = vGreen;
	Out.vColor.b = vBlue;

	float3 tangentNormal = tex2D(NormalSampler, In.vUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);

	vector worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	float4		vLook		= vector(normalize(In.vLook).xyz, 0.f);
	float		fRimLight = smoothstep(1.f - 0.6f, 1.f, 1 -max(0, dot(worldNormal, vLook)))/* + 0.5*/;
	fRimLight = pow(fRimLight , 3.f);

	Out.vColor		= Out.vColor + (fRimLight * 2.f);
	Out.vNormal	= worldNormal;
	Out.vDepth		= vector(In.vViewPos.z * 0.001f, In.vViewPos.z * 0.001f,  In.vViewPos.z * 0.001f, In.vProjPos.z * 0.001f);
	//Out.vEffect		= 0.f;
	return Out;
}

// technique : ��ġ�� �������ο����� �ٸ� ���̵��� �� �� �ֵ��� �Ѵ�.
technique Default
{
	// pass : ����� ĸ��ȭ
	pass Render_StaticMesh
	{		
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};