
cbuffer ViewProjectionConstantBuffer : register(b11)
{
	matrix gmtxViewProjection;
};
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

struct GSOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 WPos : TEXCOORD1;
};

struct VS_OUTPUT {
	float3 position : POSITION;
};

[maxvertexcount(4)]
void main(	point VS_OUTPUT input[1],
			inout TriangleStream< GSOutput > stream
)
{
	GSOutput output[4];
	float4 localPos[4];

	float fLength = 1.f;
	localPos[0] = float4(-fLength, -fLength, 0.0, 1.0);
	output[0].texCoord = float2(0.f, 1.f);
	localPos[1] = float4(-fLength, fLength, 0.0, 1.0);
	output[1].texCoord = float2(0.f, 0.f);
	localPos[2] = float4(fLength, -fLength, 0.0, 1.0);
	output[2].texCoord = float2(1.f, 1.f);
	localPos[3] = float4(fLength, fLength, 0.0, 1.0);
	output[3].texCoord = float2(1.f, 0.f);


	for (uint i = 0; i < 4; ++i)
	{
		output[i].position = mul(localPos[i], g_matWorld);
		output[i].WPos = output[i].position;
		output[i].position = mul(output[i].position, gmtxViewProjection);
		output[i].texCoord.x = (output[i].texCoord.x + (int)g_fFrameCnt) / g_iU;
		output[i].texCoord.y = (output[i].texCoord.y + g_iFrameHeight) / g_iV;
	
		stream.Append(output[i]);
	}
}