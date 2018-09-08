//여긴 b0가 없으니까 이전의 down scale CS에서 사용한 cb를 사용 가능 하다.
StructuredBuffer<float4> MiniDepth : register(t0);
RWTexture2D<float> AO : register(u0);

cbuffer stCSDynamicData : register(b9) {
	float4x4 ViewMtx: packoffset(c0);//camera
};
cbuffer stCSStaticData : register(b10) {
	float4 ProjParams: packoffset(c0);   // camera
	float MaxDepth : packoffset(c1.x); ;//camera
};
cbuffer Static : register(b11)
{
	float2 ResRcp  : packoffset(c0.x);
	float OffsetRadius : packoffset(c0.z);
	float Radius : packoffset(c0.w);
}
cbuffer stCSGlobalBufferData : register(b13) {
	uint nWidth : packoffset(c0);
	uint nHeight: packoffset(c0.y);
	uint nHalfWidth: packoffset(c0.z);
	uint nHalfHeight: packoffset(c0.w);;
	uint nQuadWidth: packoffset(c1.x);
	uint nQuadHeight: packoffset(c1.y);
};
groupshared float SharedDepths[1024];

static const float NumSampleRcp = 1.0 / 8.0;
static const uint NumSamples = 8;
static const float2 SampleOffsets[NumSamples] = {
	float2(0.2803166, 0.08997212),
	float2(-0.5130632, 0.6877457),
	float2(0.425495, 0.8665376),
	float2(0.8732584, 0.3858971),
	float2(0.0498111, -0.6287371),
	float2(-0.9674183, 0.1236534),
	float2(-0.3788098, -0.09177673),
	float2(0.6985874, -0.5610316),
};
float GetDepth(int2 pos){
	// Clamp the input pixel position
	float x = clamp(pos.x, 0, nHalfWidth - 1);
	float y = clamp(pos.y, 0, nHalfHeight - 1);

	// find the mini-depth index position and retrive the detph value
	int minDepthIdx = x + y * nHalfWidth;
	return MiniDepth[minDepthIdx].x;
}
float3 GetNormal(int2 pos){
	// Clamp the input pixel position
	float x = clamp(pos.x, 0, nHalfWidth - 1);
	float y = clamp(pos.y, 0, nHalfHeight - 1);

	int miniDepthIdx = x + y * nHalfWidth; // find the mini-depth index position
	return MiniDepth[miniDepthIdx].yzw;
}
float ComputeAO(int2 centerPixelPos, float2 centerClipPos) {
	// Get the depths for the normal calculation
	float centerDepth = GetDepth(centerPixelPos.xy);

	bool isNotSky = centerDepth < MaxDepth;
	if (false == isNotSky) return 1.0f;

	// Find the center pixel veiwspace position
	float3 centerPos;
	centerPos.xy = centerClipPos * ProjParams.xy * centerDepth;
	centerPos.z = centerDepth;

	// Get the view space normal for the center pixel
	float3 centerNormal = GetNormal(centerPixelPos.xy);
	centerNormal = normalize(centerNormal);

	// random sampling 준비
	float rotationAngle = 0.0;
	//float rotationAngle = dot(float2(centerClipPos), float2(73.0, 197.0));
	float2 randSinCos;
	sincos(rotationAngle, randSinCos.x, randSinCos.y);
	float2x2 randRotMat = float2x2(randSinCos.y, -randSinCos.x, randSinCos.x, randSinCos.y);
	float ao = 0.0;
	[unroll]
	for (uint i = 0; i < NumSamples; i++){
		float2 sampleOff = OffsetRadius.xx * mul(SampleOffsets[i], randRotMat);
		float curDepth = GetDepth(centerPixelPos + int2(sampleOff.x, -sampleOff.y));
		// view space position구하기
		float3 curPos;
		curPos.xy = (centerClipPos + 2.0 * sampleOff * ResRcp) * ProjParams.xy * curDepth;
		curPos.z = curDepth;
		float3 centerToCurPos = curPos - centerPos;
		float lenCenterToCurPos = length(centerToCurPos);
		float angleFactor = 1.0 - dot(centerToCurPos / lenCenterToCurPos, centerNormal);
		float distFactor = lenCenterToCurPos / Radius;

		ao += saturate(max(distFactor, angleFactor));// *isNotSky;
	}

	return ao * NumSampleRcp;
}

//총 픽셀 수를 1024로 나눈 만쿰의 X스레드 그룹을 SSAO 컴퓨트 세이더에 적용한다. 
[numthreads(1024, 1, 1)]
void main( uint3 groupThreadId : SV_GroupThreadID, uint3 dispatchThreadId : SV_DispatchThreadID ){
	uint2 CurPixel = uint2(dispatchThreadId.x % nHalfWidth, dispatchThreadId.x / nHalfWidth);

	SharedDepths[groupThreadId.x] = MiniDepth[dispatchThreadId.x].x;

	GroupMemoryBarrierWithGroupSync();

	// Skip out of bound pixels
	if (CurPixel.y < nHalfHeight)
	{
		// Find the XY clip space position for the current pixel
		// Y has to be inverted
		float2 centerClipPos = 2.0 * float2(CurPixel)* ResRcp;
		centerClipPos = float2(centerClipPos.x - 1.0, 1.0 - centerClipPos.y);

		AO[CurPixel] = ComputeAO(CurPixel, centerClipPos);
	}
}
