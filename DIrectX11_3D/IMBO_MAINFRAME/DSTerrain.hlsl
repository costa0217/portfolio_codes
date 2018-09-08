
texture2D gtxtHeightMap : register(t0);
Buffer<float> gtxtControllHeightMap : register(t1);

SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);


cbuffer ViewProjectionConstantBuffer : register(b11){
	matrix gmtxViewProjection;
};

//#define TEXTURE_SIZE 256
//cbuffer HightMapBuffer:register(b1) {
//	uint gHightValue[TEXTURE_SIZE * TEXTURE_SIZE];
//}

cbuffer GlobalTerrainInfo : register(b10){
	float gOneSpaceSizeRcp;//�ϳ��� ������ ũ��
	float gOneSideSpaceNumRcp;//1 / ���� ���̵忡 �ִ� ���� ��
	float gOneSideSpaceNum;//���� ���̵忡 �ִ� ���� ��
	float gHeightScale;
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
struct DS_OUT {
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float2 texCoord : TEXCOORD;
	float2 detailTexCoord : TEXCOORD1;
	float2 baseTexCoord : TEXCOORD2;
	//float3 tangentW : TANGENT;
	//float3 bitangentW : BITANGET;
};

static const int nWidth = 512;

// ��ġ ��� �����͸� ����մϴ�.
struct TERRAIN_HS_CONSTANT
{
	float EdgeTessFactor[4]			: SV_TessFactor; // ���� ��� ���� �����ο� ���� [4]�Դϴ�.
	float InsideTessFactor[2]		: SV_InsideTessFactor; // ���� ��� ���� �����ο� ���� Inside[2]�Դϴ�.
														   // TODO: �ٸ� ������ ����/�߰��մϴ�.
};


int GetIndex(int x, int y) {

	int X = clamp(x, 0, nWidth - 1);
	int Z = clamp(y, 0, nWidth - 1);

	return X + Z * nWidth;
}
float GetHeight(float2 uv) {
	float2 fPos = float2(1 - uv.x, uv.y) * (nWidth - 1);
	// Clamp the input pixel position
	int x = clamp(fPos.x, 0, nWidth - 1);
	int z = clamp(fPos.y, 0, nWidth - 1);

	//float fHeight = gtxtControllHeightMap[GetIndex(x, z)];
	//return fHeight * 256 * gHeightScale;

	float fxPercent = fPos.x - x;
	float fzPercent = fPos.y - z;

	float fBottomLeft = gtxtControllHeightMap[GetIndex(x, z)];
	float fBottomRight = gtxtControllHeightMap[GetIndex(x + 1, z)];
	float fTopLeft = gtxtControllHeightMap[GetIndex(x, z + 1)];
	float fTopRight = gtxtControllHeightMap[GetIndex(x + 1, z + 1)];

	float fTopHeight = lerp(fTopLeft, fTopRight, fxPercent);
	float fBottomHeight = lerp(fBottomLeft, fBottomRight, fxPercent);
	float fHeight = lerp(fBottomHeight, fTopHeight, fzPercent);

	return (fHeight * gHeightScale);
}
[domain("quad")]
DS_OUT main(TERRAIN_HS_CONSTANT input, float2 uv : SV_DomainLocation, OutputPatch<TERRAIN_HS_OUTPUT, 4> quad)
{
	DS_OUT output;

	output.positionW = lerp(lerp(quad[0].positionW, quad[1].positionW, uv.x), lerp(quad[2].positionW, quad[3].positionW, uv.x), uv.y);
	output.texCoord = lerp(lerp(quad[0].texCoord, quad[1].texCoord, uv.x), lerp(quad[2].texCoord, quad[3].texCoord, uv.x), uv.y);
	output.detailTexCoord = lerp(lerp(quad[0].detailTexCoord, quad[1].detailTexCoord, uv.x), lerp(quad[2].detailTexCoord, quad[3].detailTexCoord, uv.x), uv.y);
	output.baseTexCoord = output.detailTexCoord * 32;
	output.detailTexCoord = output.detailTexCoord * 32;

	//output.tangentW = lerp(lerp(quad[0].tangentW, quad[1].tangentW, uv.x), lerp(quad[2].tangentW, quad[3].tangentW, uv.x), uv.y);
	//output.bitangentW = lerp(lerp(quad[0].bitangentW, quad[1].bitangentW, uv.x), lerp(quad[2].bitangentW, quad[3].bitangentW, uv.x), uv.y);

	output.positionW.y = GetHeight(output.texCoord);//gtxtControllHeightMap.SampleLevel(gssWRAP_LINEAR, output.texCoord, 0).r * 256 *gHeightScale;//get height 

													//calc _view projection
	output.position = mul(float4(output.positionW, 1.0f), gmtxViewProjection);

	return output;
}
