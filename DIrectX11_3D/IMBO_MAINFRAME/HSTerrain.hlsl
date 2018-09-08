
cbuffer gbTessFacter : register(b1) {
	float3 camera_pos : packoffset(c0.x);
	float zn : packoffset(c0.w);

	float zf : packoffset(c1.x);
	float max_facter_value : packoffset(c1.z);
	float min_facter_value : packoffset(c1.w);
	float space_lavel_pow : packoffset(c1.y);
};

// �������� �Է��մϴ�.
struct VS_OUTPUT {
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD;
	float2 detailTexCoord : TEXCOORD1;
	//float3 tangentW : TANGENT;
	//float3 bitangentW : BITANGET;
};

// �������� ����մϴ�.
struct TERRAIN_HS_OUTPUT
{
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD;
	float2 detailTexCoord : TEXCOORD1;
	//float3 tangentW : TANGENT;
	//float3 bitangentW : BITANGET;
};

// ��ġ ��� �����͸� ����մϴ�.
struct TERRAIN_HS_CONSTANT
{
	float fTessEdges[4]			: SV_TessFactor; // ���� ��� ���� �����ο� ���� [4]�Դϴ�.
	float fTessInsides[2]		: SV_InsideTessFactor; // ���� ��� ���� �����ο� ���� Inside[2]�Դϴ�.
	// TODO: �ٸ� ������ ����/�߰��մϴ�.
};
float CalculateTessFactor(float3 p) {
	float fDistToCamera = distance(p, camera_pos);
	float s = saturate((fDistToCamera - zn) / (zf - zn));
	return pow(2, (lerp(max_facter_value, min_facter_value, s)));
}
// ��ġ ��� �Լ�
TERRAIN_HS_CONSTANT ConstantHS(InputPatch<VS_OUTPUT, 4> input, uint nPatchID : SV_PrimitiveID)
{
	TERRAIN_HS_CONSTANT output;

	float3 e0 = 0.5f * (input[0].positionW + input[2].positionW);
	float3 e1 = 0.5f * (input[0].positionW + input[1].positionW);
	float3 e2 = 0.5f * (input[1].positionW + input[3].positionW);
	float3 e3 = 0.5f * (input[2].positionW + input[3].positionW);
	float3 c = 0.25f * (input[0].positionW + input[1].positionW + input[2].positionW + input[3].positionW);

	output.fTessEdges[0] = CalculateTessFactor(e0);
	output.fTessEdges[1] = CalculateTessFactor(e1);
	output.fTessEdges[2] = CalculateTessFactor(e2);
	output.fTessEdges[3] = CalculateTessFactor(e3);
	output.fTessInsides[0] = CalculateTessFactor(c);
	output.fTessInsides[1] = output.fTessInsides[0];

	return output;
}


[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
TERRAIN_HS_OUTPUT main(InputPatch<VS_OUTPUT, 4> input, uint i : SV_OutputControlPointID)
{
	TERRAIN_HS_OUTPUT output;
	output.positionW = input[i].positionW;
	output.texCoord = input[i].texCoord;
	output.detailTexCoord = input[i].detailTexCoord;
	//output.tangentW = input[i].tangentW;
	//output.bitangentW = input[i].bitangentW;
	return output;
}
