#include "Shader_Setter.hpp"

// 전역변수 (Constant Table)

vector			g_Color;
vector			g_ColorG;
vector			g_ColorB;

vector			g_WorldCamPos;

vector			g_LightDir;

texture			g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture		= g_NormalTexture;
	minfilter		= linear;
	magfilter		= linear;
	mipfilter		= linear;
};

texture			g_EnvironmentTexture;
sampler EnvironmentSampler = sampler_state
{
	texture		= g_EnvironmentTexture;
	minfilter		= linear;
	magfilter		= linear;
	mipfilter		= linear;
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

	Out.vColor = tex2D(BaseSampler, In.vUV);

	float vRed;		// R 종합 값
	float vGreen;	// G 종합 값
	float vBlue;		// B 종합 값

	vRed		= ( Out.vColor.r * g_Color.r ) + ( Out.vColor.g * g_ColorG.r ) + ( Out.vColor.b * g_ColorB.r );
	vGreen	= ( Out.vColor.r * g_Color.g ) + ( Out.vColor.g * g_ColorG.g ) + ( Out.vColor.b * g_ColorB.g );
	vBlue		= ( Out.vColor.r * g_Color.b ) + ( Out.vColor.g * g_ColorG.b ) + ( Out.vColor.b * g_ColorB.b );

	Out.vColor.r = vRed;
	Out.vColor.g = vGreen;
	Out.vColor.b = vBlue;

	//float fResult	= max(Out.vColor.r, max(Out.vColor.g, Out.vColor.b));
	//float fGray		= dot(Out.vColor.xyz, float3(0.222, 0.707, 0.071));
	//if( Out.vColor.r == fResult )
	//{
	//	Out.vColor = fGray * g_Color;
	//}
	//else if( Out.vColor.g == fResult )
	//{
	//	Out.vColor = fGray * g_ColorG;
	//}
	//else if( Out.vColor.b == fResult )
	//{
	//	Out.vColor = fGray * g_ColorB;
	//}

	
	float3 tangentNormal = tex2D(NormalSampler, In.vUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
	TBN = transpose(TBN);

	vector worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	float4		vLook		= vector(normalize(In.vLook).xyz, 0.f);
	float		fRimLight = smoothstep(1.f - 0.4f, 1.f, 1 -max(0, dot(worldNormal, vLook)));

	Out.vColor		= Out.vColor + fRimLight;
	Out.vNormal	= worldNormal;
	Out.vDepth		= vector(In.vViewPos.z * 0.001f, In.vViewPos.z * 0.001f,  In.vViewPos.z * 0.001f, In.vProjPos.z * 0.001f);

	Out.vColor.a = 1.f;

	//Out.vEffect = 0.f;
	return Out;
}

////// Pass 2 ////////////////////////////////////////////////////////////////////////////////////////////////////////

// VertexShading : 정점의 변환 (영역의 변환)
VS_OUT VS_Metal(VS_IN In)
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

PS_OUT PS_Metal(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vUV);

	float vRed;		// R 종합 값
	float vGreen;	// G 종합 값
	float vBlue;		// B 종합 값

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

	vector	worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	float4		vLook		= vector(normalize(In.vLook).xyz, 0.f);
	float		fRimLight = smoothstep(1.f - 0.8f, 1.f, 1 -max(0, dot(worldNormal, vLook)));
	fRimLight += 0.f;
	fRimLight = pow(fRimLight, 10.f);

	float3		viewReflect = reflect(-vLook.xyz, worldNormal.xyz);
	float3		encironment = texCUBE(EnvironmentSampler, viewReflect).rgb;


	Out.vColor		= Out.vColor + fRimLight + vector(encironment, 0.f) * 0.5f;
	Out.vNormal	= worldNormal;
	Out.vDepth		= vector(In.vViewPos.z * 0.001f, In.vViewPos.z * 0.001f,  In.vViewPos.z * 0.001f, In.vProjPos.z * 0.001f);

	Out.vColor.a = 1.f;

	//Out.vEffect = 0.f;
	return Out;
}



////// Pass 3 ////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_OUTSKIN
{
	vector	vPosition			: POSITION;
	float2		vUV				: TEXCOORD0;
	vector	vViewPos		: TEXCOORD1;
	//vector	vWorldNormal	: TEXCOORD2;
	float3		T					: TEXCOORD2;
	float3		B					: TEXCOORD3;
	float3		N					: TEXCOORD4;
	float3		vLook				: TEXCOORD5;
	vector	vDiffuse			: TEXCOORD6;
	vector	vProjPos			: TEXCOORD7;
};

VS_OUTSKIN VS_Skin(VS_IN In)
{
	VS_OUTSKIN		Out = (VS_OUTSKIN)0;

	matrix		matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);
	matWVP			= mul(matWV, g_matProj);	

	Out.vViewPos	= mul(In.vPosition, matWV);

	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vUV			= In.vUV;
	Out.vProjPos	= Out.vPosition;

	vector	vDiffuse;
	Out.vLook		= normalize(g_WorldCamPos - mul(In.vPosition, g_matWorld)).xyz;

	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.N = normalize(worldNormal);
	Out.vDiffuse = dot(normalize(g_LightDir), Out.N);

	float3 Tangent = cross(float3(0.f, 1.f, 0.f), In.vNormal);
	float3 worldTangent = mul(Tangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);

	float3 Binormal = cross(Tangent,  In.vNormal);
	float3 worldBinormal = mul(Binormal, (float3x3)g_matWorld);
	Out.B = normalize(worldBinormal);


	return Out;
}

struct PS_INSKIN
{
	float2		vUV				: TEXCOORD0;
	vector	vViewPos		: TEXCOORD1;
	float3         T				: TEXCOORD2;
	float3         B				: TEXCOORD3;
	float3         N				: TEXCOORD4;
	float3		vLook				: TEXCOORD5;
	vector	vDiffuse			: TEXCOORD6;
	vector	vProjPos			: TEXCOORD7;
};

PS_OUT PS_Skin(PS_INSKIN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vUV);
	float fAlpha = Out.vColor.a;

	float vRed;		// R 종합 값
	float vGreen;	// G 종합 값
	float vBlue;		// B 종합 값

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

	vector	worldNormal = vector(normalize(mul(TBN, tangentNormal)), 0.f);

	//rim right
	float4		vLook		= vector(normalize(In.vLook).xyz, 0.f);
	float		fRimLight = smoothstep(1.f - 0.4f, 1.f, 1 -max(0, dot(worldNormal, vLook)));

	//fakeSSS
	vector	vDiffuse = 1 -saturate(In.vDiffuse);
	vDiffuse	= pow(vDiffuse, 40);


	Out.vColor		= Out.vColor + fRimLight - vector(0.05f, 0.05f, 0.05f, 0.f);
	Out.vColor.r  += vDiffuse.r * 0.12f;
	Out.vNormal	= worldNormal;
	Out.vDepth		= vector(In.vViewPos.z * 0.001f, In.vViewPos.z * 0.001f,  In.vViewPos.z * 0.001f, In.vProjPos.z * 0.001f);

	clip( 0.1 >= Out.vColor.a ? -1 : 1);

	///*Out*/.vEffect = 0.f;

	return Out;
}


// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Render_PlayerDefault
	{		
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass Render_PlayerMetal
	{		
		VertexShader = compile vs_3_0 VS_Metal();
		PixelShader = compile ps_3_0 PS_Metal();
	}
	pass Render_PlayerSkin
	{		
		VertexShader = compile vs_3_0 VS_Skin();
		PixelShader = compile ps_3_0 PS_Skin();

	/*	AlphaBlendenable = true;
		Srcblend = srcalpha;
		Destblend = invsrcalpha;*/
	}
};