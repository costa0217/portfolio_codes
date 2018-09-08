
cbuffer ViewProjectionConstantBuffer : register(b11)
{
	matrix gmtxViewProjection;
};
cbuffer TrailInfo : register(b5)
{
	float4		g_xmf4Vtx[80];
	float4		g_xmf4Color;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT {
	float3 position : POSITION;
};

//[maxvertexcount(4)]
[maxvertexcount(80)]
void main(point VS_OUTPUT input[1],
	inout TriangleStream< GSOutput > stream
)
{
	GSOutput output[80];
	//GSOutput output[4];

	[unroll]
	for (uint i = 0; i < 80; ++i)
	{
		output[i] = (GSOutput)0;
		float4 localPos = g_xmf4Vtx[i];
		output[i].position = mul(localPos, gmtxViewProjection);

		if (i < 40)
		{
			output[i].texCoord = float2(1 - (i  * 0.025f), 1.f);
		}
		else
		{
			output[i].texCoord = float2(1 - ((i - 40) * 0.025f), 0.f);
		}
	}
	[unroll]
	for (int i = 0; i < 40; ++i)
	{
		for (int j = 1; j >= 0; --j)
		{
			stream.Append(output[i + j * 40]);
		}
	}	

	/*float4 localPos[4];
	float fLength = 10.f;
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
		output[i].position = mul(localPos[i], g_xmWorld);
		output[i].position = mul(output[i].position, gmtxViewProjection);

		stream.Append(output[i]);
	}*/
}
