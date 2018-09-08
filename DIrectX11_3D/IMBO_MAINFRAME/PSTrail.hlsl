
Texture2D    gtxtDefault : register(t5);
SamplerState gssWRAP_LINEAR : register(s0);

cbuffer TrailInfo : register(b5)
{
	float4		g_xmf4Vtx[80];
	float4		g_xmf4Color;
};

struct PS_OUT {
	float4 Color : SV_TARGET0;
	float4 Alpha : SV_TARGET1;
};
struct VSOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};
PS_OUT main(VSOutput input) : SV_TARGET
{
	PS_OUT output = (PS_OUT)0;

	output.Color = gtxtDefault.Sample(gssWRAP_LINEAR, input.texCoord);
	clip(output.Color.r + output.Color.g + output.Color.b < 0.3 ? -1 : 1);
	output.Color *= g_xmf4Color;
	output.Alpha = float4(output.Color.a, 0.f, 0.f, 1.f);
	return output;	
}