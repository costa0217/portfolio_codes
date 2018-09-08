
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

struct VS_OUTPUT {
	float3 position : POSITION;
};

VS_OUTPUT main()
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//output.position = mul(float4(position, 1.0f), g_matWorld);
	//output.position = mul(output.position, gmtxViewProjection);
	////output.texCoord = texCoord;

	//output.texCoord.x = (texCoord.x + (int)g_fFrameCnt) / g_iU;
	//output.texCoord.y = (texCoord.y + g_iFrameHeight) / g_iV;

	return output;
}
