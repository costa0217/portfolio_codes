
texture2D gDepthtexture : register(t6);
texture2D gSkyBoxtexture : register(t4);
texture2D gtexture : register(t5);
TextureCube gtxtSkyBox : register(t13);
sampler gSampler : register(s0);

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 WPos : TEXCOORD1;
};

cbuffer cbPsCameraDynamic : register(b12) {
	float4x4 ViewInv : packoffset(c0);
	float4 EyePosition : packoffset(c4);
	float4x4 ViewProj : packoffset(c5);
}
cbuffer EffectInfo : register(b5)
{
	int			g_iTexOpt;
	float		g_fFrameSpd;
	int			g_iU;
	int			g_iV;
	float		g_fFrameCnt;
	int			g_iFrameHeight;
	float		noUse1;
	float		noUse2;
	float4		g_xmColor;
	matrix		g_matWorld;
};
struct PS_EFFECT_OUT {
	float4 Color : SV_TARGET0;
	float4 Alpha : SV_TARGET1;
};

PS_EFFECT_OUT main(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	PS_EFFECT_OUT output = (PS_EFFECT_OUT)0;
	float4 cBaseTexColor = gtexture.Sample(gSampler, input.texCoord);

	/*float fDepth = gDepthtexture.Sample(gSampler, input.texCoord).x;
	return float4(fDepth, 0.f, 0.f, 1.f);*/

	if (g_iTexOpt == 1)
	{
		cBaseTexColor.a = (cBaseTexColor.r + cBaseTexColor.g + cBaseTexColor.b);
		clip(all(cBaseTexColor.rgb) < 0.3 ? -1 : 1);
	}
	else
	{
		clip(cBaseTexColor.a < 0.1 ? -1 : 1);
	}

	cBaseTexColor.a = saturate(cBaseTexColor.a);
	cBaseTexColor *= g_xmColor;
	cBaseTexColor = saturate(cBaseTexColor);
	//float4 pxlPos = mul(float4(input.WPos.xyz /*- EyePosition.xyz*/, 1.f), ViewProj);
	//float2 f2uv = pxlPos.xy / pxlPos.w;
	//f2uv.xy = 0.5 * f2uv + 0.5;
	//f2uv.y = 1 - f2uv.y;

	//float fDepth = gDepthtexture.Sample(gSampler, f2uv).x;
	////return float4(fDepth, 0.f, 0.f, 1.f);
	//if (fDepth > 0.999)
	//{
	//	float4 cSkyBoxColor = gSkyBoxtexture.Sample(gSampler, f2uv);
	//	cBaseTexColor.rgb = cBaseTexColor.rgb * cBaseTexColor.a + cSkyBoxColor.rgb * (1 - cBaseTexColor.a);
	//	cBaseTexColor.a = 1.f;
	//}
	output.Color = cBaseTexColor;
	output.Alpha = float4(cBaseTexColor.a, 0.f, 0.f, 1.f);


	return output;
}