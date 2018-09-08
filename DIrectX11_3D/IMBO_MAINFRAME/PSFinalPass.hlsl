
sampler gSampler : register(s0);

texture2D gAlbedoTexture : register(t1);
texture2D gAlphaTexture : register(t2);
texture2D gSkyBoxTexture : register(t3);
texture2D gDistTexture : register(t4);

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

cbuffer OptInfo : register(b5)
{
	int			g_iOpt;	// 0 - noRadial | 1 - Radial | 2 - Gray
	float		g_BlurStart;
	float		g_BlurWidth;
	int			noUse;
};

float4 main(VS_TEXTURED_OUTPUT input) : SV_TARGET
{

	float2		Trans = input.texCoord;
	float4		Noise = gDistTexture.Sample(gSampler, Trans);
	float2		UV = input.texCoord + Noise.xy * 0.02f;		// 0.05 -> ø÷∞Ó¿« ¡§µµ

	float Alpha = gAlphaTexture.Sample(gSampler, UV).r;

	float4 AlbedoColor = gAlbedoTexture.Sample(gSampler, UV);
	float4 SkyboxColor = gSkyBoxTexture.Sample(gSampler, UV);
	float4 outColor;

	if (g_iOpt == 1)
	{
		float2		Center = { 0.5, 0.5 };
		//float		BlurStart = 1.f;
		//float		BlurWidth = -0.2f;
		int			ample = 10;

		float4 Sum = (float4)0;
		float fDivide = 1 / (float)(ample - 1);
		float dist = distance(UV, float2(0.5, 0.5))* 1.5;
		UV -= Center;

		if (Alpha == 0) {
			outColor = SkyboxColor;
			for (int i = 0; i < 10; ++i){
				float soale = g_BlurStart + g_BlurWidth * (i * fDivide);
				Sum += gSkyBoxTexture.Sample(gSampler, UV * soale + Center);
			}
		}
		else if (Alpha == 1) {
			outColor = AlbedoColor;
			for (int i = 0; i < 10; ++i){
				float soale = g_BlurStart + g_BlurWidth * (i * fDivide);
				Sum += gAlbedoTexture.Sample(gSampler, UV * soale + Center);
			}
		}
		else {
			outColor = SkyboxColor + AlbedoColor * (Alpha + 0.01);
			for (int i = 0; i < 10; ++i){
				float soale = g_BlurStart + g_BlurWidth * (i * fDivide);
				Sum += gAlbedoTexture.Sample(gSampler, UV * soale + Center) + gAlbedoTexture.Sample(gSampler, UV * soale + Center)* (Alpha + 0.01);
			}
		}
		Sum *= fDivide;
		float fLinear = smoothstep(0, 1, dist);
		return (1 - fLinear) * outColor + fLinear * Sum;
	}
	
	if (Alpha == 0)		{	outColor = SkyboxColor;	}
	else if (Alpha == 1){	outColor = AlbedoColor;	}
	else {	outColor = SkyboxColor + AlbedoColor * (Alpha + 0.01);}


	if (g_iOpt == 2)
	{
		float3 fAvgColor = (outColor.r + outColor.g + outColor.b) / 3.f;
		outColor = float4(fAvgColor, 1.f);
	}
	return outColor;
}