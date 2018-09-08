


sampler gSampler : register(s0);

texture2D gAlphatexture : register(t1);
texture2D gEffecttexture : register(t2);
texture2D gOritexture : register(t3);
texture2D gDistexture : register(t4);
texture2D gDepthtexture : register(t5);
texture2D gEffectDepthtexture : register(t6);



struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

struct PS_EFFECT_OUT {
	float4 Color : SV_TARGET0;
	float4 Alpha : SV_TARGET1;
};

PS_EFFECT_OUT main(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	PS_EFFECT_OUT output = (PS_EFFECT_OUT)0;
	//float2		Trans = input.texCoord;
	//float4		Noise = gDistexture.Sample(gSampler, Trans);
	//float2		UV = input.texCoord + Noise.xy * 0.05f;		// 0.05 -> ø÷∞Ó¿« ¡§µµ
	//float4		cBaseTexColor = gOritexture.Sample(gSampler, UV);

	float fObjDepth = gDepthtexture.Sample(gSampler, input.texCoord);
	float fEftDepth = gEffectDepthtexture.Sample(gSampler, input.texCoord);
	float Alpha = saturate((fObjDepth - fEftDepth) * 100.f);
	float fEffectAlpha = gAlphatexture.Sample(gSampler, input.texCoord);
	//cBaseTexColor = saturate(cBaseTexColor);
	//fEffectAlpha = fEffectAlpha - Alpha;
	float4		cBaseTexColor = gOritexture.Sample(gSampler, input.texCoord);
	float4		cEffectTexColor = gEffecttexture.Sample(gSampler, input.texCoord);
	//float3 color;
	//color = saturate((cBaseTexColor.rgb * (1 - cEffectTexColor.a)) + (cEffectTexColor.rgb * cEffectTexColor.a));
	//cBaseTexColor = saturate((cBaseTexColor * (1 - Alpha)) * (1 - cEffectTexColor.a) + (cEffectTexColor * Alpha) * cEffectTexColor.a);
	cBaseTexColor.rgb = (cBaseTexColor.rgb) + saturate(cEffectTexColor.rgb * (Alpha));
	//cBaseTexColor.a = cEffectTexColor.a - Alpha;
	output.Color = cBaseTexColor;
	output.Alpha.r = fObjDepth != 1 ? 1.f : fEffectAlpha;
	return output;
}