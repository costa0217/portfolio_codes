
static const float2 arrBasePos[4] = {
	float2(-1.0, -1.0),
	float2(-1.0, 1.0),
	float2(1.0, -1.0),
	float2(1.0, 1.0)
};
static const float2 arrUV[4] = {
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0)
};


cbuffer ViewProjectionConstantBuffer : register(b11)
{
	matrix		g_mtxViewProjection;//Á÷±³
	//float		g_fAlpha;
};

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

VS_OUTPUT main(uint vertexID : SV_VertexID)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(float4(arrBasePos[vertexID], 0.0f, 1.0f), g_mtxViewProjection);
	output.texCoord = arrUV[vertexID];

	return output;
}