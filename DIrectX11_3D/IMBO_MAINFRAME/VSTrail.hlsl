

cbuffer ViewProjectionConstantBuffer : register(b11)
{
	matrix gmtxViewProjection;
};

cbuffer TrailInfo : register(b5)
{
	float4		g_xmf4Vtx[80];	
	float4		g_xmf4Color;
};
struct VS_OUTPUT
{
	float3 position : POSITION;
	//float2 texCoord : TEXCOORD0;
};
VS_OUTPUT main()
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//output.position = mul(float4(position, 1.f), mtxWorld[instanceID]);
	//output.position = mul(output.position, gmtxViewProjection);
	//output.texCoord = tex;

	return output;
}