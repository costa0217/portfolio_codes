cbuffer ViewProjectionConstantBuffer : register(b11)
{
	matrix gmtxViewProjection;
};
cbuffer DefaultCB : register(b1){
	matrix mtxWorld[1000];
};
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

// 픽셀 셰이더를 통과한 픽셀당 색 데이터입니다.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangentW : TANGENT;
	float3 binormalW : BINORMAL;
};


VS_OUTPUT main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.positionW = mul(float4(input.position, 1.0f), mtxWorld[instanceID]).xyz;
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);
	output.normal = mul(input.normal, (float3x3)mtxWorld[instanceID]);
	output.tangentW = mul(input.tangent, (float3x3)mtxWorld[instanceID]);
	output.binormalW = mul(input.binormal, (float3x3)mtxWorld[instanceID]);

	output.uv = input.uv;
	//output.color = float4(1.0f,1.0f,0.0f, 1.0f);

	return output;
}
