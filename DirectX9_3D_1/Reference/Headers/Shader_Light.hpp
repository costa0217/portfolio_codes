#include "Shader_Setter.hpp"
vector			g_vLightDir;
vector			g_vLightPos;

vector			g_LightDiffuse;
vector			g_Mtrl;

float				g_fRange;
float				g_fLightCnt;

texture			g_NormalTexture;
sampler NormalSampler = sampler_state{
	texture = g_NormalTexture; };

texture			g_DepthTexture;
sampler DepthSampler = sampler_state{
	texture = g_DepthTexture; };


matrix	g_matWVP;
// Shadow //////////////////////////////////
matrix	g_matLightView;
matrix	g_matLightProj;
texture	g_ShadowTexture;
sampler ShadowMapSampler = sampler_state
{
	texture = g_ShadowTexture;

	AddressU = Clamp;
	AddressV = Clamp;
};
//
vector	g_vWorldCamPos;
matrix	g_matInvVP;

float		g_fWidthTexel;
float		g_fHeightTexel;

float		g_fHeightFxl;
float		g_fWidthFxl;

struct VS_OUT
{
	vector	vPos		: POSITION;	
	float2		vUV		: TEXCOORD0;
	float3		vRayDir	: TEXCOORD1;
};
// DIRECTION //////////////////////////////////////////
VS_OUT VS_MAIN_DIRECTIONAL(float4 vPos : POSITION
										  ,float2 vUV : TEXCOORD0 )
{
	VS_OUT Out = (VS_OUT)0;

	vUV.x	 += g_fWidthTexel;
	vUV.y	 += g_fHeightTexel;
	Out.vUV		= vUV;
	//Out.vUV.x	+= 1.f / 1600.f;
	//Out.vUV.y	+= 1.f / 900.f;

	Out.vPos	= vPos;

	float2 uv;

	uv.x = vUV.x * 2.f - 1.f;
	uv.y = vUV.y * -2.f + 1.f;

	Out.vRayDir = mul(float4(uv.xy, 1.f, 1.f) * 1000.f, g_matInvVP).xyz;
	Out.vRayDir -= g_vWorldCamPos.xyz;

	return Out;
}
struct PS_IN
{
	float2			vUV		: TEXCOORD0;
	float3			vRayDir : TEXCOORD1;
};
struct PS_OUT
{
	vector	vColor : COLOR0;
	//vector	vSpec  : COLOR1;
};
PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	///// ±×¸²ÀÚºÎºÐ //////
	float	fLinearDepth = tex2D(DepthSampler, In.vUV).r;
	vector	vPxlPosW = g_vWorldCamPos + (vector(In.vRayDir.xyz, 1.f) * fLinearDepth);
	////
	if(1 == fLinearDepth)
	{
		Out.vColor = vector(1.f, 1.f, 1.f, g_fLightCnt);
		return Out;
	}
	float sum = 0;
	if(1 != tex2D(DepthSampler, In.vUV).b)
	{
		vector	vPxlPosLVP;
		vector	vPxlPosLV = vPxlPosW;
		vPxlPosLV.w	= 1.f;
		vPxlPosLV		= mul(vPxlPosLV, g_matLightView);
		vPxlPosLVP		= mul(vPxlPosLV, g_matLightProj);

		float	fDepth = vPxlPosLV.z * 0.001f;
		float2 uv	 = vPxlPosLVP.xy / vPxlPosLVP.w;
		uv.y		= -uv.y;
		uv			= uv * 0.5f + 0.5f;


		//float fShadowDepth = tex2D(ShadowMapSampler, uv).r;

		// pcf---------------------------------------
		float fShadowDepth = /*tex2D(ShadowMapSampler, uv).r*/ 0;
		
		float x, y;
		for(y = -1.f; y <= 1.f; y += 1.f)
		{
			for(x = -1.f;  x <= 1.f; x += 1.f)
			{
				float2 pcfUV = uv + float2(x/*4096.f*/* g_fWidthFxl , y /*/ 2304.f*/* g_fHeightFxl);
				float DepthSize = tex2D(ShadowMapSampler, pcfUV).r;

				if(fDepth > DepthSize + 0.0003f) 
				{
					sum += 0.065f;
				}
			}
		}
	}	
	vector	vNormal = tex2D(NormalSampler, In.vUV);
	vNormal = normalize(vector(vNormal.xyz * 2.f - 1.f, 0.f));
	vector	vLightDir = normalize(vector(g_vLightDir.xyz * -1.f, 0.f));

	Out.vColor = saturate(dot(vector(vLightDir.xyz, 0.f), vNormal));
	Out.vColor.xyz *=  g_fLightCnt;


	Out.vColor += vector(0.12f, 0.12f, 0.12f, 0.f);		// ¾Úºñ¾ðÆ®
	Out.vColor *= (1.f - sum);

	Out.vColor.a = 1.f;

	return Out;
}
// POINT //////////////////////////////////////////

VS_OUT VS_MAIN_POINT(float4 vPos : POSITION
								,float2 vUV : TEXCOORD0 )
{
	VS_OUT Out = (VS_OUT)0;

	vUV.x	 += g_fWidthTexel;
	vUV.y	 += g_fHeightTexel;

	Out.vUV		= vUV;
	Out.vPos	= vPos;

	float2 uv;

	uv.x = vUV.x * 2.f - 1.f;
	uv.y = vUV.y * -2.f + 1.f;

	Out.vRayDir = mul(float4(uv.xy, 1.f, 1.f) * 1000.f, g_matInvVP).xyz;
	Out.vRayDir -= g_vWorldCamPos.xyz;

	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{	
	PS_OUT			Out = (PS_OUT)0;

	vector	vNormalCol	= tex2D(NormalSampler, In.vUV);
	vector	vNormal		= vector(vNormalCol.xyz * 2.f - 1.f, 0.f);

	float	fLinearDepth = tex2D(DepthSampler, In.vUV).r;

	if(0 == fLinearDepth)
	{
		Out.vColor = vector(1.f, 1.f, 1.f, g_fLightCnt);
		return Out;
	}

	vector	vPxlPosW = g_vWorldCamPos + (vector(In.vRayDir.xyz, 1.f) * fLinearDepth);

	vector vLightDir =  vPxlPosW - g_vLightPos;
	float	fLength = length(vLightDir);
	vLightDir = normalize(-vLightDir);

	float	fAtt = max((g_fRange - fLength) / g_fRange, 0.f);

	Out.vColor = saturate(dot(vector(vLightDir.xyz, 0.f), vNormal)) * fAtt;
	Out.vColor = Out.vColor * (g_LightDiffuse * g_Mtrl);
	Out.vColor.a = 1.f;
	return Out;
}

// pass 3  No Shadow /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VS_OUT VS_MAIN_DIRNoShadow(float4 vPos : POSITION
											,float2 vUV : TEXCOORD0 )
{
	VS_OUT Out = (VS_OUT)0;

	vUV.x	 += g_fWidthTexel;
	vUV.y	 += g_fHeightTexel;

	Out.vUV		= vUV;
	Out.vPos	= vPos;

	float2 uv;

	uv.x = vUV.x * 2.f - 1.f;
	uv.y = vUV.y * -2.f + 1.f;

	Out.vRayDir = mul(float4(uv.xy, 1.f, 1.f) * 1000.f, g_matInvVP).xyz;
	Out.vRayDir -= g_vWorldCamPos.xyz;

	return Out;
}
PS_OUT PS_MAIN_DIRNoShadow(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float	fLinearDepth = tex2D(DepthSampler, In.vUV).r;
	if(1 == fLinearDepth)
	{
		Out.vColor = vector(1.f, 1.f, 1.f, g_fLightCnt);
		return Out;
	}
	vector	vPxlPosW = g_vWorldCamPos + (vector(In.vRayDir.xyz, 1.f) * fLinearDepth);

	vector	vNormal = tex2D(NormalSampler, In.vUV);
	vNormal = normalize(vector(vNormal.xyz * 2.f - 1.f, 0.f));
	vector	vLightDir = normalize(vector(g_vLightDir.xyz * -1.f, 0.f));

	Out.vColor = saturate(dot(vector(vLightDir.xyz, 0.f), vNormal));
	Out.vColor *=  g_fLightCnt;

	///// ½ºÆäÅ§·¯ ///// 
	//vector specular = 0;

	//vector reflection = reflect(-vLightDir, vNormal);
	//vector viewDir = normalize(vector(vPxlPosW.xyz - g_vWorldCamPos.xyz, 0.f));

	//if(Out.vColor.x != 0)
	//{
	//	specular = saturate(dot(reflection, -viewDir));
	//	specular = pow(specular, 40.f);
	//}

	Out.vColor += vector(0.12f, 0.12f, 0.12f, 0.f); // ¾Úºñ¾ðÆ®
	//Out.vColor += specular * 0.2f;

	return Out;
}

technique Default
{
	//VertexShader = compile vs_3_0 VS_MAIN();
	// pass : ±â´ÉÀÇ Ä¸½¶È­
	pass RenderToLightTarget_Directional
	{		
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		VertexShader	= compile vs_3_0 VS_MAIN_DIRECTIONAL();
		PixelShader		= compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}	
	pass RenderToLightTarget_Point
	{		
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		VertexShader	= compile vs_3_0 VS_MAIN_POINT()/*compile vs_3_0 VS_MAIN_DIRECTIONAL()*/;
		PixelShader		= compile ps_3_0 PS_MAIN_POINT();
	}
	pass RenderToLightTarget_DirectionalNoShadow
	{		
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		VertexShader	= compile vs_3_0 VS_MAIN_DIRNoShadow();
		PixelShader		= compile ps_3_0 PS_MAIN_DIRNoShadow();
	}	
};
